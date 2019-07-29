<?php
/**
 *
 */

class WindSpeedChartsJSGraph extends myChartsJSGraph {

    protected $temp_data = array();

    public function load_data() {
        $today = $this->date;
        $cache_key = get_class($this)."_".$today;
        $cache_data = $this->cache->get($cache_key);
        if ($cache_data === false) {
            $collection = weatherDataObject::find_all("datetime like :date order by datetime asc",
                                                      array(':date'=> $today.'%'));

            $total = 0;
            $cnt = 1;
            foreach($collection as $wx) {
                /**
                 * @param $wx weatherDataObject
                 */
		$date = $wx->get_datetime();
                $value = $wx->get_wind_speed();
                $total = $total + $value;
                $avg = ($total) / $cnt;
                $this->graph_average[] = "{t: '".$date."', y: '".$avg."'}"; 
                $this->avg_json_data[] = array($date, floatval($avg));
                $cnt++;
                $this->wind_data[] =  "{t: '".$date."', y: '".$value."'}";
                $this->wind_json_data[] = array($date, floatval($value));

            }
            $cache_data = array('wind_data' => $this->wind_data,
                                'wind_json_data' => $this->wind_json_data,
                                'graph_avg' => $this->graph_average,
                                'avg_json_data' => $this->avg_json_data);
            $this->cache->set($cache_key, $cache_data, $this->cache_lifetime);
        } else {
            $this->wind_data = $cache_data['wind_data'];
            $this->wind_json_data = $cache_data['wind_json_data'];
            $this->graph_average = $cache_data['graph_avg'];
            $this->avg_json_data = $cache_data['avg_json_data'];
        }
    }

    public function render_graph() {
        $js = "
var windspeeddata = [ ".join($this->wind_data, ', ')." ];
var windspeedavgdata = [ ".join($this->graph_average, ', ')." ];
windctx = document.getElementById('idWindSpeed').getContext('2d');
windctx.canvas.width = 510;
windctx.canvas.height = 200;

var color = Chart.helpers.color;
var windoptions = {
    type: 'line',
    data: {
         datasets: [
	     { label: 'Wind Speed mph',
               backgroundColor: color('#333399').alpha(0.5).rgbString(),
	       borderColor: '#333399',
               data: windspeeddata,
	       showLine: false,
               pointRadius: .5,
               borderWidth: 1 },
	     { label: 'Average mph',
               backgroundColor: color('#FF0000').alpha(0.5).rgbString(),
	       borderColor: '#FF0000',
               data: windspeedavgdata,
	       pointRadius: .5,
               borderWidth: 1 },
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
                   labelString: 'Wind Speed mph',
               },
               ticks: {
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

var windc = new Chart(windctx, windoptions);
";

        return "<script type=\"text/javascript\">".$js."</script>";
    }


    public function render_data() {
        $wind_series = array("name" => "Wind Speed (mph)",
                             "type" => "scatter",
                             "color" => "#0000ff",
                             "marker" => array("radius" => 2 ),
                             "shadow" => "true",
                             "data" => $this->wind_json_data);
        $avg_series = array("name" => "Average",
                             "type" => "line",
                             "color" => "#ff0000",
                             "shadow" => "true",
                             "data" => $this->avg_json_data);
        $series = array($wind_series, $avg_series);
        return json_encode($series);
    }
}
?>
