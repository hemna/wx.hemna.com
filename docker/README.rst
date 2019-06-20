Prior to running docker build:
1) create a .local.sh file that contains
  export CAM_USERNAME="some user here"
  export CAM_PASSWORD="some password here"

2) create update-config.sh that contains sed commands to update the DB_USERNAME
   and DB_PASSWORD in the /root/wx.hemna.com/lib/config.inc

#!/bin/bash

sed -i "s/$config\->set('DB_USERNAME'.*/$config\->set('DB_USERNAME', 'ASS');/" /root/wx.hemna.com/lib/config.inc
sed -i "s/$config\->set('DB_PASSWORD'.*/$config\->set('DB_PASSWORD', 'ASS');/" /root/wx.hemna.com/lib/config.inc


BUILDING
docker build -t hemna/wx.hemna.com:latest

Starting the container
docker run -d --name wx.hemna.com2 -p 8888:80 --mount type=bind,source="/home/waboring/docker/wx.hemna.com",target="/wx.hemna.com" hemna/wx.hemna.com:latest build
