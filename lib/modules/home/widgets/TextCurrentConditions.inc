<?php
/**
 * This file contains the CurrentConditions 
 * widget class.
 */
 


class TextCurrentConditions extends CurrentConditions {
    
	/**
	 * the object's controller target
	 *
	 */
	const ID="text";
    
    public function get_content() {
        
        //build the title
        //$title = DIVtag::factory('cctitle', Atag::factory(AjaxRequestBuilder::build_replace_url('CurrentConditions', array(), $this->get_ajax_replacement_divid()), 'Current Conditions', 'linkrefresh'));
        //now add the update block
        $this->wx->get_datetime();
       
        //outside temp
        $outside_temp = $this->wx->get_temp_out();
	$output .= " ".$outside_temp." F / ".
                   WXConversion::fahrenheit_to_celsius($outside_temp).' C / '.
                   WXConversion::fahrenheit_to_kelvin($outside_temp). " K " .
                   'Feels like '.$this->wx->get_wind_chill()." F ".
                   $high.' F / '.$low.' F   High/Low ';
                                                              
        $output .= "Relative Humidity : ". $this->wx->get_rel_hum_out().' % ';
        
        //wind
        $output .=  $this->wx->get_wind_direction()." ".$this->wx->get_wind_angle(). "@".$this->wx->get_wind_speed().'mph ';
                                                                                            
        $pressure = $this->wx->get_rel_pressure();         
        $output .= "Pressure : ". $pressure. ' inHG / '.WXConversion::inchmercury_to_millibars($pressure). ' mbar ';

        //rainfall 
        $output .= 'Rainfall : '.$this->wx->get_rain_1h().'" last hour '.$this->wx->get_rain_24h().'" in 24 hours  '.$this->rain_total.'" total. ';
	
        //inside temp 
        $inside_temp = $this->wx->get_temp_in();        
	$output .= " ".$inside_temp." F / ".
                   WXConversion::fahrenheit_to_celsius($inside_temp).' C / '.
                   WXConversion::fahrenheit_to_kelvin($inside_temp). " K ";
        
        return $output;
    }
}
?>
