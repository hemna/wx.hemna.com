<?php
/**
 *
 */
 
abstract class myGraph {
	
	protected $date = null;

    /**
     * Class Constructor
     *
     */
    public function __construct() {        
        //do the nasty includes?
        //include ("external/jpgraph-2.2/src/jpgraph.php");
        include ("external/jpgraph-3.5.0b1/src/jpgraph.php");
        $this->init_includes(); 
        
        $this->date = Request::singleton()->get("date", date("Y-m-d"));
        if (empty($this->date) || is_null($this->date)) {
        	$this->date = date("Y-m-d");
        }
        
        $this->load_data();          
    }
    
    public function render() {
            
    }
    
    protected function do_include($type) {
        //include("external/jpgraph-2.2/src/jpgraph_".$type.".php");
        include("external/jpgraph-3.5.0b1/src/jpgraph_".$type.".php");
    }
    
    abstract protected function init_includes();

    abstract protected function load_data();
    
}
?>
