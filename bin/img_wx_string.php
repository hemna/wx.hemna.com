#!/usr/bin/php -q
<?php
/**
 * This script is used to get the text to show
 * on the webcam image
 *
 * @package open2300
 */

$lib_path = realpath('../lib');
ini_set('include_path', '.:/usr/local/lib:'.$lib_path);

define('PHPHTMLLIB', realpath('../lib/external/phphtmllib'));
$GLOBALS['path_base'] = realpath('..');

// autoload function for all our classes
require($GLOBALS['path_base'].'/lib/autoload.inc');

// setup error handling and required parameters
require($GLOBALS['path_base'].'/lib/init.inc');

$GLOBALS['config']->set('uncaught_exception_output', 'text');

$db = open2300DB::singleton();

$ret = $db->queryBindOneRowCache("select * from weather order by datetime desc limit 0,1", array(), 5);
//var_dump($ret);
echo $ret->temp_out;
exit;

//first we load the last entry from the DB
$wx = weatherDataObject::find("1=1 order by datetime desc limit 0,1");

//now construct the url call
$log->debug("remote_update: called");
//var_dump($wx);
$temp_out = $wx->get_temp_out();
settype($temp_out, "float");
$temp_out +=1;
$temp_out -=1;
//var_dump($temp_out);

echo "$temp_out F";
?>
