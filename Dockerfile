# Test environment for linux build
#
# $ docker build -t clawn-build .
# $ docker run -it --rm -v $(readlink -f .):/src:ro clawn-build
# docker$ cp -R /src /root/ && cd /root/src
# docker$ mkdir build && cd build
# docker$ cmake .. && make

FROM debian:bullseye

ARG BISON_VER=3.8.2

RUN apt-get update && \
    apt-get install -y \
      cmake \
      clang \
      clang-11 \
      libclang-11-dev \
      liblld-11 \
      g++ \
      llvm \
      llvm-9-dev \
      bison \
      flex \
      zlib1g-dev \
      python3-pip \
      wget
