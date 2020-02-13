build: current_test.hex

upload: current_test.hex
	-python3 ../bootloader/software/bootloadercmd.py -i current_test.hex -w

current_test.hex: current_test.c
	-scons

clean:
	-rm *.hex
