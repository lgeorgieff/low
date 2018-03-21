FROM ubuntu:16.04

# Install all required software for building and configurin the project.
RUN apt-get update && apt-get -y install \
    sudo \
    git \
    emacs \
    clang-5.0 \
    clang-format-5.0 \
    clang-tidy-5.0 \
    cmake

# Copy a .bashrc to the image.
COPY ./.bashrc /root/.bashrc
# Copy a .emacs to the image.
COPY ./.emacs /root/.emacs

# Build proxygen and all its dependencies.
RUN mkdir -p /pre_build/
RUN git clone https://github.com/facebook/proxygen.git /pre_build/proxygen
WORKDIR /pre_build/proxygen/proxygen
RUN ./deps.sh
RUN ./reinstall.sh

# Create the mountpoint for low's folder structure and make it the working directory.
RUN mkdir -p /project/low
WORKDIR /project/low

# Start bash as default command.
CMD /bin/bash
