#!/bin/bash
WX_BASE="/wx.hemna.com"
WX_DIR="$WX_BASE/wx.hemna.com"

cd $WX_BASE && git clone https://github.com/hemna/wx.hemna.com.git wx.hemna.com
cd $WX_DIR && git pull --all
cd $WX_BASE && git clone https://git.code.sf.net/p/phphtmllib/git phphtmllib-git && \
    ln -s $WX_BASE/phphtmllib-git/phphtmllib $WX_DIR/lib/external/phphtmllib

# Update the config.inc
sed -i "s/$config\->set('videodir'.*/$config\->set('videodir','wx.hemna.com:wx.hemna.com\/htdocs\/video');/" $WX_DIR/lib/config.inc
sed -i "s/$config\->set('imagesdir'.*/$config\->set('imagesdir','\/wxcam');/" $WX_DIR/lib/config.inc
sed -i "s/$config\->set('imgs2avi'.*/$config\->set('imgs2avi', '\/root\/.bin\/imgs2avi.sh');/" $WX_DIR/lib/config.inc
sed -i "s/$config\->set('encoder'.*/$config\->set('encoder', '\/usr\/bin\/ffmpeg');/" $WX_DIR/lib/config.inc
sed -i "s/$config\->set('file_cache_dir'.*/$config\->set('file_cache_dir', '\/wx.hemna.com\/cache');/" $WX_DIR/lib/config.inc

sed -i 's/WXCAM=.*/WXCAM="\/wxcam"/' $WX_DIR/bin/aircam.sh
sed -i 's/PHP=.*/PHP="\/usr\/local\/bin\/php"/' $WX_DIR/bin/aircam.sh

chown -R root:waboring /wx.hemna.com
mkdir /wx.hemna.com/cache
chmod 775 /wx.hemna.com/cache
