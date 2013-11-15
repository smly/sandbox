#!/bin/bash

echo "deb http://us.archive.ubuntu.com/ubuntu/ precise universe" >> /etc/apt/sources.list
echo "deb http://us.archive.ubuntu.com/ubuntu/ precise-updates universe" >> /etc/apt/sources.list
apt-get update
apt-get -y install wget protobuf-compiler bzr mercurial bison flex g++ make git

mkdir /opt
cd /opt

cat <<EOF > /opt/goenv
export GOROOT=/opt/go
export PATH=${PATH}:/opt/go/bin
EOF

wget http://go.googlecode.com/files/go1.1.2.linux-amd64.tar.gz
tar zxvf go1.1.2.linux-amd64.tar.gz
rm go1.1.2.linux-amd64.tar.gz

wget https://github.com/influxdb/influxdb/archive/v0.0.7.tar.gz
tar zxvf v0.0.7.tar.gz
rm v0.0.7.tar.gz

ln -s /opt/influxdb-0.0.7 /opt/influxdb
cd /opt/influxdb

cat <<EOF > /opt/influxdb/run
#!/bin/bash

source /opt/goenv
cd /opt/influxdb
./server
EOF

chmod a+x /opt/influxdb/run

source /opt/goenv
./build.sh