<?php
/**
 *
 */

class PressureChartsJSGraph extends myChartsJSGraph {

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
                $value = $wx->get_rel_pressure();
                $total = $total + $value;
                $avg = ($total) / $cnt;
                $this->graph_average[] = "{t: '".$date."',y: '".$avg."'}"; 
                $this->average_json_data[] = array($date, floatval($avg));
                $cnt++;
                $this->pressure_data[] =  "{t: '".$date."', y: '".$value."'}";
                $this->pressure_json_data[] = array($date, floatval($value));

            }
            $cache_data = array('pressure_data' => $this->pressure_data,
                                'pressure_json_data' => $this->pressure_json_data,
                                'graph_avg' => $this->graph_average,
                                'avg_json_data' => $this->average_json_data);
            $this->cache->set($cache_key, $cache_data, $this->cache_lifetime);
        } else {
            $this->pressure_data = $cache_data['pressure_data'];
            $this->pressure_json_data = $cache_data['pressure_json_data'];
            $this->graph_average = $cache_data['graph_avg'];
            $this->average_json_data = $cache_data['avg_json_data'];
        }
    }

    public function render_graph() {

        $js = "
var presdata = [ ".join($this->pressure_data, ', ')." ];
var presavgdata = [ ".join($this->graph_average, ', ')." ];
prctx = document.getElementById('idPressure').getContext('2d');
prctx.canvas.width = 510;
prctx.canvas.height = 200;

var color = Chart.helpers.color;
var presoptions = {
    type: 'line',
    data: {
         datasets: [
             { label: 'Barometric Pressure inHG',
               backgroundColor: color('#333399').alpha(0.5).rgbString(),
               borderColor: '#333399',
               data: presdata,
               type: 'line',
               pointRadius: 0,
               fill: false,
               lineTension: 0,
               borderWidth: 1 },
             { label: 'Average inHg',
               backgroundColor: color('#FF0000').alpha(0.5).rgbString(),
               borderColor: '#FF0000',
               data: presavgdata,
               type: 'line',
               pointRadius: 0,
	       fill: false,
               lineTension: 0,
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
                   labelString: 'Temperature °F',
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

var chart = new Chart(prctx, presoptions);
";

        return "<script type=\"text/javascript\">".$js."</script>";
    }


    public function render_data() {
        $pressure_series = array( "name" => "Pressure",
                                  "color" => "#0000ff",
                                  "shadow" => "true",
                                  "data" => $this->pressure_json_data);
        $average_series = array( "name" => "Average",
                                  "color" => "#ff0000",
                                  "shadow" => "true",
                                  "data" => $this->average_json_data);
        $series = array($pressure_series, $average_series);
        return json_encode($series);
    }
}
?>
