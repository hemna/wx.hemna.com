<?php
/**
 *
 */

abstract class myChartsJSGraph {

    protected $date = null;

    protected $cache = null;

    protected $cache_lifetime = 3600;

    protected $sunrise = null;
    protected $sunset = null;

    /**
     * Class Constructor
     *
     */
    public function __construct($date=null, $load=True) {
        $this->cache = $this->get_cache_object();

        if (!is_null($date)) {
            $this->date = $date;
        } else {
            $this->date = Request::singleton()->get("date", date("Y-m-d"));
            if (empty($this->date) || is_null($this->date)) {
        	    $this->date = date("Y-m-d");
            }
        }
        $this->build_sunrise_sunset();

	if ($load) {
	    $this->load_data();
	}
    }

    public function render() {
        $data_only = Request::singleton()->get("dataOnly", False);
        if ($data_only) {
            print $this->render_data();
        } else {
            return $this->render_graph();
        }
    }

    protected function get_cache_object() {
        $cache_enabled = Config::singleton()->get('cache_enabled');
        if (!$cache_enabled) {
            return NoCache::singleton();
        } else {
	    return FileCache::singleton();
	}
    }

    abstract protected function render_data();
    abstract protected function render_graph();

    abstract protected function load_data();

    protected function build_sunrise_sunset() {
        $lat = $GLOBALS["config"]->get("latitude");
        $long = $GLOBALS["config"]->get("longitude");

        $this->sunrise = date_sunrise(time(), SUNFUNCS_RET_TIMESTAMP, $lat, $long)*1000;
        $this->sunset = date_sunset(time(), SUNFUNCS_RET_TIMESTAMP, $lat, $long)*1000;
    }

}
?>
