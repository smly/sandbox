#!/bin/bash

apt-get -y install protobuf-compiler bzr mercurial bison flex g++ make git

mkdir /opt
cd /opt

cat <<EOF > /opt/goenv
export GOROOT=/opt/go
export PATH=${PATH}:/opt/go/bin
EOF

wget http://go.googlecode.com/files/go1.1.2.linux-amd64.tar.gz
tar zxvf go1.1.2.linux-amd64.tar.gz
rm go1.1.2.linux-amd64.tar.gz

wget https://github.com/influxdb/influxdb/archive/v0.2.0.tar.gz
tar zxvf v0.2.0.tar.gz
rm v0.2.0.tar.gz

ln -s /opt/influxdb-0.2.0 /opt/influxdb

source /opt/goenv
cd /opt/influxdb
./build.sh

cat <<EOF > /opt/influxdb/run
#!/bin/bash

source /opt/goenv
cd /opt/influxdb
./server
EOF

chmod a+x /opt/influxdb/run

cat <<EOF > /etc/supervisor/conf.d/supervisord.conf
[supervisord]
nodaemon=true

[program:influxdb]
command=/opt/influxdb/run
stdout_logfile=/var/log/supervisor/%(program_name)s.log
stderr_logfile=/var/log/supervisor/%(program_name)s.log
autorestart=true

[program:sshd]
command=/usr/sbin/sshd -D
stdout_logfile=/var/log/supervisor/%(program_name)s.log
stderr_logfile=/var/log/supervisor/%(program_name)s.log
autorestart=true
EOF
