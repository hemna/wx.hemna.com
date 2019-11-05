From php:7.3.6-apache
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
# RUN mkdir -p /app
# ADD app/ /app
RUN mkdir -p /wxcam
RUN mkdir -p /wx.hemna.com

#Environment variables to configure php
ENV PHP_UPLOAD_MAX_FILESIZE 10M
ENV PHP_POST_MAX_SIZE 10M

# Update Apache to point to the web dir
RUN sed -i 's/\/var\/www\//\/wx.hemna.com\/wx.hemna.com/' /etc/apache2/apache2.conf
RUN sed -i 's/\/var\/www\/html/\/wx.hemna.com\/wx.hemna.com\/htdocs/' /etc/apache2/sites-available/000-default.conf
RUN sed -i 's/\/var\/www\/html/\/wx.hemna.com\/wx.hemna.com\/htdocs/' /etc/apache2/sites-available/default-ssl.conf

# ADD .ssh /root/.ssh
RUN groupadd -g 1000 waboring
RUN useradd -M -u 1000 -g 1000 -G waboring,root waboring
RUN usermod -a -G root,waboring www-data

# Setup the root shell
#RUN cd /root && wget -qO - https://raw.githubusercontent.com/hemna/.dotfiles/master/bootstrap.sh | bash

# Now get the app
# RUN ls -al
RUN cp /usr/local/etc/php/php.ini-development /usr/local/etc/php.ini

COPY .local.sh /root/.local.sh
COPY crontab.sh /root/crontab.sh
RUN crontab /root/crontab.sh
RUN echo "EXTRA_OPTS='-L 5'" >> /etc/default/cron
CMD cron

COPY run.sh /root
RUN chmod 755 /root/run.sh
COPY update-config.sh /root
RUN chmod 755 /root/update-config.sh
COPY install-app.sh /root
RUN chmod 755 /root/install-app.sh

WORKDIR /wxcam

VOLUME ["/wxcam", "/wx.hemna.com"]
ENTRYPOINT ["/root/run.sh"]
# CMD ["/root/run.sh"]
