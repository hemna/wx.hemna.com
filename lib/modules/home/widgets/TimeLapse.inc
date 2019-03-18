<?php


class TimeLapse extends AjaxableHTMLWidget {

	const CACHE_KEY = "webcamcaldates";

	protected $cal_dates;

	public function __construct() {		
		parent::__construct();

		$cache = FileCache::singleton();

		//determine which dates should be enabled
		//for the calendar
		$this->cal_dates = $cache->get(WebCam::CACHE_KEY);
		if (!$this->cal_dates) {
			$this->find_valid_calendar_dates();
			$cache->set(WebCam::CACHE_KEY, $this->cal_dates, 3600);
		} else {
			$log = Log::singleton();
			$log->info("using calendar cache value");
		}
		 

	}


	public function get_content() {
		global $config;

        $curdate = Request::singleton()->get('date', date("Ymd"));
        if (strstr($curdate, '-')) {
            $curdate = date('Ymd', strtotime($curdate));
        }

		$cam_title_div = new DIVtag(array('id' => 'idTimeLapseTitle'));

		//only show the flash overlay if we
		//have today's video file on disk
		$filename = $config->get('videodir')."/".$curdate."/timelapse.webm";
		if (file_exists($filename)) {
            $nowtime = date("H:i:s");
            $filetime = date("H:i:s", filemtime($filename));
			$cam_title_div->add(new SPANtag(array('id' => 'idTimeLapseUpdate',
			                                      'style' => 'font-size: .7em'),
										    "(Updated ".$filetime." )"));
		} else {
			$cam_title_div->add("&nbsp;");
		}

        $cam_video = "<video width=\"320\" height=\"240\" controls>".
                     "<source src=\"/video/".$curdate."/timelapse.webm?junk=".time()."\" type=\"video/webm\"></video>";
		$cam_video_div = new DIVtag(array('id' => 'idCamVideoDiv'), $cam_video);

		$cam_div = new DIVtag(array('id' => $this->get_ajax_replacement_divid().'Contents',
									'style' => 'padding-bottom:10px;margin-bottom:5px;',
		                            'class' => 'yui-skin-sam'), $cam_title_div, $cam_video_div);

		return $cam_div;
	}


	/**
	 * This walks the video dir and figues out what
	 * directories that coorespond to dates have
	 * valid timelapse.flv files.  These dirs/dates
	 * will be enabled in the calendar popup.
	 *
	 */
	private function find_valid_calendar_dates() {
		global $config;
		$dates = array();
		//$log = Log::singleton();

		$videodir = $config->get('videodir');
		if (file_exists($videodir)) {
			if ($dir = opendir($videodir)) {
				while (($file = readdir($dir)) !== false) {
					if ($file != "." && $file != ".." && $file != "player.swf" && $file != ".svn") {
						//$log->info("::find_valid_calendar_dates() - found file ".$file);
						//now check to see if it contains timelapse.flv
						if (file_exists($videodir."/".$file."/timelapse.webm")) {
							$dates[] = $file;
						}
					}					
				}
				closedir($dir);
			}
		}
		sort($dates);
		$this->cal_dates = $dates;
		//$log->info(var_export($this->cal_dates, true));
		
	}
}
?>
