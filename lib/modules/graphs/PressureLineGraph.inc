<?php
/**
 *
 */
 
class PressureLineGraph extends SingleDataDateLineGraph {

    /**
     *  The main Title
     *
     * @var unknown_type
     */
    protected $title = "Barometric Pressure";
    
    /**
     * The X axis title
     */
    protected $x_title = "Time";
    
    /**
     * the Y axis title
     *
     * @var unknown_type
     */
    protected $y_title = "in Hg";
    
   
    public function load_data() {
        $today = $this->date;
        $collection = weatherDataObject::find_all("datetime like :date order by datetime asc", array(':date'=> $today.'%'));
        
        $cnt=1;
        $total=0;
        foreach($collection as $wx) {
            /**
             * @param $wx weatherDataObject
             */
            $value = $wx->get_rel_pressure();
            $this->graph_data[] = $value;
            $total = $total + $value;
            $this->graph_average[] = ($total) / $cnt;
            $cnt++;          
            $this->time_data[] = strtotime($wx->get_datetime());            
        }
    }
   

}
?>
