<?php
/**
 *
 */

class RainHighChartsGraph extends myHighChartsGraph {
    
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
                $datetime = strtotime($wx->get_datetime()) * 1000;
                $value = $wx->get_rain_total();
                $this->rain_data[] =  '['.$datetime.','.$value.']';
                $this->rain_json_data[] = array($datetime, floatval($value));

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
        $data = "[".join($this->temp_data, ",")."]";
        $js = "
var rainlineoptions = {
    chart: {
        type: 'line',
        renderTo: 'idRainLineGraphdiv',
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
                duration: 3000
            }
        }
    },
    title: {
        text: 'Daily Rain'
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
        title: 'Rain',
        //tickInterval: .01,
        tickPixelInterval: 10,
        tickwidth: 0,
        gridLineWidth: 1,
    },
    series: [{
        name: 'Rain (inches)',
        color: '#ff0000',
        data:  [".join($this->rain_data, ',')."],
    },
    ]
}
    
var rainlinegraph = new Highcharts.Chart(rainlineoptions)";

        return "<script type=\"text/javascript\">".$js."</script>";
    }

    public function render_data() {
        $series = array("name" => "Rain (inches)",
                        "color" => "#ff0000",
                        "shadow" => "true",
                        "data" => $this->rain_json_data);
        return json_encode($series);
    }
}
?>
