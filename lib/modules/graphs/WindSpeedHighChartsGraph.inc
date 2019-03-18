<?php
/**
 *
 */

class WindSpeedHighChartsGraph extends myHighChartsGraph {
    
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
                $datetime = strtotime($wx->get_datetime()) * 1000;
                $value = $wx->get_wind_speed();
                $total = $total + $value;
                $avg = ($total) / $cnt;
                $this->graph_average[] = '['.$datetime.','.$avg.']'; 
                $this->avg_json_data[] = array($datetime, floatval($avg));
                $cnt++;
                $this->wind_data[] =  '['.$datetime.','.$value.']';
                $this->wind_json_data[] = array($datetime, floatval($value));

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
var windlineoptions = {
    chart: {
        //type: 'line',
        renderTo: 'idWindLineGraphdiv',
        borderWidth: 1,
        marginBottom: 50,
        marginTop: 32,
        plotShadow: true,
        plotBackgroundColor: '#ffffff', 
        backgroundColor: {
            linearGradient: [0,0,500,500],
            stops: [
                [0, '#ffffff'],
                [1, '#cccacc']
            ]
        }
    },
    plotOptions: {
        series: {
            animation: {
                duration: 2800
            }
        }
    },
    title: {
        text: 'Wind Speed'
    },
    xAxis: {
        type: 'datetime',
        //tickInterval: 60*60*24*100,
        tickPixelInterval: 50,
        tickwidth: 0,
        gridLineWidth: 1,
        /*plotBands: {
            color: '#e6e4e6',
            from: ".($this->sunrise*1000).",
            to: ".($this->sunset*1000).",
        }*/
    },
    yAxis:{
        title: 'Wind',
        //tickInterval: 2,
        tickPixelInterval: 10,
        tickwidth: 0,
        gridLineWidth: 1,
        startOnTick: false,
    },
    series: [{
        name: 'Wind Speed (mph)',
        type: 'scatter',
        color: '#0000ff',
        marker: { radius: 2 },
        data:  [".join($this->wind_data, ',')."],
    },
    {
        name: 'Average',
        type: 'line',
        color: '#ff0000',
        data: [".join($this->graph_average, ',')."],
    }
    ]
}
    
var windlinegraph = new Highcharts.Chart(windlineoptions);";

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
