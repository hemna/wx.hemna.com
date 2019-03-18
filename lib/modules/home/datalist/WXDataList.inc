<?php
/**
  * An AjaxableDataList
  *
  */
  
class WXDataList extends AjaxDefaultHTMLDataList {
    
    const LABEL_DATE  = "Date";
    const LABEL_OUT_TEMP = "Outside Temp";
    const LABEL_DEWPOINT = "Dewpoint";
    const LABEL_WINDSPEED = "Wind Speed";
    const LABEL_WIND_DIR = "Wind Direction";
    const LABEL_RAIN_1H = "Rain last hour";
    const LABEL_RAIN_24H = "Rain last 24hrs";
    const LABEL_RAIN_TOTAL = "Rain Total";
    const LABEL_PRESSURE = "Pressure";
    const LABEL_FORECAST = "Forecast";

    /**
     * the constructor
     */
     public function __construct() {
         parent::__construct("Weather Table", "100%", "datetime", TRUE);
     }
     
    public function request_vars() {
        return array();
    }

    protected function get_data_source() {
        $source = new DataBaseSQLDataListSource(open2300DB::singleton());
        $this->set_data_source($source);
    }
    
    public function is_cacheable() {
        return false;
    }
     
     public function user_setup() {

         $this->process_action();
         
        $this->add_header_item(self::LABEL_DATE, "120", "datetime", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE, "left");
        $this->add_header_item(self::LABEL_OUT_TEMP, "20", "temp_out", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        $this->add_header_item(self::LABEL_DEWPOINT, "20", "dewpoint", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        $this->add_header_item(self::LABEL_WINDSPEED, "20", "wind_speed", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        $this->add_header_item(self::LABEL_WIND_DIR, "20", "wind_direction", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        
        $this->add_header_item(self::LABEL_RAIN_1H, "20", "rain_1h", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        $this->add_header_item(self::LABEL_RAIN_24H, "20", "rain_24h", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        $this->add_header_item(self::LABEL_RAIN_TOTAL, "20", "rain_total", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        
        $this->add_header_item(self::LABEL_PRESSURE, "20", "rel_pressure", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        
        $this->add_header_item(self::LABEL_FORECAST, "20", "forecast", HTMLDataList::SORTABLE,  HTMLDataList::SEARCHABLE,"center");
        
        //$this->add_action_column(DefaultHTMLDataList::ACTION_CHECKBOX, DefaultHTMLDataList::ACTION_FIRST, 'datetime');

		$this->_datalistsource->setup_db_options("*", "weather");
		//$this->set_default_num_rows(5);
		$this->set_form_method('POST');
        $this->save_checked_items(TRUE);
        $this->search_enable();
     }
     
    function actionbar_cell() {
        //don't actually do anything.
        //just show how to add a button
        return $this->action_button('Test');
    }
    
    function process_action() {
        if (Request::singleton()->get('_form_action') == "Test") {
            var_dump('shit');
        }
    }
     
     public function build_column_item($row_data, $col_name) {
         switch ($col_name) {
             case self::LABEL_WINDSPEED:
                 $obj = $row_data['wind_speed'] . " mph";
                 break;
                 
            /* case self::LABEL_OUT_TEMP:
                 $obj = $row_data['temp_out']. " F / ". WXConversion::fahrenheit_to_celsius($row_data['temp_out']). " C";
                 break;
                 
             case self::LABEL_DEWPOINT:
                 $obj = $row_data['dewpoint']. " F / ". WXConversion::fahrenheit_to_celsius($row_data['dewpoint']). " C";
                 break;
                 */
		
		default:
			$obj = parent::build_column_item($row_data, $col_name);
			break;
		}
		return $obj;         
     }

}
?>