
import usb.core

class ws2812btest:

    def __init__(self):
        self.TOGGLE_LED1 = 0
        self.TOGGLE_LED2 = 1
        self.TOGGLE_LED3 = 2
        self.READ_SW1 = 3
        self.READ_SW2 = 4
        self.READ_SW3 = 5
        self.WRITE_WS2812BS = 6

        self.dev = usb.core.find(idVendor = 0x6666, idProduct = 0x0003)
        if self.dev is None:
            raise ValueError('no USB device found matching idVendor = 0x6666 and idProduct = 0x0003')
        self.dev.set_configuration()

    def close(self):
        self.dev = None

    def toggle_led1(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED1)
        except usb.core.USBError:
            print("Could not send TOGGLE_LED1 vendor request.")

    def toggle_led2(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED2)
        except usb.core.USBError:
            print("Could not send TOGGLE_LED2 vendor request.")

    def toggle_led3(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED3)
        except usb.core.USBError:
            print("Could not send TOGGLE_LED3 vendor request.")

    def read_sw1(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.READ_SW1, 0, 0, 1)
        except usb.core.USBError:
            print("Could not send READ_SW1 vendor request.")
        else:
            return int(ret[0])

    def read_sw2(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.READ_SW2, 0, 0, 1)
        except usb.core.USBError:
            print("Could not send READ_SW2 vendor request.")
        else:
            return int(ret[0])

    def read_sw3(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.READ_SW3, 0, 0, 1)
        except usb.core.USBError:
            print("Could not send READ_SW3 vendor request.")
        else:
            return int(ret[0])

    #
    # Here colors should be a list of three-value tuples containing 8-bit
    # integers representing RGB values of colors to write out to a string 
    # of WS2812Bs (e.g. a strip of Neopixels).  The first tuple will be 
    # written to the first WS2812B in the string, the second to the second,
    # and so on.
    #
    def write_leds(self, colors):
        b = []
        for c in colors:
            b.extend(list(c))
        try:
            self.dev.ctrl_transfer(0x40, self.WRITE_WS2812BS, 0, 0, b)
        except usb.core.USBError:
            print("Could not send WRITE_WS2812BS vendor request.")

