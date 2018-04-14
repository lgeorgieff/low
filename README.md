# Description
TBDL

# Installation
This project provides docker containers for building and running the software. You may also install
the software which is part of the docker containers and build and run everything locally.

For normal installation you need to:
1. Install [docker](https://docs.docker.com/install/)
1. git clone https://github.com/lgeorgieff/low.git
1. cd low
1. ./infrastructure/docker.sh build http
1. ./infrastructure/docker.sh build mongodb
1. ./infrastructure/docker.sh start mongodb
1. ./infrastructure/docker.sh start http
1. mkdir build
1. cd build
1. cmake -DCMAKE_BUILD_TYPE=Debug ..
1. make
1. ./bin/low_service

# License
This project is licensed under [BSD 3-Clause license](./LICENSE.txt).
