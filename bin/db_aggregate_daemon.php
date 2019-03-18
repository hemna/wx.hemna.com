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
$i_path = ini_get('include_path');
ini_set('include_path', $i_path.':/home/waboring/local/php5/lib/php/:'.$lib_path);

define('PHPHTMLLIB', realpath('../lib/external/phphtmllib'));
$GLOBALS['path_base'] = realpath('..');

// autoload function for all our classes
require($GLOBALS['path_base'].'/lib/autoload.inc');

// setup error handling and required parameters
require($GLOBALS['path_base'].'/lib/init.inc');

$config->set('uncaught_exception_output', 'txt');

//other includes...
require_once('Console/Getargs.php');

function &condense_args($params) {
    $new_params = array();
    foreach ($params[0] as $param) {
        $new_params[$param[0]] = $param[1];
    }
    return $new_params;
}

//We have to process this stuff PRIOR to includes
//to properly set the MAC_ADDRESS and EMAILLABS_APP_TYPE env var.
$arg_config = array('start' => array(
                                    'short' => 's',
                                    'min' => 0,
                                    'max' => 0,
                                    'desc' => 'Start the daemon.  This is the default'),
                    'kill' => array(
                                    'short' => 'k',
                                    'min' => 0,
                                    'max' => 0,
                                    'desc' => 'Gracefully kill/stop the daemon. Cleanup is done prior to exit.  '.
                                              'If the daemon cron checker is still running, then the scheduler '.
                                              'daemon will get automatically restarted.'),
                    'clean' => array(
                                    'short' => 'c',
                                    'min' => 0,
                                    'max' => 0,
                                    'desc' => 'Run Daemon cleanup. This nukes the db locks for this host.  '.
                                              'This is automatically done internally when the daemon is '.
                                              'started or shut down'),
                    'disable' => array(
                                    'short' => 'd',
                                    'min' => 0,
                                    'max' => 0,
                                    'desc' => 'Disable the daemon from starting. '.
                                              'This writes a file to disk, which start() looks for to enable '.
                                              'daemonizing'),
                    'enable' => array(
                                    'short' => 'e',
                                    'min' => 0,
                                    'max' => 0,
                                    'desc' => 'Enable the daemon to be started. '.
                                              'This deletes the suppression file created by disable.'),
);

/*
$longoptions = array ("start", "stop", "clean", "disable", "enable");

$con = new Console_Getopt;
$args = $con->readPHPArgv();
array_shift($args);


$params = $con->getopt2($args, null, $longoptions);
var_dump($params);
var_dump(condense_args($params));
exit;
*/

//command parser
$cmd =& Console_Getargs::factory($arg_config);
//qqq($args);
if ( PEAR::isError($cmd) ) {
	if ( $cmd->getCode() === CONSOLE_GETARGS_ERROR_USER ) {
		echo Console_Getargs::getHelp($arg_config, null, $cmd->getMessage())."\n";
	} else if ( $cmd->getCode() === CONSOLE_GETARGS_HELP ) {
		echo Console_Getargs::getHelp($arg_config)."\n";
   }
	exit;
}

$start = $cmd->getValue('start');
$kill = $cmd->getValue('kill');
$clean = $cmd->getValue('clean');
$enable = $cmd->getValue('enable');
$disable = $cmd->getValue('disable');

function use_error() {
	global $arg_config;

	echo "Only 1 command at a time\n\n";
	echo Console_Getargs::getHelp($arg_config)."\n";
	exit;
}


//make sure we only have 1 command
if ($start) {
	if ($kill || $clean || $enable || $disable) {
		use_error();
	}
	$command = 'start';
} else if ($kill) {
	if ($start || $clean || $enable || $disable) {
		use_error();
	}
	$command = 'kill';
} else if ($clean) {
	if ($start || $kill || $enable || $disable) {
		use_error();
	}
	$command = 'clean';
} else if ($disable) {
	if ($start || $kill || $enable || $clean) {
		use_error();
	}
	$command = 'disable';
} else if ($enable) {
	if ($start || $kill || $disable || $clean) {
		use_error();
	}
	$command = 'enable';
} else {
	$command = 'start';
}

$daemon = new DBAggreegateDaemon();
$date = "[".date(DATE_RFC822)."]";
switch ($command) {
	default:
	case 'start':
		echo $date."  Starting Scheduler\n";
		try {
			$daemon->start();
		} catch (Exception $e) {
			if ($e->getCode() != DaemonException::ERR_ALREADY_RUNNING) {
				throw $e;
			} else {
				echo $date."  ".$e->getMessage()."\n";
			}

		}
		break;

	case 'kill':
	case 'stop':
		echo $date."  Stopping Daemon\n";
		try {
			$daemon->stop();
		} catch (Exception $e) {
			echo $date."  ".$e->getMessage()."\n";
		}
		break;
}
?>
