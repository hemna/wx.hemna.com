<?php
/**
 * This is the generated config.inc file
 *
 *
 * @package open2300
 */

$GLOBALS['config'] = Config::singleton();

$config->set('widget_image_path', '/images/widgets');
$config->set('file_cache_dir', '/home/waboring/wx-cache-dir/file');
$config->set('airport_file_cache_dir', '/home/waboring/wx-cache-dir/airports');
$config->set('controller_request_debug', 0);
$config->set('cache_enabled', 1);
$config->set('uncaught_exception_trace_depth', '5');
$config->set('uncaught_exception_output', 'html');


$config->set('DB_TYPE', 'mysql');
$config->set('DB_HOST', 'mysql.newsblob.com');
$config->set('DB_PORT', '3306');
$config->set('DB_NAME', 'open2300');
$config->set('DB_USERNAME', 'user');
$config->set('DB_PASSWORD', 'password');


//allow page caching?
$config->set('page_cache_enabled', 1);
$config->set("debug", False);

//app specific settings
$config->set('station_name', 'HEMNA');

//this should just be the host
$config->set('remote_server_name', 'wxtest.hemna.com');

$config->set('has_date_time', true);

$GLOBALS['config']->set('db_debug', false);


//the directory on the gentoo box that the camera
//dumps the images.
$config->set('imagesdir', "/var/www/wxcam");
//the video dir where the movies go on hosting provider
$config->set('videodir', realpath($GLOBALS["path_base"])."/htdocs/video");

//the current season (for banner images)
$config->set("season", "winter");
$config->set("sitename", "wx.hemna.com");
//error_reporting(E_ALL ^ E_DEPRECATED)
error_reporting(0)
?>
