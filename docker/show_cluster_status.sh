#!/bin/bash

show_status() {
  local host=$1
  local username=$2
  local passwd=$3
  local cmd=$4

  echo "-----"
  echo "show ${host} status"
  echo "-----"
  expect -c "
  set timeout 10
  spawn ssh ${username}@${host} ${cmd}
  expect \"Are you sure you want to continue connecting (yes/no)?\" {
    send \"yes\n\"
    expect \"${username}@${host}'s password:\"
    send \"${passwd}\n\"
  } \"${username}@${host}'s password:\" {
    send \"${passwd}\n\"
  }
  interact
  "
}

show_status kafka01 root secret '/usr/bin/supervisorctl status'
show_status nimbus01 root secret '/usr/bin/supervisorctl status'
show_status storm01 root secret '/usr/bin/supervisorctl status'
show_status storm02 root secret '/usr/bin/supervisorctl status'
show_status storm03 root secret '/usr/bin/supervisorctl status'
show_status storm04 root secret '/usr/bin/supervisorctl status'
