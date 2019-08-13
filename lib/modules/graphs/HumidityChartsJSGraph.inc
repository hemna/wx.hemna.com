<?php
/**
 *
 */

class HumidityChartsJSGraph extends myChartsJSGraph {

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
                $value = $wx->get_rel_hum_out();
                $this->humid_data[] =  "{t: '".$date."', y:'".$value."'}";
                $this->json_data[] = array("t" => $date, "y" => floatval($value));
            }
            $cache_data = array('humid_data' => $this->humid_data,
                                'json_data' => $this->json_data);
            $this->cache->set($cache_key, $cache_data, $this->cache_lifetime);
        } else {
            $this->temp_data = $cache_data['humid_data'];
            $this->json_data = $cache_data['json_data'];
        }
    }

    public function render_graph() {
        $js = "
var humiddata = [ ".join($this->humid_data, ', ')." ];
humctx = document.getElementById('idHumidityGraph').getContext('2d');
humctx.canvas.width = 510;
humctx.canvas.height = 215;

var color = Chart.helpers.color;
var humidoptions = {
    type: 'line',
    data: {
         datasets: [
             { label: 'Humidity %',
               backgroundColor: color('#009933').alpha(0.5).rgbString(),
               borderColor: '#009933',
               data: humiddata,
               type: 'line',
               pointRadius: 0,
               fill: false,
               lineTension: 0,
               borderWidth: 1 },
        ]
    },
    options: {
	animation: {
	    duration: 2000,
            easing: 'easeInExpo'
        },
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
                   labelString: 'Humidity %',
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

var humidchart = new Chart(humctx, humidoptions);
";

        return "<script type=\"text/javascript\">".$js."</script>";
    }

    public function render_data() {
        $series = array( "name" => "Humidity",
                         "data" => $this->json_data);
        return json_encode($series);
    }
}
?>
