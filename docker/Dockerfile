From php:7.3-cli
MAINTAINER Walter Boring <waboring@hemna.com>
ENV REFRESHED_AT 2019-03-17

ENV DOCKER_USER_ID 1000
ENV DOCUER_USER_GID 1000

ENV BOOT2DOCKER_ID 1000
ENV BOOT2DOCKER_GID 1000

WORKDIR /app

# Install packages
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && \
  apt-get -y upgrade && \
  apt-get -y install sudo make build-essential automake libtool xclip zsh vim && \
  apt-get -y install supervisor wget git zip unzip curl && \
  apt-get -y install imagemagick ffmpeg lnav && \
  apt-get -y autoremove

# Add needed php extensions
RUN export CPU_COUNT=$(cat /proc/cpuinfo | grep processor |wc -l) && \
    docker-php-ext-install -j $CPU_COUNT mysqli pdo_mysql

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


# Now get the app
RUN cd /root && git clone https://github.com/hemna/wx.hemna.com.git wx.hemna.com
RUN cd /root && git clone https://git.code.sf.net/p/phphtmllib/git phphtmllib-git && \
    ln -s /root/phphtmllib-git/phphtmllib /root/wx.hemna.com/lib/external/phphtmllib

# Update the config.inc
RUN sed -i "s/$config\->set('imagesdir'.*/$config\->set('imagesdir','\/wxcam');/" /root/wx.hemna.com/lib/config.inc

RUN cd /root && wget -qO - https://raw.githubusercontent.com/hemna/.dotfiles/master/bootstrap.sh | bash

VOLUME ["/app", "/wxcam"]
CMD ["/bin/bash"]
