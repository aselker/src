build: usb_encoder_time.hex

upload: usb_encoder_time.hex
	-python3 ../bootloader/software/bootloadercmd.py -i usb_encoder_time.hex -w

usb_encoder_time.hex: usb_encoder_time.c
	-scons

clean:
	-rm *.hex
