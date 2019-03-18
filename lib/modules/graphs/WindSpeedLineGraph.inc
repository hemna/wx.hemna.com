<?php
/**
 *
 */
 
class WindSpeedLineGraph extends SingleDataDateScatterGraph {

    /**
     *  The main Title
     *
     * @var unknown_type
     */
    protected $title = "Wind Speed";
    
    /**
     * The X axis title
     */
    protected $x_title = "Time";
    
    /**
     * the Y axis title
     *
     * @var unknown_type
     */
    protected $y_title = "mph";

    
   protected function init_includes() {
       parent::init_includes();
        $this->do_include("scatter");
    }
   
    public function load_data() {
        $today = $this->date;
        $collection = weatherDataObject::find_all("datetime  like :date order by datetime asc", array(':date'=> $today.'%'));
        
        /**
           * @var weatherDataObject
           */
        $cnt=1;
        $total=0;        
        foreach($collection as $wx) {            
            $speed = $wx->get_wind_speed();
            
            $this->graph_data[] = $speed;
            
            $total = $total + $speed;
            $this->graph_average[] = ($total) / $cnt;
            $cnt++;
            
            $this->direction_data[] = $wx->get_wind_angle();            
            $this->time_data[] = strtotime($wx->get_datetime());            
        }

    }    

}
?>