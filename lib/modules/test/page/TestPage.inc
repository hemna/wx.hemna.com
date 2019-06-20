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
        $this->wx = weatherDataObject::find("1=1 order by datetime desc limit 0,1");
        $this->include_ticker = FALSE;
    }

    public function request_vars() {
        return array();
    }


    function head_content() {
        $this->add_css_link("/css/main.css");
        $this->add_css_link("/css/menu.css");
        $this->add_css_link("/css/fonts.css");
        $this->add_css_link("/css/daily-weather-graph.css");
        $this->add_css_link("https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.8.0/Chart.css");

        //$this->add_js_link("http://ajax.googleapis.com/ajax/libs/jquery/2.1.0/jquery.min.js");
        $this->add_js_link("https://cdn.jsdelivr.net/gh/jquery/jquery@3/dist/jquery.min.js");
        $this->add_js_link("https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.8.0/Chart.bundle.min.js");
        $this->add_js_link("/js/hc.js");
        $this->add_js_link("/js/main.js");
        $this->add_js_link("/js/ticker.js");
        $this->add_js_link("/js/banner.js");
        $this->add_js_link( 'http://d3js.org/d3.v5.min.js' );
        $this->add_js_link( '/js/daily-weather-graph.min.js' );
    }

    /**
     *
     */
	function left_block() {

		$container = Container::factory();
        $this->add_head_js("$(window).load( start_hc );");
        $this->add_head_js("$(window).load( main_init );");
        $this->add_head_js("$(window).load( start_updates );");
        $this->add_head_js("$(window).load( start_ticker );");
        $this->add_head_js("$(window).load( start_banner );");

        //$this->add_js_link("/js/cal.js");

		$left_div = new DIVtag(array('id'=> ''));

        $left_div->add(new DIVtag(array('style' => "width: 320px;"),new CurrentConditions($this->wx)));
        $left_div->add(BRtag::factory());
        $winddiv = new DIVtag(array("id" => "idWindContainer",
                                    "style" => "position:relative;height:200px;-moz-border-radius: 5px; -webkit-border-radius: 5px;border:1px solid blue;padding:5px;"));
        $winddiv->add(new DIVtag(
            array('id' => 'idWindDirGaugediv',
            'style'=> 'float:left;padding-top:0px;padding-bottom:5px;height:200px;width:150px;')
        ));
        $winddiv->add(new WindDirectionHighChartsGauge());
        $winddiv->add(new DIVtag(
            array('id' => 'idWindSpeedGaugediv',
            'style'=> 'float:right;padding-top:0px;padding-bottom:5px;height:200px;width:150px;')
        ));
        $winddiv->add(new WindSpeedHighChartsGauge());
        $left_div->add($winddiv);
        $left_div->add(BRtag::factory());

        $curdate = date("Ymd");
        $video = "<video width=\"320\" height=\"240\" controls><source src=\"video/".$curdate."/timelapse.webm\" type=\"video/webm\"></video>";
        $video_div = new DIVtag(array('id'=> 'idTimelapseWebM',
                                      "style"=>"position:absolute"),
                                $video);
        //$left_div->add($video_div);
        $left_div->add(new TimeLapse());
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

        $right_div->add(new DIVtag(
            array('id' => 'idDailyWXGraph')
        ));
        $right_div->add(new DailyWXGraph(20));

        $right_div->add(new DIVtag(
            array('id' => 'idTestGraphdiv',
            'style'=> 'padding-top:0px;padding-bottom:5px;height:200px;width:510px;')
        ));
        $right_div->add(new TestChartsGraph());


        //$this->add_js_link('/js/lightview/lightview.js');
		//$this->add_css_link('/css/lightview/lightview.css');

		$this->add_js_link('http://yui.yahooapis.com/combo?2.8.0r4/build/yahoo-dom-event/yahoo-dom-event.js&2.8.0r4/build/calendar/calendar-min.js');
		$this->add_css_link('http://yui.yahooapis.com/combo?2.8.0r4/build/calendar/assets/skins/sam/calendar.css');

        $sat_img = IMGtag::factory("http://images.intellicast.com/WxImages/SatelliteLoop/hiatlsat_None_anim.gif",
                                   480, 320);
        $sat_img->set_id('idSatImg');
        $sat_div = new DIVtag(array('id' => 'idSatDiv'), $sat_img);

        $radar_img = IMGtag::factory("https://radblast.wunderground.com/cgi-bin/radar/WUNIDS_map?station=FCX&brand=wui&num=6&delay=15&type=N0R&frame=0&scale=0.5&noclutter=0&showstorms=0&mapx=400&mapy=240&centerx=170&centery=316&transx=-230&transy=76&showlabels=1&severe=0&rainsnow=0&lightning=1&smooth=0&rand=25471752&lat=37.34&lon=-78.83&label=Appomattox",
                                     480,320);
        $radar_img->set_id('idRadarImg');
        $radar_div = new DIVtag(array('id' => 'idRadardiv'),$radar_img);

		$radar2_img = IMGtag::factory("http://sirocco.accuweather.com/sat_mosaic_640x480_public/rs/isarcac.gif", 480,320);
		$radar2_img->set_id('idRadar2Img');
		$radar2_div = new DIVtag(array('id' => 'idRadar2div'), $radar2_img);

        $table = TABLEtag::factory("100%",0,0,0);
        $table->add_row(new TDtag(array('style' => 'vertical-align:top;width:510px;'),$right_div),
        			    new TDtag(array('style' => 'vertical-align:top;padding-left:8px;'),
				                  new WebCam(), $sat_div, $radar_div, $radar2_div));
        return $table;
    }

}
?>
