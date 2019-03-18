<?php
/**
 * This file contains the open2300AjaxDefaultHTMLDataList
 * class.
 *
 * @package open2300
 */

/**
 * This class is a wrapper to automatically set the db
 * object to point to the open2300 db.
 *
 * @package open2300
 */
abstract class open2300AjaxDefaultHTMLDataList extends AjaxDefaultHTMLDataList  {
    /**
     * Get data list source
     *
     */
    protected function get_data_source() {
        $source = new DataBaseSQLDataListSource(open2300DB::singleton());
        $this->set_data_source($source);
    }
}


?>
