<?php
$lib_path = realpath('../lib');
ini_set('include_path', '.:/usr/local/lib:'.$lib_path);

define('PHPHTMLLIB', realpath('../lib/external/phphtmllib'));
$GLOBALS['path_base'] = realpath('..');

// autoload function for all our classes
require($GLOBALS['path_base'].'/lib/autoload.inc');

// setup error handling and required parameters
require($GLOBALS['path_base'].'/lib/init.inc');

$GLOBALS['config']->set('uncaught_exception_output', 'text');



class wiki_parse {
    
    protected $file = null; 
    protected $filename = null; 
    protected $data = array();
    
    protected $airports = array();
    
    
    function __construct($file) {
        //open the file, throw exception
        $this->filename = $file;
        if (!file_exists($file)) {                      
            throw new Exception("File doesn't exist' ".$file);
        }
        
    }
    
    public function parse () {
        //read one line..get the code call code parser.
        $lines = file($this->filename);
        $cache = AirportCache::singleton();
        
        echo "Number of lines = ". var_export(count($lines), true)."\n\n";
        
        $airport = array();
        $x = 1;
        foreach ($lines as $line_num => $line) {            
            $airportInfo = $this->parse_line($line, $airport);
            $this->airports[] = $airportInfo;
            $airport = $cache->get($airportInfo["icao"]);
            
            if ($airport != null && is_array($airport)) {
                $airport["wikipedia_url"] = $airportInfo["url"];
                $cache->set($airportInfo["icao"], $airport);
            }                                               
        }
        
        echo "\nNumber of Airports = ".var_export(count($this->airports), true);
        return $this->airports;                     
    }
    
    public function parse_line($line, $airport) {
        if ($line != null && strlen($line) > 0) {
            //$line_arr = $this->clean_line($line);
            
            $airport = array("icao" => "", "url" => "");
            $line = str_replace("<li><b>", "", $line);
            $line_arr = split("<li><b>", $line, 10);
            //echo "Line is now ".$line."\n";
        
            //var_dump($line_arr);
            $index = strpos($line_arr[0], "<");
        
            $code = substr($line_arr[0], 0,$index);
            //var_dump($code);
            $airport["icao"] = $code;
        
            $index = strpos($line_arr[0], '"', $index);
            $stop = strpos($line_arr[0], '"', $index+1); 
            //var_dump($index);
            //var_dump($stop);       
            $url = substr($line_arr[0], $index+1, $stop-$index-1);
            //var_dump($url);
            
            $airport["url"] = $url;
            return $airport;                        
        }
    } 
}

ini_set('memory_limit', '1024M');
//$parser = new apt_parse("apt_short.dat");
$parser = new wiki_parse("wiki_icao.html");
//$parser = new apt_parse("apt_kaun.dat");


$result = $parser->parse();
//echo "\nNumber of airports = ".var_export(count($result), true);
//var_dump(json_encode($result, JSON_FORCE_OBJECT));
//var_dump($result);
//var_dump($result);


?>
