<?php

/**
 * This file initializes and loads
 * neccessary classes, configs and
 * libraries
 *
 * @package open2300
 */
if (!defined('PHPHTMLLIB')) {
   define('PHPHTMLLIB', "external/phphtmllib");
}

//$GLOBALS['path_base'] needs to be declared prior to this.
//it is needed by scripts that include the external libs
//for adodb
$GLOBALS['path_lib'] = $GLOBALS['path_base'].'/lib';
$GLOBALS['path_external'] = $GLOBALS['path_base'].'/lib/external';

//used for the OFC classes
$ofc_path = $GLOBALS['path_external'].'/open-flash-chart2/php5-ofc-library/lib';
$include_path = ini_get('include_path');
$include_path .= ':'.realpath($ofc_path);
ini_set('include_path', $include_path);


/******************* EXCEPTION HANDLER **************************/

function exception_handler($exception) {
    //This is a fatal problem.
    //an unhandled exception.

    if (method_exists($exception, 'get_extra_as_string')) {
        $extra = $exception->get_extra_as_string();
    } else {
        $extra = NULL;
    }


    $trace = $exception->getTraceAsString();
    $depth = $GLOBALS['config']->get('uncaught_exception_trace_depth',5);
    if ($depth) {
        $depth = "#".$depth;
        $end = strpos($trace, "\n", strpos($trace, $depth));
        $trace = substr($trace, 0, $end);
    }

    try {
        $target = Request::singleton()->get_target();
    } catch (Exception $e) {
        //doh.
        if ($e->getCode() == phphtmllibException::ERR_REQUEST_INVALID_TARGET) {
            //manually get it
            $target = Request::singleton()->get('target');
        } else {
            $target = 'Invalid Target';
        }

    }

    $data = array('Time' => date(DATE_RFC2822),
                  'Type' => get_class($exception),
                  'Message' => $exception->getMessage(),
                  'Extra'   => $extra,
                  'File'    => $exception->getFile(),
                  'Line'    => $exception->getLine(),
                  'Target'  => $target,
                  'Trace'   => "\n".$trace
                  );
                  
    if (Config::singleton()->get('uncaught_exception_output', 'html') == 'html') {
		if (!Config::singleton()->get('debug')) {
            $page = new ExceptionErrorPage();
            print $page->render();
            $GLOBALS["log"]->emerg("Uncaught Exception", $exception);
            return;
        } else {
            $html = '<table width="100%" cellpadding="2" cellspacing="0" style="border:1px solid #9999ff;">';
            $html .= '<tr><td colspan="2" style="background-color:#afafff;">Uncaught Exception</td></tr>';

            foreach ($data as $key => $value) {
                if ($key == 'Trace') {
                    $value = '<pre>'.$value.'</pre>';
                }
                $html .= '<tr><td style="border-bottom:1px solid #afafaf;padding-right:20px;vertical-align:top;">' . $key .
                         '</td><td style="border-bottom:1px solid #afafaf;">' . $value . '</td></tr>';
            }

            $html .= '</table>';
            print $html;
        }
    } else {
        //not in html output mode.
        //assume raw text output is best now (shell script)
        echo "!!!Uncaught Exception!!!\n";
        foreach( $data as $key => $value ) {
            $keylen = strlen($key);
            $prefix = str_repeat(" ", strlen($key)+2);
            echo $key.": ".str_replace("\n", "\n".$prefix,$value)."\n";
        }
        echo "\n\n";
    }

}

set_exception_handler('exception_handler');

/******************* REQUIRED INCLUDES **************************/
error_reporting(0);

// include essential phphtmllib files
require($GLOBALS['path_external'].'/phphtmllib/required_includes.inc');

//include the config
require($GLOBALS['path_lib'].'/config.inc');


//create the main logger object.
$log = Log::singleton();
$log->add_writer(FileLogWriter::factory(realpath($GLOBALS["path_base"])."/logs/application.log"));

?>
