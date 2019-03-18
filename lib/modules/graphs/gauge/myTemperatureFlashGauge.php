<?php

class myTemperatureFlashGauge extends myFlashGauge {


	protected $xml = "";
	
	protected $date;
	
	protected $data_object;
	
	public function __construct() {
	parent::__construct();
	
	$ts = time();
	$this->xml = <<<EOL
<gauge>

	<!-- thermometer design -->
	<rect x='40' y='46' width='56' height='208' fill_color='000000' line_thickness='8' line_color='333333' />
	<text x='0' y='0' width='136' size='14' align='center' color='999999'>T E M P</text>
	<text x='12' y='26' width='200' size='14' color='cccccc'>°C                    °F</text>

	<line x1='100' y1='230' x2='104' y2='230' thickness='2' color='666666' /><text x='105' y='222' width='30' align='left' size='12' color='999999'>30</text>
	<line x1='100' y1='210' x2='104' y2='210' thickness='2' color='666666' /><text x='105' y='202' width='30' align='left' size='12' color='999999'>40</text>
	<line x1='100' y1='190' x2='104' y2='190' thickness='2' color='666666' /><text x='105' y='182' width='30' align='left' size='12' color='999999'>50</text>
	<line x1='100' y1='170' x2='104' y2='170' thickness='2' color='666666' /><text x='105' y='162' width='30' align='left' size='12' color='999999'>60</text>
	<line x1='100' y1='150' x2='104' y2='150' thickness='2' color='666666' /><text x='105' y='142' width='30' align='left' size='12' color='999999'>70</text>
	<line x1='100' y1='130' x2='104' y2='130' thickness='2' color='666666' /><text x='105' y='122' width='30' align='left' size='12' color='999999'>80</text>
	<line x1='100' y1='110' x2='104' y2='110' thickness='2' color='666666' /><text x='105' y='102' width='30' align='left' size='12' color='999999'>90</text>
	<line x1='100' y1='90' x2='104' y2='90' thickness='2' color='666666' /><text x='105' y='82' width='30' align='left' size='12' color='999999'>100</text>
	<line x1='100' y1='70' x2='104' y2='70' thickness='2' color='666666' /><text x='105' y='62' width='30' align='left' size='12' color='999999'>110</text>
	
	<line x1='32' y1='226' x2='36' y2='226' thickness='2' color='666666' /><text x='0' y='218' width='30' align='right' size='12' color='999999'>0</text>
	<line x1='32' y1='208' x2='36' y2='208' thickness='2' color='666666' /><text x='0' y='200' width='30' align='right' size='12' color='999999'>5</text>
	<line x1='32' y1='190' x2='36' y2='190' thickness='2' color='666666' /><text x='0' y='182' width='30' align='right' size='12' color='999999'>10</text><line x1='32' y1='172' x2='36' y2='172' thickness='2' color='666666' /><text x='0' y='164' width='30' align='right' size='12' color='999999'>15</text><line x1='32' y1='154' x2='36' y2='154' thickness='2' color='666666' /><text x='0' y='146' width='30' align='right' size='12' color='999999'>20</text><line x1='32' y1='136' x2='36' y2='136' thickness='2' color='666666' /><text x='0' y='128' width='30' align='right' size='12' color='999999'>25</text><line x1='32' y1='118' x2='36' y2='118' thickness='2' color='666666' /><text x='0' y='110' width='30' align='right' size='12' color='999999'>30</text><line x1='32' y1='100' x2='36' y2='100' thickness='2' color='666666' /><text x='0' y='92' width='30' align='right' size='12' color='999999'>35</text><line x1='32' y1='82' x2='36' y2='82' thickness='2' color='666666' /><text x='0' y='74' width='30' align='right' size='12' color='999999'>40</text><line x1='32' y1='64' x2='36' y2='64' thickness='2' color='666666' /><text x='0' y='56' width='30' align='right' size='12' color='999999'>45</text>

	<!-- thermometer pointer animated only if temperature has changed -->
	<scale x='44' y='250' start_scale='0' end_scale='XX_TEMP_XX' direction='vertical' step='1' shake_span='0' shake_frequency='0' shadow_alpha='0'>
		<rect x='44' y='50' width='48' height='200' fill_color='ff0000' />
	</scale>

	<!-- current temp text -->
        <text x='40' y='260' width='56' size='14' align='center' color='cccccc'>XX_CURR_TEMP_XX°F</text>

	<!-- high / low watermarks animated only if temperature has changed -->

	<move start_y_offset='00' end_y_offset='XX_HIGH_TEMP_XX' shake_frequency='0'>
		<line x1='40' y1='251' x2='44' y2='251' thickness='2' color='ff0000' />
		<line x1='92' y1='251' x2='96' y2='251' thickness='2' color='ff0000' />
	</move>
	<move start_y_offset='0' end_y_offset='XX_LOW_TEMP_XX' shake_frequency='0'>
		<line x1='40' y1='251' x2='44' y2='251' thickness='2' color='487aff' />
		<line x1='92' y1='251' x2='96' y2='251' thickness='2' color='487aff' />
	</move>

	<update url='/index.php?target=my-temperature-flash-gauge&foo=$ts' delay='60' delay_type='1' />

</gauge>	
EOL;
	}
	
	protected function load_data() {
	    $today = date("Y-m-d");
        $this->date = $today;
        $this->do = weatherDataObject::find("1=1 order by datetime desc limit 0,1");
        
        $this->getMinMaxTemps($today);
	}
	
    protected function getMinMaxTemps($today = null) {
    	if ($today == null) {
    		$today = $today = date("Y-m-d");
    	}
    	$stmt = $this->db->queryBindOneRowCache("Select min(temp_out) as low, max(temp_out) as high from weather where datetime like :date", 
                                          array(":date" => $today.'%'));
        $this->temp_low_high["low"] = $stmt->low;
        $this->temp_low_high["high"] = $stmt->high;
        
        return $this->temp_low_high; 
    }
	
    /**
     * get the high temperature
     *
     */
	protected function get_high() {
		$high = $this->temp_low_high["high"];
		
		return $this->translate_num($high);
	}
	
	protected function get_low() {
		$low = $this->temp_low_high["low"];
	
		return $this->translate_num($low);
	}
	
	protected function translate_num($num) {
		$absolute_low = -1;
		$absolute_high = -200;
		
		return (($num-20)*-2);
	}
	
	public function render() {			
		$search_arr = array("XX_CURR_TEMP_XX", "XX_TEMP_XX", "XX_HIGH_TEMP_XX", "XX_LOW_TEMP_XX");
		$replace_arr = array($this->do->get_temp_out(), ($this->do->get_temp_out()-20), 
		                     $this->get_high(), $this->get_low());		                     

		$output = str_replace($search_arr, $replace_arr, $this->xml);
		echo $output;
	}
}
?>