#!/bin/bash

#####
# Create new virtualenv
if [[ ! -e venv ]]; then
  virtualenv venv
  source venv/bin/activate
fi

######
# install kafka-python for 0.7
if [[ ! -e venv ]]; then
  mkdir ws && pushd ws
  git clone https://github.com/mumrah/kafka-python
  cd kafka-python
  git checkout 0.7
  python setup.py install
  popd && rm -rf ws
fi

#####
# install nose
pip install nose rednose

#####
# Run test script
nosetests --rednose -v test_kafka01.py

#####
# Remove virtualenv
#rm -rf venv
