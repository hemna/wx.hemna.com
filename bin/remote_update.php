#!/usr/bin/php
<?php
/**
 * This script is used to update the open2300
 * website that runs on a different machine
 * from the wx station.
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

//first we load the last entry from the DB
$wx = weatherDataObject::find("1=1 order by datetime desc");

//now construct the url call
$log->debug("remote_update: called");
//var_dump($wx);
$rb = new RequestBuilder("RemoteUpdate");
$rb->set_server($GLOBALS['config']->get('remote_server_name'));
$rb->set_url_type(Request::URL_TYPE_ABSOLUTE);

//now construct the uri call
foreach($wx as $key => $value) {
	$rb->set($key, $value);	
}

$url = html_entity_decode($rb->get_url());
$opts = array(
   'http' => array('timeout' => 15));

$context = stream_context_create($opts);
var_dump($context);

var_dump(file_get_contents($url,0,$context));


if ($GLOBALS['config']->get('send_to_dev') == true) {
        $url2 = "wxdev.hemna.com";
        $rb = new RequestBuilder("RemoteUpdate");
        $rb->set_server($url2);
        $rb->set_url_type(Request::URL_TYPE_ABSOLUTE);

        //now construct the uri call
        foreach($wx as $key => $value) {
                $rb->set($key, $value);
        }

        $url = html_entity_decode($rb->get_url());
        $log->debug("Remote DEBUG updated calling : ".$url);
        echo($url."\n\n");
        //var_dump(strlen($url));
        //echo(html_entity_decode($url)."\n");
        var_dump(file_get_contents($url,0,$context));
}



?>
