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

ini_set('memory_limit', '1024M');
$_GET['airports'] = 'KAUN';

$aw = new AviationWeather();

var_dump($aw->render());


//echo "\nNumber of airports = ".var_export(count($result), true);
//var_dump(json_encode($result, JSON_FORCE_OBJECT));
//var_dump($result);
//var_dump($result);


?>
