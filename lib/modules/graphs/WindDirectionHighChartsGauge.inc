<?php
/**
 *
 */

class WindDirectionHighChartsGauge extends myHighChartsGraph {
    
    protected $temp_data = array();

    public function load_data() {
        $today = $this->date;
		$wx = NULL;
		try {
            $wx = weatherDataObject::find("datetime like :date order by datetime desc",
                                          array(':date'=> $today.'%'));
            $this->wind_angle = $wx->get_wind_angle()+180;
		} catch (Exception $ex) {
		    $this->wind_angle = 0;
		}
    }

    public function render_graph() {
        $js = "
var langWindDir = new Array(\"N\", \"NNE\", \"NE\", \"ENE\",\"E\", \"ESE\", \"SE\", \"SSE\",
                            \"S\", \"SSW\", \"SW\", \"WSW\",\"W\", \"WNW\", \"NW\", \"NNW\");
function windDirLang (winddir) {
    return langWindDir[Math.floor(((parseInt(winddir,10) + 11.25) / 22.5))];
}

var winddiroptions = {
    chart: {
        type: 'gauge',
        renderTo: 'idWindDirGaugediv',
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false,
    },
    pane: {
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
        text: 'Wind Direction'
    },
    credits: { 
        enabled: false
    },
    yAxis: {
        min: 0,
        max: 360,
               
        /*minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',*/
       
        tickInterval: 22.5,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 3,
        tickColor: '#666',
        labels: {
            step: 2,
            rotation: 'auto',
            formatter: 
                function() {
                    return windDirLang(this.value);
                }
        },
        title: {
            text: ''
        },                 
    },
    series: [{
        name: 'Wind Direction',
        color: '#0000ff',
        data:  [".$this->wind_angle."],
        },
    ]
    }

var winddirgraph = new Highcharts.Chart(winddiroptions);
";

        return "<script type=\"text/javascript\">".$js."</script>";
    }

    public function render_data() {
        return json_encode(array(
            array("name" => "Wind Direction",
                  "color" => "#0000ff",
                  "data" => array((float)$this->wind_angle)
              )));
    }
}
?>
