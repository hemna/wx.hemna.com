<?php
/**
 *
 */

class PressureHighChartsGraph extends myHighChartsGraph {
    
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
                $value = $wx->get_rel_pressure();
                $total = $total + $value;
                $avg = ($total) / $cnt;
                $this->graph_average[] = '['.$datetime.','.$avg.']'; 
                $this->average_json_data[] = array($datetime, floatval($avg));
                $cnt++;
                $this->pressure_data[] =  '['.$datetime.','.$value.']';
                $this->pressure_json_data[] = array($datetime, floatval($value));

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
var pressureoptions = {
    chart: {
        type: 'line',
        renderTo: 'idPressureLineGraphdiv',
        zoomType: 'x',
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
                duration: 2400
            },
            marker: { radius: 2 },
        }
    },
    title: {
        text: 'Barometric Pressure'
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
        title: 'Pressure',
        //tickInterval: .01,
        tickPixelInterval: 10,
        tickwidth: 0,
        gridLineWidth: 1,
        startOnTick: false,
    },
    series: [{
        name: 'Pressure',
        color: '#0000ff',
        data:  [".join($this->pressure_data, ',')."],
    },
    {
        name: 'Average',
        color: '#ff0000',
        data: [".join($this->graph_average, ',')."],
    }
    ]
}

var pressuregraph = new Highcharts.Chart(pressureoptions);
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
