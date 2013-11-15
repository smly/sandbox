#!/bin/bash

REGION_MIRROR=ap-northeast-1.ec2.archive.ubuntu.com

STORM_DIST=https://dl.dropboxusercontent.com/s/p5wf0hsdab5n9kn/storm-0.9.0-rc2.zip
STORM_NAME=`basename ${STORM_DIST}`
STORM_DIRNAME=storm-0.9.0-rc2

ZK_DIST=http://ftp.riken.jp/net/apache/zookeeper/current/zookeeper-3.4.5.tar.gz
ZK_NAME=`basename ${ZK_DIST}`
ZK_DIRNAME=zookeeper-3.4.5

ZEROMQ_DIST=http://download.zeromq.org/zeromq-2.1.7.tar.gz
JZMQ_DIST=https://github.com/nathanmarz/jzmq.git

cat <<EOF > /etc/apt/sources.list
deb http://${REGION_MIRROR}/ubuntu precise main
deb http://${REGION_MIRROR}/ubuntu precise-updates main
deb http://${REGION_MIRROR}/ubuntu precise-security main
EOF

apt-get update
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

echo "Run zookeeper server"
/opt/zk/bin/zkServer.sh start
sleep 3
echo "Run storm ui server"
/opt/storm/bin/storm ui &!
sleep 3
echo "Run storm nimbus server"
/opt/storm/bin/storm nimbus
EOF

chmod a+x /opt/storm/bin/run
