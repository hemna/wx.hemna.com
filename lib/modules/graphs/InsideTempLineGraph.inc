<?php
/**
 *
 */

class InsideTempLineGraph extends myGraph {
    
    protected $temperature_data = array();
    protected $dewpoint_data = array();
    protected $time_data = array();

    protected function init_includes() {
        $this->do_include('log');
        $this->do_include("line");
        $this->do_include("date");
    }
    
    public function load_data() {
        $today = $this->date;
        $collection = weatherDataObject::find_all("datetime like :date order by datetime asc", array(':date'=> $today.'%'));
        
        
        foreach($collection as $wx) {
            /**
             * @param $wx weatherDataObject
             */
            $this->temperature_data[] = $wx->get_temp_in();
            $this->time_data[] = strtotime($wx->get_datetime());            
        }                
    }

    public function render() {

        // Create the graph. These two calls are always required
        $graph = new Graph(510,200,"auto");
        $graph->img->SetImgFormat('gif');
        $graph->SetScale("datlin");
        //$graph->SetY2Scale("lin");
        //$graph->SetTickDensity(TICKD_DENSE, TICKD_VERYSPARSE);       
        

        $graph->SetShadow();
        $graph->SetMargin(40,20,30,80);

        $graph->ygrid->Show(true,true);
        $graph->xgrid->Show(true,true);

        // Create the linear plot
        $lineplot=new LinePlot($this->temperature_data, $this->time_data);
        $lineplot->SetLegend("Temperature ");
        $lineplot->SetColor("red");
        $graph->yaxis->SetColor("red");
        $lineplot->SetWeight(1);

        //$graph->yaxis->scale->ticks->Set(4,20);
        //$graph->yaxis->scale->ticks->SupressFirst();
        //$graph->xaxis->HideTicks(true, false);
        //$graph->xaxis->SetTickLabels($xaxis_labels);
        //$graph->xaxis->SetTextTickInterval(38,20);
        $graph->xaxis->scale->SetDateAlign(DAYADJ_1);
        //$graph->xaxis->scale->SetTimeAlign(HOURADJ_12);
        $graph ->xaxis->scale-> SetDateFormat( 'H:i');
        $graph->xaxis->SetLabelAngle(90);

        // Add the plot to the graph
        $graph->Add($lineplot);

        $graph->title->Set("Inside Temperature");
        //$graph->title->SetPos(300,100);
        $graph->yaxis->title->Set("Farhenheit");

        $graph->title->SetFont(FF_FONT1,FS_BOLD);
        $graph->yaxis->title->SetFont(FF_FONT1,FS_BOLD);
        //$graph->y2axis->title->SetFont(FF_FONT1,FS_BOLD);
        $graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);                

        $graph->legend->Pos(0.5,.94,"center","bottom");
        $graph->legend->SetColumns(3);

        // Display the graph
        $graph->Stroke();
    }
}
?>
