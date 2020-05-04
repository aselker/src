build: led_array.hex

upload: led_array.hex
	-python3 ../bootloader/software/bootloadercmd.py -i led_array.hex -w

led_array.hex: led_array.c
	-scons

clean:
	-rm *.hex
