#
## Copyright (c) 2018, Bradley A. Minch
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
##
##     1. Redistributions of source code must retain the above copyright
##        notice, this list of conditions and the following disclaimer.
##     2. Redistributions in binary form must reproduce the above copyright
##        notice, this list of conditions and the following disclaimer in the
##        documentation and/or other materials provided with the distribution.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
## ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
## LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
## CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
## SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
## INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
## CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.
#

import usb.core

class usbservo:

    def __init__(self):
        self.TOGGLE_MODE = 0
        self.READ_MODE = 1

        self.SET_PARAM1 = 5
        self.SET_PARAM2 = 6
        self.SET_PARAM3 = 7
        self.SET_PARAM4 = 8
        self.SET_PARAM5 = 9

        self.dev = usb.core.find(idVendor = 0x6666, idProduct = 0x0003)
        if self.dev is None:
            raise ValueError('no USB device found matching idVendor = 0x6666 and idProduct = 0x0003')
        self.dev.set_configuration()

    def connect(self):
        self.dev = usb.core.find(idVendor = 0x6666, idProduct = 0x0003)
        if self.dev is None:
            raise ValueError('no USB device found matching idVendor = 0x6666 and idProduct = 0x0003')
        self.dev.set_configuration()


    def close(self):
        self.dev = None

    def toggle_mode(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_MODE)
        except usb.core.USBError:
            print "Could not send TOGGLE_MODE vendor request."



    def read_mode(self):
        try:
            mode = int(self.dev.ctrl_transfer(0xC0, self.READ_MODE, 0, 0, 1)[0])
        except usb.core.USBError:
            print "Could not send READ_MODE vendor request."
        else:

            if mode == 0:
                return "Constant Force"
            if mode == 1:
                return "Spring"
            if mode == 2:
                return "Damper"
            if mode == 3:
                return "Wall"
            if mode == 4:
                return "Bump"



    def set_param1(self, val):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_PARAM1, val)
        except usb.core.USBError:
            print "Could not send SET_PARAM1 vendor request."

    def set_param2(self, val):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_PARAM2, val)
        except usb.core.USBError:
            print "Could not send SET_PARAM2 vendor request."

    def set_param3(self, val):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_PARAM3, val)
        except usb.core.USBError:
            print "Could not send SET_PARAM3 vendor request."

    def set_param4(self, val):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_PARAM4, val)
        except usb.core.USBError:
            print "Could not send SET_PARAM4 vendor request."

    def set_param5(self, val):
        try:
            self.dev.ctrl_transfer(0x40, self.SET_PARAM5, val)
        except usb.core.USBError:
            print "Could not send SET_PARAM5 vendor request."
