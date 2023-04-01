#!/bin/bash
PREFIX="/opt"
PICOSDK="1.5.0"
PICOTOOL="1.1.0"

# You will want to install dependencies:
#
# mac: 
#  brew install cmake libusb
#  brew tap ArmMbed/homebrew-formulae
#  brew install arm-none-eabi-gcc
#
# fedora:
#   dnf install -y g++ gcc-arm-linux-gnu arm-none-eabi-gcc-cs-c++ arm-none-eabi-gcc-cs arm-none-eabi-binutils arm-none-eabi-newlib \
#                  libusb-devel libusb \
#                  cmake

###############################################################
# Check for dependencies

if [ ! -x "$(command -v curl)" ]; then
  echo "curl is not installed. Please install curl before continuing."
  exit 1
fi
if [ ! -x "$(command -v git)" ]; then
  echo "git is not installed. Please install git before continuing."
  exit 1
fi
if [ ! -x "$(command -v make)" ]; then
  echo "make is not installed. Please install make before continuing."
  exit 1
fi
if [ ! -x "$(command -v cmake)" ]; then
  echo "cmake is not installed. Please install cmake before continuing."
  exit 1
fi


###############################################################
# Check for /opt

cd "${PREFIX}" || exit -1

###############################################################
# Pico SDK

# Download pico-sdk
PICOSDK_SRC="https://github.com/raspberrypi/pico-sdk.git"
PICOSDK_DEST="pico-sdk-${PICOSDK}"
if [ -d "${PREFIX}/${PICOSDK_DEST}" ] ; then
  echo "${PICOSDK_DEST} installed"
else
  cd "${PREFIX}" && git clone -q -c advice.detachedHead=false --branch "${PICOSDK}" --single-branch "${PICOSDK_SRC}"  "${PICOSDK_DEST}" || exit -1
  cd "${PREFIX}" && git submodule update --init || exit -1
fi
if [ -d "${PREFIX}/${PICOSDK_DEST}" ]; then
  rm -f "${PREFIX}/pico-sdk" || exit -1
  cd "${PREFIX}" && ln -s "${PREFIX}/${PICOSDK_DEST}" pico-sdk || exit -1
fi
