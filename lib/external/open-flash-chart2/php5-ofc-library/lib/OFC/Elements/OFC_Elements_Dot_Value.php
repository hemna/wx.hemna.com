<?php

require_once('OFC/Elements/OFC_Elements_Base.php');
require_once('OFC/Elements/OFC_Elements_Tooltip.php');

class OFC_Elements_Dot_Value extends OFC_Elements_Base {

	/**
     * Constructor of the dot value
     * 
     * @param $value
     * @param $colour
     */
	public function __construct($value) {
        parent::__construct();
        
        $this->value = $value;
	}	
	
	
	/**
	 * Set the color of the value.
	 * 
	 * @param $colour in hex
	 */
	public function set_colour( $colour )	{
		$this->colour = $colour;
	}
	
	/**
	 * Set the size of the value.
	 * 
	 * @param $size in hex
	 */
	public function set_size( $size )	{
		$this->size = $size;
	}
	
	/**
	 * Set the tooltip.
	 * 
	 * @param $tooltip
	 */
	public function set_tooltip( $tip ) {
		$this->tip = $tip;
	}
}
?>