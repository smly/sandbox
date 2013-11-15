FROM ubuntu:precise
MAINTAINER Kohei Ozaki <i@ho.lc>

ADD kafka-install.sh /kafka-install.sh
RUN /bin/bash /kafka-install.sh

EXPOSE 2888 3888 2181 9092
# CMD /bin/bash /opt/kafka/run