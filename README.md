# xpad-avr

Xpad AVR is a controller emulator for the original Xbox console. 
It attempts to simulate the HID reports sent from the controller to the console.

## Building

You need:

* An ATMega32u4 (or any AVR with built-in USB) microcontroller (Arduino Leonardo or chinese knockoff)
* A 16 MHz clock connected to the AVR. (This is standard on Arduinos)
* WinAVR or a unix with avr-gcc.

To program run make and program it through avrdude:

    make
    avrdude -p m32u4 -cavr109 -P /dev/ttyACM0  -b57600 -D -Uflash:w:XpadEmu.hex:i

The above command assumes an Arduino or clone. If you happen to have a vanilla chip, use `dfu-programmer`.


## LUFA 

The project uses the open-source USB stack [LUFA](http://www.github.com/abcminiuser/lufa/) to emulate the device. 

The included LUFA library has two additions in the `HIDClassCommon.h` file line 565. Upstreaming these changes is planned, but not until the project is working.