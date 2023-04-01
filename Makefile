BUILD_DIR := build
PICO_SDK_PATH ?= /opt/pico-sdk
PICO_PLATFORM ?= rp2040
PICO_BOARD ?= pico_w

# Targets
all: build

config: mkdir
	@PICO_SDK_PATH=${PICO_SDK_PATH} cmake -B ${BUILD_DIR} -DPICO_PLATFORM=${PICO_PLATFORM} -DPICO_BOARD=${PICO_BOARD}

build: config
	@PICO_SDK_PATH=${PICO_SDK_PATH} cmake --build ${BUILD_DIR}

mkdir:
	@echo Mkdir ${BUILD_DIR}
	@install -d ${BUILD_DIR}

clean:
	@echo Clean
	@rm -fr $(BUILD_DIR)
