<?php
/**
 * This is a target that
 * is called to add a new entry to the
 * database from a remote wx box.
 */

class RemoteUpdate extends Container {
	
	function __construct() {
		
	}
	
	function render($indent_level=0, $output_debug=0) {
		$GLOBALS['log']->debug('RemoteUpdate called');
		$r = Request::singleton();
		
		//first lets make sure we don't already have this entry
		//in the db
		$datetime = $r->get('datetime');
		if (!is_null($datetime)) {
			try {
				$wx = weatherDataObject::find("datetime = :time", array(":time"=> $datetime));
				$GLOBALS['log']->debug('RemoteUpdate -- Dupe.');
				return "Duplicate";
			} catch (Exception $ex) {
				//probably a not found
				if ($ex->getCode() != phphtmllibException::DBDATAOBJECT_FIND_NOT_FOUND) {
					throw $ex;									
				} else{
					//we didn't find it, so lets insert it.
					$wx = new weatherDataObject();					
				}								
			}			
		}
		
		
		$meta = $wx->get_meta_data();
		
		foreach($meta as $key => $val) {
			//look for the request var
			$method = "set_".$key;
			$r_val = $r->get($key);
			call_user_func(array($wx, $method), $r_val);
		}
		
		//now save it to the db
		$wx->save();
		$GLOBALS['log']->debug('RemoteUpdate -- saved new entry.');		
	}
}
?>
