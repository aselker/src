build: motor_encoder_test.hex

upload: motor_encoder_test.hex
	-python3 ../bootloader/software/bootloadercmd.py -i motor_encoder_test.hex -w

motor_encoder_test.hex: motor_encoder_test.c
	-scons

clean:
	-rm *.hex
