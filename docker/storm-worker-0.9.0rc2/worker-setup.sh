#!/bin/bash

cat <<EOF > /opt/storm/conf/storm.yaml
storm.zookeeper.servers:
    - "nimbus01"

storm.local.dir: "/opt/storm/local"

nimbus.host: "nimbus01"

supervisor.slots.ports:
  - 6700
  - 6701
  - 6702
  - 6703
EOF

cat <<EOF > /opt/storm/bin/run
#!/bin/bash

exec /usr/bin/supervisord
EOF

chmod a+x /opt/storm/bin/run

cat <<EOF > /etc/supervisor/conf.d/supervisord.conf
[supervisord]
nodaemon=true

[program:storm]
command=/opt/storm/bin/storm supervisor
stdout_logfile=/var/log/supervisor/storm.log
stderr_logfile=/var/log/supervisor/storm.log
autorestart=true

[program:zookeeper]
command=/opt/zk/bin/zkServer.sh start-foreground
stdout_logfile=/var/log/supervisor/%(program_name)s.log
stderr_logfile=/var/log/supervisor/%(program_name)s.log
autorestart=true

[program:sshd]
command=/usr/sbin/sshd -D
stdout_logfile=/var/log/supervisor/%(program_name)s.log
stderr_logfile=/var/log/supervisor/%(program_name)s.log
autorestart=true
EOF
