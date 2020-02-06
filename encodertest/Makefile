build: encodertest.hex

upload: encodertest.hex
	-python3 ../bootloader/software/bootloadercmd.py -i encodertest.hex -w

encodertest.hex: encodertest.c
	-scons

clean:
	-rm *.hex
