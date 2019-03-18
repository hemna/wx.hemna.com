<?php
/**
 *
 */
abstract class myD3Graph {

	protected $date = null;

    /**
     * Class Constructor
     *
     */
    public function __construct() {
        $this->current_date = date("Y-m-d");
        $this->date = Request::singleton()->get("date", date("Y-m-d"));
        if (empty($this->date) || is_null($this->date)) {
        	$this->date = date("Y-m-d");
        }

		$ts = strtotime($this->date);
		$this->year = date("Y", $ts);
		$this->month = date("m", $ts);
		$this->day = date('j',$ts);
		Log::Singleton()->debug("date=".$this->date." year = ".$this->year." month = ".$this->month." day = ".$this->day);

        $this->load_data();
    }

    abstract public function render();

    abstract protected function load_data();
}
?>
