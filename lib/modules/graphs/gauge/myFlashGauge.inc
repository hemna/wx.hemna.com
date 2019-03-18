<?php
/**
 *
 */
 
abstract class myFlashGauge {

	protected $db = null;

    /**
     * Class Constructor
     *
     */
    public function __construct() {
    	$this->db = open2300DB::singleton(); 
        $this->load_data();          
    }
    
    abstract public function render();       
    abstract protected function load_data();    
    
}
?>