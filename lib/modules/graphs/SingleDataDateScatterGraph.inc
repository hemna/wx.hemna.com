<?php
/**
 *
 */
 
abstract class SingleDataDateScatterGraph extends myGraph {
    
    /**
     *  The main Title
     *
     * @var unknown_type
     */
    protected $title;
    
    /**
     * The X axis title
     */
    protected $x_title;
    
    /**
     * the Y axis title
     *
     * @var unknown_type
     */
    protected $y_title;
    
    /**
     * The line color
     *
     * @var unknown_type
     */
    protected $line_color = "blue";
    
    /**
     * This holds the loaded data to graph
     *
     * @var array
     */
    protected $graph_data = array();
    
    /**
     * This holds the date values 
     * associated with the graph data
     *
     * @var array
     */
    protected $time_data = array();
    
    protected function init_includes() {
        $this->do_include('log');
        $this->do_include("line");
        $this->do_include("date");
    }
    
     public function render() {

        // Create the graph. These two calls are always required
        $graph = new Graph(510,200,"auto");
        $graph->img->SetImgFormat('gif');
        $graph->SetScale("datlin");
        //$graph->SetTickDensity(TICKD_DENSE, TICKD_VERYSPARSE);       
        

        $graph->SetShadow();
        $graph->SetMargin(60,20,30,50);

        $graph->ygrid->Show(true,true);
        $graph->xgrid->Show(true,true);

        // Create the linear plot
        $lineplot=new ScatterPlot($this->graph_data, $this->time_data);        
        $lineplot->SetColor($this->line_color);
        $lineplot->SetWeight(1);

        $graph->xaxis->scale->SetDateAlign(DAYADJ_1);
        $graph ->xaxis->scale-> SetDateFormat( 'H:i');
        $graph->xaxis->SetLabelAngle(90);

        // Add the plot to the graph
        $graph->Add($lineplot);
        
        //see if we need to render an 'average' line plot
        if (isset($this->graph_average)) {
            $lineplot->SetLegend($this->title);
            
            //adjust the right Y margin
            $graph->SetMargin(40,20,30,80);
            $avgplot = new LinePlot($this->graph_average, $this->time_data);
            $avgplot->SetLegend("Average");
            $avgplot->SetColor('red');
            $avgplot->SetWeight(2);
            $graph->Add($avgplot);            
        }
        

        $graph->title->Set($this->title);
        $graph->yaxis->title->Set($this->y_title);
        $graph->yaxis->SetColor($this->line_color);
        $graph->yaxis->SetTitleMargin(45);

        $graph->title->SetFont(FF_FONT1,FS_BOLD);
        $graph->yaxis->title->SetFont(FF_FONT1,FS_BOLD);
        $graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);                

        $graph->legend->Pos(0.5,.90,"center","center");
        $graph->legend->SetColumns(3);

        // Display the graph
        $graph->Stroke();
     }

}
?>