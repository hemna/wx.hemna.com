<?php

class WXConversion {
	
	/**
	 * mulitply miles per hour by this
	 * to get knots.
	 *
	 * @var float
	 */
	const MPHKNOTS =  0.868976242;
    
    /**
     * This method converts Fahrenheit to
     * Celsius
     *
     * @param integer $temp
     * @return float
     */
    public static function fahrenheit_to_celsius($temp, $format="%.1f") {
        return (float)sprintf($format,(($temp - 32) / 1.8));         
    }
    
    /**
     * This method converts Celsius to Fahrenheit
     *
     * @param float $temp
     * @return float
     */
    public static function celsius_to_fahrenheit($temp, $format="%.1f") {
        return (float)sprintf($format,(($temp * 1.8) + 32));        
    }
    
    /**
     * This method converts Celsius to Kelvin
     * 
     * @param float $temp
     * @return float
     */
    public static function celsius_to_kelvin($temp, $format="%.2f") {
        return (float)sprintf($format, ($temp + 273.15));
    }
    
    /**
     * This method converts Kelvin to Celsius
     * 
     * @param float $temp
     * @return float
     */
    public static function kelvin_to_celsius($temp, $format="%.2f") {
        return (float)sprintf($format, (273.15 - $temp));        
    }
    
    /**
     * This method converts Fahrenheit to Kelvin
     * 
     * @param float $temp
     * @return float
     */
    public static function fahrenheit_to_kelvin($temp, $format="%.1f") {
        return self::celsius_to_kelvin(self::fahrenheit_to_celsius($temp, $format));
    }
    
    /**
     * This method converts Kelvin to Fahrenheit
     * 
     * @package float $temp
     * @return float
     */
    public static function kelvin_to_fahrenheit($temp, $format="%.1f") {
        return self::celsius_to_fahrenheit(self::kelvin_to_celsius($temp, $format));
    }
    
    
    /**
     * This method converts inches of Mercury to Millibars
     * 
     * @param float $pressure
     * @return float
     */
    public static function inchmercury_to_millibars($pressure, $format="%.2f") {
        return (float)sprintf($format, ($pressure * 33.8638));        
    }
    
    /**
     * This method converts millibars to inches of mercury
     * 
     * @param float $pressure
     * @return float
     *
     * @param unknown_type $pressure
     */
    public static function millibars_to_inchmercury($pressure, $format="%.2f") {
        return (float)sprintf($format, ($pressure / 33.8638));        
    }
    
    /**
     * Convert inches of mercury to atmospheres
     * 
     * @param float $pressure
     * @return float
     */
    public static function inchmercury_to_atmospheres($pressure, $format="%.2f") {
        return sprintf($format, ($pressure * .0334211));
    }
    
    /**
     * Convert atmospheres to inches of mercury
     * 
     * @param float $pressure
     * @return float
     */
    public static function atmospheres_to_inchmercury($pressure, $format="%.2f") {
        return (float)sprintf($format, ($pressure / .0334211));
    }
    
    
    /**
     * This method converts mph to knots
     * 
     * @param float $mph
     * @return float
     */
    public static function mph_to_knots($mph) {
    	return (float)sprintf("%.2f", ($mph*WXConversion::MPHKNOTS));
    }
    
    
    
    
}

?>