FROM ubuntu:18.04

# Install all required software for building and configuring the project.
RUN apt-get update && apt-get -y install \
    sudo \
    git \
    emacs \
    curl \
    doxygen \
    clang-6.0 \
    clang-format-6.0 \
    clang-tidy-6.0 \
    cmake

# Install all dependencies for the mongoc client
RUN apt-get update && apt-get -y install \
    pkg-config \
    libssl-dev \
    libsasl2-dev

# Copy a .bashrc to the image.
COPY ./.bashrc /root/.bashrc
# Copy a .emacs to the image.
COPY ./.emacs /root/.emacs

# Generate a folder for all pre built software
RUN mkdir -p /pre_built/

# Install mongoc
RUN mkdir /pre_built/mongoc
RUN curl -L https://github.com/mongodb/mongo-c-driver/releases/download/1.9.4/mongo-c-driver-1.9.4.tar.gz \
    --output /pre_built/mongoc/mongo-c-driver-1.9.4.tar.gz
RUN tar -xf /pre_built/mongoc/mongo-c-driver-1.9.4.tar.gz -C /pre_built/mongoc
RUN rm /pre_built/mongoc/mongo-c-driver-1.9.4.tar.gz
WORKDIR /pre_built/mongoc/mongo-c-driver-1.9.4
RUN ./configure --disable-automatic-init-and-cleanup
RUN make && make install

# Install mongocxx
RUN mkdir /pre_built/mongocxx
RUN git clone --branch releases/stable --single-branch \
    https://github.com/mongodb/mongo-cxx-driver.git /pre_built/mongocxx/mongo-cxx-driver-stable
WORKDIR /pre_built/mongocxx/mongo-cxx-driver-stable/build
RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
RUN make && make install

# Install jsoncpp
RUN mkdir /pre_built/jsoncpp
RUN git clone --branch 1.8.4 --single-branch \
    https://github.com/open-source-parsers/jsoncpp.git /pre_built/jsoncpp/jsoncpp
RUN mkdir /pre_built/jsoncpp/jsoncpp/build
WORKDIR /pre_built/jsoncpp/jsoncpp/build
RUN cmake -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_STATIC_LIBS=OFF -DBUILD_SHARED_LIBS=ON ..
RUN make && make install

# Build proxygen and all its dependencies.
RUN git clone https://github.com/facebook/proxygen.git /pre_built/proxygen
WORKDIR /pre_built/proxygen
RUN ./proxygen/deps.sh
RUN ./proxygen/reinstall.sh

# Generate proxygen's documentation
RUN doxygen Doxyfile

# Create the mountpoint for low's folder structure and make it the working directory.
RUN mkdir -p /project/low
WORKDIR /project/low

# Expose the TCP port 80 for HTTP
EXPOSE 80/tcp

# Start bash as default command.
CMD /bin/bash
