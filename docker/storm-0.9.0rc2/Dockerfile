FROM ubuntu:precise
MAINTAINER Kohei Ozaki <i@ho.lc>

ADD storm-install.sh /storm-install.sh
RUN /bin/bash /storm-install.sh

# zk: 2888, 3888, 2181
# storm ui: 8080
# storm nimbus: 6627
EXPOSE 2888 3888 2181 8080 6627
# CMD /bin/bash /opt/storm/bin/run
