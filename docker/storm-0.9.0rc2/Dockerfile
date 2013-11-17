# smly/base <- sandbox/docker/base
FROM smly/base
MAINTAINER Kohei Ozaki <i@ho.lc>

ADD storm-install.sh /tmp/storm-install.sh
RUN /bin/bash /tmp/storm-install.sh

# zk: 2888, 3888, 2181
# storm ui: 8080
# storm nimbus: 6627
EXPOSE 22 2888 3888 2181 8080 6627
CMD ["/opt/storm/bin/run"]
