<?php
/**
 * This file contains the CurrentConditions
 * widget class.
 */



class CurrentConditions extends AjaxableHTMLWidget {

	/**
	 * This contains the current
	 * weatherDataObject for
	 * the conditions that you want to display
	 *
	 * @var weatherDataObject
	 */
	protected $wx;

	protected $rain_total;

	protected $temp_low_high = array("low" => 0,
                                     "high" => 0);  

	/**
	 * Class Constructor
	 *
	 */
	public function __construct($load=True) {
		$log = Log::singleton();
		parent::__construct();
		if ($load) {
			//we don't have the current conditions.
			//we need to get them
			$this->wx = weatherDataObject::find("1=1 order by datetime desc limit 0,1");

            //load min max temps
            $db = open2300DB::singleton();
            $today = date("Y-m-d");
            $stmt = $db->queryBindOneRowCache("Select min(temp_out) as low, max(temp_out) as high from weather where datetime like :date",
            array(":date" => $today.'%'));
            $this->temp_low_high["low"] = $stmt->low;
            $this->temp_low_high["high"] = $stmt->high;

            $year = date("Y");
            $stmt = $db->queryBindOneRowCache("Select rain_total from weather where datetime like :date order by datetime asc limit 0,1",
            array(":date" => $year.'%'));
            if (!is_null($stmt) && $stmt) {
                $year_rain_start = $stmt->rain_total;
            } else {
                $year_rain_start = 0;
            }

            $this->rain_total = sprintf("%0.2f", $this->wx->get_rain_total() - $year_rain_start);

            //get the peak winds
            //$db->set_debug_flag(true);
            $sql = "select datetime,wind_speed, wind_angle, wind_direction from weather where ".
                   "wind_speed = (select max(wind_speed) from weather where datetime like :date) ".
                   "and datetime like :date2";
            $stmt = $db->queryBindOneRow($sql,
                                         array(":date" => $today.'%', ":date2" => $today.'%'));
            $log->debug(var_export($stmt,true));
            if (!is_null($stmt) && $stmt) {
                $this->peak_wind['speed'] = $stmt->wind_speed;
                $this->peak_wind['angle'] = $stmt->wind_angle;
                $this->peak_wind['direction'] = $stmt->wind_direction;
                $this->peak_wind['time'] = substr($stmt->datetime,10);
                //$log->debug("peak wind was ".$stmt->wind_speed);
            } else {
                $this->peak_wind['speed'] = 0;
                $this->peak_wind['angle'] = 0;
                $this->peak_wind['direction'] = 0;
                $this->peak_wind['time'] = 0;
            }
            }
	}

	public function get_content() {

		$div = new DIVtag(array('id'=>$this->get_ajax_replacement_divid().'contents'));

		//build the title
		//$title = DIVtag::factory('cctitle', Atag::factory(AjaxRequestBuilder::build_replace_url('CurrentConditions', array(), $this->get_ajax_replacement_divid()), 'Current Conditions', 'linkrefresh'));
		$title = DIVtag::factory('cctitle', "Current Conditions  -- Updated : ", new SPANtag(array('id' => 'idUpdateSeconds'),''));
		$div->add($title);

        //now add the update block
        if ($this->wx) {
            $datetime = $this->wx->get_datetime();
        } else {
            $datetime = date('Y-m-d');
        }
		$update_div = new DIVtag(array('id' => 'idcclastupdate'), $datetime);
		$div->add($update_div);


        $table = TABLEtag::factory("100%",0,2);
        if ($this->wx) {
            $outside_temp = $this->wx->get_temp_out();
            $wx_forecast = $this->wx->get_forecast();
            $windchill = $this->wx->get_wind_chill();
            $humidity_out = $this->wx->get_rel_hum_out();
            $wind_angle = $this->wx->get_wind_angle();
            $wind_direction = $this->wx->get_wind_direction();
            $wind_speed = $this->wx->get_wind_speed();
		    $pressure = $this->wx->get_rel_pressure();
            $rain_1h = $this->wx->get_rain_1h();
            $rain_24h = $this->wx->get_rain_24h();
		    $inside_temp = $this->wx->get_temp_in();
        } else {
            $outside_temp = 'N/A';
            $wx_forecast = 'N/A';
            $windchill = 'N/A';
            $humidity_out = 'N/A';
            $wind_angle = 0;
            $wind_direction = 'N/A';
            $wind_speed = 0;
            $pressure = 0;
            $rain_1h = 0;
            $rain_24h = 0;
            $inside_temp = 0;
        }
		$table->add_row(new TDtag(array("width" => "120"),"Outside :"),
		new DIVtag(array('style' => 'text-align:center; padding-top:2px;height:75px; background-color:#ffffcc;'),
		new SPANtag(array('id' => 'idTempF', 'style' => 'font-weight:bold;'),$outside_temp)," F / ",
		new SPANtag(array('id' => 'idTempC','style' => 'font-weight:bold;'),WXConversion::fahrenheit_to_celsius($outside_temp)),' C / ',
		new SPANtag(array('id' => 'idTempK','style' => 'font-weight:bold;'),WXConversion::fahrenheit_to_kelvin($outside_temp)), " K",
		BRtag::factory(), $wx_forecast,
		new DIVtag(array('style' => 'padding-top:5px;'), 'Feels like '.$windchill." F"),
		new DIVtag(array('style' => 'padding-top:5px;'), $this->temp_low_high["low"].' F / '.$this->temp_low_high["high"].' F   Low/High')));

		$table->add_row("Relative Humidity : ", $humidity_out.' %');

		//wind
		$table->add_row("Wind : ",new TDtag(array(), SPANtag::factory('',"From the ".$wind_direction." ".$wind_speed.'mph@'.$wind_angle),
		$this->get_wind_angle_image($wind_angle),
		SPANtag::factory('',"Peak winds ".$this->peak_wind['direction']." ".$this->peak_wind['speed'].'mph@'.$this->peak_wind['angle']."&nbsp;".$this->peak_wind['time'])));

		$table->add_row("&nbsp;", "&nbsp;");
		$table->add_row("Pressure : ", $pressure. ' inHG / '.WXConversion::inchmercury_to_millibars($pressure). ' mbar');
		 
		 
		$table->add_row('Rainfall : ',$rain_1h.'"/ '.$rain_24h.'" / '.$this->rain_total.'"<br>'.
                        '&nbsp;&nbsp;1hr / 24hrs / total.');

		$table->add_row("&nbsp;", "&nbsp;");
		$table->add_row('Inside : ', $inside_temp." F / ". WXConversion::fahrenheit_to_celsius($inside_temp).'C / '.WXConversion::fahrenheit_to_kelvin($inside_temp). " K");;


		$div->add($table);

		//$div->add(new DIVtag(array('style' => 'padding-top:10px;background-color: #aaaaaa;'),$this->wx));

		$script = SCRIPTtag::factory();
		$ts = time();
		$script->add("set_orig_time_ts(".$ts.");");

		//$div->add($script);
		return $div;
	}


	protected function get_wind_angle_image($angle) {

		//calculate the offset to the image from the angle.
		if ($angle == 360) {
			$angle = 0;
		}

		$offset = -1*round($angle) * 14;
		$img = new IMGtag(array('id' => 'idWindArrow', 'style' => 'overflow: hidden; left: '.$offset.'px; position: relative;',
                                                        'src' =>'/images/360arrows_blue.gif'));
		return new DIVtag(array('id' => 'idWindArrowDiv', 'style' => 'position:relative;overflow:hidden;top:1px;left:1px;width:14px;height:14px;'), $img);
	}
}
?>
