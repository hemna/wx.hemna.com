<?php
$lib_path = realpath('../lib');
ini_set('include_path', '.:/usr/local/lib:'.$lib_path);

define('PHPHTMLLIB', realpath('../lib/external/phphtmllib'));
$GLOBALS['path_base'] = realpath('..');

// autoload function for all our classes
require($GLOBALS['path_base'].'/lib/autoload.inc');

// setup error handling and required parameters
require($GLOBALS['path_base'].'/lib/init.inc');

$GLOBALS['config']->set('uncaught_exception_output', 'text');

/**
 * This file is used to parse the x-plane apt.parse file.
 * 
 */
 
// class airport { 
// 	 	
// 	//columns from header id 1
// 	//land = 0 sea = 1 heliport = 2
// 	public $type = 0;
// 	
// 	//feet
// 	public $elevation;
// 	
// 	//0 for no, 1 for yes
// 	public $control_tower; 	
// 	public $icao; 	
// 	public $name; 	 	 	 
//
// 	//public $runways = array();
// 	//public $helipads = array(); 	
// 	//public $comms = array();
// 	//public $lighting = array(); 	 	 	 	 	 	 	 	 	
// }
// 
// class runway {
// 	//columns from header id 100
// 	//land runways 	
// 	public $runway_number;
// 	
// 	//in meters
// 	public $runway_width;
// 	
// 	//1 asphault, 2 concrete, 3, turf/grass, 
// 	//4 dirt(brown), 5 gravel(grey), 12 dark lakebed
// 	//13 water runways, 14 snow/ice, 15 transparent.
// 	public $runway_surface_type = 1;
// 	
// 	//0 no shoulder, 1=asphault 2=concrete
// 	public $runway_shoulder_type = 0;
// 	
// 	//0.00 smooth  1.00 rough
// 	public $runway_smoothness = 0.25;
// 	
// 	//0 no lights, 1 center line lights.
// 	public $runway_center_lights = 0;
// 	
// 	//0 no lights, 2 medium intensity edge lights
// 	public $runway_edge_lights = 0;
// 	
// 	//following fields are repeated for each end of runway. 	
// }
// 
// class helipad {
// 	
// }
// 
// class comm {
// 	public $type;
// 	public $frequency;
// 	public $name; 	
// }
// 
// class lighting {
// 	public $type;
// 	public $type_code;
// 	public $orientation;
// 	
// 	public $glidescope_angle;
// 	public $associated_runway;
// 	
// 	public $latitude;
// 	public $longitude;
// 	public $description;
// }
 
 class apt_parse {
 	
 	protected $file = null; 
 	protected $filename = null;	
 	protected $data = array();
 	
 	protected $airports = array();
 	
 	
 	function __construct($file) {
	 	//open the file, throw exception
	 	$this->filename = $file;
	 	if (!file_exists($file)) { 		 		 		
		 	throw new Exception("File doesn't exist' ".$file);
	 	}
 	}
 	
 	
 	public function parse () {
 		//read one line..get the code call code parser.
 		$lines = file($this->filename);
 		$cache = AirportCache::singleton();
 		
 		echo "Number of lines = ". var_export(count($lines), true)."\n\n";
 		
 		$airport = array();
 		$x = 1;
 		foreach ($lines as $line_num => $line) {
 			 			
 			if (ord($line) == 10) {
 				//we have a newline
 				//which means we are done with
 				//current airport.
 				//echo "newline\n";
 				$this->airports[] = $airport;
 				//get more information
				if (isset($airport["icao"])) {
					$pos = stripos($airport["icao"], 'K');
					if ($pos !== false && $pos == 0) {
						$tmp = $cache->get($airport["icao"]);
						if (is_null(@$tmp["images"]) || !is_array(@$tmp["images"]) || !is_array(@$tmp["images"]["images"]) ||
						    count(@$tmp["images"]["images"]) <= 0) {
							var_dump(@$tmp["images"]);
							echo "\rFetching(".$x.") ".$airport["icao"]."\n";
							$airport = $this->do_images($airport);
							var_dump(@$airport["images"]);
							$cache->set($airport["icao"], $airport);
						}
					} 				    
				} else {
 				    //var_dump($airport);
				    //exit;
				}
 				if ($x % 1000 == 0) {
 					echo  "\rNumber of Airports = ".$x." - ".$airport["icao"] ."  -  ".$airport["runways"][0]["runway_threshold_longitude"]." , ".$airport["runways"][0]["runway_threshold_latitude"];
				}
 				$airport = array();
 				$x++;
 				
 			} else { 				
 				$airport = $this->parse_line($line, $airport);
 			} 		
 			 				 			
 		}
 		
 		echo "\nNumber of Airports = ".var_export(count($this->airports), true);
 		return $this->airports;  			 		
 	}  
 	
 	protected function clean_line($line) {
 		$line_arr = explode(" ", $line);
 		$new_arr = array();
 		$len = count($line_arr);
 		for ($x=0; $x<=$len-1; $x++) {
 			if ($line_arr[$x] != "") {
 				$new_arr[] = $line_arr[$x]; 
 			}
 		}
 		
 		return $new_arr; 		 		
 	}
 	
 	
 	public function parse_land_header($line_arr, $airport) {
 		//var_dump($line_arr);		
 		$airport["elevation"] = $line_arr[1];
 		$airport["control_tower"] = $line_arr[2];
 		//skip 3 (deprecated)
 		$airport["icao"] = $line_arr[4];
 		if (isset($line_arr[5])) {
 			$name = array_splice($line_arr,5);	
 			$airport["name"] = trim(implode(" ", $name)); 
 		}
 		
 		return $airport;
 	}
 	
 	public function parse_seaplane_header($line_arr, $airport) {
 		
 		
 		return $airport;
 	}
 	
 	public function parse_helipad_header($line_arr, $airport) {
 		
 		
 		return $airport;
 	}
 	
 	public function parse_land_runway($line_arr, $airport) { 		
 		//var_dump($line_arr);
 		$runway = array();
 		$runway["runway_width"] = $line_arr[1];
 		$runway["runway_surface_type"] = $line_arr[2];
 		$runway["runway_shoulder_type"] = $line_arr[3];
 		$runway["runway_smoothness"] = $line_arr[4];
 		$runway["runway_center_lights"] = $line_arr[5];
 		$runway["runway_edge_lights"] = $line_arr[6];
 		
 		$runway["runway_number"] = $line_arr[8];
 		$runway["runway_threshold_latitude"] = $line_arr[9];
 		$runway["runway_threshold_longitude"] = $line_arr[10];
 		$runway["length_displaced_threshold"] = $line_arr[11];
 		$runway["length_overrun"] = $line_arr[12];
 		$runway["markings_code"] = $line_arr[13];
 		$runway["lighting_code"] = $line_arr[14];
 		$runway["touchdown_zone_lighting_code"] = $line_arr[15];
 		$runway["reil_code"] = $line_arr[16]; 
 		 		
 		$runway2 = $runway; 		 
 		$runway2["runway_number"] = $line_arr[17];
 		$runway2["runway_threshold_latitude"] = $line_arr[18];
 		$runway2["runway_threshold_longitude"] = $line_arr[19];
 		$runway2["length_displaced_threshold"] = $line_arr[20];
 		$runway2["length_overrun"] = $line_arr[21];
 		$runway2["markings_code"] = $line_arr[22];
 		$runway2["lighting_code"] = $line_arr[23];
 		$runway2["touchdown_zone_lighting_code"] = $line_arr[24];
 		$runway2["reil_code"] = $line_arr[25]; 		 		 		

 		$distance = $this->runway_distance($runway["runway_threshold_latitude"],
 		                                   $runway["runway_threshold_longitude"],
 		                                   $runway2["runway_threshold_latitude"],
 		                                   $runway2["runway_threshold_longitude"]);
		settype($distance, "integer"); 		                                   
		//var_dump($distance); 		                                   
		$runway["length"] = $distance;
		$runway2["length"] = $distance;
		 		                                  
 		$airport["runways"][] = $runway;
 		$airport["runways"][] = $runway2;
 		return $airport;
 	} 
 	
 	public function runway_distance($lat1, $long1, $lat2, $long2) {
 		return $this->distance($lat1, $long1, $lat2, $long2, "f");
 	}
 	
 	public function distance($lat1, $lon1, $lat2, $lon2, $unit) { 
	 	
	 	$theta = $lon1 - $lon2; 
	 	$dist = sin(deg2rad($lat1)) * sin(deg2rad($lat2)) +  cos(deg2rad($lat1)) * cos(deg2rad($lat2)) * cos(deg2rad($theta)); 
	 	$dist = acos($dist); 
	 	$dist = rad2deg($dist); 
	 	$miles = $dist * 60 * 1.1515;
	 	$unit = strtoupper($unit);	 	
	 	
	 	if ($unit == "K") {
		 	return round(($miles * 1.609344),1); 
	 	} else if ($unit == "N") {
		 	return round(($miles * 0.8684),1);
	 	} else if ($unit == "F") {
	 		return round($miles*5280,1);
	 	} else {	 		
		 	return round($miles,1);
	 	}
 	}
 	
 	public function parse_water_runway($line_arr, $airport) {
 		//var_dump($line_arr);
 		$runway = array();
 		$runway["runway_width"] = $line_arr[1];
 		$runway["edge_buoys"] = $line_arr[2];
 		
 		$runway["runway_number"] = $line_arr[3];
 		$runway["runway_threshold_latitude"] = $line_arr[4];
 		$runway["runway_threshold_longitude"] = $line_arr[5];
 		
 		return $airport;
 	}
 	
 	public function parse_helipad($line_arr, $airport) {
 		//var_dump($line_arr);
 		$runway = array();
 		$runway["runway_width"] = $line_arr[1];
 		$runway["edge_buoys"] = $line_arr[2];
 		
 		return $airport;
 	}
 	
 	public function parse_lighting($line_arr, $airport) {
 		$light = array();
 		$light["latitude"] = $line_arr[1];
 		$light["longitude"] = $line_arr[2]; 		
 		$light["type"] = $this->light_code($line_arr[3]);
 		$light["type_code"] = $line_arr[3];
 		
 		$light["orientation"] = $line_arr[4];
 		$light["glidescope_angle"] = $line_arr[5];
 		$light["associated_runway"] = $line_arr[6];
 		$light["description"] = @$line_arr[7]; 
 		
 		$airport["lights"][] = $light;		 
 		
 		return $airport;
 	}
 	
 	public function light_code($code) {
 		switch($code) {
 			case "1":
 				return "VASI";
 				break;
 			case "2":
 				return "PAPI-4L, on left of runway";
 				break;
 			case "3":
 				return "PAPI-4R, on right of runway";
 				break;
 			case "4":
 				return "Space Shuttle PAPI, 20 degree glidepath";
 				break;
 			case "5":
 				return "Tri-colour VASI";
 				break;
 			case "6":
 				return "Runway guard (“wig-wag”) lights";
 				break; 			
 		}
 	}
 	
 	public function parse_atc_recorded($line_arr, $airport) {
 		
 		
 		return $airport;
 	}
 	
 	public function parse_comm($line_arr, $airport) {
 		
 		$comm = array();
 		$comm["type"] = $this->comm_type($line_arr[0]);
 		$comm["type_code"] = $line_arr[0]; 
 		$comm["frequency"] = $line_arr[1];
 		$comm["name"] = $line_arr[2];
 		
 		$airport["comms"][] = $comm;
 		
 		return $airport;
 	} 	
 	
 	public function comm_type($type) {
 		switch ($type) {
 			case "51":		//ATC-UNICOM, CTAF
 				return "UNICOM";
 				break;
 				
 			case "52":		//ATC-CLD Clearance Delivery
 				return "CLD";
 				break;
 				
 			case "53":		//ATC-GND Ground
 				return "Ground";
 				break;
 				 					
 			case "54":		//ATC-Tower
 				return "Tower";
 				break;
 				 					
 			case "55":		//ATC-approach
 				return "ATC Approach";
 				break;
 				 				
 			case "56":		//ATC-Departure
 				return "ATC Departure";
 				break;
 		}
 	}
 	
 	
 	
 	public function parse_line($line, $airport) {
 		if ($line != null && strlen($line) > 0) {
 			$line_arr = $this->clean_line($line); 			
 			
 			switch ($line_arr[0]) {
 				//main header
 				case "1":
 				    //found the airport line.
 				    $airport = $this->parse_land_header($line_arr, $airport);
 				    break;
 				case "16":
 				    $airport = $this->parse_seaplane_header($line_arr, $airport);
 				    break;
 				case "17":
 				    $airport = $this->parse_helipad_header($line_arr, $airport);
 				    break;
 				    
 				//runway/helipad
 				case "100":
 				    $airport = $this->parse_land_runway($line_arr, $airport);
 				    break;
 				    
 				case "101":
 				    $airport = $this->parse_water_runway($line_arr, $airport);
 				    break;
 				    
 				case "102":
 				    $airport = $this->parse_helipad($line_arr, $airport);
 				    break;
 				    
 				    
 				case "21": 		//lighting objects vasi, papi, wig-wags, etc.
 				    $airport = $this->parse_lighting($line_arr, $airport);
 				    break;
 				    
 				case "50":		//ATC recorded (AWOS, ASOS or ATIS)
 					
 					break;
 					
 				case "51":		//ATC-UNICOM, CTAF
 				case "52":		//ATC-CLD Clearance Delivery
 				case "53":		//ATC-GND Ground 					
 				case "54":		//ATC-Tower 					
 				case "55":		//ATC-approach 				
 				case "56":		//ATC-Departure
 					$airport = $this->parse_comm($line_arr, $airport);
 					break;
 				     				    
 				     
 				     
 				//don't care
 				case "110":    //pavement taxiway
 				case "120":    //linear feature
 				case "130":    //airport boundary
 				case "111":    //node plain
 				case "112":
 				case "113":
 				case "114":
 				case "115":
 				case "116":
 				case "14":     //viewpoint
 				case "15":     //startup location includes runup location.
 				case "18":     //light beacon
 				case "19":		//windsock
 				case "20":		//signs (taxiway or runway ) 				
 				default:
 					break;     
 			} 			 			
 		}
 		
 		return $airport; 		 			 		
 	}
 	
 	 	
 	protected function do_images($airport) {
 		$airport_images = array();
 		$airportdiagram = "";
 		$icao = $airport["icao"];
 	
 		//first lets see if we can find some airport images.
 		$flyagogo_url =  'http://flyagogo.net/uploads/';
 	
 		for ($x=1; $x<=3; $x++) {
 			$url = $flyagogo_url.$icao."/".$x.".jpg";
 			$img = @file_get_contents($url);
 			if (!is_null($img) && !empty($img)) {
 				$airport_images[] = $url;
 			} else {
 				break;
 			}
//  			$result = $this->http_head_curl();
//  			if ($result !== false && $result["http_code"] == 200) {
//  				$airport_images[] = $result["url"];
//  			} else {
 				//if n doesn't work then n+1 won't either
//  				break;
//  			}
 		}
 	
 		//lets see if we can snag an airport diagram
 		$url = "http://airnav.com/airport/".$icao;
 		$page = @file_get_contents($url);
 		//$page = $this->http_get_curl($url);
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
 		if (count($airport_images) > 0) {
 			$airport["images"] = $images;
 		}
 		return $airport;
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
 			curl_setopt($ch, CURLOPT_TIMEOUT, 30); // times out after 30s
 			curl_setopt($ch, CURLOPT_HEADER, 1);
 			curl_setopt($ch, CURLOPT_NOBODY, 1);
 			curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
 			curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
 			curl_setopt($ch, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
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
 			curl_setopt($ch, CURLOPT_TIMEOUT, 30); // times out after 30s
 			curl_setopt($ch, CURLOPT_HEADER, 1);
 			curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
 			curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
 			$result = curl_exec($ch);
 			$info = curl_getinfo($ch);
 			curl_close($ch);
 			if ($info["http_code"] == 200) {
 				return $result;
 			} else {
 				echo "Failed to get $url code = ".$info["http_code"];
 				return false;
 			}
 		}
 		return false;
 	}
 }

ini_set('memory_limit', '1024M');
//$parser = new apt_parse("apt_short.dat");
$parser = new apt_parse("apt.dat");
//$parser = new apt_parse("apt_kaun.dat");

$result = $parser->parse();
//echo "\nNumber of airports = ".var_export(count($result), true);
//var_dump(json_encode($result, JSON_FORCE_OBJECT));
//var_dump($result);
//var_dump($result);


?>
