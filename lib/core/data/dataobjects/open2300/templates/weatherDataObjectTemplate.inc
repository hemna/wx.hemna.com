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
abstract class weatherDataObjectTemplate extends open2300DBDataObject {

    protected $meta_data = array('datetime' => array(Validator::TYPE_DATETIME, 19),
                                 'temp_in' => array(Validator::TYPE_FLOAT, 4),
                                 'temp_out' => array(Validator::TYPE_FLOAT, 4),
                                 'dewpoint' => array(Validator::TYPE_FLOAT, 4),
                                 'rel_hum_in' => array(Validator::TYPE_INT, 1),
                                 'rel_hum_out' => array(Validator::TYPE_INT, 1),
                                 'wind_speed' => array(Validator::TYPE_NULL_FLOAT, 4),
                                 'wind_angle' => array(Validator::TYPE_NULL_FLOAT, 4),
                                 'wind_direction' => array(Validator::TYPE_CHAR, 3),
                                 'wind_chill' => array(Validator::TYPE_FLOAT, 4),
                                 'rain_1h' => array(Validator::TYPE_NULL_FLOAT, 4),
                                 'rain_24h' => array(Validator::TYPE_NULL_FLOAT, 4),
                                 'rain_total' => array(Validator::TYPE_NULL_FLOAT, 4),
                                 'rel_pressure' => array(Validator::TYPE_NULL_FLOAT, 4),
                                 'tendency' => array(Validator::TYPE_STRING, 7),
                                 'forecast' => array(Validator::TYPE_STRING, 6));

    protected $related_data_objects = array();

    protected $table_name = 'weather';

    protected $primary_key = 'no_id';

    /**
     * This method finds a single data object
     *
     * @param string $where ie. 'foo=:foo'
     * @param array $bind_vars
     * @return weatherDataObject
     */
    public static function find($where_clause, $bind_vars=array()) {
        return DBDataObject::find('weatherDataObject', $where_clause, $bind_vars);
    }

    /**
     * This method finds a single data object
     * by a specific field and value
     *
     * @param string $field_name
     * @param string $field_value
     * @return weatherDataObject
     */
    public static function find_by_field($field_name, $field_value) {
        return DBDataObject::find_by_field('weatherDataObject', $field_name, $field_value);
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
        return DBDataObject::find_all('weatherDataObject', $where_clause, $bind_vars, $order_clause, $limit_clause);
    }

    /**
     * This method finds a single data object
     * using a join
     *
     * @param array $tables
     * @param string $where ie. 'foo=:foo'
     * @param array $bind_vars
     * @return weatherDataObject
     */
    public static function join($tables, $where_clause, $bind_vars=array()) {
        return DBDataObject::join('weatherDataObject', $tables, $where_clause, $bind_vars);
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
        return DBDataObject::join_all('weatherDataObject', $tables, $where_clause, $bind_vars, $order_clause, $limit_clause);
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
    public function set_temp_in($value) { $this->set('temp_in', $value); }

    /**
     * @return string
     */
    public function get_temp_in() { return $this->get('temp_in'); }

    /**
     * @param string $value
     */
    public function set_temp_out($value) { $this->set('temp_out', $value); }

    /**
     * @return string
     */
    public function get_temp_out() { return $this->get('temp_out'); }

    /**
     * @param string $value
     */
    public function set_dewpoint($value) { $this->set('dewpoint', $value); }

    /**
     * @return string
     */
    public function get_dewpoint() { return $this->get('dewpoint'); }

    /**
     * @param integer $value
     */
    public function set_rel_hum_in($value) { $this->set('rel_hum_in', $value); }

    /**
     * @return integer
     */
    public function get_rel_hum_in() { return $this->get('rel_hum_in'); }

    /**
     * @param integer $value
     */
    public function set_rel_hum_out($value) { $this->set('rel_hum_out', $value); }

    /**
     * @return integer
     */
    public function get_rel_hum_out() { return $this->get('rel_hum_out'); }

    /**
     * @param string $value
     */
    public function set_wind_speed($value) { $this->set('wind_speed', $value); }

    /**
     * @return string
     */
    public function get_wind_speed() { return $this->get('wind_speed'); }

    /**
     * @param string $value
     */
    public function set_wind_angle($value) { $this->set('wind_angle', $value); }

    /**
     * @return string
     */
    public function get_wind_angle() { return $this->get('wind_angle'); }

    /**
     * @param string $value
     */
    public function set_wind_direction($value) { $this->set('wind_direction', $value); }

    /**
     * @return string
     */
    public function get_wind_direction() { return $this->get('wind_direction'); }

    /**
     * @param string $value
     */
    public function set_wind_chill($value) { $this->set('wind_chill', $value); }

    /**
     * @return string
     */
    public function get_wind_chill() { return $this->get('wind_chill'); }

    /**
     * @param string $value
     */
    public function set_rain_1h($value) { $this->set('rain_1h', $value); }

    /**
     * @return string
     */
    public function get_rain_1h() { return $this->get('rain_1h'); }

    /**
     * @param string $value
     */
    public function set_rain_24h($value) { $this->set('rain_24h', $value); }

    /**
     * @return string
     */
    public function get_rain_24h() { return $this->get('rain_24h'); }

    /**
     * @param string $value
     */
    public function set_rain_total($value) { $this->set('rain_total', $value); }

    /**
     * @return string
     */
    public function get_rain_total() { return $this->get('rain_total'); }

    /**
     * @param string $value
     */
    public function set_rel_pressure($value) { $this->set('rel_pressure', $value); }

    /**
     * @return string
     */
    public function get_rel_pressure() { return $this->get('rel_pressure'); }

    /**
     * @param string $value
     */
    public function set_tendency($value) { $this->set('tendency', $value); }

    /**
     * @return string
     */
    public function get_tendency() { return $this->get('tendency'); }

    /**
     * @param string $value
     */
    public function set_forecast($value) { $this->set('forecast', $value); }

    /**
     * @return string
     */
    public function get_forecast() { return $this->get('forecast'); }


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
         return " datetime = ". $this->get_datetime(). " temp_in = ". $this->get_temp_in(). " temp_out = ". $this->get_temp_out(). " dewpoint = ". $this->get_dewpoint(). " rel_hum_in = ". $this->get_rel_hum_in(). " rel_hum_out = ". $this->get_rel_hum_out(). " wind_speed = ". $this->get_wind_speed(). " wind_angle = ". $this->get_wind_angle(). " wind_direction = ". $this->get_wind_direction(). " wind_chill = ". $this->get_wind_chill(). " rain_1h = ". $this->get_rain_1h(). " rain_24h = ". $this->get_rain_24h(). " rain_total = ". $this->get_rain_total(). " rel_pressure = ". $this->get_rel_pressure(). " tendency = ". $this->get_tendency(). " forecast = ". $this->get_forecast();
     }    
    
}
?>