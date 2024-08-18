BUILD_DIR := build
SRC_DIR := $(filter-out src/old, $(wildcard src/*))
EXAMPLES_DIR := $(wildcard examples/*)
TESTS_DIR := $(wildcard tests/*)
PREFIX ?= /usr/local

# Paths to tools needed in dependencies
GIT := $(shell which git)
CMAKE := $(shell which cmake)

# Pico variables
PICO_PLATFORM ?= rp2040
PICO_BOARD ?= pico_w

# Targets
all: config picotool test examples

test: config
	@make -C ${BUILD_DIR} all test

examples: config
	@make -C ${BUILD_DIR} all examples

config: dependencies mkdir
	@echo cmake config
	@${CMAKE} -B ${BUILD_DIR} -DPICO_PLATFORM=${PICO_PLATFORM} -DPICO_BOARD=${PICO_BOARD}

picotool: dependencies mkdir 
	@echo make picotool
	@PICO_SDK_PATH=../../../lib/pico-sdk ${CMAKE} -S lib/picotool -B ${BUILD_DIR}/lib/picotool
	@make -C ${BUILD_DIR}/lib/picotool
	@echo "\nRun:\n  install -s ${BUILD_DIR}/lib/picotool/picotool ${PREFIX}/bin\n"

src: $(SRC_DIR)

$(TESTS_DIR): dependencies mkdir
	@echo make $(notdir $@)
	@make -C ${BUILD_DIR}/$@

$(SRC_DIR): dependencies mkdir
	@echo make $(notdir $@)
	@make -C ${BUILD_DIR}/$@

$(EXAMPLES_DIR): dependencies mkdir
	@echo make $(notdir $@)
	@make -C ${BUILD_DIR}/$@

# Update submodule to the 2.0.0 version
submodule-update: dependencies
	@echo "Updating submodules"
	@${GIT} pull --recurse-submodules	
	@cd lib/pico-sdk && ${GIT} pull origin 2.0.0 && cd ../..
	@cd lib/picotool && ${GIT} pull origin 2.0.0 && cd ../..
	@${GIT} submodule sync --recursive

# Submodule checkout
submodule: dependencies
	@echo "Checking out submodules"
	@${GIT} submodule update --init --recursive 

# Submodule clean
submodule-clean: dependencies
	@echo "Cleaning submodules"
	@${GIT} reset --hard
	@${GIT} submodule sync --recursive
	@${GIT} submodule update --init --force --recursive
	@${GIT} clean -ffdx
	@${GIT} submodule foreach --recursive git clean -ffdx	

mkdir:
	@echo mkdir ${BUILD_DIR}
	@install -d ${BUILD_DIR}

clean: submodule-clean
	@echo clean
	@rm -fr $(BUILD_DIR)

dependencies:
	@test -f "${CMAKE}" && test -x "${CMAKE}"  || (echo "Missing cmake binary" && exit 1)
	@test -f "${GIT}" && test -x "${GIT}"  || (echo "Missing git binary" && exit 1)
