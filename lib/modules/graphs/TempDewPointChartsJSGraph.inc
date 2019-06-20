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
            $collection = weatherDataObject::find_all("datetime like :date order by datetime asc",
                                                      array(':date'=> $today.'%'));

            foreach($collection as $wx) {
                /**
                 * @param $wx weatherDataObject
                 */
                $datetime = strtotime($wx->get_datetime()) * 1000;
                //$date = date("m-d-Y H:i:s", $wx->get_datetime());
                $date = $wx->get_datetime();
                $temp_value = $wx->get_temp_out();
                $this->temp_data[] =  "{t:".$datetime.",y:".$temp_value."}";
                $this->temp_json_data[] = array("t"=>$datetime,
                                                "y"=>floatval($temp_value));

                $dewp_value = $wx->get_dewpoint();
                $this->dewp_data[] = '['.$datetime.','.$dewp_value.']';
                $this->dewp_json_data[] = array($datetime, floatval($dewp_value));
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
        //$data = "[".join($this->temp_data, ",")."]";
//var tempdewdata = [".join($this->temp_data, ',')."];
        $js = "
var tempdewdata = [".join($this->temp_data[0], ',')."];
var tempdewpointoptions = {
    type: 'bar',
    data: {
        datasets: [{
            label: 'Temperature °F',
            backgroundColor: '#ffffff',
            borderColor: '#ff0000',
            data: tempdewdata,
            type: 'line',
            pointRadius: 0,
            fill: false,
            lineTension: 0,
            borderWidth: 2
        }],
    options: {
        scales: {
           xAxes: [{
               type: 'time',
               distribution: 'series',
           }],
           yAxes: [{
               scaleLabel: {
                   display: true,
                   labelString: 'Temperature °F',
               }
           }],
        },
        tooltips: {
            interect: false,
			mode: 'index',
			callbacks: {
				label: function(tooltipItem, myData) {
					var label = myData.datasets[tooltipItem.datasetIndex].label || '';
					if (label) {
						label += ': ';
					}
					label += parseFloat(tooltipItem.value).toFixed(2);
					return label;
				}
			}
		 }
      }
    }
}

ctx = document.getElementById('idTempDewPoint').getContext('2d');
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
