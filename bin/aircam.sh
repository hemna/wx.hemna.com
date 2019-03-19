#!/bin/bash
TIME=`date +%H%M%S`
DATE=`date +%Y%m%d`
HUMANDATE=`TZ=America/New_York date +%A\ %b\ %e,\ %Y\ \ %T\ %Z`

WX_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/.. && pwd )"
PHP=`which php`

FONT="${WX_DIR}/bin/Verdana_Bold.ttf"
#WXINFO="60F"
WXINFO=`cd ${WX_DIR}/bin && $PHP -f img_wx_string.php`
WXCAM="${WX_DIR}/wxcam"
TMP="${WX_DIR}/tmp"

# Username and password for the
# image wget url should be in the shell ENV var
CAM_USERNAME="${CAM_USERNAME:-ass}"
CAM_PASSWORD="${CAM_PASSWORD:-ass}"

if [ -f ~/.local.sh ]; then
    source ~/.local.sh
fi

if [ "$CAM_USERNAME" == "ass" ]; then
    echo "CAM_USERNAME not set in environement!!!"
    echo "export CAM_USERNAME=<username here>"
    exit -1
fi

if [ "$CAM_PASSWORD" == "ass" ]; then
    echo "CAM_PASSWORD not set in environment!!!"
    echo "export CAM_PASSWORD=<password>"
    exit -1
fi

if [ ! -d ${WXCAM} ]; then
    mkdir ${WXCAM}
fi

if [ ! -d ${TMP} ]; then
    mkdir ${TMP}
fi

if [ ! -d ${WX_DIR}/htdocs/cam ]; then
    mkdir ${WX_DIR}/htdocs/cam
fi

if [ -e ${WXCAM}/${DATE} ]
then
  echo "${WXCAM}/${DATE} exsts.";
else
  echo "creating ${WXCAM}/${DATE}";
  mkdir ${WXCAM}/${DATE}
fi


cd $TMP
wget -nv -O jpeg.cgi -t 1 --timeout=8 "http://hemna.mynetgear.com:9999/cgi-bin/api.cgi?cmd=Snap&channel=0&rs=wuuPhkmUCeI9WG7C&user=${CAM_USERNAME}&password=${CAM_PASSWORD}"

if [ -e jpeg.cgi ]
then
  mv jpeg.cgi snapshot.cgi
  mv snapshot.cgi cam.jpg
  #now add the stamp and such
  echo "Add annotations"
  convert cam.jpg -quality 90 \
   -font $FONT \
   -fill '#0004' -draw 'rectangle 0,2000,2560,1820' \
   -pointsize 48 -gravity southwest \
   -fill white -gravity southwest -annotate +2+20 "$HUMANDATE" \
   -fill white -gravity south -annotate +480+20 "$WXINFO°F" \
   -fill white -gravity southeast -annotate +2+20 "wx.hemna.com" \
    cam2.jpg

  echo "move to latest cam image"
  #cp cam2.jpg ~/wx.hemna.com/htdocs/cam/cam.jpg
  cp cam2.jpg ${WX_DIR}/htdocs/cam/cam.jpg
  echo cp cam2.jpg ${WXCAM}/${DATE}/${TIME}.jpg
  cp cam2.jpg ${WXCAM}/${DATE}/${TIME}.jpg
fi
