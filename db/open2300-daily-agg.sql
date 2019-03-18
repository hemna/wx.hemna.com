DROP TABLE IF EXISTS `weather_daily_agg`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `weather_daily_agg` (
  `datetime` datetime NOT NULL default '2000-01-01 00:00:00',
  `max_temp_in` decimal(4,1) NOT NULL default '0.0',
  `low_temp_in` decimal(4,1) NOT NULL default '0.0',
  `max_temp_in_time` datetime NOT NULL default '2000-01-01 00:00:00',
  `max_temp_out` decimal(4,1) NOT NULL default '0.0',
  `low_temp_out` decimal(4,1) NOT NULL default '0.0',
  `max_temp_out_time` datetime NOT NULL default '2000-01-01 00:00:00',
  `max_wind_speed` decimal(5,2) default '0.00',
  `max_wind_angle` decimal(5,2) default '0.00',
  `max_wind_speed_time` datetime NOT NULL default '2000-01-01 00:00:00',
  `rainfall` decimal(5,2) default '0.00',
  UNIQUE KEY `datetime` (`datetime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;
