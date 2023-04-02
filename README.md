# pico-frame

(please note these instructions are in progress and not yet tested)

These are some experiments with the Raspberry Pi Pico W.

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

Finally, you need to compile the programs from the source code in the `src` folder:

```bash
install -d ${HOME}/projects && cd ${HOME}/projects
git clone git@github.com:mutablelogic/pico-frame.git
cd pico-frame
make
```

## Blink

The blink example is a simple example that blinks the LED on the Pico W board (so pretty much just tests that the board is working).
To load the example onto your Pico W, plug it in using the USB cable whilst holding down the "BOOTSEL" button on the top of the
Pico. Then run the following command:

```bash
cd ${HOME}/projects/pico-frame
/opt/bin/picotool load  -f -x build/src/blink/blink.uf2
```

Fingers crossed, the LED should start blinking. The code for this example is in the [`src/blink`](main/src/blink) folder.

## References

  * Pico SDK
    * Repository https://github.com/raspberrypi/pico-sdk
    * Documentation https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf


