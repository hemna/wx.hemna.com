<?php
/**
 * This file is responsible for displaying the 
 * weather on a specific date (in the past).
 * 
 */


class WxDate extends open2300Page {
	
	private $wx = null;
	private $wxdate = null;
	
	protected $centering_width = "810px";
	
    function __construct() {
        parent::__construct('Hemna WX Station');

        $this->wxdate = Request::singleton()->get("date", date("Y-m-d"));
        //make sure we have a valid date here
    	if (empty($this->wxdate) || is_null($this->wxdate)) {
        	$this->wxdate = date("Y-m-d");
        }
    }

    function head_content() {

        $this->add_css_link("/css/main.css");
        $this->add_css_link("/css/menu.css");
        $this->add_css_link("/css/fonts.css");

        $this->add_js_link("http://ajax.googleapis.com/ajax/libs/jquery/2.1.0/jquery.min.js");
        $this->add_js_link("/js/highcharts/js/highcharts.js");
        $this->add_js_link("/js/hc.js");
        $this->add_js_link("/js/main.js");
        $this->add_js_link("/js/ticker.js");
        $this->add_js_link("/js/banner.js");
    }

    protected function left_block() {
    	global $config;
    	
    	$div = new DIVtag(array("style" => "margin-top:0px; width: 320px;"), 
    	                  new WxDateCalendar(),
    	                  BRtag::factory(2),
    	                  new WxMonthData());
    	                  
		$div->add(BRtag::factory(2),new TimeLapse());
		    	                  
    	return $div;    	
    }
    
	/**
     * build the main content for the page
     * and return it all inside a container object
     *
     * @param Container object
     */
    function content_block() {        
        //add some graphs
        $right_div = new DIVtag(array('id' => ''));
        $right_div->add(new DIVtag(
            array('id' => 'idTempDewPointLineGraphdiv',
            'style'=> 'padding-top:0px;padding-bottom:5px;height:200px;width:800px;')
        ));
        $right_div->add(new TempDewPointHighChartsGraph($this->wxdate));
        $right_div->add(new DIVtag(
            array('id' => 'idInsideTempLineGraphdiv',
            'style'=> 'padding-top:0px;padding-bottom:5px;height:200px;width:800px;')
        ));
        $right_div->add(new InsideTempHighChartsGraph($this->wxdate));

        $right_div->add(new DIVtag(
            array('id' => 'idPressureLineGraphdiv',
            'style'=> 'padding-top:0px;padding-bottom:5px;height:200px;width:800px;')
        ));
        $right_div->add(new PressureHighChartsGraph($this->wxdate));

        $right_div->add(new DIVtag(
            array('id' => 'idWindLineGraphdiv',
            'style'=> 'padding-top:0px;padding-bottom:5px;height:200px;width:800px;')
        ));
        $right_div->add(new WindSpeedHighChartsGraph($this->wxdate));

        $right_div->add(new DIVtag(
            array('id' => 'idRainLineGraphdiv',
            'style'=> 'padding-top:0px;padding-bottom:5px;height:200px;width:800px;')
        ));
        $right_div->add(new RainHighChartsGraph($this->wxdate));
        
        $this->add_js_link('/js/lightview/lightview.js');
		$this->add_css_link('/css/lightview/lightview.css');

		$c = new Container();
		
		$c->add( //H3tag::factory("Weather on ".date("l M d, Y", strtotime($this->wxdate))),		           
		         //Brtag::factory(1), 
		         $right_div );
		         
		return $c;
        
//        $table = TABLEtag::factory("100%",0,0,0);        
//        $table->add_row(new TDtag(array('style' => 'vertical-align:top;width:510px;'),$right_div));
//        return $table;
    }

	 function byte_size($bytes)  { 
      $size = $bytes / 1024; 
      if ($size < 1024) { 
			$size = number_format($size, 2); 
	      $size .= ' KB'; 
	   }  else  { 
        if ($size / 1024 < 1024)  { 
            $size = number_format($size / 1024, 2); 
	         $size .= ' MB'; 
	     } else if ($size / 1024 / 1024 < 1024) { 
           $size = number_format($size / 1024 / 1024, 2); 
           $size .= ' GB'; 
        }  
      } 
      return $size; 
	 } 
    
    public function get_cache_key() {
        return get_class($this).$this->wxdate;
    }

    public function get_cache_lifetime() {
        return 60;
    }
}
?>
