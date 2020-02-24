build: usbservo.hex

upload: usbservo.hex
	-python3 ../bootloader/software/bootloadercmd.py -i usbservo.hex -w

usbservo.hex: usbservo.c
	-scons

clean:
	-rm *.hex
