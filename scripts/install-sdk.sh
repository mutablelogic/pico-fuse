#!/bin/bash
PREFIX="/opt"
PICOSDK="1.5.0"
PICOTOOL="1.1.1"

###############################################################
# Check for dependencies

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

install -d "${PREFIX}/bin" || exit -1

###############################################################
# Pico SDK

# Download pico-sdk
PICOSDK_SRC="https://github.com/raspberrypi/pico-sdk.git"
PICOSDK_DEST="pico-sdk-${PICOSDK}"
if [ -d "${PREFIX}/${PICOSDK_DEST}" ] ; then
  echo "${PICOSDK_DEST} installed"
else
  cd "${PREFIX}" && git clone -q -c advice.detachedHead=false --branch "${PICOSDK}" --single-branch "${PICOSDK_SRC}"  "${PICOSDK_DEST}" || exit -1
fi
if [ -d "${PREFIX}/${PICOSDK_DEST}" ]; then
  rm -f "${PREFIX}/pico-sdk" || exit -1
  cd "${PREFIX}" && ln -s "${PREFIX}/${PICOSDK_DEST}" pico-sdk || exit -1
fi

# Compile pico-sdk
if [ -d "${PREFIX}/${PICOSDK_DEST}" ]; then
  cd "${PREFIX}/${PICOSDK_DEST}"
  git submodule update --init || exit -1
  install -d build && cd build || exit -1
  PICO_SDK_PATH="${PREFIX}/pico-sdk" cmake .. || exit -1
  make || exit -1
fi

###############################################################
# picotool

PICOTOOL_SRC="https://github.com/raspberrypi/picotool.git"
PICOTOOL_DEST="picotool-${PICOTOOL}"
if [ -d "${PREFIX}/${PICOTOOL_DEST}" ] ; then
  echo "${PICOTOOL_DEST} installed"
else
  cd "${PREFIX}" && git clone -q -c advice.detachedHead=false --branch "${PICOTOOL}" --single-branch "${PICOTOOL_SRC}" "${PICOTOOL_DEST}" || exit -1
fi
if [ -d "${PREFIX}/${PICOTOOL_DEST}" ]; then
  rm -f "${PREFIX}/picotool" || exit -1
  cd "${PREFIX}" && ln -s "${PREFIX}/${PICOTOOL_DEST}" picotool || exit -1
fi

# Compile picotool
if [ -d "${PREFIX}/${PICOTOOL_DEST}" ]; then
  cd "${PREFIX}/${PICOTOOL_DEST}"
  git submodule update --init || exit -1
  install -d build && cd build || exit -1
  PICO_SDK_PATH="${PREFIX}/pico-sdk" cmake .. || exit -1
  make || exit -1
fi

###############################################################
# Install binaries

# Install binaries
if [ -d "${PREFIX}/bin" ]; then
  cd "${PREFIX}/bin"
  install "${PREFIX}/${PICOSDK_DEST}/build/pioasm/pioasm" pioasm || exit -1
  install "${PREFIX}/${PICOSDK_DEST}/build/elf2uf2/elf2uf2" elf2uf2 || exit -1
  install "${PREFIX}/${PICOTOOL_DEST}/build/picotool" picotool || exit -1
fi
