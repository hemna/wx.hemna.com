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
class DBAggreegateDaemon extends ScheduleDaemon {

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
	protected $wait_period = 21600;


	/**
	 * the log object.
	 */
	protected $log;

    protected $db = NULL;

    protected $cache = NULL;

	/**
	 * we need to return the logger.
	 *
	 */
	protected function get_logger() {
		$log = new Log();
		$log->add_writer(FileLogWriter::factory(realpath($GLOBALS["path_base"])."/logs/db_agg_daemon.log"));

		return $log;
	}


	/**
	 * Do any initialization here.
	 *
	 */
	protected function init() {
        $config = Config::singleton();
        if (!isset($this->cache)) {
            $this->cache = FileCache::singleton();
        }
	}

	/**
	 * any cleanup here
	 */
	protected function cleanup() {

	}

    protected function can_run() {
        //Run only if we have

		//used for directory
        $ymdDate = date("Y-m-d");
        return True;
    }

    private function get_max_mins($year) {
		$this->log->info("::get_max_mins for ".$year);
        $year_month = $year."-%";
        $sql = "select cast(datetime as date) as date, ".
               "min(temp_out) as low_out, ".
               "max(temp_out) as high_out, ".
               "min(temp_in) as low_in, ".
               "max(temp_in) as high_in, ".
               "max(wind_speed) as high_wind ".
            "from weather where datetime like :datetime group by 1 ".
            "order by 1";
		$this->log->info("::get_max_mins do query");
        $values = $this->db->queryBindAllRowsCache($sql,
            array(":datetime" => $year_month));

        $entries = array();
        foreach ($values as $obj) {
            $entries[$obj->date] = array(
                "low_in" => $obj->low_in,
                "high_in" => $obj->high_in,
                "low_out" => $obj->low_out,
                "high_out" => $obj->high_out,
                "high_wind" => $obj->high_wind);
        }
        return $entries;
    }

    private function get_wind_angle($wind_speed, $date) {
		$this->log->debug("::get_wind_angle ".$wind_speed." for ".$date);
        $sql = "select wind_angle from weather where wind_speed = :wind_speed".
            " and datetime like '".$date."%'";
        $stmt = $this->db->queryBindOneRowCache($sql,
            array(":wind_speed" => $wind_speed));
        return $stmt->wind_angle;
    }

    private function get_max_temp_in_time($max_temp, $date) {
		$this->log->debug("::get_max_temp_in_time ".$wind_temp." for ".$date);
        $sql = "select datetime from weather where temp_in = :max_temp".
            " and datetime like '".$date."%'";
        $stmt = $this->db->queryBindOneRowCache($sql,
            array(":max_temp" => $max_temp));
        return $stmt->datetime;
    }

    private function get_max_temp_out_time($max_temp, $date) {
		$this->log->debug("::get_max_temp_out_time ".$wind_temp." for ".$date);
        $sql = "select datetime from weather where temp_out = :max_temp".
            " and datetime like '".$date."%'";
        $stmt = $this->db->queryBindOneRowCache($sql,
            array(":max_temp" => $max_temp));
        return $stmt->datetime;
    }

    private function get_max_wind_time($max_wind, $date) {
		$this->log->debug("::get_max_wind_time ".$max_wind." for ".$date);
        $sql = "select datetime from weather where wind_speed = :max_wind".
            " and datetime like '".$date."%'";
        $stmt = $this->db->queryBindOneRowCache($sql,
            array(":max_wind" => $max_wind));
        return $stmt->datetime;
    }

    private function get_rain_totals($month) {
        //Get the rain totals for last 15 days
		$this->log->debug("::get_rain_totals ".$month->format('Y-m-d'));

        $sql = "select (select rain_total from weather where datetime = :dateupper) - ".
            "       (select rain_total from weather where datetime = :datelower) ".
            " as daily_rain";

        $start_date = $month->format('Y-m-d');
        $finish_day = $month->format('Y-m-t');
        $finish_date = new DateTime($finish_day);
        $this->log->debug("rain start_date=".$start_date);
        $this->log->debug("rain finish_date=".$finish_date->format('Y-m-d'));

        $rain_totals = array();

        $done = False;
        //For each date we want to find the rain
        //We have to subtract the date from the previous date's values.
        $dateupper = $start_date;
        //$this->log->debug("dateupper=".$dateupper);
        while (!$done) {
            //start with $dateupp and subtract it from previous date.
            //until we reach $finish_date
            $lower = new DateTime($dateupper);
            //P 1D = 1 day
            $lower->sub(new DateInterval('P1D'));
            $datelower = $lower->format('Y-m-d');
            //$this->log->debug("find rain between ".$datelower." and ".$dateupper);
            $stmt = $this->db->queryBindOneRowCache($sql,
                array(":dateupper" => $dateupper,
                ":datelower" => $datelower));
            if ($stmt->daily_rain) {
                $rain_totals[$dateupper] = $stmt->daily_rain;
            } else {
                $rain_totals[$dateupper] = 0.00;
            }

            $next = new DateTime($dateupper);
            $next->add(new DateInterval('P1D'));
            //$this->log->debug("next = ".$next->format("Y-m-d")."  == finish =".$finish_date->format("Y-m-d"));
            if ($next > $finish_date) {
                //We reached the end
                $done = True;
            } else {
                $dateupper = $next->format('Y-m-d');
            }
        }

        return $rain_totals;
    }

    private function get_agg_for_year($year) {
        $year_max_mins = $this->get_max_mins($year);
		$this->log->info("::get_agg_for_year(".$year.") year_max_mins done");
        //Now loop over every month
        for ($month = 1; $month <=12; $month++) {
            $this->get_agg_for_month($year, $month, $year_max_mins);
        }
    }

    private function get_agg_for_month($year, $month, $year_max_mins) {
        $ymDate = $year."-".$month;
        $rain_totals_month = $this->get_rain_totals(new DateTime($ymDate));
		$this->log->info("::schedule() get_rain_totals done");
		$this->log->info($rain_totals_month);
        //now loop over every day in the month
        $days_in_month = cal_days_in_month(CAL_GREGORIAN, $month, $year);
        for ($day=1; $day <= $days_in_month; $day++) {
            $ymdDate = $ymDate."-".$day;
            $this->get_agg_for_day(new DateTime($ymdDate), $year_max_mins,
                                   $rain_totals_month);
        }

    }

    private function get_agg_for_day($date, $max_mins, $rain_month) {
		$this->log->debug("::get_agg_for_day ".$date->format('Y-m-d'));
        $ymd = $date->format('Y-m-d');
        if (!$max_mins[$ymd]) {
		    $this->log->debug("::get_agg_for_day no data for ".$ymd);
            return;
        }

        $mm = $max_mins[$ymd];
        $wind_angle = $this->get_wind_angle($mm["high_wind"], $ymd);

		$this->log->debug("::get_agg_for_day ".$ymd);
		$this->log->debug($mm);
        try {
            $agg_obj = weatherDailyAggDataObject::find_by_field("datetime", $ymd);
        } catch (DataObjectException $e) {
		    $this->log->debug("::get_agg_for_day Can't find DATE ".$ymd);
            $agg_obj = new weatherDailyAggDataObject();
            $agg_obj->set_datetime($ymd);
        }
        $max_temp_in_time = $this->get_max_temp_in_time($mm["high_in"], $ymd);
        $max_temp_out_time = $this->get_max_temp_out_time($mm["high_out"], $ymd);
        $max_wind_time = $this->get_max_wind_time($mm["high_wind"], $ymd);
        $agg_obj->set_max_temp_in($mm['high_in']);
        $agg_obj->set_low_temp_in($mm['low_in']);
        $agg_obj->set_max_temp_in_time($max_temp_in_time);
        $agg_obj->set_max_temp_out($mm['high_out']);
        $agg_obj->set_low_temp_out($mm['low_out']);
        $agg_obj->set_max_temp_out_time($max_temp_out_time);
        $agg_obj->set_max_wind_speed($mm['high_wind']);
        $agg_obj->set_max_wind_speed_time($max_wind_time);
        $agg_obj->set_max_wind_angle($wind_angle);
        $agg_obj->set_rainfall($rain_month[$ymd]);
        $agg_obj->save();
    }


    private function _get_first_date() {
        $stmt = $this->db->queryBindOneRowCache("select datetime from weather order by datetime asc", array());
        return $stmt->datetime;
    }


	/**
	 * do the actual work here.
	 *
	 */
	protected function schedule() {
        if (!$this->db) {
            $this->db = open2300DB::singleton();
        }
		//used for directory
		$ymdDate = date("Y-m-d");
		$ymDate = date("Y-m");
		$yDate = date("Y");

		$this->log->info("::schedule() Run for ".$ymDate);

        //find last updated entry
        $start_date = NULL;
        try {
            $stmt = $this->db->queryBindOneRowCache("select datetime from weather_daily_agg order by datetime desc", array());
            $need_all = False;
            $start_date = $stmt->datetime;
		    $this->log->info("::schedule() Run for ".$start_date);
        } catch (PDOException $e) {
            $this->log->info("::schedule() No existing daily aggregate results");
            $start_date = $this->_get_first_date();
        }
        if (!$stmt->datetime) {
            $this->log->info("::schedule() No existing daily aggregate results");
            $start_date = $this->_get_first_date();
        }

	    $this->log->info("::schedule() DailyAgg start datetime = ".$start_date);
        sleep(1);

        $start = new DateTime($start_date);
        $end = new DateTime(date('Y-m-d'));
        $end = $end->modify( '+1Year');

        $interval = new DateInterval('P1Y');
        $daterange = new DatePeriod($start, $interval, $end);

        $max_mins_years = $this->cache->get("DBAGG_MaxMinsYears");
        if (!$max_mins_years) {
            $max_mins_years = array();
            //Get max and mins for every year
            //from start year to current year
            foreach($daterange as $date) {
                echo $date->format('Y-m-d')."\n";
                $year = $date->format('Y');
                $max_mins_years[$year] = $this->get_max_mins($year);
            }
            $this->log->info("::schedule() Got Max_Mins_years Saving to CACHE");
            $this->cache->set("DBAGG_MaxMinsYears", $max_mins_years, 3600);
        }
        $rain_totals = $this->cache->get("DBAGG_RainTotals");
        if (!isset($rain_totals)) {
            $rain_totals = array();
        }
	    $this->log->info("::schedule() Got Max_Mins_years. ".array_keys($max_mins_years));
        sleep(1);

        //Now walk every day from the start day
        //to today and update the aggregate data.
        $start = new DateTime($start_date);
        $end = new DateTime(date('Y-m-d'));
        $interval = new DateInterval('P1D');
        $daterange = new DatePeriod($start, $interval, $end);

	    $this->log->info("::schedule() Loop over every day starting ".$start->format('Y-m-d'));
        sleep(1);
        foreach($daterange as $date) {
            $month = new DateTime($date->format('Y-m'));
            $mf = $date->format('Y-m');
            if (!isset($rain_totals[$mf])) {
                //get the rain totals for that month
                $rain_totals_month = $this->get_rain_totals($month);
                $rain_totals[$mf] = $rain_totals_month;
                $this->cache->set("DBAGG_RainTotals", $rain_totals, 3600);
                sleep(1);
            }
            $this->get_agg_for_day(
                $date,
                $max_mins_years[$date->format('Y')],
                $rain_totals[$mf]);
        }

        /*$current_year = date("Y");
        $current_month = date("m");
        $current_day = date("d");

        $start = new DateTime($start_date);
        $start_year = $start->format("Y");
        $start_month = $start->format("m");
        $start_day = $start->format("d");

        $end = new DateTime(date('Y-m-d'));
        $interval = new DateInterval('P1D');
        $daterange = new DatePeriod($start, $interval, $end);

        //from start year to current year
        //from start month to current


        if ($start_year != $current_year) {
            for ($year=$start_year; $year <= $current_year; $year++) {
                $this->get_agg_for_year($start_year);
            }
        } else {
            $year_max_mins = $this->get_max_mins($db, $start_year);
            $this->get_agg_for_month($start_year, $start_month,
                                     $year_max_mins);
        }

        */

        //make sure we have all the data for today.
        /*$year_max_mins = $this->get_max_mins($db, $yDate);
		$this->log->info("::schedule() year_max_mins done");
        $rain_totals_month = $this->get_rain_totals($db, new DateTime($ymDate));
		$this->log->info("::schedule() get_rain_totals done");
        $this->get_agg_for_day($db, new DateTime($ymdDate), $year_max_mins,
                               $rain_totals_month);
        */
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
