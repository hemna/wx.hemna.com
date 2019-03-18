<?php
/**
 *
 */

class DailyWXGraph extends myD3Graph {

    public function __construct($num_days=30) {
        $this->num_days = $num_days;
        parent::__construct();
    }

    public function load_data() {
        $db = open2300DB::singleton();

        $sql = "select * from weather_daily_agg order by datetime desc limit 0,".
            $this->num_days;

        $values = $db->queryBindAllRowsCache($sql, array());
        $this->wx_data = array_reverse($values);
    }

    public function render() {

        //first dump the data
        $data = "var data = [";
        $start=1;
        foreach ($this->wx_data as $value) {
            $date = new DateTime($value->datetime);
            $data_str = "{date: \"".$date->format('Y-m-d')."\",".
               " highTemperature:".$value->max_temp_out.",".
               " lowTemperature: ".$value->low_temp_out.",".
               " rainfall: ".$value->rainfall.",".
               " highWindGust: ".$value->max_wind_speed.",".
               " highWindGustBearing: ".$value->max_wind_angle."}";
            if ($start == 1) {
                $data .= $data_str;
                $start = 2;
            } else {
                $data .= ",\n".$data_str;
            }
        }
//{date: \"2015-06-12\", highTemperature: 17, lowTemperature: 10, rainfall: 1.4, highWindGust:52, highWindGustBearing: 236},
//{date: \"2015-06-13\", highTemperature: 10, lowTemperature: 10, rainfall: 1.4, highWindGust:52, highWindGustBearing: 236},
        $data .= "\n];";


        //now dump the graph
        $str = "
var graph = new DailyWeatherGraph({data: data,
    container: document.getElementById('idDailyWXGraph'),
    width: 510,
    height: 440,
    temperatureUnit: 'F',
    rainfallUnit: 'inches',
    windUnit: 'mph'});";

        $script = SCRIPTtag::factory();
        $script->add($data, $str);
        return $script;
    }
}
?>
