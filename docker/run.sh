#!/bin/bash

# Now run the config password updater
/root/update-config.sh

# we need syslog to log cron and trap errors
/usr/sbin/syslog-ng
/usr/sbin/cron

# this is so we can get a shell into the container
/bin/bash

/usr/bin/supervisord -n
