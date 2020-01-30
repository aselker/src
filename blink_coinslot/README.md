# Sparkfun coin acceptor example

This program toggles an LED every time a coin is inserted in a coin slot.

## Materials
* A desktop computer, with this repo, configured with `scons` the Elecanisms PIC bootloader
* An Elecanisms PIC board, programmed with the Elecanisms bootloader
* A mini-USB cable and 12v barrel-jack power supply
* A Sparkfun coin acceptor, sold [here](https://www.sparkfun.com/products/11719)
* A 10kΩ resistor
* A breadboard and some jumper wires

## Setup
* Program the PIC board.
	* Go to this directory and run `scons`.
	* Plug the PIC board into the computer with the USB cable.
	* Hold `btn1` on the PIC board, and press `reset`.
	* In `src/bootloader/software` in this repo, run `./bootloadercmd.py -i ../../blink_coinslot/blink.hex -w`.
	* Press `reset` on the PIC board.
* Assemble the circuit.
	* Plug the 12v barren jack into PIC board.
	* Connect `gnd` and `vin` to the ground and 12v lines on the coin acceptor, respectively.
	* Connect the `coin` line on the acceptor to pin D2 on the PIC, and also to one end of the 10kΩ resistor.  Connect the other end of the resistor to a 3v3 pin on the PIC board.
* Configure the coin acceptor.
	* See the documentation on the Sparkfun website for how to program the slot to accept coins of your choice.
	* Set the acceptor to output one slow pulse for each coin.
