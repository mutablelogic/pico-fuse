BUILD_DIR := build
SRC_DIR := $(filter-out src/lwipopts.h, $(wildcard src/*))

# Paths to tools needed in dependencies
GIT := $(shell which git)
CMAKE := $(shell which cmake)

# Pico variables
PICO_PLATFORM ?= rp2040
PICO_BOARD ?= pico_w

# Targets
all: config picotool src

config: dependencies mkdir
	@echo cmake config
	@cd lib/pico-sdk && ${GIT} submodule update --init
	@cd lib/picotool && ${GIT} submodule update --init
	@${CMAKE} -B ${BUILD_DIR} -DPICO_PLATFORM=${PICO_PLATFORM} -DPICO_BOARD=${PICO_BOARD}

picotool: config
	@echo make picotool
	@PICO_SDK_PATH=../../../lib/pico-sdk ${CMAKE} -S lib/picotool -B ${BUILD_DIR}/lib/picotool
	@make -C ${BUILD_DIR}/lib/picotool

src: $(SRC_DIR)

$(SRC_DIR): dependencies mkdir
	@echo make $(notdir $@)
	@make -C ${BUILD_DIR}/$@

mkdir:
	@echo mkdir ${BUILD_DIR}
	@install -d ${BUILD_DIR}

clean:
	@echo clean
	@cd lib/pico-sdk && ${GIT} submodule deinit --force --all
	@cd lib/picotool && ${GIT} submodule deinit --force --all
	@rm -fr $(BUILD_DIR)

dependencies:
	@test -f "${CMAKE}" && test -x "${CMAKE}"  || (echo "Missing cmake binary" && exit 1)
	@test -f "${GIT}" && test -x "${GIT}"  || (echo "Missing git binary" && exit 1)
