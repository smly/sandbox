# smly/base <- sandbox/docker/base
FROM smly/base
MAINTAINER Kohei Ozaki <i@ho.lc>

ADD influxdb-install.sh /tmp/influxdb-install.sh
RUN /bin/bash /tmp/influxdb-install.sh

EXPOSE 22 8083 8086
CMD ["/usr/bin/supervisord"]
