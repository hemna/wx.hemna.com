<?php
/**
 * This file contains the Metar conditions
 * for our local area.
 */
 

/**
 * We need to do this due to cross domain js issues.
 * We can't make a direct js ajax call to the url, unless
 * we are running in the same domain as the url.
 * 
 * @author waboring
 *
 */
class RemoteMetarConditions extends JSONWidget {
		
	const TOD_MORNING = "morning";
	const TOD_MIDDAY = "midday";
	const TOD_AFTERNOON = "afternoon";
	const TOD_EVENING = "evening";
	const TOD_NIGHT = "night";
	
	
	public static $tod_lookup = array(0=> RemoteMetarConditions::TOD_NIGHT,
	                                  1=>RemoteMetarConditions::TOD_MORNING,
	                                  2=>RemoteMetarConditions::TOD_MIDDAY,
	                                  3=>RemoteMetarConditions::TOD_AFTERNOON,
	                                  4=>RemoteMetarConditions::TOD_EVENING);
	                                  
	public static $cloud_lookup = array(0 => "clear",
	                                    1 => "few",
	                                    2 => "scattered",
	                                    3 => "broken",
	                                    4 => "overcast");	                                  
	
	const ID = "rmetar";
    
//    protected $url="http://api.itimeteo.com/metar.ims?icao=kpvf&format=json&decoded=true";
    protected $url="http://api.itimeteo.com/getMetar.ims?format=json&icao=kpvf&decoded=true";

    /**
     * Class Constructor
     *
     */
    public function init() {    	
        
    }
    
    
    
    public function build_object() {

    	$proxy = $GLOBALS["config"]->get("proxy");
    	if (!empty($proxy)) {
    		$context = stream_context_create(array("http" => array("proxy" => "tcp://".$proxy)));
    	} else {
    		$context = null;
    	}
    	
    	$json = file_get_contents($this->url,0, $context);
    	//Log::singleton()->debug("json returned is ".$json);
    	$metar = json_decode($json);
    	//echo var_export($metar, true)."\n";
    	$metar->season = "Spring";
    	//Log::singleton()->debug(var_export($metar, true));

    	$conditions = array();
		$conditions["metar"] = $metar->Metar->Report;
    	$conditions["season"] = $GLOBALS["config"]->get("season");    	
    	$clouds = $this->decode_clouds($metar->Metar->DecodeLine0->Clouds);    	
    	$conditions["clouds"] = $clouds;
    	$rained = $this->has_rained(); 
    	$conditions["rained"] = $rained;   	        	
    	$conditions = $this->get_banner_filename($conditions, $rained);
    	
    	return $conditions;
    	//return $metar;
    }
    
    private function decode_clouds($cloudVar, $offset=null) {
    	$clouds = 0;
   
        if (!$cloudVar) {
            return RemoteMetarConditions::$cloud_lookup[$clouds];
        }
     
    	if ($cloudVar->Overcast) {
    		$clouds = 4;
    	} else if ($cloudVar->Broken) {
    		$clouds = 3;
    	} else if ($cloudVar->Scattered) {
    		$clouds = 2;
    	} else if ($cloudVar->Few) {
    		$clouds = 1;
    	} else {
    		$clouds = 0;
    	}
    	
    	if (!is_null($offset)) {
    		$clouds += $offset;
    		
    		if ($clouds > 4) {
    			$clouds = 0;
    		} else if ($clouds < 0) {
    			$clouds = 4;
    		}
    	}
    	
    	return RemoteMetarConditions::$cloud_lookup[$clouds];
    }
    
    private function get_time_of_day($offset=null) {
    	$hour = date("G");
    	$tod = null;
    	
    	if ($hour == 00 || $hour > 0 && $hour <6) {
    		$tod = 0;
    	} else if ($hour >= 6 && $hour <11) {
    		$tod = 1;
    	} else if ($hour >=11 && $hour < 16 ) {
    		$tod = 2;
    	} else if ($hour >=16 && $hour <19) {
    		$tod = 3;
    	} else if ($hour >=19 && $hour <24) {
    		$tod = 4;
    	}
    	
    	if (!is_null($offset)) {
    		$tod += $offset;
    		echo "tod = ".$tod."    ";
    		
    		if ($tod > 4) {
    			$tod = 0;
    		} else if ($tod < 0) {
    			$tod = 4;
    		}   

    		echo "tod2 = ".$tod."\n";
    	}
    	
    	return RemoteMetarConditions::$tod_lookup[$tod];
    }
    
    
    private function get_banner_filename($conditions, $rain, $offset=null) {
    	$season = $conditions["season"];
    	$clouds = $conditions["clouds"];
    	$tod = $this->get_time_of_day($offset);
        //echo (var_export($tod, true)."\n");
        //echo (var_export($conditions, true));
    	$filename = "/images/banners/".$season."/".$tod."_";
    	
    	$conditions["tod"] = $tod;
    	
    	if ($rain) {
    		$filename .= "rain.jpg";
    		$conditions["wx"] = "rain";
    	} else {
    		$filename .= $clouds.".jpg";
    		$conditions["wx"] = $clouds;
    	}
    	
    	$conditions["filename"] = "http://".$GLOBALS["config"]->get("sitename").$filename;
        if (file_exists($GLOBALS['path_base']."/htdocs".$filename)) {
    		$conditions["banner"] = "yes";
    	} else {
    		$conditions["banner"] = "no";
    		Log::singleton()->debug("Need banner image ".$filename);
//    		if (!is_null($offset)) {
//    			$offset += 1;
//    		} else {
//    			$offset = 1;
//    		}
//    		return $this->get_banner_filename($season, $clouds, $rain, $offset);    		    		
    	}
    	return $conditions;
    }
    
    /**
     * Has it rained in the last 2 hours?
     * If so, we are in "rain" mode.
     * 
     * @return unknown_type
     */
    private function has_rained() {
    	$db = open2300DB::singleton();    	
    	$today = date("Y-m-d");
    	
    	$sql = "select rain_1h from weather where datetime like :date order by datetime desc limit 0,1";
    	$stmt = $db->queryBindOneRow($sql, array(":date" => $today."%"));
    	
    	//echo var_export($stmt->rain_1h,true)."\n";
    	
    	if ( !is_null($stmt) && $stmt ) {
    		$rain = $stmt->rain_1h;
    		if ($rain > 0) {
    			return true;
    		} else {
    			return false;
    		}    	
    	} else {
    		return false;
    	}    	        	
    }
}
?>
