
FROM ubuntu:23.04 AS cit

LABEL name=cit version=0.1.0

ARG DEBIAN_FRONTEND=noninteractive
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

RUN apt-get -y update \
 && apt-get -y install --no-install-recommends \
    software-properties-common=0.99.35 \
    gpg \
    gpg-agent \
 && add-apt-repository ppa:apt-fast/stable \
 && apt-get -y install --no-install-recommends \
    apt-fast=1.9.* \
 && apt-fast update \
 && apt-fast -y install --no-install-recommends \
    build-essential=12.9* \
    gcc-13=13.* \
    g++-13=13.* \
    clang-16=1:16* \
    libclang-16-dev=1:16.* \
    libclang-rt-16-dev=1:16.* \
    llvm-16-dev=1:16.* \
    git=1:2.39.* \
    cmake=3.25.* \
    ninja-build=1.11.* \
    grcov=0.8.* \
 && update-alternatives --install \
    /usr/bin/gcc gcc /usr/bin/gcc-13 60 --slave \
    /usr/bin/g++ g++ /usr/bin/g++-13 \
 && update-alternatives --install \
    /usr/bin/clang clang /usr/bin/clang-16 60 --slave \
    /usr/bin/clang++ clang++ /usr/bin/clang++-16

RUN apt-fast -y install --no-install-recommends \
    python3-full \
    python3-pip \
 && rm /usr/lib/python3.11/EXTERNALLY-MANAGED \
 && pip install conan

ENV LC_ALL C.UTF-8

RUN usermod -l user ubuntu
# RUN useradd -ms /bin/bash user

USER user
WORKDIR /home/user/work

HEALTHCHECK NONE