<?php

/**
 * This class is responsible for processing
 * the daily dir of images into an flv
 * and shipping it off to the hosting provider
 * where wx.hemna.com lives.
 *
 *
 * @author waboring
 *
 */
class AviProcessorDaemon extends ScheduleDaemon {

	/**
	 * User ID
	 */
	protected $user_id = 1000;

	/**
	 * Group ID
	 */
	protected $group_id = 100;

	/**
	 * how long to wait in between
	 * processing. in seconds
	 */
	protected $wait_period = 1800;


	/**
	 * the log object.
	 */
	protected $log;

	protected $imagesdir;
	protected $videodir;

	/**
	 * track the number of images we are
	 * processing.  If we have the same number
	 * as last run...no need to process.
	 */
    protected $image_count = 0;

    protected $imgs2avi = NULL;
    protected $encoder = "";


	/**
	 * we need to return the logger.
	 *
	 */
	protected function get_logger() {
		$log = new Log();
		$log->add_writer(FileLogWriter::factory(realpath($GLOBALS["path_base"])."/logs/daemon.log"));

		return $log;
	}


	/**
	 * Do any initialization here.
	 *
	 */
	protected function init() {
		$config = Config::singleton();
        $this->imagesdir = $config->get("imagesdir");
        $this->imgs2avi = $config->get("imgs2avi");
        $this->videodir = $config->get("videodir");
        $this->encoder = $config->get("encoder");
        $this->wait_period = $config->get("avi_daemon_wait");
	}

	/**
	 * any cleanup here
	 */
	protected function cleanup() {

	}

	protected function can_run() {

		//used for directory
		$ymdDate = date("Ymd");

		$dirname = $this->imagesdir."/".$ymdDate;

		//make sure the images dir exists.
		$this->test_images_dir($dirname);

		$num_images = $this->count_images($dirname);
		$this->num_images = $num_images;

		if ($num_images == 0 || ($num_images == $this->image_count)) {
			$this->log->info("::can_run() no images or same count");
			return false;
		} else {

			$this->log->info("::do_task() found ".$num_images." to process into a movie.");
			return true;
		}
	}


	/**
	 * do the actual work here.
	 *
	 */
	protected function schedule() {

		//used for directory
		$ymdDate = date("Ymd");

		$dirname = $this->imagesdir."/".$ymdDate;

		$rate = $this->calculate_rate($this->num_images);

		$this->log->info("::do_task() rate is ".$rate." -> movie length = ".$this->num_images/$rate."s");
		//generate the avi
		//$cmd = $this->imgs2avi." -w 640 -h 480 -r ".$rate;
		$cmd = $this->imgs2avi." -w 1920 -h 1080 -r ".$rate;
		$this->exec_command($dirname, $cmd);

		//now convert the avi movie to a flash file
		//$aviname = "timelapse_640x480_".$rate."fps.avi";
		$aviname = "timelapse_1920x1080_".$rate."fps.avi";
		#$cmd = "/usr/bin/avconv -y -i ".$aviname." -b 1900k -f flv ".$ymdDate."/timelapse.flv";
		#$this->exec_command($dirname, $cmd);
		//$cmd = $this->encoder." -y -i ".$aviname." ".$ymdDate."/timelapse.webm";
		$cmd = $this->encoder." -y -c:v libvpx -qmin 2 -qmax 30 -c:a libvorbis -i ".$aviname." ".$ymdDate."/timelapse.webm";
		$this->exec_command($dirname, $cmd);

		//now send it to the hosting provider
		//$cmd = "/usr/bin/scp timelapse.flv wx.hemna.com:wx.hemna.com/htdocs/video/".$ymdDate;
		//$cmd = "/usr/bin/rsync -av ".$ymdDate." wx.hemna.com:wx.hemna.com/htdocs/video";
		$cmd = "/usr/bin/rsync -av ".$ymdDate." ".$this->videodir;
		$this->exec_command($dirname, $cmd);

		//track the image count
		$this->image_count = $num_images;
	}

	/**
	 * This is used to execute an external command
	 *
	 */
	protected function exec_command($dirname, $cmd) {
		$command = "cd ".$dirname."; ".$cmd." 2>&1 >>".$dirname."/processing.log";
		$this->log->info("::exec_command() exec ".$command);
		$last_line = system($command, $return);
		$this->log->debug("::exec_command() done :".$last_line);
	}

	/**
	 * calculate the frame rate to use based
	 * on the # of images we have to process.
	 *
	 * @param $numimages
	 * @return rate
	 */
	protected function calculate_rate($num_images) {
		$rate = 30;
		if ($num_images < 10) {
			$rate = 1;
		} else if ($num_images < 100) {
			$rate = 10;
		} else if ($num_images < 400) {
			$rate = 15;
		} else if ($num_images < 800) {
			$rate = 20;
		} else if ($num_images < 1200) {
			$rate = 25;
		}

		return $rate;
	}




	/**
	 * Find out how many images are in the dir
	 *
	 * @return int
	 */
	protected function count_images($dirname) {
		$count = 0;

		$this->log->debug("::count_images() : check ".$dirname);
		if (file_exists($dirname)) {
			if ($dir = opendir($dirname)) {
				while (($file = readdir($dir)) !== false) {
					if (strstr($file, '.jpg')) {
						$count++;
					}
				}
				closedir($dir);
			}
		}

		$this->log->debug("::count_images() : found ".$count);
		return $count;
	}


	/**
	 * make sure the video dir exists.
	 *
	 */
	protected function test_images_dir($dirname) {

		$ymdDate = date("Ymd");
		$flashdirname =  $dirname."/".$ymdDate;

		if (!file_exists($dirname)) {
			$this->create_dir($dirname);
		}

		if (!file_exists($flashdirname)) {
			$this->create_dir($flashdirname);
		}
	}

	/**
	 * Create the image directory
	 * (this is also done from the camera)
	 *
	 */
	protected function create_dir($dirname) {
		$this->log->info("Creating directory (".$dirname.")");
		$ret = mkdir($dirname);
		if (!$ret) {
			$ex = new DaemonException("couldn't create directory (".$dirname.")");
			$this->log->err("Failed to creat directory", $ex);
			throw $ex;
		}
	}




	/**
	 * Create the process id file on disk
	 * so that we can store it for later use.
	 *
	 * @return string
	 */
	public function generate_pid_filename() {
		return realpath($GLOBALS["path_base"]).'/bin/'.__CLASS__.'.pid';
	}


   /**
	 * no need to set the user id
	 */
	public function set_identity() {
	  return true;
	}
}

?>
