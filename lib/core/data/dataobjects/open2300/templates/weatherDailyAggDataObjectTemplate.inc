<?php
/**
 * Auto-generated meta data definition file
 *
 * Generated 04/03/2019 16:56:59
 *
 * @package open2300
 * @subpackage generated
 *
 */
abstract class weatherDailyAggDataObjectTemplate extends open2300DBDataObject {

    protected $meta_data = array('datetime' => array(Validator::TYPE_DATETIME, 19),
                                 'max_temp_in' => array(Validator::TYPE_FLOAT, 4),
                                 'low_temp_in' => array(Validator::TYPE_FLOAT, 4),
                                 'max_temp_in_time' => array(Validator::TYPE_DATETIME, 19),
                                 'max_temp_out' => array(Validator::TYPE_FLOAT, 4),
                                 'low_temp_out' => array(Validator::TYPE_FLOAT, 4),
                                 'max_temp_out_time' => array(Validator::TYPE_DATETIME, 19),
                                 'max_wind_speed' => array(Validator::TYPE_NULL_FLOAT, 4),
                                 'max_wind_angle' => array(Validator::TYPE_NULL_FLOAT, 4),
                                 'max_wind_speed_time' => array(Validator::TYPE_DATETIME, 19),
                                 'rainfall' => array(Validator::TYPE_NULL_FLOAT, 4));

    protected $related_data_objects = array();

    protected $table_name = 'weather_daily_agg';

    protected $primary_key = 'no_id';

    /**
     * This method finds a single data object
     *
     * @param string $where ie. 'foo=:foo'
     * @param array $bind_vars
     * @return weatherDailyAggDataObject
     */
    public static function find($where_clause, $bind_vars=array()) {
        return DBDataObject::find('weatherDailyAggDataObject', $where_clause, $bind_vars);
    }

    /**
     * This method finds a single data object
     * by a specific field and value
     *
     * @param string $field_name
     * @param string $field_value
     * @return weatherDailyAggDataObject
     */
    public static function find_by_field($field_name, $field_value) {
        return DBDataObject::find_by_field('weatherDailyAggDataObject', $field_name, $field_value);
    }

    /**
     * This method finds a single data object
     * populates it and returns it.
     *
     * @param string $where ie. 'foo=:foo'
     * @param array $bind_vars
     * @param string $order_clause 'order by foo'
     * @param string $limit clause 'limit 10,20'
     * @return DataObjectCollection
     */
    public static function find_all($where_clause, $bind_vars=array(), $order_clause=NULL, $limit_clause=NULL) {
        return DBDataObject::find_all('weatherDailyAggDataObject', $where_clause, $bind_vars, $order_clause, $limit_clause);
    }

    /**
     * This method finds a single data object
     * using a join
     *
     * @param array $tables
     * @param string $where ie. 'foo=:foo'
     * @param array $bind_vars
     * @return weatherDailyAggDataObject
     */
    public static function join($tables, $where_clause, $bind_vars=array()) {
        return DBDataObject::join('weatherDailyAggDataObject', $tables, $where_clause, $bind_vars);
    }

    /**
     * This method finds all matching
     * data objects using a join
     *
     * @param array $tables
     * @param string $where ie. 'foo=:foo'
     * @param array $bind_vars
     * @param string $order_clause 'order by foo'
     * @param string $limit clause 'limit 10,20'
     * @return DataObjectCollection
     */
    public static function join_all($tables, $where_clause, $bind_vars=array(), $order_clause=NULL, $limit_clause=NULL) {
        return DBDataObject::join_all('weatherDailyAggDataObject', $tables, $where_clause, $bind_vars, $order_clause, $limit_clause);
    }



    // ------------- DATA ACCESS FUNCTIONS ---------------

    /**
     * @param string $value
     */
    public function set_datetime($value) { $this->set('datetime', $value); }

    /**
     * @return string
     */
    public function get_datetime() { return $this->get('datetime'); }

    /**
     * @param string $value
     */
    public function set_max_temp_in($value) { $this->set('max_temp_in', $value); }

    /**
     * @return string
     */
    public function get_max_temp_in() { return $this->get('max_temp_in'); }

    /**
     * @param string $value
     */
    public function set_low_temp_in($value) { $this->set('low_temp_in', $value); }

    /**
     * @return string
     */
    public function get_low_temp_in() { return $this->get('low_temp_in'); }

    /**
     * @param string $value
     */
    public function set_max_temp_in_time($value) { $this->set('max_temp_in_time', $value); }

    /**
     * @return string
     */
    public function get_max_temp_in_time() { return $this->get('max_temp_in_time'); }

    /**
     * @param string $value
     */
    public function set_max_temp_out($value) { $this->set('max_temp_out', $value); }

    /**
     * @return string
     */
    public function get_max_temp_out() { return $this->get('max_temp_out'); }

    /**
     * @param string $value
     */
    public function set_low_temp_out($value) { $this->set('low_temp_out', $value); }

    /**
     * @return string
     */
    public function get_low_temp_out() { return $this->get('low_temp_out'); }

    /**
     * @param string $value
     */
    public function set_max_temp_out_time($value) { $this->set('max_temp_out_time', $value); }

    /**
     * @return string
     */
    public function get_max_temp_out_time() { return $this->get('max_temp_out_time'); }

    /**
     * @param string $value
     */
    public function set_max_wind_speed($value) { $this->set('max_wind_speed', $value); }

    /**
     * @return string
     */
    public function get_max_wind_speed() { return $this->get('max_wind_speed'); }

    /**
     * @param string $value
     */
    public function set_max_wind_angle($value) { $this->set('max_wind_angle', $value); }

    /**
     * @return string
     */
    public function get_max_wind_angle() { return $this->get('max_wind_angle'); }

    /**
     * @param string $value
     */
    public function set_max_wind_speed_time($value) { $this->set('max_wind_speed_time', $value); }

    /**
     * @return string
     */
    public function get_max_wind_speed_time() { return $this->get('max_wind_speed_time'); }

    /**
     * @param string $value
     */
    public function set_rainfall($value) { $this->set('rainfall', $value); }

    /**
     * @return string
     */
    public function get_rainfall() { return $this->get('rainfall'); }


    // ------------- RELATED DATA OBJECTS ----------------


    // ------------- RELATIONS ADD METHODS ----------------


    // ------------- DATA OBJECT COLLECTIONS -------------


  // -------- MISC --------
  
    /**
     * This method returns a string representation
     * of the DataObject
     *
     * @return string
     */
     public function __toString() {
         return " datetime = ". $this->get_datetime(). " max_temp_in = ". $this->get_max_temp_in(). " low_temp_in = ". $this->get_low_temp_in(). " max_temp_in_time = ". $this->get_max_temp_in_time(). " max_temp_out = ". $this->get_max_temp_out(). " low_temp_out = ". $this->get_low_temp_out(). " max_temp_out_time = ". $this->get_max_temp_out_time(). " max_wind_speed = ". $this->get_max_wind_speed(). " max_wind_angle = ". $this->get_max_wind_angle(). " max_wind_speed_time = ". $this->get_max_wind_speed_time(). " rainfall = ". $this->get_rainfall();
     }    
    
}
?>