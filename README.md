# pico-frame

(please note these instructions are in progress and not yet tested)

## Dependencies

On Fedora Linux, the following packages need installed:

```bash
sudo dnf install gcc-arm-linux-gnu \
 arm-none-eabi-gcc-cs-c++ \
 arm-none-eabi-gcc-cs \
 arm-none-eabi-binutils \
 arm-none-eabi-newlib \
 libusb1-devel
```

On Mac with homebrew, the following packages need installed:

```bash
brew install cmake
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

On all platforms, you need an `/opt` directory:

```bash
sudo install -d /opt -m 777
```

Once you've done this, you can proceed to download and install the Pico SDK
and the Pico toolchain:

```bash
cd ${HOME}
git clone git@github.com:mutablelogic/pico-frame.git
cd pico-frame
./scripts/install-sdk.sh
```


## References

  * Pico SDK
    * Repository https://github.com/raspberrypi/pico-sdk
    * Documentation https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf


