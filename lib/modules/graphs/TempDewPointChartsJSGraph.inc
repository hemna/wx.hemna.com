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
                'order by datetime asc');

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
                $this->dewp_data[] = "{t: '".$date."', y: '".$dewp_value."'}";
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
var tempdata = [ ".join($this->temp_data, ', ')." ];
var dewpdata = [ ".join($this->dewp_data, ', ')." ];
ctx = document.getElementById('idTempDewPoint').getContext('2d');
ctx.canvas.width = 510;
ctx.canvas.height = 200;

var color = Chart.helpers.color;
var tempdewpointoptions = {
    type: 'line',
    data: {
         datasets: [
             { label: 'Temperature °F',
	       backgroundColor: color('#FF0000').alpha(0.5).rgbString(),
               borderColor: '#FF0000',
	       data: tempdata,
	       type: 'line',
	       pointRadius: 0,
	       fill: false,
	       lineTension: 0,
               borderWidth: 1 },
             { label: 'Dew Point °F',
		   backgroundColor: color('#333399').alpha(0.5).rgbString(),
		   borderColor: '#333399',
		   data: dewpdata,
		   type: 'line',
		   pointRadius: 0,
		   fill: false,
		   lineTension: 0,
		   borderWidth: 1 }
        ]
    },
    options: {
        scales: {
           xAxes: [{
               type: 'time',
               distribution: 'linear',
               bounds: 'data',
               time: {
                   unit: 'minute',
               },
               ticks: {
                   autoSkip: true,
                   maxTicksLimit: 12,
                   labelOffset: 10,
                   lineHeight: 1.0
               },
               gridLines: {
                   color: '#bbbbbb',
                   drawBorder: true,
                   tickMarkLength: 5
               }
           }],
           yAxes: [{
               scaleLabel: {
                   display: false,
                   labelString: 'Temperature °F',
               },
               ticks: {
                   min: -10,
                   max: 110,
                   autoSkip: true,
                   maxTicksLimit: 12
               },
               gridLines: {
                   color: '#bbbbbb',
                   drawBorder: true
               }
           }],
        }
      }
}

var chart = new Chart(ctx, tempdewpointoptions);
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
