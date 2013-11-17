#!/bin/bash

DNSMASQ=10.0.3.1

echo "----"
echo "Setting up kafka broker"
echo "----"
KAFKA_ID=$(docker run -dns ${DNSMASQ} -p 22 -p 2888 -p 3888 -p 2181 -p 9092 -h kafka01 -d smly/kafka-0.7 /opt/kafka/bin/run)
echo "container_id: ${KAFKA_ID}"
KAFKA_ADDR=$(docker inspect ${KAFKA_ID} | grep IPAddress | cut -d '"' -f 4)
echo "ip_address: ${KAFKA_ADDR}"
sudo ghost add kafka01 ${KAFKA_ADDR} || sudo ghost modify kafka01 ${KAFKA_ADDR}
sudo kill -HUP $(pidof dnsmasq)

echo "----"
echo "Setting up storm nimbus/ui"
echo "----"
NIMBUS_ID=$(docker run -dns ${DNSMASQ} -p 22 -p 2888 -p 3888 -p 2181 -p 8080 -p 6627 -h nimbus01 -d smly/storm-0.9.0rc2 /opt/storm/bin/run)
echo "container_id: ${NIMBUS_ID}"
NIMBUS_ADDR=$(docker inspect ${NIMBUS_ID} | grep IPAddress | cut -d '"' -f 4)
echo "ip_address: ${NIMBUS_ADDR}"
sudo ghost add nimbus01 ${NIMBUS_ADDR} || sudo ghost modify nimbus01 ${NIMBUS_ADDR}
sudo kill -HUP $(pidof dnsmasq)

echo "----"
echo "Setting up storm supervisor"
echo "----"
STORM01_ID=$(docker run -dns 10.0.3.1 -p 22 -p 6700 -p 6701 -p 6702 -p 6703 -h storm01 -d smly/storm-worker-0.9.0rc2 /opt/storm/bin/run)
echo "container_id: ${STORM01_ID}"
STORM01_ADDR=$(docker inspect $STORM01_ID | grep IPAddress | cut -d '"' -f 4)
echo "ip_address: ${STORM01_ADDR}"
sudo ghost add storm01 ${STORM01_ADDR} || sudo ghost modify storm01 ${STORM01_ADDR}
sudo kill -HUP $(pidof dnsmasq)

echo "----"
echo "Setting up storm supervisor"
echo "----"
STORM02_ID=$(docker run -dns 10.0.3.1 -p 22 -p 6700 -p 6701 -p 6702 -p 6703 -h storm02 -d smly/storm-worker-0.9.0rc2 /opt/storm/bin/run)
echo "container_id: ${STORM02_ID}"
STORM02_ADDR=$(docker inspect $STORM02_ID | grep IPAddress | cut -d '"' -f 4)
echo "ip_address: ${STORM02_ADDR}"
sudo ghost add storm02 ${STORM02_ADDR} || sudo ghost modify storm02 ${STORM02_ADDR}
sudo kill -HUP $(pidof dnsmasq)

echo "----"
echo "Setting up storm supervisor"
echo "----"
STORM03_ID=$(docker run -dns 10.0.3.1 -p 22 -p 6700 -p 6701 -p 6702 -p 6703 -h storm03 -d smly/storm-worker-0.9.0rc2 /opt/storm/bin/run)
echo "container_id: ${STORM03_ID}"
STORM03_ADDR=$(docker inspect $STORM03_ID | grep IPAddress | cut -d '"' -f 4)
echo "ip_address: ${STORM03_ADDR}"
sudo ghost add storm03 ${STORM03_ADDR} || sudo ghost modify storm03 ${STORM03_ADDR}
sudo kill -HUP $(pidof dnsmasq)

echo "----"
echo "Setting up storm supervisor"
echo "----"
STORM04_ID=$(docker run -dns 10.0.3.1 -p 22 -p 6700 -p 6701 -p 6702 -p 6703 -h storm04 -d smly/storm-worker-0.9.0rc2 /opt/storm/bin/run)
echo "container_id: ${STORM04_ID}"
STORM04_ADDR=$(docker inspect $STORM04_ID | grep IPAddress | cut -d '"' -f 4)
echo "ip_address: ${STORM04_ADDR}"
sudo ghost add storm04 ${STORM04_ADDR} || sudo ghost modify storm04 ${STORM04_ADDR}
sudo kill -HUP $(pidof dnsmasq)
