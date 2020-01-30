# Sparkfun coin acceptor example

This program toggles an LED every time a coin is inserted in a coin slot.

## Materials
* A desktop computer with this repo, configured with the Microchip compiler for the Elecanisms PIC board and `SCons`. Detailed instructions for setting up the environment are available [here](http://elecanisms.olin.edu/handouts/1.1_BuildTools.pdf).
* An Elecanisms PIC board, programmed with the Elecanisms bootloader
* A mini-USB cable and 12v barrel-jack power supply
* A Sparkfun coin acceptor, sold [here](https://www.sparkfun.com/products/11719)
* A 10kΩ resistor
* A breadboard and some jumper wires

## Setup
* Program the PIC board.
	* Plug the PIC board into the computer with the USB cable.
	* Hold `btn1` on the PIC board, and press `reset`.
	* run `make upload`
	* Press `reset` on the PIC board.
* Assemble the circuit.
	* Plug the 12v barrel jack into PIC board.
	* Connect `gnd` and `vin` to the ground and 12v lines on the coin acceptor, respectively.
	* Connect the `coin` line on the acceptor to pin D2 on the PIC, and also to one end of the 10kΩ resistor.  Connect the other end of the resistor to a 3v3 pin on the PIC board.
* Configure the coin acceptor.
	* See the [documentation on the Sparkfun website](http://cdn.sparkfun.com/datasheets/Components/General/3.jpg) for how to program the slot to accept coins of your choice.
	* Set the acceptor to output one slow pulse for each coin.
