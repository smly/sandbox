FROM smly/storm-0.9.0rc2
MAINTAINER Kohei Ozaki <i@ho.lc>

ADD worker-setup.sh /tmp/worker-setup.sh
RUN /bin/bash /tmp/worker-setup.sh

EXPOSE 22 6700 6701 6702 6703
CMD ["/opt/storm/bin/run"]
