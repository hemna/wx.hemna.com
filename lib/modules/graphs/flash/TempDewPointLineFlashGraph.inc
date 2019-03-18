<?php
/**
 *
 */

class TempDewPointLineFlashGraph extends myFlashGraph {
    
    protected $temperature_data = array();
    protected $dewpoint_data = array();
    protected $time_data = array();

    protected function init_includes() {
        $this->do_include('OFC_Chart.php');
    }
    
    public function load_data() {
        $today = date("Y-m-d");
        //var_dump($today);
        //$today = "2008-01-16";
        $this->date = $today;
        $collection = weatherDataObject::find_all("datetime like :date order by datetime asc", array(':date'=> $today.'%'));

        $cnt=1;
        $this->temperature_data = array();
        $this->temp_values = array();
        $this->dew_values = array();
        foreach($collection as $wx) {

            $this->temp_values[] = (float)$wx->get_temp_out();
            $dot_value = new OFC_Elements_Dot_Value((float)$wx->get_temp_out());
            $dot_value->set_tooltip("#val# F<br>".$wx->get_datetime());
            $this->temperature_data[] = $dot_value;

            $dew_value = new OFC_Elements_Dot_Value((float)$wx->get_dewpoint());
            $dew_value->set_tooltip("#val# F<br>".$wx->get_datetime());
            $this->dewpoint_data[] = $dew_value;
            $this->dew_values[] = (float)$wx->get_dewpoint();
            
            $this->time_data[] = $wx->get_datetime();  
        }
    }

    public function render() {
        
    	$title = new OFC_Elements_Title( $this->date );

		$line_1 = new OFC_Charts_Line_Dot();
		$line_1->set_values( $this->temperature_data );
		//$line_1->set_values( $data_1 );
		$line_1->set_halo_size( 0 );
		$line_1->set_width( 2 );
		$line_1->set_dot_size( 1 );
		$line_1->set_key("Temperature", 10);
		
		
		$line_2 = new OFC_Charts_Line_Dot();
		$line_2->set_values( $this->dewpoint_data );
		$line_2->set_halo_size( 0 );
		$line_2->set_width( 1 );
		$line_2->set_dot_size( 1 );
		$line_2->set_colour("#FF0000");
		$line_2->set_key("Dew Point", 10);
				
		$y = new OFC_Elements_Axis_Y();
		$y->set_range( min($this->temp_values)-6, max($this->temp_values)*1.1,  10);
		$y->set_steps(2);
		$x = new OFC_Elements_Axis_X();
		$x->set_steps(60);
		$x->set_tick_height(5);
		$time_arr = array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23);

//		$x->set_labels($time_arr);		
		$x->set_labels_from_array($this->time_data);		
		
		$chart = new OFC_Chart();
		$chart->set_title( $title );
		$chart->add_element( $line_1 );
		$chart->add_element( $line_2 );
		$chart->set_y_axis( $y );
		$chart->set_x_axis( $x );
		
		echo $chart->toPrettyString();   
    }
}
?>
