FROM ubuntu

ADD influxdb-install.sh /influxdb-install.sh
RUN /bin/bash /influxdb-install.sh

EXPOSE 8083 8086
CMD /opt/influxdb/run