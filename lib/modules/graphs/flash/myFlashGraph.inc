<?php
/**
 *
 */
 
abstract class myFlashGraph {

    /**
     * Class Constructor
     *
     */
    public function __construct() {
        
        //do the nasty includes?
        //include ("external/jpgraph-2.2/src/jpgraph.php");
        $this->init_includes(); 
        $this->load_data();          
    }
    
    public function render() {
            
    }
    
    protected function do_include($type) {
        require_once("OFC/".$type);
    }
    
    abstract protected function init_includes();

    abstract protected function load_data();
    
}
?>