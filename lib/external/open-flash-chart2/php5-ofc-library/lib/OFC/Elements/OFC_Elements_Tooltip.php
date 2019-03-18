<?php

require_once('OFC/Elements/OFC_Elements_Base.php');

class OFC_Elements_Tooltip extends OFC_Elements_Base {

	public function __construct() {
		parent::__construct();		
		$this->type = 'tooltip';
	}

	public function set_shadow( $shadow )
	{
		$this->shadow = $shadow;
	}
	
	// stroke in pixels (e.g. 5 )
	public function set_stroke( $stroke )
	{
		$this->stroke = $stroke;
	}
	
	public function set_colour( $colour )
	{
		$this->colour = $colour;
	}
	
	public function set_background_colour( $bg )
	{
		$this->background = $bg;
	}
	
	// a css style
	public function set_title_style( $style )
	{
		$this->title = $style;
	}
	
    public function set_body_style( $style )
	{
		$this->body = $style;
	}
	
	public function set_proximity()
	{
		$this->mouse = 1;
	}
	
	public function set_hover()
	{
		$this->mouse = 2;
	}
}
?>