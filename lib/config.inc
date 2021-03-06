<?php
/**
 * This is the generated config.inc file
 *
 *
 * @package open2300
 */

$GLOBALS['config'] = Config::singleton();

$config->set('widget_image_path', '/images/widgets');
$config->set('file_cache_dir', '/wx.hemna.com/cache');
$config->set('airport_file_cache_dir', '/home/waboring/wx-cache-dir/airports');
$config->set('controller_request_debug', 0);
$config->set('cache_enabled', 1);
$config->set('uncaught_exception_trace_depth', '5');
$config->set('uncaught_exception_output', 'html');


$config->set('DB_TYPE', 'mysql');
$config->set('DB_HOST', 'mysql.newsblob.com');
$config->set('DB_PORT', '3306');
$config->set('DB_NAME', 'open2300');
$config->set('DB_USERNAME', 'hemnawx');
$config->set('DB_PASSWORD', 'mywxstation');


//allow page caching?
$config->set('page_cache_enabled', 1);
$config->set("debug", True);

//app specific settings
$config->set('station_name', 'HEMNA');

//this should just be the host
$config->set('remote_server_name', 'wxtest.hemna.com');

$config->set('has_date_time', true);

$GLOBALS['config']->set('db_debug', false);


//the directory on the gentoo box that the camera
//dumps the images.
$config->set('imagesdir','/wxcam');
//the video dir where the movies go on hosting provider
$config->set('videodir','wx.hemna.com:wx.hemna.com/htdocs/video');

//the current season (for banner images)
$config->set("season", "winter");
$config->set("sitename", "wx.hemna.com");


//For the avi daemon
$config->set('imgs2avi', '/root/.bin/imgs2avi.sh');
// Which encoder to use?
$config->set('encoder', '/usr/bin/ffmpeg');
//How many seconds to wait in between each encoding run
// 15 minutes
$config->set('avi_daemon_wait', 900);

//error_reporting(E_ALL ^ E_DEPRECATED)
//error_reporting(0);
date_default_timezone_set("America/New_York");

?>
