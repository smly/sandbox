# smly/base <- sandbox/docker/base
FROM smly/base
MAINTAINER Kohei Ozaki <i@ho.lc>

ADD kafka-install.sh /tmp/kafka-install.sh
RUN /bin/bash /tmp/kafka-install.sh

EXPOSE 22 2888 3888 2181 9092
CMD ["/opt/kafka/bin/run"]
