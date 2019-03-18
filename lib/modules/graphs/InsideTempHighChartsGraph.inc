<?php
/**
 *
 */

class InsideTempHighChartsGraph extends myHighChartsGraph {
    
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
                $value = $wx->get_temp_in();
                $this->temp_data[] =  '['.  $datetime.','.$value.']';
                $this->json_data[] = array($datetime, floatval($value));
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
var itempoptions = {
    chart: {
        type: 'line',
        renderTo: 'idInsideTempLineGraphdiv',
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
                duration: 2200
            },
            marker: { radius: 2 },
        }
    },
    title: {
        text: 'Inside Temperature °F'
    },
    xAxis: {
        type: 'datetime',
        //tickInterval: 60*60*24*100,
        tickPixelInterval: 50,
        tickwidth: 0,
        gridLineWidth: 1,
        /*plotBands: {
            color: '#e6e4e6',
            from: ".$this->sunrise.",
            to: ".$this->sunset.",
        }*/
    },
    yAxis:{
        title: 'Temperature',
        //tickInterval: 1,
        tickPixelInterval: 10,
        tickwidth: 0,
        gridLineWidth: 1,
    },
    series: [{
        data:  [".join($this->temp_data, ',')."],
        name: 'Temperature °F',
        color: '#ff0000',
    }]
}
var insidetemp = new Highcharts.Chart(itempoptions);";

        //data:  [],
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
