# pico-fuse

This is a library to use with the Raspberry Pi Pico boards using the RP2040 microprocessor,
which provides an event-driven architecture for interacting with devices in C. 
For example, here's a typical program which blinks an LED once per second:

```c
#include <picofuse/picofuse.h>

picofuse_state_t main_timer_init(picofuse_t *self,
                           picofuse_state_t state,
                           picofuse_event_t event,
                           void *data)
{
   ((picofuse_timer_t *)data)->delay_ms = 1000;
   return ANY;
}

int main()
{
    picofuse_t *picofuse = picofuse_init(PICOFUSE_DEBUG);
    picofuse_register(picofuse, ANY, EV_TIMER_INIT, main_timer_init);
    picofuse_register(picofuse, ANY, EV_TIMER, main_timer);
    picofuse_main(picofuse);
}
```

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

## Configuration

Download and configure the toolchain using the following set of commands in your terminal:

```bash
install -d ${HOME}/projects && cd ${HOME}/projects
git clone git@github.com:mutablelogic/pico-frame.git
cd pico-frame
PICO_BOARD=pico_w make config
make picotool
```

If this completes successfully then congratulations. If you are targetting a different
board then the Pico W then here are some values for PICO_BOARD you can use instead:

TODO

Whenever you target a different board, use `make clean` before `make config`.

## Blink

The blink example is a simple example that blinks the LED on the Pico W board (so pretty much just tests that the board is working).
To load the example onto your Pico W, plug it in using the USB cable whilst holding down the "BOOTSEL" button on the top of the
Pico. Then run the following command:

```bash
cd ${HOME}/projects/pico-frame
make src/blink
build/src/picotool/picotool load  -f -x build/src/blink/blink.uf2
```

Fingers crossed, the LED should start blinking. The code for this example is in the [`src/blink`](src/blink) folder:

  * The `CMakeLists.txt` file defines how to build the example
  * The `blink.c` file contains the source code for the example

This one is a little complicated as the Pico hardware and the Pico W hardware use different GPIO pins for the LED.

## References

  * Pico SDK
    * Repository https://github.com/raspberrypi/pico-sdk
    * Documentation (in PDF) https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf


