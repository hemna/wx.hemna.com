<?php


class WxMonthData extends AjaxableHTMLWidget {
	
	public function __construct($date = null) {
		parent::__construct();
		if ($date == null) {
			$this->date = Request::singleton()->get("date", date("Y-m-d"));
			if (empty($this->date) || is_null($this->date)) {
        		$this->date = date("Y-m-d");
        	}
		} else {
			$this->date = $date;			
		}

		$this->ts = strtotime($this->date);
		$this->year = date("Y", $this->ts);
		$this->month = date("m", $this->ts);		
		$this->day = date('j', $this->ts);
		$this->year_month = $this->year."-".$this->month;		
		
//		Log::Singleton()->debug("date=".$this->date." year = ".$this->year." month = ".$this->month." day = ".$this->day);
	}
	
	
	public function get_content() {
		$div = new DIVtag(array('id'=>$this->get_ajax_replacement_divid().'contents'));

		$db = open2300DB::singleton();	
		
		$stmt = $db->queryBindOneRowCache("Select min(temp_out) as low, max(temp_out) as high from weather where datetime like :date", 
                                          array(":date" => $this->year_month.'%'));
        
        $title = DIVtag::factory('cctitle', "Wx conditions for&nbsp;&nbsp;&nbsp;".date("F Y", $this->ts));
        $div->add($title);
        
        $table = TABLEtag::factory("100%",0,2);
        
        $table->add_row("Temp Month", "High/Low : ".$stmt->high." F /".$stmt->low." F" );
        
        $stmt = $db->queryBindOneRowCache("Select min(temp_out) as low, max(temp_out) as high from weather where datetime like :date", 
                                          array(":date" => $this->date.'%'));

		$table->add_row("Temp Day", "High/Low : ".$stmt->high." F /".$stmt->low." F" );

		
		$sql = "select datetime,wind_speed, wind_angle, wind_direction from weather where ".
               "wind_speed = (select max(wind_speed) from weather where datetime like :date) ".
               "and datetime like :date2";
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $this->year_month.'%', ":date2" => $this->year_month.'%'));
        
    	//var_dump($stmt);					 
		$this->peak_wind['speed'] = $stmt->wind_speed;
		$this->peak_wind['angle'] = $stmt->wind_angle;
		$this->peak_wind['direction'] = $stmt->wind_direction;
		$this->peak_wind['time'] = substr($stmt->datetime,8);
		
		$table->add_row("Peak Wind Month", $this->peak_wind['direction']." ".$this->peak_wind['speed'].'mph@'.$this->peak_wind['angle']."&nbsp;".$this->peak_wind['time']);
		
		$sql = "select datetime,wind_speed, wind_angle, wind_direction from weather where ".
               "wind_speed = (select max(wind_speed) from weather where datetime like :date) ".
               "and datetime like :date2";
        $stmt = $db->queryBindOneRowCache($sql, array(":date" => $this->date.'%', ":date2" => $this->date.'%'));
        
    	//var_dump($stmt);					 
		$this->peak_wind['speed'] = $stmt->wind_speed;
		$this->peak_wind['angle'] = $stmt->wind_angle;
		$this->peak_wind['direction'] = $stmt->wind_direction;
		$this->peak_wind['time'] = substr($stmt->datetime,10);
		
		$table->add_row("Peak Wind Day", $this->peak_wind['direction']." ".$this->peak_wind['speed'].'mph@'.$this->peak_wind['angle']."&nbsp;".$this->peak_wind['time']);
        
		$div->add($table);
		return $div;
	}

}

?>
