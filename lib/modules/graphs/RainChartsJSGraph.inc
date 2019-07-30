<?php
/**
 *
 */

class RainChartsJSGraph extends myChartsJSGraph {
    
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
                $value = $wx->get_rain_total();
                $this->rain_data[] =  "{t: '".$date."', y: '".$value."'}";
                $this->rain_json_data[] = array("t"=>$date, "y"=>floatval($value));

            }
            $cache_data = array('rain_data' => $this->rain_data,
                                'rain_json_data' => $this->rain_json_data);
            $this->cache->set($cache_key, $cache_data, $this->cache_lifetime);
        } else {
            $this->rain_data = $cache_data['rain_data'];
            $this->rain_json_data = $cache_data['rain_json_data'];
        }
    }

    public function render_graph() {
        $js = "
var raindata = [ ".join($this->rain_data, ', ')." ];
rainctx = document.getElementById('idRainLineGraph').getContext('2d');
rainctx.canvas.width = 510;
rainctx.canvas.height = 215;

var color = Chart.helpers.color;
var rainoptions = {
    type: 'line',
    data: {
         datasets: [
	     { label: 'Rain',
               backgroundColor: color('#333399').alpha(0.5).rgbString(),
	       borderColor: '#333399',
               data: raindata,
	       showLine: false,
               pointRadius: .5,
               borderWidth: 1 },
	     { label: 'Rain',
               backgroundColor: color('#333399').alpha(0.5).rgbString(),
	       borderColor: '#333399',
               type: 'bar',
               data: raindata,
	       showLine: false,
               pointRadius: .5,
               borderWidth: 1 },
        ]
    },
    options: {
	animation: {
            duration: 5000
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
                   labelString: 'Rain inches',
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

var rainc = new Chart(rainctx, rainoptions);
";

        return "<script type=\"text/javascript\">".$js."</script>";
    }


    public function render_data() {
        $series = array("name" => "Rain (inches)",
                        "data" => $this->rain_json_data);
        return json_encode($series);
    }
}
?>
