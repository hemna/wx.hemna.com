<?php

/**
 * This class is meant to act as an API
 * to fetch METAR information from a remote
 * site, then process it and combine it with the
 * airport data and return it as a JSON object.
 * 
 * This can handle requests for multiple airports
 * at once.
 * 
 * @author waboring
 *
 */
class AviationWeather extends JSONWidget {
	
	const ID = "avwx";
	const METAR_CACHE_TIMEOUT = 300;
	
	//list of airports to fetch.
	protected $airports;
	protected $airports_raw;
	
	protected $metars = array();

	protected $timer = NULL;
	
	private $cache;

	private $log;
	
	public function init() {
		$airports = Request::singleton()->get("airports");
		$this->airports_raw = $airports;
		if (strlen($airports) >0) {
			$this->airports = explode(',', strtoupper($airports));
		}		
		
		$this->cache = AirportCache::singleton();
		//$this->timer = new Timer();
		$this->log = new Log();
		$this->log->add_writer(FileLogWriter::factory(realpath($GLOBALS["path_base"])."/logs/aviation_weather.log"));

        PiwikTracker::$URL = 'http://wx.hemna.com/piwik/';
		$this->tracker = new PiwikTracker(1);
		$this->tracker->setUrl($_SERVER["SCRIPT_URI"]);
		$this->tracker->setBrowserLanguage($_SERVER["HTTP_ACCEPT_LANGUAGE"]);
		$this->tracker->setUserAgent($_SERVER["HTTP_USER_AGENT"]);
		$this->tracker->setIp($_SERVER["REMOTE_ADDR"]);

		$this->tracker->setCustomData(array("airports" => $this->airports));
		$this->tracker->doTrackPageView('AviationWeather');
	}
	
	public function build_object() {

		$size = count($this->airports);
		if ($size > 0) {
			foreach($this->airports as $icao) {
			   //$this->timer->start();
				$airport = $this->process_airport($icao);
				if (!empty($airport) && !is_null($airport)) {
					$this->log->info($_SERVER["REMOTE_ADDR"]." ".$this->airports_raw." yes");
				} else {
					$this->log->info($_SERVER["REMOTE_ADDR"]." ".$this->airports_raw." no");
				}
				//echo "Time took ".$this->timer->get_elapsed()."<br>";
				$this->metars[] = $airport;							
			}
		}
		
		return $this->metars;
	}
	
	
	public function process_airport($icao) {			
	   $icao = strtoupper($icao);

	   $cache_key = "METAR_".$icao;
	   $info =  $this->cache->get($cache_key);
		if ($info) {
		    $wxInfo = $info;
			 return $wxInfo;
		}

		$airport = $this->cache->get($icao);

		$wxInfo = array("icao" => $icao);
		$metar = $this->get_metar($icao, $wxInfo);
		if ($metar != null && strlen($metar) > 0) {
			$this->process_metar($metar, $wxInfo);
			$wxInfo["metar"] = $metar;
			$this->decide_vfr($wxInfo);
			
			if ($airport != null) {
				$wxInfo["runways"] = $this->get_runways($airport, $wxInfo);
				$wxInfo["altitude"] = $airport["elevation"];


				if (!isset($airport["details"]) || true) {
					$info = $this->get_airport_info($icao);
					if (!is_null($info)) {
						$airport["details"] = 1;
						$wxInfo = array_merge($wxInfo, $info);						
						$this->cache->set($icao,$airport);
					} else {
						//we need at least the airport name
						$wxInfo["Name"] = $airport["name"];
					}
				}

				//now do Pressure Altitude and Density Altitude
			   $wxInfo["PressureAltitude"] = $this->pressure_altitude($wxInfo["BaroHPA"], $wxInfo["altitude"]);
			   $wxInfo["DensityAltitude"] = $this->density_altitude($wxInfo["altitude"], $wxInfo["BaroHPA"], 
			                                                        $wxInfo["BaroHg"], $wxInfo["TempC"], $wxInfo["DewpointC"]);
			}

			if (is_null($airport["images"]) || !isset($airport["images"]) ||
			    is_null($airport["images"]["images"]) || !isset($airport["images"]["images"])) {
		       $images = $this->do_images($icao);
				 if ($images) {
				     $wxInfo["images"] = $images;
				     $airport["images"] = $images;
				     $this->cache->set($icao, $airport);
				 }
			} else {
				$wxInfo["images"] = $airport["images"];
			}

			if (!isset($airport["taf"]) || $airport["taf"] == 1) {
			    $taf = $this->get_taf($icao);

				 if (is_null($taf)) {
				     //if we couldn't find the TAF
					  //no reason to try again.
					  //not many airports have taf reports
				     $airport["taf"] = 0;
				 } else {
				     $airport["taf"] = 1;
					  $wxInfo["taf"] = $taf;
				 }

		       //we changed settings, save them to the cache.
				 $this->cache->set($icao,$airport);
			}
            
            if (isset($airport["wikipedia_url"])) {
                $wxInfo["wikipedia_url"] = $airport["wikipedia_url"];
            }


			//$this->metars[] = $wxInfo;
			//need to combine this info w/ the airport data cache.
		}

		$this->cache->set($cache_key, $wxInfo, AviationWeather::METAR_CACHE_TIMEOUT);
		return $wxInfo;		
	}
	
	protected function get_runways(&$airport, &$wxInfo) {
		$runways = &$airport["runways"];
		$info = array();
        
		if ($runways != null && is_array($runways)) {
		    foreach($runways as &$runway) {
			    $number = $runway["runway_number"];
			    $width = $runway["runway_width"] * 3.2808399;
			    $xw = $this->do_crosswind($runway, $wxInfo["Wind"]);					
			    $info[] = array("runway" => $runway["runway_number"],
			                    "length" => $runway["length"],
				    			"crosswind" => $runway["crosswind"],
			                    "headwind" => $runway["headwind"]);

		    }
		}

		
		return $info;
	}
	
	protected function do_crosswind(&$runway, $wind) {
		$number = substr($runway["runway_number"],0,2)."0";				
		$direction = $wind["angle"];
		$speed = $wind["speed"];				
		
		$xw = $speed * sin(($direction - $number)*.0174);

		$runway["crosswind1"] = $xw;
		
		$left_right = $direction - $number;
		$diff = abs($left_right);
		$p = .0174*$diff;
		
		$q = round(abs($speed*sin($p)),1);
		$m = round(abs($speed*cos($p)),1);
		
		$runway["crosswind2"] = $q;

		if ($left_right <0) {
  		   $runway["xw_direction"] =  "left";
		} else if ($left_right > 0) {
  		   $runway["xw_direction"] =  "right";
		} else if ($left_right == 0 || $left_right == 360 || 
		           $left_right == 180) {
		   $runway["xw_direction"] = "none";
	   }

		if ($diff < 90 || $diff > 270) {
		    $runway["xw_head"] = "headwind";
		} else {
		    $runway["xw_head"] = "tailwind";
	   }

      $runway["crosswind"] = $q."kt ".$runway["xw_direction"];
      $runway["headwind"] = $m."kt ".$runway["xw_head"];
	
		return $xw;
	}
	

	public function get_metar($station, &$wxInfo) {

		// This function retrieves METAR information for a given station from the
		// National Weather Service. It assumes that the station exists.
		// A slower URL is "ftp://weather.noaa.gov/data/observations/metar/stations/$station.TXT"
		$fileName = "http://weather.noaa.gov/pub/data/observations/metar/stations/$station.TXT";
		$metar = '';
		
		
		$proxy = $GLOBALS["config"]->get("proxy");
    	if (!empty($proxy)) {
    		$context = stream_context_create(array("http" => array("proxy" => "tcp://".$proxy)));
    	} else {
    		$context = null;
    	}						
		
		$fileData = @file($fileName, 0, $context);  // or die('Data not available');
		if ($fileData != false) {
			list($i, $date) = each($fileData);
			$utc = strtotime(trim($date));
			$this->set_time_data($utc,$wxInfo);
			while (list($i, $line) = each($fileData)) {
				$metar .= ' ' . trim($line);
			}
			$metar = trim(str_replace('  ', ' ', $metar));
		}

		$wxInfo["metar"] = $metar;

		return $metar;
	}

	public function get_taf($station) {

		// This function retrieves TAF information for a given station from the
		// National Weather Service. It assumes that the station exists.
		// A slower URL is "ftp://weather.noaa.gov/data/forecasts/taf/stations/$station.TXT"
		$fileName = "http://weather.noaa.gov/pub/data/forecasts/taf/stations/$station.TXT";
		$taf = '';
		
		
		$proxy = $GLOBALS["config"]->get("proxy");
    	if (!empty($proxy)) {
    		$context = stream_context_create(array("http" => array("proxy" => "tcp://".$proxy)));
    	} else {
    		$context = null;
    	}						
		
		$fileData = @file($fileName, 0, $context);  // or die('Data not available');
		if ($fileData != false) {
			list($i, $date) = each($fileData);
			$utc = strtotime(trim($date));
			$this->set_time_data($utc,$wxInfo);
			while (list($i, $line) = each($fileData)) {
				//$taf .= ' ' . trim($line);
				$taf .= ' ' . $line;
			}
			$taf = trim(str_replace('  ', ' ', $taf));
		   return $taf;
		} else {
		   return null;
		}

	}

	protected function set_time_data($utc, &$wxInfo) {
		// This function formats observation time in the local time zone of server, the
		// current local time on server, and time difference since observation. $utc is a
		// UNIX timestamp for Universal Coordinated Time (Greenwich Mean Time or Zulu Time).
		$timeZoneOffset = date('Z');
		$local = $utc + $timeZoneOffset;
		$wxInfo['Observed'] = date('D M j, H:i T',$local);
		$now = time();
		$wxInfo['Now'] = date('D M j, H:i T',$now);
		$timeDiff = floor(($now - $local) / 60);
		if ($timeDiff < 91) $wxInfo['Age'] = "$timeDiff min";
		else {
			$min = $timeDiff % 60;
			if ($min < 10) $min = '0' . $min;
			$wxInfo['Age'] = floor($timeDiff / 60) . ":$min hr";
		}
	}

	public function process_metar($metar, &$wxInfo) {
		//   This function directs the examination of each group of the METAR. The problem
		// with a METAR is that not all the groups have to be there. Some groups could be
		// missing. Fortunately, the groups must be in a specific order. (This function
		// also assumes that a METAR is well-formed, that is, no typographical mistakes.)
		//   This function uses a function variable to organize the sequence in which to
		// decode each group. Each function checks to see if it can decode the current
		// METAR part. If not, then the group pointer is advanced for the next function
		// to try. If yes, the function decodes that part of the METAR and advances the
		// METAR pointer and group pointer. (If the function can be called again to
		// decode similar information, then the group pointer does not get advanced.)
		if ($metar != '') {
			$metarParts = explode(' ',$metar);
			$groupName = array('get_station','get_time','get_station_type','get_wind','get_var_wind','get_visibility','get_runway','get_conditions','get_cloud_cover','get_temperature','get_altimeter');
			$metarPtr = 1;  // get_station identity is ignored
			$group = 1;
			while ($group < count($groupName)) {
				$part = $metarParts[$metarPtr];
				call_user_func_array(array($this, $groupName[$group]), array($part,&$metarPtr,&$group,&$wxInfo));
				//$groupName[$group]($part,$metarPtr,$group,$wxInfo);  // $groupName is a function variable
			}
		}
		else $wxInfo['ERROR'] = 'Data not available';
	}

	protected function get_station($part, &$metarPtr, &$group, &$wxInfo) {
		// Ignore station code. Script assumes this matches requesting $station.
		// This function is never called. It is here for completeness of documentation.
		if (strlen($part) == 4 and $group == 0) {
			$group++;
			$metarPtr++;
		}
	}

	protected function get_time($part, &$metarPtr, &$group, &$wxInfo) {
		// Ignore observation time. This information is found in the first line of the NWS file.
		// Format is ddhhmmZ where dd = day, hh = hours, mm = minutes in UTC time.
		if (substr($part,-1) == 'Z') $metarPtr++;
		$group++;
	}

	protected function get_station_type($part, &$metarPtr, &$group, &$wxInfo) {
		// Ignore station type if present.
		if ($part == 'AUTO' || $part == 'COR') $metarPtr++;
		$group++;
	}

	protected function speed($part, $unit) {
			// Convert wind speed into miles per hour.
			// Some other common conversion factors (to 6 significant digits):
			//   1 mi/hr = 1.15080 knots  = 0.621371 km/hr = 2.23694 m/s
			//   1 ft/s  = 1.68781 knots  = 0.911344 km/hr = 3.28084 m/s
			//   1 knot  = 0.539957 km/hr = 1.94384 m/s
			//   1 km/hr = 1.852 knots    = 3.6 m/s
			//   1 m/s   = 0.514444 knots = 0.277778 km/s
			//if ($unit == 'KT') $speed = round(1.1508 * $part);         // from knots
			if ($unit == 'KT') $speed = $part;         // from knots
			elseif ($unit == 'MPS') $speed = round(2.23694 * $part);   // from meters per second
			else $speed = round(0.621371 * $part);                     // from km per hour
			//$speed = "$speed mph";
			return $speed;
	}

	protected function get_wind($part, &$metarPtr, &$group, &$wxInfo) {
		// Decodes wind direction and speed information.
		// Format is dddssKT where ddd = degrees from North, ss = speed, KT for knots,
		// or dddssGggKT where G stands for gust and gg = gust speed. (ss or gg can be a 3-digit number.)
		// KT can be replaced with MPH for meters per second or KMH for kilometers per hour.


		if (preg_match('/^([0-9G]{5,10}|VRB[0-9]{2,3})(KT|MPS|KMH)$/',$part,$pieces)) {
			$part = $pieces[1];
			//$unit = $pieces[2];
			$unit = $pieces[2];
			//var_dump($pieces);
			//var_dump($unit);
			if ($part == '00000') {
				$wxInfo['WIND'] = 'calm';  // no wind
				$wxInfo['Wind']["human"] = 'calm';
				$wxInfo['Wind']["speed"] = 0;
				$wxInfo['Wind']["angle"] = 0;
				$wxInfo['Wind']["gust"] = 0;				
			}
			else {
				preg_match('/([0-9]{3}|VRB)([0-9]{2,3})G?([0-9]{2,3})?/',$part,$pieces);
				if ($pieces[1] == 'VRB') {
					$direction = 'varies';
					$angle = 'VRB';
				}	else {
					$angle = (integer) $pieces[1];
					$compass = array('N','NNE','NE','ENE','E','ESE','SE','SSE','S','SSW','SW','WSW','W','WNW','NW','NNW');
					$direction = $compass[round($angle / 22.5) % 16];
				}
				if ($pieces[3] == 0) { 
					$gust = '';
					$wxInfo['Wind']["gust"] = 0;
				} else {
					$spd = $this->speed($pieces[3], $unit);
//					$gust = ', gusting to ' . $spd;
					$wxInfo['Wind']["gust"] = $spd;
				}
				$wxInfo['WIND'] = $direction . ' at ' . $this->speed($pieces[2], $unit) ." ".$unit. $gust;
				$wxInfo['Wind']['human'] = $direction . ' at ' . $this->speed($pieces[2], $unit) ." ".$unit. $gust;
				$wxInfo['Wind']["speed"] = $pieces[2];
				$wxInfo['Wind']["angle"] = $angle;
			}
			$metarPtr++;
		}
		$group++;
	}

	protected function get_var_wind($part, &$metarPtr, &$group, &$wxInfo) {
		// Ignore variable wind direction information if present.
		// Format is fffVttt where V stands for varies from fff degrees to ttt degrees.
		if (preg_match('/([0-9]{3})V([0-9]{3})/',$part,$pieces)) $metarPtr++;
		$group++;
	}

	protected function get_visibility($part, &$metarPtr, &$group, &$wxInfo) {
		// Decodes visibility information. This function will be called a second time
		// if visibility is limited to an integer mile plus a fraction part.
		// Format is mmSM for mm = statute miles, or m n/dSM for m = mile and n/d = fraction of a mile,
		// or just a 4-digit number nnnn (with leading zeros) for nnnn = meters.
		static $integerMile = '';
		if (strlen($part) == 1) {  // visibility is limited to a whole mile plus a fraction part
			$integerMile = $part . ' ';
			$metarPtr++;
			$wxInfo["vis_distance"] = $integerMile;
		} else if (substr($part,-2) == 'SM') {  // visibility is in miles
			$part = substr($part,0,strlen($part)-2);
			if (substr($part,0,1) == 'M') {
				$prefix = 'less than ';
				$part = substr($part, 1);
			} else {
				$prefix = '';
			}
			if (($integerMile == '' && preg_match('/[\/]/',$part,$pieces)) || $part == '1') {
				$unit = ' mile';
			} else {
				$unit = ' miles';
			}
			$wxInfo['Visibility'] = $prefix . $integerMile . $part . $unit;
			$metarPtr++;
			$group++;

			$wxInfo["vis_distance"] = $integerMile.$part;
			
		} else if (substr($part,-2) == 'KM') {  // unknown (Reported by NFFN in Fiji)
			$metarPtr++;
			$group++;
			
		} elseif (preg_match('/^([0-9]{4})$/',$part,$pieces)) {  // visibility is in meters
			$distance = round($part/ 621.4, 1);          // convert to miles
			if ($distance > 5) $distance = round($distance);
			if ($distance <= 1) $unit = ' mile';
			else $unit = ' miles';
			$wxInfo['Visibility'] = $distance . $unit;
			$metarPtr++;
			$group++;

			$wxInfo["vis_distance"] = $distance;
			
		} else if ($part == 'CAVOK') {  // good weather
			$wxInfo['Visibility'] = 'greater than 7 miles';  // or 10 km
			$wxInfo['Conditions'] = '';
			$wxInfo['Clouds'] = 'clear skies';
			$metarPtr++;
			$group += 4;  // can skip the next 3 groups

			//just need to mark this as > 5
			$wxInfo["vis_distance"] = 10;
			
		} else {
			$group++;
		}
	}

	protected function get_runway($part, &$metarPtr, &$group, &$wxInfo) {
		// Ignore runway information if present. Maybe called a second time.
		// Format is Rrrr/vvvvFT where rrr = runway number and vvvv = visibility in feet.
		if (substr($part,0,1) == 'R') {
			$metarPtr++;
		} else {
			$group++;
		}
	}

	protected function get_conditions($part, &$metarPtr, &$group, &$wxInfo) {
		// Decodes current weather conditions. This function maybe called several times
		// to decode all conditions. To learn more about weather condition codes, visit section
		// 12.6.8 - Present Weather Group of the Federal Meteorological Handbook No. 1 at
		// www.nws.noaa.gov/oso/oso1/oso12/fmh1/fmh1ch12.htm
		static $conditions = '';
		static $wxCode = array(
		'VC' => 'nearby',
		'MI' => 'shallow',
		'PR' => 'partial',
		'BC' => 'patches of',
		'DR' => 'low drifting',
		'BL' => 'blowing',
		'SH' => 'showers',
		'TS' => 'thunderstorm',
		'FZ' => 'freezing',
		'DZ' => 'drizzle',
		'RA' => 'rain',
		'SN' => 'snow',
		'SG' => 'snow grains',
		'IC' => 'ice crystals',
		'PE' => 'ice pellets',
		'GR' => 'hail',
		'GS' => 'small hail',  // and/or snow pellets
		'UP' => 'unknown',
		'BR' => 'mist',
		'FG' => 'fog',
		'FU' => 'smoke',
		'VA' => 'volcanic ash',
		'DU' => 'widespread dust',
		'SA' => 'sand',
		'HZ' => 'haze',
		'PY' => 'spray',
		'PO' => 'well-developed dust/sand whirls',
		'SQ' => 'squalls',
		'FC' => 'funnel cloud, tornado, or waterspout',
		'SS' => 'sandstorm/duststorm');
		if (preg_match('/^(-|\+|VC)?(TS|SH|FZ|BL|DR|MI|BC|PR|RA|DZ|SN|SG|GR|GS|PE|IC|UP|BR|FG|FU|VA|DU|SA|HZ|PY|PO|SQ|FC|SS|DS)+$/',$part,$pieces)) {
			if (strlen($conditions) == 0) $join = '';
			else $join = ' &amp; ';
			if (substr($part,0,1) == '-') {
				$prefix = 'light ';
				$part = substr($part,1);
			}
			elseif (substr($part,0,1) == '+') {
				$prefix = 'heavy ';
				$part = substr($part,1);
			}
			else $prefix = '';  // moderate conditions have no descriptor
			$conditions .= $join . $prefix;
			// The 'showers' code 'SH' is moved behind the next 2-letter code to make the English translation read better.
			if (substr($part,0,2) == 'SH') $part = substr($part,2,2) . substr($part,0,2). substr($part, 4);
			while ($code = substr($part,0,2)) {
				$conditions .= $wxCode[$code] . ' ';
				$part = substr($part,2);
			}
			$wxInfo['Conditions'] = $conditions;
			$metarPtr++;
		}
		else {
			$wxInfo['Conditions'] = $conditions;
			$group++;
		}
	}

	protected function get_cloud_cover($part, &$metarPtr, &$group, &$wxInfo) {
		// Decodes cloud cover information. This function maybe called several times
		// to decode all cloud layer observations. Only the last layer is saved.
		// Format is SKC or CLR for clear skies, or cccnnn where ccc = 3-letter code and
		// nnn = altitude of cloud layer in hundreds of feet. 'VV' seems to be used for
		// very low cloud layers. (Other conversion factor: 1 m = 3.28084 ft)
		static $cloudCode = array(
		'SKC' => 'clear skies',
		'CLR' => 'clear skies',
		'FEW' => 'partly cloudy',
		'SCT' => 'scattered clouds',
		'BKN' => 'mostly cloudy',
		'OVC' => 'overcast',
		'VV'  => 'vertical visibility');
		if ($part == 'SKC' || $part == 'CLR') {
			$wxInfo['Clouds'] = $cloudCode[$part];
			$this->add_cloud_entry($part, $cloudCode[$part], "", $wxInfo);
			$metarPtr++;
			$group++;
		}
		else {
			if (preg_match('/^([A-Z]{2,3})([0-9]{3})/',$part,$pieces)) {  // codes for CB and TCU are ignored
				$wxInfo['Clouds'] = $cloudCode[$pieces[1]];
//				if ($pieces[1] == 'VV') {
					$altitude = (integer) 100 * $pieces[2];  // units are feet
					$wxInfo['Clouds'] .= " at $altitude ft";
//				}
				$code = substr($part,0,2);
				if ($code != "VV") {
					$code = substr($part,0,3);
				}
				$this->add_cloud_entry($code, $clodeCode[$part], $altitude, $wxInfo);
				$metarPtr++;
			}
			else {
				$group++;
			}
		}
	}

	private function add_cloud_entry($code, $human, $altitude, &$wxInfo) {
			$newEntry =  array("code" => $code, "human" => $human, "altitude" => $altitude);
			$wxInfo['CloudArr'][] = $newEntry;
			if (!isset($wxInfo['lowestClouds'])) {
				$wxInfo['lowestClouds'] = $newEntry;
			} else if (!empty($altitude)) {
				if ($wxInfo['lowestClouds']['altitude'] > $altitude) {
					$wxInfo['lowestClouds'] = $newEntry;
				}
			}
	}


	protected function get_heat_index($tempF, $rh, &$wxInfo) {
			// Calculate Heat Index based on temperature in F and relative humidity (65 = 65%)
			if ($tempF > 79 && $rh > 39) {
				$hiF = -42.379 + 2.04901523 * $tempF + 10.14333127 * $rh - 0.22475541 * $tempF * $rh;
				$hiF += -0.00683783 * pow($tempF, 2) - 0.05481717 * pow($rh, 2);
				$hiF += 0.00122874 * pow($tempF, 2) * $rh + 0.00085282 * $tempF * pow($rh, 2);
				$hiF += -0.00000199 * pow($tempF, 2) * pow($rh, 2);
				$hiF = round($hiF);
				$hiC = round(($hiF - 32) / 1.8);
				$wxInfo['HeatIndex'] = "$hiC&deg;C ($hiF&deg;F)";
			}
	}

	protected function get_wind_chill($tempF, &$wxInfo) {
			// Calculate Wind Chill Temperature based on temperature in F and
			// wind speed in miles per hour
			if ($tempF < 51 && $wxInfo['WIND'] != 'calm') {
				$pieces = explode(' ', $wxInfo['WIND']);
				$windspeed = (integer) $pieces[2];   // wind speed must be in miles per hour
				if ($windspeed > 3) {
					$chillF = 35.74 + 0.6215 * $tempF - 35.75 * pow($windspeed, 0.16) + 0.4275 * $tempF * pow($windspeed, 0.16);
					$chillF = round($chillF);
					$chillC = round(($chillF - 32) / 1.8);
					$wxInfo['WindChill'] = "$chillC&deg;C ($chillF&deg;F)";
				}
			}
	}

	protected function get_temperature($part, &$metarPtr, &$group, &$wxInfo) {
		// Decodes temperature and dew point information. Relative humidity is calculated. Also,
		// depending on the temperature, Heat Index or Wind Chill Temperature is calculated.
		// Format is tt/dd where tt = temperature and dd = dew point temperature. All units are
		// in Celsius. A 'M' preceeding the tt or dd indicates a negative temperature. Some
		// stations do not report dew point, so the format is tt/ or tt/XX.



		if (preg_match('/^(M?[0-9]{2})\/(M?[0-9]{2}|[X]{2})?$/',$part,$pieces)) {
			$tempC = (integer) strtr($pieces[1], 'M', '-');
			$tempF = round(1.8 * $tempC + 32);
			$wxInfo['Temperature'] = "$tempC&deg;C ($tempF&deg;F)";
			$wxInfo['TempC'] = $tempC;
			$wxInfo['TempF'] = $tempF;
			$this->get_wind_chill($tempF, $wxInfo);
			if (strlen($pieces[2]) != 0 && $pieces[2] != 'XX') {
				$dewC = (integer) strtr($pieces[2], 'M', '-');
				$dewF = round(1.8 * $dewC + 32);
				$wxInfo['Dewpoint'] = "$dewC&deg;C ($dewF&deg;F)";
				$wxInfo['DewpointC'] = $dewC;
				$wxInfo['DewpointF'] = $dewF;
				$rh = round(100 * pow((112 - (0.1 * $tempC) + $dewC) / (112 + (0.9 * $tempC)), 8));
				$wxInfo['Humidity'] = $rh . '%';
				$this->get_heat_index($tempF, $rh, $wxInfo);
			}
			$metarPtr++;
			$group++;
		}
		else {
			$group++;
		}
	}

	protected function get_altimeter($part, &$metarPtr, &$group, &$wxInfo) {
		// Decodes altimeter or barometer information.
		// Format is Annnn where nnnn represents a real number as nn.nn in inches of Hg,
		// or Qpppp where pppp = hectoPascals.
		// Some other common conversion factors:
		//   1 millibar = 1 hPa
		//   1 in Hg = 0.02953 hPa
		//   1 mm Hg = 25.4 in Hg = 0.750062 hPa
		//   1 lb/sq in = 0.491154 in Hg = 0.014504 hPa
		//   1 atm = 0.33421 in Hg = 0.0009869 hPa
		if (preg_match('/^(A|Q)([0-9]{4})/',$part,$pieces)) {
			if ($pieces[1] == 'A') {
				$pressureIN = substr($pieces[2],0,2) . '.' . substr($pieces[2],2);  // units are inches Hg
				$pressureHPA = round($pressureIN / 0.02953);                        // convert to hectoPascals
			}
			else {
				$pressureHPA = (integer) $pieces[2];              // units are hectoPascals
				$pressureIN = round(0.02953 * $pressureHPA,2);    // convert to inches Hg
			}
			$wxInfo['Barometer'] = "$pressureHPA hPa ($pressureIN in Hg)";
			$wxInfo['BaroHg'] = $pressureIN;
			$wxInfo['BaroHPA'] = $pressureHPA;
			$metarPtr++;
			$group++;
		}
		else {
			$group++;
		}
	}
	
	
	
 	protected function get_airport_info($icao) {
 		
 		$info = null;
 		//they will fail always unless the icao is 4 chars...lame.
 		if (strlen($icao) == 4) { 			
 			//echo "\nfetch $icao";
 			
 			$url = "http://api.itimeteo.com/getAirport.ims?format=json&icao=".strtoupper($icao);

 			$proxy = $GLOBALS["config"]->get("proxy");
 			if (!empty($proxy)) {
 				$context = stream_context_create(array("http" => array("proxy" => "tcp://".$proxy)));
 			} else {
 				$context = null;
 			}
 			 
 			$raw = file_get_contents($url,0, $context);
 			$json = json_decode($raw);
 			//echo "\rfetch $icao ".$json->status;
 			//var_dump($json); 
 			if ($json->Status == 200) {
 				$info = array("Name" => $json->Airport->Name,
 				              "GeonameID" => $json->Airport->GeonameID,
				              "Timezone" => $json->Airport->Timezone,
                              "TimezoneOffset" => $this->get_timezone_offset("GMT",$json->Airport->Timezone),
                              "Latitude" => $json->Airport->Latitude,
 				              "Longitude" => $json->Airport->Longitude);
 				return $info;
 			} else {
 				return null;
 			}
 		} else {
 			return null;
 		}
 	}
 	
 	
 	protected function get_timezone_offset($remote_tz, $origin_tz = null) {
 		if($origin_tz === null) {
 			if(!is_string($origin_tz = date_default_timezone_get())) {
 				return false; // A UTC timestamp was returned -- bail out!
 			}
 		}
 		$origin_dtz = new DateTimeZone($origin_tz);
 		$remote_dtz = new DateTimeZone($remote_tz);
 		$origin_dt = new DateTime("now", $origin_dtz);
 		$remote_dt = new DateTime("now", $remote_dtz);
 		$offset = $origin_dtz->getOffset($origin_dt) - $remote_dtz->getOffset($remote_dt);
 		return $offset/3600;
 	}


	protected function pressure_altitude($pressure, $altitude) {
	    // Pressure Altitude
		 // Palt = ( 1 - (P / 1013.25) ^ .190284) * 145366.45

	    $Pst = 1013.25;
		 $Pexp = .190284;
		 $fn = 145366.45;

		 $tmp1 = $pressure/$Pst;
		 $tmp2 = pow($tmp1, $Pexp);

		 $Pa = $fn * (1 - pow(($pressure/$Pst) , $Pexp));

		 return round($Pa + $altitude,1);
	}


	
	
   private function vapor_pressure($dewC) {
   		$Vpexp = (7.5*$dewC) / (237.7+$dewC);
		$Vp = 6.11*pow(10, $Vpexp);	
		return $Vp;
   }

   // Calculate absolute air pressure given the barometric pressure(mb) and altitude(meters)
	private function calcAbsPress($Pmb, $altM) {
		$k1 = 0.190284;
		$k2 = 8.4288*Math.pow(10,-5);
		$p1 = pow($Pmb,$k1);
		$p2 = $altM*$k2;
		$p3 = 0.3+pow( ($p1-$p2), (1/$k1) );
		return $p3;
	}
 
 
	//  Calculate the air density in kg/m3
	private function calcDensity($absPmb, $e, $tc) {
		$Rv=461.4964;
		$Rd=287.0531;
	
		$tk = $tc + 273.15;
		$pv = $e*100;
		$pd = ($absPmb-$e)*100;
		$d= ($pv/($Rv*$tk)) + ($pd/($Rd*$tk));
		return $d;
	}
	
	// Calculate the ISA altitude (meters) for a given density (kg/m3)
	private function calcAltitude($d) {
		$g=9.80665;
		$Po=101325;
		$To=288.15;
		$L=6.5;
		$R=8.314320;
		$M=28.9644;

		$D=$d*1000;

		$p2=( ($L*$R)/($g*$M-$L*$R) )*log( ($R*$To*$D)/($M*$Po) );

		$H=-($To/$L)*( exp($p2)-1 );

		$h = $H*1000;

		return $h;
	}



	// Calculate the Z altitude (meters), given the H altitide (meters)
	private function calcZ($h) {
		$r=6369E3;

		return (($r*$h)/($r-$h));

	}


	// Calculate the H altitude (meters), given the Z altitide (meters)
	private function calcH($z) {
		$r=6369E3;

		return (($r*$z)/($r+$z));
	}

	
	// Calculate the actual pressure (mb)from the altimeter setting (mb) and geopotential altitude (m)
	private function calcAs2Press($As, $h) {
		$k1=.190263;
		$k2=8.417286E-5;

		$p=pow( (pow($As,$k1)-($k2*$h)),(1/$k1) );

		return $p;
	}
	
	
	private function density_altitude($altitude, $Pmb, $PinHg, $tempC, $dewC) {

		$in_per_mb = (1/33.86389);		
		$mb_per_in = 33.86389;
				
		$m_per_ft = 0.304800;
		$ft_per_m = (1/.304800);
		

		$zm = $altitude*$m_per_ft;


		// Calculate the vapor pressures (mb) given the ambient temperature (c) and dewpoint (c)

		$esmb = $this->vapor_pressure($tempC);
		$emb = $this->vapor_pressure($dewC);


		// Calculate geopotential altitude H (m) from geometric altitude (m) Z

		$hm = $this->calcH($zm);


		// Calculate the absolute pressure given the altimeter setting(mb) and geopotential elevation(meters)

		$actpressmb = $this->calcAs2Press($Pmb, $hm);


		// Calculate the air density (kg/m3) from absolute pressure (mb) vapor pressure (mb) and temp (c)

		$density = $this->calcDensity($actpressmb, $emb, $tempC);
		$relden = 100*($density/1.225);


		// Calculate the geopotential altitude (m) in ISA with that same density (kg/m3)

		$densaltm = $this->calcAltitude($density);

		// Calculate geometric altitude Z (m) from geopotential altitude (m) H

		$densaltzm = $this->calcZ($densaltm);


		// Convert Units for output

		$actpress = $actpressmb*$in_per_mb;
		$densalt = $densaltzm*$ft_per_m;

		if ( $densalt > 36090 || $densalt < -15000 )	{
		   Log::singleton()->err("AviationWeather::density_altitude($altitude, $Pmb, $PinHg, $tempC, $dewC)  --  Out of range for Troposhere Algorithm: Altitude =" . round($densalt,0) . " feet" );
			return null;
		}
		
		return round($densalt,0);

//		var_dump("Absolute Pressure ".round($actpress,3));
//		var_dump("Density altitude ".round($densalt,0));
//		var_dump("Relative Density ".round($relden,2));

	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	



 	
   protected function do_images($icao) {
	   $airport_images = array();
		$airportdiagram = "";

	   //first lets see if we can find some airport images.
      $flyagogo_url =  'http://flyagogo.net/uploads/';

		for ($x=1; $x<=3; $x++) {
		    $result = $this->http_head_curl($flyagogo_url.$icao."/".$x.".jpg");
			 if ($result !== false && $result["http_code"] == 200) {
			     $airport_images[] = $result["url"];
		    } else {
			     //if n doesn't work then n+1 won't either
			     break;
		    }
		}

		//lets see if we can snag an airport diagram
		$page = $this->http_get_curl("http://airnav.com/airport/".$icao);
		if ($page !== false) {
		    //first snag the airport image itself.
		    $str = strstr($page,"http://img.airnav.com/ap/");
			 if ($str) {
			     $image_url = substr($str,0,strpos($str, '"'));
				  $airport_images[] = $image_url;
			 }


			 //now the airport diagram.
		    $str = strstr($page,"http://img.airnav.com/aptdiag");
			 if ($str) {
			     $airportdiagram = substr($str,0,strpos($str, '"'));
			 }
		}


		$images = array("images" => $airport_images, "diagram" => $airportdiagram);
		return $images;
	}

	/**
	* @return boolean false on error, content otherwise
	* @param  string $url
	* @desc   makes a HEAD request using cURL
	* @author Svetoslav Marinov
	* @link http://devquickref.com/
	*/
	function http_head_curl($url) {
		  if (!extension_loaded('curl_init') || !function_exists('curl_init')) {
				$ch = curl_init();
            curl_setopt($ch, CURLOPT_URL, $url);
			   curl_setopt($ch, CURLOPT_TIMEOUT, 5); // times out after 30s
			   curl_setopt($ch, CURLOPT_HEADER, 1);
			   curl_setopt($ch, CURLOPT_NOBODY, 1);
			   curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
			   curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
			   $result = curl_exec($ch);
				$info = curl_getinfo($ch);
			   curl_close($ch);
			   return $info;
			}
			return false;
	}


	function http_get_curl($url) {
		  if (!extension_loaded('curl_init') || !function_exists('curl_init')) {
				$ch = curl_init();
            curl_setopt($ch, CURLOPT_URL, $url);
			   curl_setopt($ch, CURLOPT_TIMEOUT, 2); // times out after 30s
			   curl_setopt($ch, CURLOPT_HEADER, 1);
			   curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
			   curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
			   $result = curl_exec($ch);
				$info = curl_getinfo($ch);
			   curl_close($ch);
				if ($info["http_code"] == 200) {
				    return $result;
            } else {
				    return false;
		      }
			}
			return false;
	}



	/**
	 * decide what meteorological conditions 
	 * the airport is in.
	 *
	 * @see http://aviationweather.gov/adds/metars/description_ifr.php
	 */
	function decide_vfr(&$wxInfo) {
		//first lets try the easy approach.
		//if we have > 5 miles visibility
		if ($wxInfo["vis_distance"] >= 5) {
			//clouds need to be > 3000 AGL
			if ($wxInfo["lowestClouds"]["altitude"] >= 3000 ||
				( $wxInfo["lowestClouds"]["altitude"] < 3000 &&
				  ($wxInfo["lowestClouds"]["code"] == "CLR" ||
				   $wxInfo["lowestClouds"]["code"] == "SKC" ||
				   $wxInfo["lowestClouds"]["code"] == "FEW" ||
				   $wxInfo["lowestClouds"]["code"] == "SCT"))
			   ) {
				$wxInfo["SkyConditions"] = "VFR";
			} else {
				$wxInfo["SkyConditions"] = "MVFR";
			}

		} else if ($wxInfo["vis_distance"] >=3) {
			//clounds between 1000' and 3000'
			if ($wxInfo["lowestClouds"]["altitude"] >= 1000 ||
				( $wxInfo["lowestClouds"]["altitude"] < 1000 && 
				  ( $wxInfo["lowestClouds"]["code"] == "CLR" ||
				    $wxInfo["lowestClouds"]["code"] == "SKC" ||
				    $wxInfo["lowestClouds"]["code"] == "FEW" ||
				    $wxInfo["lowestClouds"]["code"] == "SCT"))
				  ) {
				$wxInfo["SkyConditions"] = "MVFR";
			} else {
				$wxInfo["SkyConditions"] = "IFR";
			}

		} else if ($wxInfo["vis_distance"] >=1) {
		    //clouds between 500' and 1000'	
			if ($wxInfo["lowestClouds"]["altitude"] >= 500) {
				$wxInfo["SkyConditions"] = "IFR";
			} else {
				$wxInfo["SkyConditions"] = "LIFR";
            }


		} else if ($wxInfo["vis_distance"] <1) {
			//clouds < 500'
			if ($wxInfo["lowestClouds"]["altitude"] < 500) {
				$wxInfo["SkyConditions"] = "LIFR";
			} else {
				$wxInfo["SkyConditions"] = "IFR";
            }
		}
	}
					
}

?>
