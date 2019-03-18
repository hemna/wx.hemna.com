#!/usr/bin/php
<?php
/**
 * This file is a sample script on how to use
 * the DBDataObjectTemplateGenerator class
 *
 * @package open2300
 *
 * @see DBDataObjectTemplateGenerator
 */

$lib_path = realpath('../lib');
ini_set('include_path', ini_get('include_path').':'.$lib_path);

$GLOBALS['path_base'] = realpath('..');

require_once('autoload.inc');
require_once('init.inc');

$config->set('uncaught_exception_output', 'text');

$xml = file_get_contents('open2300.xml');
$gen = new DBDataObjectTemplateGenerator($xml);
$gen->set_base_path('../lib/core/data/dataobjects');
$gen->set_project_name('open2300');
$gen->set_parent_name('open2300DBDataObject');

$gen->execute();
?>
