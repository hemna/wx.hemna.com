SHELL="/bin/bash"
MAILTO="waboring@hemna.com"
WXDIR="/root/wx.hemna.com"
PHP="/usr/local/bin/php"

* * * * * $WXDIR/bin/aircam.sh >> $WXDIR/logs/aircam.log 2>&1
* * * * * sleep 30; $WXDIR/bin/aircam.sh >> $WXDIR/logs/aircam.log 2>&1
*/5 * * * * cd $WXDIR/bin && $PHP -f $WXDIR/bin/avi_daemon.php >> $WXDIR/logs/daemon.log
#5 * * * * cd $WXDIR/bin; $WXDIR/bin/db_aggregate_daemon.php >> $WXDIR/logs/db_agg_daemon.log
