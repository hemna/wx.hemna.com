<?php
/**
 *
 */

class WindSpeedHighChartsGauge extends myHighChartsGraph {
    
    protected $temp_data = array();

    public function load_data() {
        $today = $this->date;
		try {
            $wx = weatherDataObject::find("datetime like :date order by datetime desc",
                                          array(':date'=> $today.'%'));
            $this->wind_speed = $wx->get_wind_speed();
		} catch (Exception $ex) {
            $this->wind_speed = 0;
		}

    }

    public function render_graph() {
        $js = "
var windspeedoptions = {
    chart: {
        type: 'gauge',
        renderTo: 'idWindSpeedGaugediv',
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false,
    },
    pane: {
        startAngle: -150,
        endAngle: 150,
        background: [{
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF'],
                    [1, '#333']
                ]
            },
            borderWidth: 0,
            outerRadius: '109%'
        }, {
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#333'],
                    [1, '#FFF']
                ]
            },
            borderWidth: 1,
            outerRadius: '107%'
        }, {
            // default background
        }, {
            backgroundColor: '#DDD',
            borderWidth: 0,
            outerRadius: '105%',
            innerRadius: '103%'
        }]
    },
    title: {
        text: 'Wind Speed'
    },
    credits: { 
        enabled: false
    },
    yAxis: {
        min: 0,
        max: 40,
        minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',

        tickPixelInterval: 20,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 10,
        tickColor: '#666',
        labels: {
            step: 2,
            rotation: 'auto',
        },
        title: {
            text: ''
        },   
        plotBands: [{
            from: 0,
            to: 15,
            color: '#55BF3B'
          }, {
            from : 15,
            to: 30,
            color: '#DDDF0D'
          }, {
            from : 30,
            to: 40,
            color: '#DF5353'
          }]
    },
    series: [{
        name: 'Wind Speed',
        color: '#0000ff',
        data:  [".$this->wind_speed."],
        tooltip: {
            valueSuffix: ' mph'
        }
        },
    ]
    }

var windspeedgraph = new Highcharts.Chart(windspeedoptions);
";

        return "<script type=\"text/javascript\">".$js."</script>";
    }

    public function render_data() {
        return json_encode(array(
            array("name"=> "Wind Speed",
                  "color" => "#0000ff",
                  "data" => array((float)$this->wind_speed),
                  "tooltip" => array("valueSuffix"=>"mph"))
              ));
    }
}
?>
