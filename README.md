# pico-fuse

This is a library to use with the Raspberry Pi Pico boards using the RP2040 microprocessor,
which provides an event-driven architecture for interacting with devices in C. The API documentation
is available here:

  * https://mutablelogic.github.io/pico-fuse/

The library consists of ``fuse`` which provides data structures and methods for memory pools,
hash maps, linked lists and strings, plus ``picofuse`` which contains device- and platform-
specific code for the Raspberry Pi Pico. It's possible to use ``fuse`` on its own, but
``picofuse`` depends on ``fuse``.

The following sections provide more information on how to use this library:

  * [Dependencies](#dependencies)
  * [Configuration](#configuration)
  * [References](#references)

The library is published under the Apache license, so feel free to use and fork it, but
please acknowledge the source in any forks. See the [LICENSE](LICENSE) file for more
information.

## Dependencies

On Fedora Linux, the following packages need installed:

```bash
sudo dnf install cmake gcc-arm-linux-gnu \
 arm-none-eabi-gcc-cs-c++ \
 arm-none-eabi-gcc-cs \
 arm-none-eabi-binutils \
 arm-none-eabi-newlib \
 libusb1-devel
```

On Debian Linux, the following packages need to be installed:

```bash
sudo apt install cmake gcc-arm-none-eabi libusb-1.0-0-dev
```

On Mac with homebrew, the following packages need installed:

```bash
brew install make cmake libusb git
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

If you haven't done so already, [create an ssh key](https://www.digitalocean.com/community/tutorials/how-to-create-ssh-keys-with-openssh-on-macos-or-linux)
and [add it to your github account](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account).

## Configuration

Download and configure the toolchain using the following set of commands in your terminal:

```bash
install -d ${HOME}/projects && cd ${HOME}/projects
git clone git@github.com:mutablelogic/pico-fuse.git
cd pico-fuse
PICO_BOARD=pico_w make config
make picotool
```

If this completes successfully then congratulations. If you are targetting a different
board then the Pico W then here are some values for PICO_BOARD you can use instead:

| Configuration Command | Board |
|--|--|
| `PICO_BOARD=pico make config` | Raspberry Pi Pico |
| `PICO_BOARD=pico_w make config` | Raspberry Pi Pico W |
| `PICO_BOARD=pimoroni_picolipo_4mb make config` | Pimironi Lipo with 4MB flash |
| `PICO_BOARD=pimoroni_picolipo_16mb make config` | Pimironi Lipo with 16MB flash |

Whenever you target a different board, use `make clean` before `make picotool` and `make config`.

## References

  * Pico SDK
    * Repository https://github.com/raspberrypi/pico-sdk
    * Documentation (in PDF) https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf


