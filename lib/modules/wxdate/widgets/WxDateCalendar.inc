<?php

class WxDateCalendar extends AjaxableHTMLWidget {
	
	protected $date = null;
	protected $year = null;
	protected $month = null;
	protected $day = null;
	
	protected $current_date;
	
	private $cur_month = null;
	
	public function __construct($date = null) {
		parent::__construct();
		$this->current_date = date("Y-m-d");
		if ($date == null) {
			$this->date = Request::singleton()->get("date", date("Y-m-d"));
			if (empty($this->date) || is_null($this->date)) {
        		$this->date = date("Y-m-d");
        	}
		} else {
			$this->date = $date;			
		}

		$ts = strtotime($this->date);
		$this->year = date("Y", $ts);
		$this->month = date("m", $ts);		
		$this->day = date('j',$ts);
		//Log::Singleton()->debug("date=".$this->date." year = ".$this->year." month = ".$this->month." day = ".$this->day);
	}
	
	
	public function get_content() {
		$div = new DIVtag(array('id'=>$this->get_ajax_replacement_divid().'contents'));
		 		
		$days = $this->get_days();						
		
		$pn = array('&laquo;'=>'/weblog/archive/2004/Jul', '&raquo;'=>'/weblog/archive/2004/Sep');
		$pn = $this->get_prev_next_urls();
		 
		$div->add(Calendar::factory($this->year, $this->month, $days, 3, null, 0, $pn));
		
		return $div;
	}
	
	
	protected function get_prev_next_urls() {
		
		$next_month_ts = strtotime("+1 month", strtotime($this->date));
		$prev_month_ts = strtotime("-1 month", strtotime($this->date));
		
		$prev = RequestBuilder::build_url("WxDate", array("date" => date("Y-m-d", $prev_month_ts)));
		$next = RequestBuilder::build_url("WxDate", array("date" => date("Y-m-d", $next_month_ts)));
		
		//Log::singleton()->debug("date = ".$this->date."  prev = ".$prev."   next=".$next);
		
		if (!$this->is_current_month()) {
			return $pn = array("&laquo;" => $prev, "&raquo;" => $next );
		} else {
			return $pn = array("&laquo;" => $prev);
		}
	}	
	
	protected function get_days() {
		$dt = new DateTime();
		$days = array();
		
		$cur_ts = strtotime($this->current_date);
		$ts = strtotime($this->date);
		
		$cur_month = $this->is_current_month();
		$cur_day = date("d");				
		
		$num_days = $this->get_days_in_month($this->month, $this->year);
		
		if ($cur_month) {
			$num_days = $cur_day;
		}
		
		for ($x=1; $x<=$num_days; $x++) {
			
			if ($x == $this->day) {
				$days[$x] = array(null, "selected-day");
			} else {
				$date = $this->year."-".$this->month."-".sprintf("%02d", $x);
				$url = RequestBuilder::build_url("WxDate", array("date" => $date));
				$days[$x] = array($url, "linked-day");
			}						
		}
		return $days;
	}
	
	private function is_current_month() {
		if (!is_null($this->cur_month)) {
			return $this->cur_month;
		}
		
		$date = date("Y-m");
		$cur_ts = strtotime($date."-01");		
		$ts = strtotime($this->year."-".$this->month."-01");
		
		if ($cur_ts == $ts) {
			$this->cur_month = true;
		} else {
			$this->cur_month = false;
		}			
		
		return $this->cur_month;
	}
	
	private function get_days_in_month($month, $year) {
		$ts = strtotime("-1 day", strtotime($year."-".$month."-01"));
		return date("d", $ts);
	}
	
	
}
?>
