#!/bin/bash

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
   sudo apt-get install -y \
      libboost-all-dev \
      python3 \
      python3-pip \
      python3-setuptools \
      valgrind \
      ccache

   eval "$(gimme 1.15.4)"
   source ~/.gimme/envs/go1.15.4.env

   export GOPATH=~/go:$(pwd)/build/generated/golang
   export PATH=$PATH:~/go/bin

elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
   brew install cmake \
      boost \
      go \
      ccache

   export GOPATH=~/go:$(pwd)/build/generated/golang
   export PATH=$PATH:~/go/bin

   if [ "$RUN_TYPE" = "coverage" ]; then
      brew install lcov
      sudo gem install coveralls-lcov
      go get -u github.com/jandelgado/gcov2lcov
   fi
fi

pip3 install --user dataclasses-json Jinja2 importlib_resources pluginbase

