<?php
/**
 *
 */

class TempDewPointHighChartsGraph extends myHighChartsGraph {
    
    protected $temp_data = array();
    protected $dewp_data = array();

    public function load_data() {
        $today = $this->date;
        $cache_key = "TempDewPointHighChartsGraphData_".$today;
        $cache_data = $this->cache->get($cache_key);
        if ($cache_data === false) {
            $collection = weatherDataObject::find_all(
                "datetime like :date",
                array(':date'=> $today.'%'),
                "order by datetime asc"
            );

            foreach($collection as $wx) {
                /**
                 * @param $wx weatherDataObject
                 */
                $datetime = strtotime($wx->get_datetime()) * 1000;
                $temp_value = $wx->get_temp_out();
                $this->temp_data[] =  '['.  $datetime.','.$temp_value.']';
                $this->temp_json_data[] = array($datetime, floatval($temp_value));

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
        $data = "[".join($this->temp_data, ",")."]";
        $js = "
var tempdewpointoptions = {
    chart: {
          type: 'spline',
          renderTo: 'idTempDewPointLineGraphdiv',
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
          },
    },
    plotOptions: {
        series: {
            animation: {
                duration: 2000
            },
            marker: { radius: 3 },
        }
    },
    title: {
        text: 'Temperature'
    },
    xAxis: {
        type: 'datetime',
        //tickInterval: 60*60*24*100,
        tickPixelInterval: 50,
        tickwidth: 1,
        gridLineWidth: 1,
        /*plotBands: {
            color: '#e6e4e6',
            from: ".($this->sunrise*1000).",
            to: ".($this->sunset*1000).",
        }*/
    },
    yAxis:{
        title: 'Temperature',
        //tickInterval: 2,
        tickPixelInterval: 10,
        tickwidth: 0,
        gridLineWidth: 1,
    },
    series: [{
        name: 'Temperature °F',
        color: '#ff0000',
        data:  [".join($this->temp_data, ',')."],
    },
    {
        name: 'Dewpoint °F',
        color: '#006600',
        data: [".join($this->dewp_data, ',')."],
    }]
}

var tempdewpoint = new Highcharts.Chart(tempdewpointoptions);
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
        return json_encode($series);
    }
}
?>
