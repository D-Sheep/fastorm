#!/bin/bash

sudo rm -r ext
./../zephir/bin/zephir generate
cd ext
sudo /usr/local/php5/bin/phpize --clean
sudo /usr/local/php5/bin/phpize
sudo ./configure --enable-fastorm --with-php-config=/usr/local/php5/bin/php-config
sudo make
sudo make install
cd ..