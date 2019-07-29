<?php

/**
 * This is the project generated home page
 *
 *
 * @package open2300
 *
 */


class TestPage extends open2300Page {

    private $wx = null;

    protected $centering_width = "1400px";


    function __construct() {
        parent::__construct('Hemna WX Station');
        $this->include_ticker = FALSE;
    }

    public function request_vars() {
        return array();
    }


    function head_content() {
        $this->add_css_link("/css/main.css");
        $this->add_css_link("/css/menu.css");
        $this->add_css_link("/css/fonts.css");
        //$this->add_css_link("/css/daily-weather-graph.css");

        //$this->add_css_link("https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.8.0/Chart.css");

        //$this->add_js_link("http://ajax.googleapis.com/ajax/libs/jquery/2.1.0/jquery.min.js");
        //$this->add_js_link("https://cdn.jsdelivr.net/gh/jquery/jquery@3/dist/jquery.min.js");
        //$this->add_js_link("https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.8.0/Chart.bundle.min.js");
        //$this->add_js_link("https://www.chartjs.org/samples/latest/utils.js");
        $this->add_js_link("https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.24.0/moment.min.js");
        $this->add_js_link("https://www.chartjs.org/dist/2.8.0/Chart.min.js");
        //$this->add_js_link("/js/main.js");
        //$this->add_js_link("/js/ticker.js");
        //$this->add_js_link("/js/banner.js");
        //$this->add_js_link( 'http://d3js.org/d3.v5.min.js' );
        //$this->add_js_link( '/js/daily-weather-graph.min.js' );
    }

    /**
     *
     */
	function left_block() {

		$container = Container::factory();
        //$this->add_head_js("$(window).load( start_hc );");
        //$this->add_head_js("$(window).load( main_init );");
        //$this->add_head_js("$(window).load( start_updates );");
        //$this->add_head_js("$(window).load( start_ticker );");
        //$this->add_head_js("$(window).load( start_banner );");

		$left_div = new DIVtag(array('id'=> ''));

        $left_div->add(new DIVtag(array('style' => "width: 320px;"),new CurrentConditions()));
        $left_div->add(BRtag::factory());

        $container->add($left_div);

        return $container;
    }


    /**
     * build the main content for the page
     * and return it all inside a container object
     *
     * @param Container object
     */
    function content_block() {
        //add some graphs
        $right_div = new DIVtag(array('id' => 'idMainContent'));

        /*$right_div->add(new DIVtag(
            array('id' => 'idDailyWXGraph')
        ));
        $right_div->add(new DailyWXGraph(20));
         */

        $right_div->add(
            new DIVtag(
                array('style'=> 'padding-top:0px;padding-bottom:0px;margin-bottom:5px;'.
                                'height:200px;width:510px;border: 1px solid black; background-color: #eeeeee;'),
                new CANVAStag(
                    array('id' => 'idTempDewPoint', 'width' => '510', 'height' => '200')
                )
            ));

        $right_div->add(new TempDewPointChartsJSGraph());

        $right_div->add(
            new DIVtag(
                array('style'=> 'padding-top:0px;padding-bottom:0px;margin-bottom:5px;'.
                                'height:200px;width:510px;border:1px solid black; background-color: #eeeeee;'),
                new CANVAStag(
                    array('id' => 'idInsideTemp', 'width' => '510', 'height' => '200')
                )
            ));
        $right_div->add(new InsideTempChartsJSGraph());

        $right_div->add(
            new DIVtag(
                array('style'=> 'padding-top:0px;padding-bottom:0px;margin-bottom:5px;'.
                                'height:200px;width:510px;border:1px solid black; background-color: #eeeeee;'),
                new CANVAStag(
                    array('id' => 'idPressure', 'width' => '510', 'height' => '200')
                )
            ));
        $right_div->add(new PressureChartsJSGraph());

        $table = TABLEtag::factory("100%",0,0,0);
        $table->add_row(new TDtag(array('style' => 'vertical-align:top;width:510px;'),$right_div),
        			    new TDtag(array('style' => 'vertical-align:top;padding-left:8px;'),
				                  new WebCam()));
        return $table;
    }

}
?>
