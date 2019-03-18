<?php
/**
 *
 */
 
class WeatherDataListPage  extends open2300Page {
	
	const ID = "dl";
	
	protected $centering_width = "900px";

    /**
     * Class Constructor
     *
     */
    public function __construct() {
        parent::__construct('Hemna WX Station');    
    }
    
protected function main_block() {

		$main = new DIVtag(array("id" => self::ID_BODY,
		                         "style" => "width: ".$this->centering_width."; margin-left: auto; margin-right:auto;"));

		$table = TABLEtag::factory("100%", 0);

		$table->add_row( TDtag::factory("", "", $this->content_block() ));
						 
		$table->add_row(new TDtag(array(),
						$this->footer_block()));						 
        $main->add( $table );

		return $main;
    }
    
    /**
     * build the main content for the page
     * and return it all inside a container object
     *
     * @param Container object
     */
    function content_block() {

        $container = Container::factory();       
        
        //add some graphs
        $right_div = new DIVtag(array('id' => 'idWxDataList'));
        $right_div->add(new WXDataList);
        
        $container->add($right_div);        
                       
        return $container;
    }
}
?>
