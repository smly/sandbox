#!/bin/bash

STORM_DIST=https://dl.dropboxusercontent.com/s/p5wf0hsdab5n9kn/storm-0.9.0-rc2.zip
STORM_NAME=`basename ${STORM_DIST}`
STORM_DIRNAME=storm-0.9.0-rc2

ZK_DIST=http://ftp.riken.jp/net/apache/zookeeper/current/zookeeper-3.4.5.tar.gz
ZK_NAME=`basename ${ZK_DIST}`
ZK_DIRNAME=zookeeper-3.4.5

ZEROMQ_DIST=http://download.zeromq.org/zeromq-2.1.7.tar.gz
JZMQ_DIST=https://github.com/nathanmarz/jzmq.git

apt-get -y install wget unzip openjdk-6-jdk git pkg-config \
  autoconf libtool gcc g++ make uuid-dev

export JAVA_HOME=/usr/lib/jvm/java-6-openjdk-amd64

# setup zk
cd /opt
wget ${ZK_DIST}
tar zxvf $ZK_NAME
rm $ZK_NAME
ln -s /opt/${ZK_DIRNAME} /opt/zk
mkdir /opt/zk/data
cd /opt/zk
cat <<EOF > /opt/zk/conf/zoo.cfg
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/opt/zk/data
clientPort=2181
EOF

# setup zmq
cd /tmp
wget ${ZEROMQ_DIST}
tar zxvf zeromq-2.1.7.tar.gz
cd zeromq-2.1.7
./configure
make
make install

# setup jzmq
cd /tmp
git clone ${JZMQ_DIST}
cd jzmq/src
touch classdist_noinst.stamp
CLASSPATH=.:./.:$CLASSPATH javac -d . org/zeromq/ZMQ.java \
  org/zeromq/ZMQException.java \
  org/zeromq/ZMQQueue.java \
  org/zeromq/ZMQForwarder.java \
  org/zeromq/ZMQStreamer.java
cd /tmp/jzmq
./autogen.sh
./configure
make
make install

# setup storm
cd /opt
wget --no-check-certificate ${STORM_DIST}
unzip $STORM_NAME
rm $STORM_NAME
ln -s /opt/${STORM_DIRNAME} /opt/storm
cd /opt/storm

cat <<EOF > /opt/storm/conf/storm.yaml
storm.zookeeper.servers:
    - "localhost"

storm.local.dir: "/opt/storm/local"

nimbus.host: "localhost"
EOF

cat <<EOF > /opt/storm/bin/run
#!/bin/bash

exec /usr/bin/supervisord
EOF

chmod a+x /opt/storm/bin/run

cat <<EOF > /etc/supervisor/conf.d/supervisord.conf
[supervisord]
nodaemon=true

[program:stormui]
command=/opt/storm/bin/storm ui
stdout_logfile=/var/log/supervisor/storm_ui.log
stderr_logfile=/var/log/supervisor/storm_ui.log
autorestart=true

[program:nimbus]
command=/opt/storm/bin/storm nimbus
stdout_logfile=/var/log/supervisor/nimbus.log
stderr_logfile=/var/log/supervisor/nimbus.log
autorestart=true

[program:zookeeper]
command=/opt/zk/bin/zkServer.sh start-foreground
stdout_logfile=/var/log/supervisor/%(program_name)s.log
stderr_logfile=/var/log/supervisor/%(program_name)s.log
autorestart=true

[program:sshd]
command=/usr/sbin/sshd -D
stdout_logfile=/var/log/supervisor/%(program_name)s.log
stderr_logfile=/var/log/supervisor/%(program_name)s.log
autorestart=true
EOF
