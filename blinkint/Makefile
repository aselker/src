build: blink.hex

upload: blink.hex
	-python3 ../bootloader/software/bootloadercmd.py -i blink.hex -w

blink.hex: blink.c
	-scons

clean:
	-rm *.hex
