#!/bin/bash

# we need syslog to log cron and trap errors
/usr/sbin/syslog-ng
/usr/sbin/cron

# this is so we can get a shell into the container
/bin/bash

/usr/bin/supervisord -n
