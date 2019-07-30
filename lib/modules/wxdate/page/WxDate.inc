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
        $this->add_css_link("https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.8.0/Chart.css");

        $this->add_js_link("http://ajax.googleapis.com/ajax/libs/jquery/2.1.0/jquery.min.js");
        $this->add_js_link("https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.8.0/Chart.bundle.min.js");
	$this->add_js_link("https://cdnjs.cloudflare.com/ajax/libs/highcharts/4.0.1/highcharts.js");
	$this->add_js_link("https://cdnjs.cloudflare.com/ajax/libs/highcharts/4.0.1/highcharts-more.js");
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

        $right_div->add(
            new DIVtag(
                array('style'=> 'padding-top:0px;padding-bottom:0px;margin-bottom:5px;'.
                                'height:200px;width:510px;border: 1px solid black; background-color: #eeeeee;'),
                new CANVAStag(
                    array('id' => 'idTempDewPoint', 'width' => '510', 'height' => '200')
                )
            ));

        $right_div->add(new TempDewPointChartsJSGraph($this->wxdate));

        $right_div->add(
            new DIVtag(
                array('style'=> 'padding-top:0px;padding-bottom:0px;margin-bottom:5px;'.
                                'height:200px;width:510px;border:1px solid black; background-color: #eeeeee;'),
                new CANVAStag(
                    array('id' => 'idInsideTemp', 'width' => '510', 'height' => '200')
                )
            ));
        $right_div->add(new InsideTempChartsJSGraph($this->wxdate));

        $right_div->add(
            new DIVtag(
                array('style'=> 'padding-top:0px;padding-bottom:0px;margin-bottom:5px;'.
                                'height:200px;width:510px;border:1px solid black; background-color: #eeeeee;'),
                new CANVAStag(
                    array('id' => 'idPressure', 'width' => '510', 'height' => '200')
                )
            ));
	$right_div->add(new PressureChartsJSGraph($this->wxdate));

        $right_div->add(
            new DIVtag(
                array('style'=> 'padding-top:0px;padding-bottom:0px;margin-bottom:5px;'.
                                'height:200px;width:510px;border:1px solid black; background-color: #eeeeee;'),
                new CANVAStag(
                    array('id' => 'idWindSpeed', 'width' => '510', 'height' => '200')
                )
            ));
        $right_div->add(new WindSpeedChartsJSGraph($this->wxdate));

        $right_div->add(
            new DIVtag(
                array('style'=> 'padding-top:0px;padding-bottom:0px;margin-bottom:5px;'.
                                'height:200px;width:510px;border:1px solid black; background-color: #eeeeee;'),
                new CANVAStag(
                    array('id' => 'idRainLineGraph', 'width' => '510', 'height' => '200')
                )
            ));
        $right_div->add(new RainChartsJSGraph($this->wxdate));
        
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
