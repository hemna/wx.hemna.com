<?php
/**
 *
 */

class InsideTempChartsJSGraph extends myChartsJSGraph {

    protected $temp_data = array();

    public function load_data() {
        $today = $this->date;
        $cache_key = get_class($this)."_".$today;
        $cache_data = $this->cache->get($cache_key);
        if ($cache_data === false) {
            $collection = weatherDataObject::find_all("datetime like :date order by datetime asc",
                                                      array(':date'=> $today.'%'));

            foreach($collection as $wx) {
                /**
                 * @param $wx weatherDataObject
                 */
                $date = $wx->get_datetime();
                $value = $wx->get_temp_in();
                $this->temp_data[] =  "{t: '".$date."', y:'".$value."'}";
                $this->json_data[] = array($date, floatval($value));
            }
            $cache_data = array('temp_data' => $this->temp_data,
                                'json_data' => $this->json_data);
            $this->cache->set($cache_key, $cache_data, $this->cache_lifetime);
        } else {
            $this->temp_data = $cache_data['temp_data'];
            $this->json_data = $cache_data['json_data'];
        }
    }

    public function render_graph() {
        $data = "[".join($this->temp_data, ",")."]";
        $js = "
var intempdata = [ ".join($this->temp_data, ', ')." ];
inctx = document.getElementById('idInsideTemp').getContext('2d');
inctx.canvas.width = 510;
inctx.canvas.height = 200;

var color = Chart.helpers.color;
var intempoptions = {
    type: 'line',
    data: {
         datasets: [
             { label: 'Inside Temperature °F',
               backgroundColor: color('#009933').alpha(0.5).rgbString(),
               borderColor: '#009933',
               data: intempdata,
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

var chart = new Chart(inctx, intempoptions);
";

        return "<script type=\"text/javascript\">".$js."</script>";
    }

    public function render_data() {
        $series = array( "name" => "Temperature °F",
                         "color" => "#ff0000",
                         "shadow" => "true",
                         "data" => $this->json_data);
        return json_encode($series);
    }
}
?>
