<?php

class myRainFlashGauge extends myFlashGauge {


	protected $xml = "";
	
	protected $date;
	
	protected $data_object;
	
	protected $mode = "";
	protected $rain_amount = 0;
	
	public function __construct() {		
		parent::__construct();

		$ts = time();
	
		$this->xml = <<<EOL
<gauge>

	<!-- rain gauge design -->
	<rect x='40' y='46' width='56' height='208' fill_color='000066' line_thickness='8' line_color='333333' />
	<text x='0' y='0' width='136' size='14' align='center' color='999999'>XX_DESC_XX</text>
	<text x='12' y='26' width='200' size='14' color='cccccc'>in                      in</text>

	<line x1='100' y1='250' x2='104' y2='250' thickness='2' color='666666' />
	<text x='105' y='242' width='30' align='left' size='12' color='999999'>0</text>
	<line x1='100' y1='230' x2='104' y2='230' thickness='2' color='666666' /><text x='105' y='222' width='30' align='left' size='12' color='999999'>1</text><line x1='100' y1='210' x2='104' y2='210' thickness='2' color='666666' /><text x='105' y='202' width='30' align='left' size='12' color='999999'>2</text><line x1='100' y1='190' x2='104' y2='190' thickness='2' color='666666' /><text x='105' y='182' width='30' align='left' size='12' color='999999'>3</text><line x1='100' y1='170' x2='104' y2='170' thickness='2' color='666666' /><text x='105' y='162' width='30' align='left' size='12' color='999999'>4</text><line x1='100' y1='150' x2='104' y2='150' thickness='2' color='666666' /><text x='105' y='142' width='30' align='left' size='12' color='999999'>5</text><line x1='100' y1='130' x2='104' y2='130' thickness='2' color='666666' /><text x='105' y='122' width='30' align='left' size='12' color='999999'>6</text><line x1='100' y1='110' x2='104' y2='110' thickness='2' color='666666' /><text x='105' y='102' width='30' align='left' size='12' color='999999'>7</text><line x1='100' y1='90' x2='104' y2='90' thickness='2' color='666666' /><text x='105' y='82' width='30' align='left' size='12' color='999999'>8</text><line x1='100' y1='70' x2='104' y2='70' thickness='2' color='666666' /><text x='105' y='62' width='30' align='left' size='12' color='999999'>9</text><line x1='100' y1='50' x2='104' y2='50' thickness='2' color='666666' /><text x='105' y='42' width='30' align='left' size='12' color='999999'>10</text><line x1='32' y1='250' x2='36' y2='250' thickness='2' color='666666' /><text x='0' y='242' width='30' align='right' size='12' color='999999'>0</text><line x1='32' y1='230' x2='36' y2='230' thickness='2' color='666666' /><text x='0' y='222' width='30' align='right' size='12' color='999999'>1</text><line x1='32' y1='210' x2='36' y2='210' thickness='2' color='666666' /><text x='0' y='202' width='30' align='right' size='12' color='999999'>2</text><line x1='32' y1='190' x2='36' y2='190' thickness='2' color='666666' /><text x='0' y='182' width='30' align='right' size='12' color='999999'>3</text><line x1='32' y1='170' x2='36' y2='170' thickness='2' color='666666' /><text x='0' y='162' width='30' align='right' size='12' color='999999'>4</text><line x1='32' y1='150' x2='36' y2='150' thickness='2' color='666666' /><text x='0' y='142' width='30' align='right' size='12' color='999999'>5</text><line x1='32' y1='130' x2='36' y2='130' thickness='2' color='666666' /><text x='0' y='122' width='30' align='right' size='12' color='999999'>6</text><line x1='32' y1='110' x2='36' y2='110' thickness='2' color='666666' /><text x='0' y='102' width='30' align='right' size='12' color='999999'>7</text><line x1='32' y1='90' x2='36' y2='90' thickness='2' color='666666' /><text x='0' y='82' width='30' align='right' size='12' color='999999'>8</text><line x1='32' y1='70' x2='36' y2='70' thickness='2' color='666666' /><text x='0' y='62' width='30' align='right' size='12' color='999999'>9</text><line x1='32' y1='50' x2='36' y2='50' thickness='2' color='666666' /><text x='0' y='42' width='30' align='right' size='12' color='999999'>10</text>

	<!-- rain gauge pointer animated only if rainfall has changed-->
	<scale x='44' y='250' start_scale='0' end_scale='XX_VAL_XX' direction='vertical' step='1' shake_span='0' shake_frequency='0' shadow_alpha='0'>
		<rect x='44' y='50' width='48' height='200' fill_color='0000ff' />
	</scale>

	<!-- current rainfall text -->
        <text x='40' y='260' width='56' size='14' align='center' color='cccccc'>XX_CURR_XXin</text>

	<update url='/index.php?target=my-rain-flash-gauge&mode=XX_MODE_XX&foo=$ts' delay='XX_REFRESH_XX' delay_type='1' />

</gauge>	
EOL;
				
	}
	
	protected function load_data() {
		$this->mode = Request::singleton()->get("mode");
		
		$today = date("Y-m-d");
		$current_do = weatherDataObject::find("1=1 order by datetime desc limit 0,1");				
		
		switch ($this->mode) {
		case "1hr":
			$this->rain_amount = $current_do->get_rain_1h();
			$this->desc = "1 H R  R A I N";
			$this->refresh = 120;		
		break;
			
		case "24hr":
			$this->rain_amount = $current_do->get_rain_24h();
			$this->desc = "24 H R  R A I N";
			$this->refresh = 240;
		break;
		
		case "total":
			$this->rain_amount = $current_do->get_rain_total();
			$this->desc = "Y E A R  R A I N";
			$this->refresh = 240;
			
			$db = open2300DB::singleton();
        
        	$year = date("Y");
        	
        	$stmt = $db->queryBindOneRowCache("Select rain_total from weather where datetime like :date order by datetime asc limit 0,1",
            	                              array(":date" => $year.'%'));
        	$year_rain_start = $stmt->rain_total;
        	$this->rain_amount = sprintf("%0.2f", $current_do->get_rain_total() - $year_rain_start);		
		break;
		}        
	}	

	
	protected function translate_num($num) {
		$absolute_low = -1;
		$absolute_high = -200;
		return (-10*$num);		
	}
	
	public function render() {			
		$search_arr = array("XX_CURR_XX", "XX_VAL_XX", "XX_MODE_XX", "XX_DESC_XX", 'XX_REFRESH_XX');
		$replace_arr = array($this->rain_amount, $this->translate_num($this->rain_amount), 
							 $this->mode, $this->desc, $this->refresh);

		$output = str_replace($search_arr, $replace_arr, $this->xml);
		echo $output;
	}
}
?>