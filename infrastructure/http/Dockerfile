FROM ubuntu:16.04

RUN apt-get update && apt-get -y install \
    sudo \
    git \
    emacs \
    clang-5.0 \
    clang-format-5.0 \
    clang-tidy-5.0 \
    cmake

RUN mkdir -p /pre_build/
RUN git clone https://github.com/facebook/proxygen.git /pre_build/proxygen
WORKDIR /pre_build/proxygen/proxygen
RUN ./deps.sh
RUN ./reinstall.sh

RUN mkdir -p /project/low
WORKDIR /project/low

CMD /bin/bash
