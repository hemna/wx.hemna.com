<?php

class AirportCache extends FileCache {
	
	protected static $cache_dir;
	
	private static $default_cache_dir = '/tmp/phphtmllib-cache';
	
	
	/**
     * Holds instance of the class
     *
     * @var object
     */
    static private $instance = null;
	
    public static function singleton() {
        if (!Cache::$cache_enabled) {
            return NoCache::singleton();
        }

        if (!self::$instance) {
            self::$instance = new AirportCache();
        }

        return self::$instance;
    }    
    
	/**
     * Sets cache directory
     *
     * @param string $dir
     */
    public static function set_cache_dir($dir) {
        AirportCache::$cache_dir = $dir;
    }

    /**
     * This function returns a file name
     * based on the cache key
     *
     * @param string $key
     *
     * @return string
     */
    protected function make_key($key) {
        if (substr(AirportCache::$cache_dir, -1, 1) == '/') {
            $seperator = '';
        } else {
            $seperator = FileCache::PATH_SEPERATOR;
        }
        return AirportCache::$cache_dir . $seperator . preg_replace("/[^A-Za-z0-9\_\.\-\+\040]/", '', $key) . '.cache';
    }

    /**
     * This function initializes
     * the cache directory
     *
     */
    protected function init_cache() {
        self::$cache_enabled = $GLOBALS['config']->get('cache_enabled', true);
        //if there is a config setting
        //we'll use it.
        $this->set_cache_dir($GLOBALS['config']->get('airport_file_cache_dir',AirportCache::$default_cache_dir));

        // make sure the cache directory exists
        if (!file_exists(AirportCache::$cache_dir)) {
            if (!mkdir(AirportCache::$cache_dir, 0755, TRUE)) {
                throw new CacheException("AirportCache::Failed to create " . AirportCache::$cache_dir);
            }
        }
    }
}

?>