import usb.core
import time
from datetime import datetime


class encodertest:
    def __init__(self):
        self.START_MOTOR = 0
        self.STOP_MOTOR = 1
        self.GET_ENC_TIME = 6
        self.dev = usb.core.find(idVendor=0x6666, idProduct=0x0003)
        if self.dev is None:
            raise ValueError(
                "no USB device found matching idVendor = 0x6666 and idProduct = 0x0003"
            )
        self.dev.set_configuration()

        # AS5048A Register Map
        self.ENC_NOP = 0x0000
        self.ENC_CLEAR_ERROR_FLAG = 0x0001
        self.ENC_PROGRAMMING_CTRL = 0x0003
        self.ENC_OTP_ZERO_POS_HI = 0x0016
        self.ENC_OTP_ZERO_POS_LO = 0x0017
        self.ENC_DIAG_AND_AUTO_GAIN_CTRL = 0x3FFD
        self.ENC_MAGNITUDE = 0x3FFE
        self.ENC_ANGLE_AFTER_ZERO_POS_ADDER = 0x3FFF

    def close(self):
        self.dev = None

    def start_motor(self):
        try:
            self.dev.ctrl_transfer(0x40, self.START_MOTOR)
        except usb.core.USBError:
            print("Could not send START_MOTOR vendor request.")

    def stop_motor(self):
        try:
            self.dev.ctrl_transfer(0x40, self.STOP_MOTOR)
        except usb.core.USBError:
            print("Could not send STOP_MOTOR vendor request.")

    def get_angle_time(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.GET_ENC_TIME, 0x3FFF, 0, 6)
        except usb.core.USBError:
            print("Could not send ENC_READ_REG vendor request.")
        else:
            pos = (int(ret[0]) + 256 * int(ret[1])) & 0x3FFF
            # time = ret[2:]
            time = (
                2 ** 24 * int(ret[2])
                + 2 ** 16 * int(ret[3])
                + 2 ** 8 * int(ret[4])
                + int(ret[5])
            )
            return [pos, time]

    # def get_enc_time(self, address):
    #     try:
    #         ret = self.dev.ctrl_transfer(0xC0, self.GET_ENC_TIME, address, 0, 2)
    #     except usb.core.USBError:
    #         print "Could not send GET_ENC_TIME vendor request."
    #     else:
    #         return ret

    def write_file(self):
        sampleTime = 30
        now = "data/" + str(datetime.utcnow()) + ".csv"
        start = time.time()
        f = open(now, "w")
        self.stop_motor()
        while time.time() - start <= sampleTime:
            postime = self.get_angle_time()
            print(postime)
            f.write(str(postime[0]) + "," + str(postime[1]) + "\n")

        f.close()
        # open file
        # while sample time hasn't passed
        # get position and time
        # write position and time
        # close file
