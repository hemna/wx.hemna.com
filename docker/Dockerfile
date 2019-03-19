From php:7.3-cli
MAINTAINER Walter Boring <waboring@hemna.com>
ENV REFRESHED_AT 2019-03-17

ENV DOCKER_USER_ID 1000
ENV DOCUER_USER_GID 1000

ENV BOOT2DOCKER_ID 1000
ENV BOOT2DOCKER_GID 1000

WORKDIR /root

RUN ln -sf /usr/share/zoneinfo/America/New_York /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata

# Install packages
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && \
  apt-get -y upgrade && \
  apt-get -y install sudo cron make build-essential syslog-ng && \
  apt-get -y install automake libtool-bin procps xclip zsh vim && \
  apt-get -y install supervisor wget git zip unzip curl && \
  apt-get -y install imagemagick ffmpeg mencoder lnav bc htop tig && \
  apt-get -y autoremove

# Add needed php extensions
RUN export CPU_COUNT=$(cat /proc/cpuinfo | grep processor |wc -l) && \
    docker-php-ext-install -j $CPU_COUNT mysqli pdo_mysql pcntl

# Add composer
RUN php -r "copy('https://getcomposer.org/installer', 'composer-setup.php');" && \
    php composer-setup.php && \
    php -r "unlink('composer-setup.php');" && \
    mv composer.phar /usr/local/bin/composer

# Add PHP pear
RUN pear install Console_GetArgs

# configure /app folder with sample app
RUN mkdir -p /app
# ADD app/ /app
RUN mkdir -p /wxcam

#Environment variables to configure php
ENV PHP_UPLOAD_MAX_FILESIZE 10M
ENV PHP_POST_MAX_SIZE 10M

ADD .ssh /root/.ssh

# Setup the root shell
RUN cd /root && wget -qO - https://raw.githubusercontent.com/hemna/.dotfiles/master/bootstrap.sh | bash

# Now get the app
RUN ls -al
RUN cd /root && git clone https://github.com/hemna/wx.hemna.com.git wx.hemna.com
RUN cd /root/wx.hemna.com && git pull --all
RUN cd /root && git clone https://git.code.sf.net/p/phphtmllib/git phphtmllib-git && \
    ln -s /root/phphtmllib-git/phphtmllib /root/wx.hemna.com/lib/external/phphtmllib

# Update the config.inc
RUN sed -i "s/$config\->set('videodir'.*/$config\->set('videodir','wx.hemna.com:wx.hemna.com\/htdocs\/video');/" /root/wx.hemna.com/lib/config.inc
RUN sed -i "s/$config\->set('imagesdir'.*/$config\->set('imagesdir','\/wxcam');/" /root/wx.hemna.com/lib/config.inc
RUN sed -i "s/$config\->set('imgs2avi'.*/$config\->set('imgs2avi', '\/root\/.bin\/imgs2avi.sh');/" /root/wx.hemna.com/lib/config.inc
RUN sed -i "s/$config\->set('encoder'.*/$config\->set('encoder', '\/usr\/bin\/ffmpeg');/" /root/wx.hemna.com/lib/config.inc

RUN sed -i 's/WXCAM=.*/WXCAM="\/wxcam"/' /root/wx.hemna.com/bin/aircam.sh
RUN sed -i 's/PHP=.*/PHP="\/usr\/local\/bin\/php"/' /root/wx.hemna.com/bin/aircam.sh

COPY .local.sh /root/.local.sh
COPY crontab.sh /root/crontab.sh
RUN crontab /root/crontab.sh
RUN echo "EXTRA_OPTS='-L 5'" >> /etc/default/cron
CMD cron

COPY run.sh /root
RUN chmod 755 /root/run.sh
COPY update-config.sh /root
RUN chmod 755 /root/update-config.sh

WORKDIR /wxcam

VOLUME ["/wxcam"]
CMD ["/root/run.sh"]
