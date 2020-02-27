build: haptic.hex

upload: haptic.hex
	-python3 ../bootloader/software/bootloadercmd.py -i haptic.hex -w

haptic.hex: haptic.c
	-scons

clean:
	-rm *.hex
