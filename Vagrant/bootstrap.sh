#!/usr/bin/env bash

apt-get update
apt-get install -y build-essential git

wget --no-verbose https://cmake.org/files/v3.5/cmake-3.5.1.tar.gz
tar xvfz cmake-3.5.1.tar.gz
cd cmake-3.5.1
./bootstrap
make
make install
cd ..
rm -rf cmake-3.5.1
rm cmake-3.5.1.tar.gz

cp /vagrant/clonerepo.sh /home/vagrant
cp -r /vagrant/webr /var/
chmod 755 /var/webr/

echo "DONE! To clone and build repo, run clonerepo.sh on first login"
