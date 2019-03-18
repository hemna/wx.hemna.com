<?php
/**
 *
 */
 
class RainLineGraph extends SingleDataDateLineGraph {

    /**
     *  The main Title
     *
     * @var unknown_type
     */
    protected $title = "Rain";
    
    /**
     * The X axis title
     */
    protected $x_title = "Time";
    
    /**
     * the Y axis title
     *
     * @var unknown_type
     */
    protected $y_title = "inches";
    
    /**
     * The line color
     *
     * @var unknown_type
     */
    protected $line_color = "red";
   
    public function load_data() {
        $today = $this->date;
        $collection = weatherDataObject::find_all("datetime like :date order by datetime asc", array(':date'=> $today.'%'));
        
        $offset = 0;
        foreach($collection as $wx) {
            /**
             * @param $wx weatherDataObject
             */
            if ($offset == 0) {
                $offset = $wx->get_rain_total();
            }
            $this->graph_data[] = $wx->get_rain_total() - $offset;            
            $this->time_data[] = strtotime($wx->get_datetime());            
        }                
    }

}
?>
