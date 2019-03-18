#!/usr/bin/php
<?php
/**
 * This script generates the autoload.inc file
 *
 * @package open2300
 */

$lib_path = realpath('../lib');
ini_set('include_path', '.:/usr/local/lib:'.$lib_path);

define('PHPHTMLLIB', realpath('../lib/external/phphtmllib'));
$GLOBALS['path_base'] = realpath('..');
require_once('external/phphtmllib/required_includes.inc');
require_once('external/phphtmllib/src/generator/AutoloadGenerator.inc');

$path = realpath('../lib');
$gen = new AutoloadGenerator($path);
$gen->set_project_name('open2300');
$gen->set_autoload_path($path.'/');
$gen->set_include_path($path);
$gen->add_form_content_parent('AjaxStandardFormContent');
$gen->set_debug_mode(true);
$gen->set_renderable_parents(array('Container','HTMLPage', 'HTMLWidget',
    'HTMLDataList','myGraph', 'myD3Graph',
    'myFlashGraph', 'myHighChartsGraph', 'RemoteUpdate',
    'JSONWidget'));
$gen->add_exclude('jpgraph-2.2');
$gen->add_exclude('jpgraph-3.5.0b1');
$gen->add_exclude('open-flash-chart2');

$gen->execute();
?>
