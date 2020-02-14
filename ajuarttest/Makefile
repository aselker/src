build: ajuarttest.hex

upload: ajuarttest.hex
	-python3 ../bootloader/software/bootloadercmd.py -i ajuarttest.hex -w

ajuarttest.hex: ajuarttest.c
	-scons

clean:
	-rm *.hex
