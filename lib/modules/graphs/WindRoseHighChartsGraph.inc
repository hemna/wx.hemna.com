<?php
/**
 *
 */

class WindRoseHighChartsGraph extends myHighChartsGraph {
    
    protected $temp_data = array();

    public function load_data() {
        $today = $this->date;
        $cache_key = get_class($this);
        $cache_data = $this->cache->get($cache_key);
        if ($cache_data === false) {
            $collection = weatherDataObject::find_all("datetime like :date",
                                                      array(':date'=> $today.'%'),
                                                      'order by datetime desc',
                                                      'limit 0,10');

            foreach($collection as $wx) {
                /**
                 * @param $wx weatherDataObject
                 */
                $datetime = strtotime($wx->get_datetime()) * 1000;
                $value = $wx->get_wind_angle();
                //$this->wind_data[] =  '['.$datetime.','.$value.']';
                $this->wind_data[] = $value;

            }
            $cache_data = array('wind_data' => $this->wind_data,
                                'graph_avg' => $this->graph_average);
            $this->cache->set($cache_key, $cache_data, $this->cache_lifetime);
        } else {
            $this->wind_data = $cache_data['wind_data'];
            $this->graph_average = $cache_data['graph_avg'];
        }
    }

    public function render_graph() {
        $js = "
var windrosegraph = new Highcharts.Chart({
    chart: {
        polar: true,
        renderTo: 'idWindRoseGraphdiv',
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
    pane: {
        startAngle: 0,
        endAngle: 360
    },
    title: {
        text: 'Wind Speed'
    },
    xAxis: {
        categories:
            ['N', 'NNE', 'NE', 'ENE', 'E', 'ESE',
             'SE', 'SSE', 'S', 'SSW', 'SW', 'WSW',
             'W', 'WNW', 'NW', 'NNW'],
        lables: {
            formatter: function() {
                return this.value + '°'
            }
        }
    },
    yAxis:{
        title: 'Wind',
        min: 0,
        max: 360,
        tickInterval: 45,
    },
    plotOptions: {
        series: {
            animation: {
                duration: 2200
            }
        }
        column: {
            pointPadding: 0,
            groupPadding: 0
        }
    },
    series: [{
        name: 'Wind Speed (mph)',
        type: 'column',
        color: '#0000ff',
        data:  [".join($this->wind_data, ',')."],
    },
    ]
});";

        return "<script type=\"text/javascript\">".$js."</script>";
    }

    public function render_data() {
        return json_encode($this->wind_data);
    }
}
?>
