<?php
/**
 *
 */

class TempDewPointChartsJSGraph extends myChartsJSGraph {
    
    protected $temp_data = array();
    protected $dewp_data = array();

    public function load_data() {
        $today = $this->date;
        $cache_key = "TempDewPointChartsGraphData_".$today;
        $cache_data = $this->cache->get($cache_key);
        if ($cache_data === false) {
            $collection = weatherDataObject::find_all(
                "datetime like :date",
                array(':date'=> $today.'%'),
                'order by datetime asc',
                'limit 0,10');

            foreach($collection as $wx) {
                /**
                 * @param $wx weatherDataObject
                 */
                $datetime = strtotime($wx->get_datetime()) * 1000;
                //$date = date("m-d-Y H:i:s", $wx->get_datetime());
                $date = $wx->get_datetime();
                $temp_value = $wx->get_temp_out();
                $this->temp_data[] =  "{t: '".$date."', y: '".$temp_value."'}";
                $this->temp_json_data[] = array("t"=>$date,
                                                "y"=>floatval($temp_value));

                $dewp_value = $wx->get_dewpoint();
                $this->dewp_data[] = "['".$date."',".$dewp_value."]";
                $this->dewp_json_data[] = array($date, floatval($dewp_value));
            }
            $cache_data = array('temp_data' => $this->temp_data,
                                'temp_json_data' => $this->temp_json_data,
                                'dewp_data' => $this->dewp_data,
                                'dewp_json_data' => $this->dewp_json_data);
            $this->cache->set($cache_key, $cache_data, $this->cache_lifetime);
        } else {
            $this->temp_data = $cache_data['temp_data'];
            $this->temp_json_data = $cache_data['temp_json_data'];
            $this->dewp_data = $cache_data['dewp_data'];
            $this->dewp_json_data = $cache_data['dewp_json_data'];
        }
    }

    public function render_graph() {
        $js = "
var tempdewdata = [ ".join($this->temp_data, ', ')." ];
tempdewcanvas = document.getElementById('idTempDewPoint');
console.log(tempdewcanvas.width);
console.log(tempdewcanvas.height);
var tempdewpointoptions = {
    type: 'line',
    data: {
        datasets: [{
            data: tempdewdata,
            fill: false,
            drawTicks: true
        }],
    options: {
        scales: {
           xAxes: [{
               type: 'time',
               distribution: 'linear',
               bounds: 'data',
               time: {
                   unit: 'second',
                   minUnit: 'second',
               },
               scaleLabel: {
                   display: true,
                   labelString: 'Date',
               }

           }],
           yAxes: [{
               scaleLabel: {
                   display: true,
                   labelString: 'Temperature °F',
               }
           }],
        }
      }
    }
}

var chart = new Chart(tempdewcanvas, tempdewpointoptions);
";

        return "<script type=\"text/javascript\">".$js."</script>";
    }

    public function render_data() {
        $temp_series = array( "name" => "Temperature F",
                              "color" => "#ff0000",
                              "shadow" => "true",
                              "data" => $this->temp_json_data);
        $dewp_series = array( "name" => "Dewpoint F",
                              "color" => "#006600",
                              "shadow" => "true",
                              "data" => $this->dewp_json_data);
        $series = array($temp_series, $dewp_series);
        //return json_encode($series);
        return json_encode($temp_series);
    }
}
?>
