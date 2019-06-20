<?php

/**
 * $Id: phphtmllibPage.inc,v 1.8 2005/04/13 19:18:54 waboring Exp $
 *
 * @package open2300
 */


class open2300Page extends AjaxablePage implements Cacheable {

    /**
     * Defines HTML ID for the body
     * div of the page
     *
     */
    const ID_BODY = AjaxablePage::AJAX_ID;

    /**
     * Defines HTML ID for the menu
     * area div
     *
     */
    const ID_MENU = 'idMenu';

    /**
     * Defines HTML ID for the message
     * notification area div
     *
     */
    const ID_MESSAGE = 'idMessage';


    /**
     * where the main content goes
     */
    const ID_CONTENT = 'idMiddleContent';

    /**
     * Where the nav blocks go
     */
    const ID_NAV = 'leftBlock';
    
    protected $centering_width = "500px";
    
    
    protected $include_ticker = true;

    /**
     * This is the constructor.
     *
     * @param string - $title - the title for the page and the
     *                 titlebar object.
     * @param - string - The render type (HTML, XHTML, etc. )
     *
     */
    function __construct($title, $render_type = phphtmllib::HTML) {

        parent::__construct( $title, $render_type );

        //add some meta tags
        $this->add_head_content( html_meta("html, xml, xhtml, php, phphtmllib, open source".
                                           ",LGPL, GPL",
                                           "","keywords"));
    }


    public function build_doctype($link1=NULL, $link2=NULL) {
        $attributes = array("html");
        $this->_doctype = new DOCTYPEtag($attributes);
    }

    /**
     * This function should return an array
     * of request variable names and their
     * properties in the following format:
     * array('key_name' => type)
     *
     * This function is called by the Controller
     * in order to pass it to the Request object
     *
     * @return array
     */
    public function request_vars() {
        return array();
    }

    /**
     * This method is called when we only want to
     * build the content for an ajax request.
     * Instead of building the entire page layout,
     * just build the 'main content' block that will
     * be replaced on the current page.
     *
     * @return string
     */
    protected function render_ajax_content() {
        return $this->content_block()->render();
    }

    /**
     * This is meant to be extended by the child class
     * to do any generic permissions checking for access
     * to the content that the child builds
     *
     * @return boolean - TRUE = has permissions to build
     *                          and view content.
     */
    protected function permission() {
        return true;
    }

    /**
     * The parent class' head_content is defined
     * as abstract, so we need to define it here
     * and return NULL if we don't want to add
     * anything to the head.
     *
     * @return NULL
     */
    protected function head_content() {
        $this->add_css_link("/css/main.css");
        $this->add_css_link("/css/fonts.css");
        $this->add_css_link("/css/menu.css");
        $this->add_css_link("/css/prototip.css");

        //$this->add_js_link( '/js/scriptaculous-js-1.7.0/lib/prototype.js' );
        //$this->add_js_link( '/js/scriptaculous-js-1.7.0/src/scriptaculous.js' );
        $this->add_js_link( 'http://ajax.googleapis.com/ajax/libs/prototype/1/prototype.js' );
        $this->add_js_link( 'http://ajax.googleapis.com/ajax/libs/scriptaculous/1/scriptaculous.js?load=effects' );
        $this->add_js_link( '/js/prototip.js' );
        $this->add_js_link( '/js/ajax.js' );
        $this->add_js_link( '/js/main.js' );
        $this->add_js_link( '/js/banner.js' );
        $this->add_js_link( 'http://d3js.org/d3.v5.min.js' );
        $this->add_js_link( '/js/daily-weather-graph.min.js' );
        $this->add_head_js("Event.observe(window, 'load', main_init);");
        $this->add_head_js("Event.observe(window, 'load', start_banner);");
    }

	protected function header_block() {
		$header = new Container();
		if ($this->include_ticker) {
			$this->add_js_link( '/js/ticker.js' );
			//$this->add_head_js("Event.observe(window, 'load', start_ticker);");
			$this->add_head_js("$(window).load(start_ticker);");
		}

		$header->add( new DIVtag(array('id' => 'idPageHeadText'),"Hemna's Weather Station"),
		              new DIVtag(array('id' => 'idPageSubHeadText'),"Appomattox, VA"),
		              new DIVtag(array('id' => 'idMetarContainer'),
		                new Divtag(array("id" => "idMetar"), "&nbsp;")
		                )
		               );
		              
		$table = TABLEtag::factory("1024",0,0,0,"center");

		$table->add_row(new TDtag(array("id" => "headtopleft"), "Appomattox, VA  Weather"),
		                new TDtag(array("id" => "headtopcenter"), "&nbsp;"),
		                new TDtag(array("id" => "headtopright"), IMGtag::factory("/css/prototip_loader.gif")));

		$table->add_row(new TDtag(array("id" => "pageheader", "colspan" => "3"),$header));
		
		$table->add_row(new TDtag(array('id' => self::ID_MENU, "colspan" => "3"), $this->menu_block()));

		return $table;
    }
    
    /**
     * Returns menu widget object
     * to use
     *
     * @return MenuBarWidget
     */
    protected function menu_block() {
        $div = new DIVtag(array('id' => 'idMenuItems'));
        $ul = ULtag::factory(
                 LItag::factory(Atag::factory(RequestBuilder::build_url('HomePage'), 'Home')),
                 LItag::factory(Atag::factory(RequestBuilder::build_url('WxDate'), 'Old Data')),
                 LItag::factory(Atag::factory(RequestBuilder::build_url('WeatherDataListPage'), 'Raw Data')),
                 LItag::factory(Atag::factory("http://wx.sierraglider.com/wx", 'Cool, CA Weather Station'))
                 );

        $div->add($ul);
        $div->set_collapse();
        return $div;
    }

    /**
     * We override this method to automatically
     * break up the main block into a
     * left block and a right block
     *
     * @param TABLEtag object.
     */
    protected function main_block() {

		$main = new DIVtag(array("id" => self::ID_BODY,
		                         "style" => "width: ".$this->centering_width."; margin-left: auto; margin-right:auto;"));

		$table = TABLEtag::factory("100%", 0);

		$table->add_row( TDtag::factory("leftblock", "", $this->left_block() ),
						 TDtag::factory("rightblock", "", $this->content_block() ));
						 
		$table->add_row(new TDtag(array("colspan"=> 2),
						$this->footer_block()));						 
        $main->add( $table );

		return $main;
    }

    /**
     * This builds the main content for the
     * page.
     *
     */
    function body_content() {

        //add the header area
        $this->add( html_comment( "HEADER BLOCK BEGIN") );
        $this->add( $this->header_block() );
        $this->add( html_comment( "HEADER BLOCK END") );

        //add the main body
        $this->add( html_comment( "MAIN BODY BLOCK BEGIN") );
        $this->add( $this->main_block() );
        $this->add( html_comment( "MAIN BODY BLOCK END") );
        
        $this->add( html_comment( "tracking code BEGIN"));
        $this->add( $this->tracking_code() );
        $this->add( html_comment( "tracking code END"));
    }

    /**
     * this function returns the contents
     * of the left block.  It is already wrapped
     * in a TD
     *
     * @return HTMLTag object
     */
    protected function left_block() {
        $td = TDtag::factory();
        $td->set_style("padding: 20px;");

		$td->add( "LEFT BLOCK" );
        return $td;
    }

    /**
     * this function returns the contents
     * of the right block.  It is already wrapped
     * in a TD
     *
     * @return HTMLTag object
     */
    protected function content_block() {
		return Container::factory( "CONTENT BLOCK", BRtag::factory(2),
                                   Atag::factory($_SERVER["PHP_SELF"]."?debug=1",
                                                 "Show Debug source"),
                                   BRtag::factory(10));
    }

    /**
     * This function is responsible for building
     * the footer block for every page.
     *
     * @return HTMLtag object.
     */
    protected function footer_block() {

		$footer_div = new DIVtag();
		$footer_div->set_tag_attribute("id", "footerblock");

		$p = html_p( html_img_href("http://phphtmllib.newsblob.com", "http://phphtmllib.newsblob.com/images/phphtmllib_logo.png")   );
        $p->set_tag_attributes( array("align" => "center") );

		$footer_div->add($p);
		
        return Container::factory(
                 html_comment( "FOOTER BLOCK BEGIN"),
                 $footer_div,
                 html_comment( "FOOTER BLOCK END"));
    }
    
    
    
    /**
     * Add the piwik tracking js shite
     * 
     */
    protected function tracking_code() {
        $target = Request::singleton()->get("target");
		if (is_null($target) || empty($target)) {
			$target = "home";
		}

    	$str = '<!-- Piwik -->
<script type="text/javascript">
var pkBaseURL = (("https:" == document.location.protocol) ? "https://wx.hemna.com/piwik/" : "http://wx.hemna.com/piwik/");
document.write(unescape("%3Cscript src=\'" + pkBaseURL + "piwik.js\' type=\'text/javascript\'%3E%3C/script%3E"));
</script><script type="text/javascript">
try {
var piwikTracker = Piwik.getTracker(pkBaseURL + "piwik.php", 1);
piwikTracker.trackPageView();
piwikTracker.enableLinkTracking();
piwikTracker.setDocumentTitle("'.$target.'");
} catch( err ) {}
</script><noscript><p><img src="http://wx.hemna.com/piwik/piwik.php?idsite=1" style="border:0" alt=""/></p></noscript>
<!-- End Piwik Tag -->';
   

      //return $str;
      return "";
    }


    /***********************/
    /* Cacheable Interface */
    /***********************/

    public function get_cache_key() {
        return get_class($this);
    }

    public function get_cache_lifetime() {
        return 60;
    }

    /**
     * Enable caching of this page
     * by default
     */
    public function is_cacheable() {
        return Config::singleton()->get('page_cache_enabled');
    }
}

?>
