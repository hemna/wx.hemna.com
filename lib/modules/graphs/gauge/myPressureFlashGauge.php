<?php

class myPressureFlashGauge extends myFlashGauge {


	protected $xml = "";
	
	protected $date;
	
	protected $data_object;
	
	public function __construct() {
		parent::__construct();
		$this->xml = <<<EOL

<gauge>

	<!-- barometer design -->
	<rect x='40' y='46' width='56' height='208' fill_color='42423D' line_thickness='8' line_color='333333' />
	<text x='0' y='0' width='136' size='14' align='center' color='999999'>P R E S S U R E</text>
	<text x='12' y='26' width='200' size='14' color='cccccc'>in                     in</text>

<line x1='100' y1='250' x2='104' y2='250' thickness='2' color='666666' /><text x='105' y='242' width='30' align='left' size='12' color='999999'>29.5</text><line x1='100' y1='230' x2='104' y2='230' thickness='2' color='666666' /><text x='105' y='222' width='30' align='left' size='12' color='999999'>29.6</text><line x1='100' y1='210' x2='104' y2='210' thickness='2' color='666666' /><text x='105' y='202' width='30' align='left' size='12' color='999999'>29.7</text><line x1='100' y1='190' x2='104' y2='190' thickness='2' color='666666' /><text x='105' y='182' width='30' align='left' size='12' color='999999'>29.8</text><line x1='100' y1='170' x2='104' y2='170' thickness='2' color='666666' /><text x='105' y='162' width='30' align='left' size='12' color='999999'>29.9</text><line x1='100' y1='150' x2='104' y2='150' thickness='2' color='666666' /><text x='105' y='142' width='30' align='left' size='12' color='999999'>30</text><line x1='100' y1='130' x2='104' y2='130' thickness='2' color='666666' /><text x='105' y='122' width='30' align='left' size='12' color='999999'>30.1</text><line x1='100' y1='110' x2='104' y2='110' thickness='2' color='666666' /><text x='105' y='102' width='30' align='left' size='12' color='999999'>30.2</text><line x1='100' y1='89.999999999998' x2='104' y2='89.999999999998' thickness='2' color='666666' /><text x='105' y='81.999999999998' width='30' align='left' size='12' color='999999'>30.3</text><line x1='100' y1='69.999999999997' x2='104' y2='69.999999999997' thickness='2' color='666666' /><text x='105' y='61.999999999997' width='30' align='left' size='12' color='999999'>30.4</text><line x1='32' y1='250' x2='36' y2='250' thickness='2' color='666666' /><text x='0' y='242' width='30' align='right' size='12' color='999999'>29.5</text><line x1='32' y1='230' x2='36' y2='230' thickness='2' color='666666' /><text x='0' y='222' width='30' align='right' size='12' color='999999'>29.6</text><line x1='32' y1='210' x2='36' y2='210' thickness='2' color='666666' /><text x='0' y='202' width='30' align='right' size='12' color='999999'>29.7</text><line x1='32' y1='190' x2='36' y2='190' thickness='2' color='666666' /><text x='0' y='182' width='30' align='right' size='12' color='999999'>29.8</text><line x1='32' y1='170' x2='36' y2='170' thickness='2' color='666666' /><text x='0' y='162' width='30' align='right' size='12' color='999999'>29.9</text><line x1='32' y1='150' x2='36' y2='150' thickness='2' color='666666' /><text x='0' y='142' width='30' align='right' size='12' color='999999'>30</text><line x1='32' y1='130' x2='36' y2='130' thickness='2' color='666666' /><text x='0' y='122' width='30' align='right' size='12' color='999999'>30.1</text><line x1='32' y1='110' x2='36' y2='110' thickness='2' color='666666' /><text x='0' y='102' width='30' align='right' size='12' color='999999'>30.2</text><line x1='32' y1='89.999999999998' x2='36' y2='89.999999999998' thickness='2' color='666666' /><text x='0' y='81.999999999998' width='30' align='right' size='12' color='999999'>30.3</text><line x1='32' y1='69.999999999997' x2='36' y2='69.999999999997' thickness='2' color='666666' /><text x='0' y='61.999999999997' width='30' align='right' size='12' color='999999'>30.4</text>

	<!-- barometer pointer animated only if pressure has changed -->
	<scale x='44' y='250' start_scale='29.5' end_scale='25' direction='vertical' step='1' shake_span='0' shake_frequency='0' shadow_alpha='0'>
		<rect x='44' y='50' width='48' height='200' fill_color='82827D' />
	</scale>

	<!-- current temp text -->
        <text x='30' y='260' width='76' size='14' align='center' color='cccccc'>XX_CURR_XXin</text>

	<!-- high / low watermarks animated only if pressure has changed -->

	<move start_y_offset='0' end_y_offset=XX_HIGH_XX' shake_frequency='0'>
		<line x1='40' y1='251' x2='44' y2='251' thickness='2' color='82827D' />
		<line x1='92' y1='251' x2='96' y2='251' thickness='2' color='82827D' />
	</move>
	<move start_y_offset='0' end_y_offset='XX_LOW_XX' shake_frequency='0'>
		<line x1='40' y1='251' x2='44' y2='251' thickness='2' color='82827D' />
		<line x1='92' y1='251' x2='96' y2='251' thickness='2' color='82827D' />
	</move>

	<update url='/index.php?target=my-pressure-flash-gauge' delay='10' delay_type='1' />

</gauge>	
EOL;
	}
	
	protected function load_data() {
	    $today = date("Y-m-d");
        //var_dump($today);
        //$today = "2009-02-15";
        $this->date = $today;
        $this->do = weatherDataObject::find("1=1 order by datetime desc limit 0,1");
        
        $this->getMinMaxPressure($today);
	}
	
    protected function getMinMaxPressure($today = null) {
    	if ($today == null) {
    		$today = $today = date("Y-m-d");
    	}
    	$stmt = $this->db->queryBindOneRowCache("Select min(rel_pressure) as low, max(rel_pressure) as high from weather where datetime like :date", 
                                          array(":date" => $today.'%'));
        $this->low_high["low"] = $stmt->low;
        $this->low_high["high"] = $stmt->high;
        
        return $this->temp_low_high; 
    }
	
	protected function get_high() {
		$high = $this->low_high["high"];
		
		return $this->translate_num($high);
	}
	
	protected function get_low() {
		$low = $this->low_high["low"];
	
		return $this->translate_num($low);
	}
	
	protected function translate_num($num) {
		$absolute_low = -1;
		$absolute_high = -200;

		return (-2*$num);
	}
	
	public function render() {			
		$search_arr = array("XX_CURR_XX", "XX_HIGH_XX", "XX_LOW_XX");
		$replace_arr = array($this->do->get_rel_pressure(), $this->get_high(), $this->get_low());

		$output = str_replace($search_arr, $replace_arr, $this->xml);
		echo $output;
	}
}
?>
