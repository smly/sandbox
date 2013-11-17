#!/bin/bash

MIRROR_SERVER=ftp.riken.go.jp/Linux

cat <<EOF > /etc/apt/sources.list
deb http://${MIRROR_SERVER}/ubuntu precise main universe
deb http://${MIRROR_SERVER}/ubuntu precise-updates main universe
deb http://${MIRROR_SERVER}/ubuntu precise-security main universe
EOF

apt-get update
apt-get install -y openssh-server supervisor

mkdir -p /var/log/supervisor
mkdir -p /var/run/sshd
echo "root:secret" | chpasswd

cat <<EOF > /etc/supervisor/conf.d/supervisord.conf
[supervisord]
nodaemon=true

[program:sshd]
command=/usr/sbin/sshd -D
stdout_logfile=/var/log/supervisor/%(program_name)s.log
stderr_logfile=/var/log/supervisor/%(program_name)s.log
autorestart=true
EOF

