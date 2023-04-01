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
brew install cmake libusb git
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

On all platforms, you need an `/opt` directory:

```bash
sudo install -d /opt -m 777
```

If you haven't done so already, [create an ssh key](https://www.digitalocean.com/community/tutorials/how-to-create-ssh-keys-with-openssh-on-macos-or-linux) and [add it to your 
github account](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account).

You can then proceed to download and install the Pico SDK
and the Pico toolchain. Then clone the pico-frame repository and run

```bash
install -d ${HOME}/projects && cd ${HOME}/projects
git clone git@github.com:mutablelogic/pico-frame.git
cd pico-frame
./scripts/install-sdk.sh
```

You should end up with three tools in the `/opt/bin` folder:
  
    * `picotool` - Tool used for flashing the Pico
    * `pioasm` - Assembler for the Pico
    * `elf2uf2` - Converts ELF files to UF2 files

## References

  * Pico SDK
    * Repository https://github.com/raspberrypi/pico-sdk
    * Documentation https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf


