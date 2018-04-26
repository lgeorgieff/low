FROM ubuntu:18.04

### Install all required software for building and configuring the project.
RUN apt-get update && apt-get -y install \
    sudo \
    git \
    emacs \
    curl \
    wget \
    unzip \
    doxygen \
    binutils-dev \
    pkg-config \
    clang-6.0 \
    clang-format-6.0 \
    clang-tidy-6.0 \
    autoconf \
    autoconf-archive \
    libtool \
    cmake \
    gdb

### Install all dependencies for the mongoc client.
RUN apt-get update && apt-get -y install \
    pkg-config \
    libssl-dev \
    libsasl2-dev

### Install all dependencies for folly.
RUN apt-get update && apt-get -y install \
    libboost-all-dev \
    libevent-dev \
    libiberty-dev \
    liblz4-dev \
    liblzma-dev \
    libsnappy-dev \
    zlib1g-dev \
    libjemalloc-dev \
    libssl-dev \
    libgoogle-glog-dev \
    libgflags-dev \
    libdouble-conversion-dev

### Install all dependencies for proxygen.
RUN apt-get update && apt-get install -y gperf

### Copy a .bashrc to the image.
COPY ./.bashrc /root/.bashrc
### Copy a .emacs to the image.
COPY ./.emacs /root/.emacs

### Generate a folder for all pre built software.
RUN mkdir -p /pre_built/

### Install mongoc.
RUN mkdir /pre_built/mongoc
RUN curl -L https://github.com/mongodb/mongo-c-driver/releases/download/1.9.4/mongo-c-driver-1.9.4.tar.gz \
    --output /pre_built/mongoc/mongo-c-driver-1.9.4.tar.gz
RUN tar -xf /pre_built/mongoc/mongo-c-driver-1.9.4.tar.gz -C /pre_built/mongoc
RUN rm /pre_built/mongoc/mongo-c-driver-1.9.4.tar.gz
WORKDIR /pre_built/mongoc/mongo-c-driver-1.9.4
RUN ./configure --disable-automatic-init-and-cleanup
RUN make && make install

### Install mongocxx.
RUN mkdir /pre_built/mongocxx
RUN git clone --branch releases/stable --single-branch \
    https://github.com/mongodb/mongo-cxx-driver.git /pre_built/mongocxx/mongo-cxx-driver-stable
WORKDIR /pre_built/mongocxx/mongo-cxx-driver-stable/build
RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
RUN make && make install

### Install jsoncpp.
RUN mkdir /pre_built/jsoncpp
RUN git clone --branch 1.8.4 --single-branch \
    https://github.com/open-source-parsers/jsoncpp.git /pre_built/jsoncpp/jsoncpp
RUN mkdir /pre_built/jsoncpp/jsoncpp/build
WORKDIR /pre_built/jsoncpp/jsoncpp/build
RUN cmake -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_STATIC_LIBS=OFF -DBUILD_SHARED_LIBS=ON ..
RUN make && make install

### Install folly.
RUN mkdir /pre_built/folly
RUN git clone --branch v2018.04.09.00 --single-branch \
    https://github.com/facebook/folly.git /pre_built/folly/folly
WORKDIR /pre_built/folly/folly/folly
RUN autoreconf --install
RUN ./configure
RUN make && make install

### Install wangle.
RUN mkdir /pre_built/wangle
RUN git clone --branch v2018.04.09.00 --single-branch \
    https://github.com/facebook/wangle /pre_built/wangle/wangle
RUN mkdir /pre_built/wangle/wangle/wangle/build
WORKDIR /pre_built/wangle/wangle/wangle/build
# Work around a bug when handling libatomic for clang++ in the next 2 lines
RUN echo "" > ../cmake/CheckAtomic.cmake
RUN echo "target_link_libraries(wangle atomic)" >> ../CMakeLists.txt
RUN cmake ..
RUN make && make install

### Build proxygen and all its dependencies.
RUN mkdir /pre_built/proxygen
RUN git clone --branch v2018.04.09.00 --single-branch \
    https://github.com/facebook/proxygen.git /pre_built/proxygen/proxygen
WORKDIR /pre_built/proxygen/proxygen/proxygen
RUN autoreconf -ivf
RUN ./configure
RUN make
RUN LD_LIBRARY_PATH=/usr/local/lib make check
RUN make install

### Generate proxygen's documentation.
WORKDIR /pre_built/proxygen/proxygen
RUN doxygen Doxyfile

### Build googletest and googlemock
RUN mkdir /pre_built/googletest/
RUN git clone --branch release-1.8.0 --single-branch \
    https://github.com/google/googletest.git /pre_built/googletest/googletest
RUN mkdir /pre_built/googletest/googletest/build
WORKDIR /pre_built/googletest/googletest/build
RUN cmake -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_SHARED_LIBS=ON ..
RUN make && make install

### Create the mountpoint for low's folder structure and make it the working directory.
RUN mkdir -p /project/low
WORKDIR /project/low

### Expose the TCP port 80 for HTTP.
EXPOSE 80/tcp

### Start bash as default command.
CMD /bin/bash
