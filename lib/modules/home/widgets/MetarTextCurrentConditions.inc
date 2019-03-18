<?php
/**
 * This file contains the CurrentConditions 
 * widget class.
 */
 


class MetarTextCurrentConditions extends Container {
	
	/**
	 * the object's controller target
	 *
	 */
	const ID="metar";
    
    /**
     * This contains the current
     * weatherDataObject for
     * the conditions that you want to display
     * 
     * @var weatherDataObject
     */
    protected $wx;

    /**
     * Class Constructor
     *
     */
    public function __construct(weatherDataObject $wx=null) {
        parent::__construct();
        if ($wx == null) {
            //we don't have the current conditions.
            //we need to get them
            $this->wx = weatherDataObject::find("1=1 order by datetime desc limit 0,1");    
        } else {
            $this->wx = $wx;
        }
    }
    
    public function render($indent_level=0, $output_debug=0) {    	    	
    	
    	$metar = $GLOBALS['config']->get('station_name');
        
        //get today's High, and today's Low so far.
        $db = open2300DB::singleton();
        $today = date("Y-m-d");
		
        $stmt = $db->queryBindOneRowCache("Select min(temp_out) as low, max(temp_out) as high from weather where datetime like :date", array(":date" => $today.'%'));
        
        //if (class_exists("DateTime")) {
        if (FALSE && $GLOBALS['config']->get('has_date_time', FALSE)) {
        	$cur_date = new DateTime($this->wx->get_datetime());
        	$cur_date->setTimezone(new DateTimeZone("UTC"));
        	$metar .= " ".$cur_date->format("dHi")."Z";
        } else {
        	//try using PEAR's Date object
        	include_once("Date.php");
        	$cur_date = new Date($this->wx->get_datetime());
        	        	
        	$cur_date->convertTZByID("GMT");
        	$metar .= " ".$cur_date->format("%d%H%M")."Z";        	        	        
        }
        
        
        
        $outside_temp = $this->wx->get_temp_out();
        $dewpoint = $this->wx->get_dewpoint();
        $metar .= " AUTO ";
        
        //now do wind
        $wind_dir = $this->wx->get_wind_direction();
        $wind_angle = $this->wx->get_wind_angle();
        if ($wind_angle == 0) {
        	$wind_angle = 360;
        }
        $speed = round($this->wx->get_wind_speed(),0);
        $wind_speed = sprintf("%02.0f",round(WXConversion::mph_to_knots($speed,"%02.0f"),0))."KT ";
        
        $metar .= sprintf("%3.0f",$wind_angle).$wind_speed;
        
        //we don't have sky conditions.
        //this is the best we can do
        $fcast = $this->wx->get_forecast();
        switch ($fcast) {
        	case "Sunny":
        		$metar .= "CLR ";
        		break;
        	case "Cloudy":
        		$metar .= "OVC ";
        		break;
        		
        	case "Rainy":
        		$metar .= "-RA ";
        		break;
        		
        	default:
        		$metar .= "XXX ";
        		break;
        }        
        
        //temperature and dew point
        $metar .= WXConversion::fahrenheit_to_celsius($outside_temp, "%02.0f").'/'.
                  WXConversion::fahrenheit_to_celsius($dewpoint, "%02.0f");               
        
        //now pressure        
        $metar .= " A".$this->wx->get_rel_pressure()*100;
        
        //remarks section
        $metar .= " RMK A02 ";
        //rain in the last hour
        $metar .= sprintf("P%0004.0f",($this->wx->get_rain_1h()*100));
        Log::singleton()->info($metar);
        return $metar;
    }
}
?>
