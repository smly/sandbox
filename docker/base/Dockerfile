FROM ubuntu:precise
MAINTAINER Kohei Ozaki <i@ho.lc>

ADD base-install.sh /tmp/base-install.sh
RUN /bin/bash /tmp/base-install.sh

EXPOSE 22
CMD ["/usr/bin/supervisord"]
