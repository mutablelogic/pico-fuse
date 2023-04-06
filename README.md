# pico-fuse

This is a library to use with the Raspberry Pi Pico boards using the RP2040 microprocessor,
which provides an event-driven architecture for interacting with devices.

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
brew install make cmake libusb git
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

If you haven't done so already, [create an ssh key](https://www.digitalocean.com/community/tutorials/how-to-create-ssh-keys-with-openssh-on-macos-or-linux) and [add it to your 
github account](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account).

You can then proceed to download and configure the toolchain:

```bash
install -d ${HOME}/projects && cd ${HOME}/projects
git clone git@github.com:mutablelogic/pico-frame.git
cd pico-frame
make config
```

Finally, you need to install the compile and install the toolchain binaries:

TODO

You should end up with three tools in your `${HOME}/bin` folder:

  * `picotool` - Tool used for flashing the Pico
  * `pioasm` - Assembler for the Pico
  * `elf2uf2` - Converts ELF files to UF2 files

## Blink

The blink example is a simple example that blinks the LED on the Pico W board (so pretty much just tests that the board is working).
To load the example onto your Pico W, plug it in using the USB cable whilst holding down the "BOOTSEL" button on the top of the
Pico. Then run the following command:

```bash
cd ${HOME}/projects/pico-frame
/opt/bin/picotool load  -f -x build/src/blink/blink.uf2
```

Fingers crossed, the LED should start blinking. The code for this example is in the [`src/blink`](src/blink) folder:

  * The `CMakeLists.txt` file defines how to build the example
  * The `blink.c` file contains the source code for the example

This one is a little complicated as the Pico hardware and the Pico W hardware use different GPIO pins for the LED.

## References

  * Pico SDK
    * Repository https://github.com/raspberrypi/pico-sdk
    * Documentation (in PDF) https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf


