#!/bin/bash

REGION=ap-northeast-1

KAFKA_DIST=http://ftp.riken.jp/net/apache/incubator/kafka/kafka-0.7.2-incubating/kafka-0.7.2-incubating-src.tgz
KAFKA_NAME=`basename ${KAFKA_DIST}`
KAFKA_DIRNAME=${KAFKA_NAME%.*}

cat <<EOF > /etc/apt/sources.list
deb http://${REGION}.ec2.archive.ubuntu.com/ubuntu precise main universe
deb http://${REGION}.ec2.archive.ubuntu.com/ubuntu precise-updates main universe
deb http://${REGION}.ec2.archive.ubuntu.com/ubuntu precise-security main universe
EOF

apt-get update
apt-get -y install wget openjdk-6-jdk

cd /opt
wget $KAFKA_DIST
tar zxvf $KAFKA_NAME
ln -s /opt/${KAFKA_DIRNAME} /opt/kafka
cd /opt/kafka
./sbt update
./sbt package

# zookeeper dir
mkdir /opt/kafka/zookeeper

cat <<EOF > /opt/kafka/config/zookeeper.properties
dataDir=/opt/kafka/zookeeper
clientPort=2181
maxClientCnxns=0
initLimit=5
syncLimit=2
#server.1=kafka01:2888:3888
#server.2=kafka02:2888:3888
#server.3=kafka03:2888:3888
EOF

cat <<EOF > /opt/kafka/config/server.properties
brokerid=@brokerid@
hostname=@hostname@
port=9092
num.threads=8
socket.send.buffer=1048576
socket.receive.buffer=1048576
max.socket.request.bytes=104857600

log.dir=/tmp/kafka-logs
num.partitions=1

log.flush.interval=10000
log.default.flush.interval.ms=1000
log.default.flush.scheduler.interval.ms=1000

log.retention.hours=168
log.file.size=536870912
log.cleanup.interval.mins=1

enable.zookeeper=true
zk.connect=kafka01:2181
zk.connectiontimeout.ms=1000000
EOF

cat <<EOF > /opt/kafka/bin/run
#!/bin/bash

HOSTNAME=\`hostname\`
BROKER_ID=\${HOSTNAME: -1}

echo \${BROKER_ID} > /opt/kafka/zookeeper/myid

if [[ ! -e /opt/kafka/config/server.properties.bak ]]; then
  cp /opt/kafka/config/server.properties{,.bak}
  sed "s/@brokerid@/\${BROKER_ID}/" \
    /opt/kafka/config/server.properties.bak > \
    /opt/kafka/config/server.properties

  cp /opt/kafka/config/server.properties{,.bak}
  sed "s/@hostname@/\${HOSTNAME}/" \
    /opt/kafka/config/server.properties.bak > \
    /opt/kafka/config/server.properties
fi

/opt/kafka/bin/zookeeper-server-start.sh /opt/kafka/config/zookeeper.properties &!
/opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/server.properties
EOF

chmod a+x /opt/kafka/bin/run