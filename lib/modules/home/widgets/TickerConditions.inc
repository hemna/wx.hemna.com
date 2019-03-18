<?php
/**
 * This file contains the CurrentConditions 
 * widget class.
 */
 


class TickerConditions extends JSONWidget {
	
	const ID = "ticker";
    
    /**
     * This contains the current
     * weatherDataObject for
     * the conditions that you want to display
     * 
     * @var weatherDataObject
     */
    protected $wx;
    
    protected $rain_total;
    
    protected $temp_low_high = array("low" => 0,
                                     "high" => 0);  

    /**
     * Class Constructor
     *
     */
    public function init() {    	
        $log = Log::singleton();
        $hots = "<span class=\"hot\">";
        $hote = "</span>";
        $colds = "<span class=\"cold\">";
        $colde = "</span>";
        
        
        if ($this->wx == null) {
            //we don't have the current conditions.
            //we need to get them
            $this->wx = weatherDataObject::find("1=1 order by datetime desc limit 0,1");    
        } else {
            $this->wx = $wx;
        }
        
        //load min max temps
        $db = open2300DB::singleton();
        $year = date("Y");
        $month = date("Y-m");
        $today = date("Y-m-d");

        
        //TEMPERATURE
        $sql = "select min(temp_out) as low, ".
               "(select max(datetime) from weather where temp_out=".
                  "(select min(temp_out) from weather where datetime like :date) and datetime like :date)".
               " as datetime from weather where datetime like :date";    
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $today.'%'), 60);
        $this->temp_low_high["low"] = $stmt->low;                                  
        $this->temp_low_high["low_str"] = $colds.$stmt->low. "°F".$colde."  at " . substr($stmt->datetime,10);
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $year.'%'), 60);
        $this->temp_low_high["year_low"] = $stmt->low;
        $this->temp_low_high["year_low_str"] = $colds.$stmt->low. "°F".$colde."  at " . $stmt->datetime;
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $month.'%'), 60);
        $this->temp_low_high["month_low"] = $stmt->low;
        $this->temp_low_high["month_low_str"] = $colds.$stmt->low. "°F".$colde."  at " . $stmt->datetime;
        
        $sql = "select max(temp_out) as high, ".
               "(select max(datetime) from weather where temp_out=".
                  "(select max(temp_out) from weather where datetime like :date) and datetime like :date)".
               " as datetime from weather where datetime like :date";
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $today.'%'), 60);
        $this->temp_low_high["high"] = $stmt->high;
        $this->temp_low_high["high_str"] = $hots.$stmt->high . "°F".$hote."  at " . substr($stmt->datetime,10);
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $year.'%'), 60);
        $this->temp_low_high["year_high"] = $stmt->high;
        $this->temp_low_high["year_high_str"] = $hots.$stmt->high . "°F".$hote."  at " . $stmt->datetime;
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $month.'%'), 60);
        $this->temp_low_high["month_high"] = $stmt->high;
        $this->temp_low_high["month_high_str"] = $hots.$stmt->high . "°F".$hote."  at " . $stmt->datetime;

		//Inside TEMP
        $sql = "select min(temp_in) as low, ".
               "(select max(datetime) from weather where temp_in=".
                  "(select min(temp_in) from weather where datetime like :date) and datetime like :date)".
               " as datetime from weather where datetime like :date";    
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $today.'%'), 60);
        $this->intemp_low_high["low"] = $stmt->low;                                  
        $this->intemp_low_high["low_str"] = $colds.$stmt->low. "°F".$colde."  at " . substr($stmt->datetime,10);
        $sql = "select max(temp_in) as high, ".
               "(select max(datetime) from weather where temp_in=".
                  "(select max(temp_in) from weather where datetime like :date) and datetime like :date)".
               " as datetime from weather where datetime like :date";
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $today.'%'), 60);
        $this->intemp_low_high["high"] = $stmt->high;
        $this->intemp_low_high["high_str"] = $hots.$stmt->high . "°F".$hote."  at " . substr($stmt->datetime,10);
        
        //HUMIDITY
        $sql = "select max(rel_hum_out) as high, ".
               "(select max(datetime) from weather where rel_hum_out=".
                  "(select max(rel_hum_out) from weather where datetime like :date) and datetime like :date)".
               " as datetime from weather where datetime like :date";
        $stmt = $db->queryBindOneRowCache($sql, 
                                          array(":date" => $today.'%'), 60);
                
        $this->temp_low_high["high_hum"] = $stmt->high;
        $this->temp_low_high["high_hum_str"] = $hots.$stmt->high . "%".$hote." at " . substr($stmt->datetime,10);
        
        $sql = "select min(rel_hum_out) as low, ".
               "(select max(datetime) from weather where rel_hum_out=".
                  "(select min(rel_hum_out) from weather where datetime like :date) and datetime like :date)".
               " as datetime from weather where datetime like :date";
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $today.'%'), 60);
                
        $this->temp_low_high["low_hum"] = $stmt->low;
        $this->temp_low_high["low_hum_str"] = $colds.$stmt->low . "%".$colde." at " . substr($stmt->datetime,10);
        
                
        $year = date("Y");
        $stmt = $db->queryBindOneRowCache("Select rain_total from weather where datetime like :date order by datetime asc limit 0,1",
                                          array(":date" => $year.'%'));
        if (!is_null($stmt) && $stmt) {
            $year_rain_start = $stmt->rain_total;
        } else {
            $year_rain_start = "N/A";
        }

        
        $this->rain_total = sprintf("%0.2f", $this->wx->get_rain_total() - $year_rain_start);
        
        
        
        //WIND
        $stmt = $db->queryBindOneRowCache("Select avg(wind_speed) as average from weather where datetime like :date", 
                                          array(":date" => $today.'%'), 60);
        $this->wind['average'] = sprintf("%0.2f",$stmt->average);                                  

        //get the peak winds
        //$db->set_debug_flag(true);
		$sql = "select datetime,wind_speed, wind_angle, wind_direction from weather where ".
               "wind_speed = (select max(wind_speed) from weather where datetime like :date) ".
               "and datetime like :date2";
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $today.'%',":date2" => $today.'%'), 60);
    	$log->debug(var_export($stmt,true));
        if (!is_null($stmt) && $stmt) {
		    $this->peak_wind['speed'] = $stmt->wind_speed;
		    $this->peak_wind['angle'] = $stmt->wind_angle;
		    $this->peak_wind['direction'] = $stmt->wind_direction;
		    $this->peak_wind['time'] = substr($stmt->datetime,10);
		    $log->debug("peak wind was ".$stmt->wind_speed);
        } else {
		    $this->peak_wind['speed'] = 0;
		    $this->peak_wind['angle'] = 0;
		    $this->peak_wind['direction'] = 0;
		    $this->peak_wind['time'] = 0;
        }
    }
    
    
    
    public function build_object() {

    	$outside_temp = $this->wx->get_temp_out();
    	$outside_tempC = WXConversion::fahrenheit_to_celsius($outside_temp);
    	

    	$data = array();
    	//the middle td
    	$data["middle"] = array("Last Update: ".$this->wx->get_datetime(),
    	                     "Low Temp: ".$this->temp_low_high["low_str"],
    	                     "High Temp: ".$this->temp_low_high["high_str"],
    	                     "Low Inside Temp: ".$this->intemp_low_high["low_str"],
    	                     "High Inside Temp: ".$this->intemp_low_high["high_str"],
    	                     "Average Wind: ".$this->wind['average']."mph",
    	                     "Peak Wind ".$this->peak_wind['direction']." ".
    	                                  $this->peak_wind['speed'].'mph@'.$this->peak_wind['angle'].
    	                                  "&nbsp;".$this->peak_wind['time'],
    	                     "Low Humidity: ".$this->temp_low_high["low_hum_str"],
                             "High Humidity: ".$this->temp_low_high["high_hum_str"],
                             "Low for the Month: ".$this->temp_low_high["month_low_str"],
                             "Low for the Year: ".$this->temp_low_high["year_low_str"],
                             "High for the Month: ".$this->temp_low_high["month_high_str"],
                             "High for the Year: ".$this->temp_low_high["year_high_str"]);
    	
    	//the right td
    	$data["right"] = array(date("G:i:s"), 
    	                    $outside_temp."°F / ".$outside_tempC."°C",
    	                    $this->wx->get_wind_direction()." ".$this->wx->get_wind_speed().'mph@'.$this->wx->get_wind_angle(),
    	                    $this->wx->get_rel_pressure()." inHG",
    	                    "Humidity: ".$this->wx->get_rel_hum_out().'%',
    	                    "Rain 1h: ".$this->wx->get_rain_1h(),
    	                    "Rain 24h: ".$this->wx->get_rain_24h(),
    	                    "Rain Year: ".$this->rain_total);    	        
    	                    
    	return $data;    	
    }
}
?>
