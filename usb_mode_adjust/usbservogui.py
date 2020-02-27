#!/usr/bin/env python2

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

import Tkinter as tk
import usbservo


class usbservogui:
    def __init__(self):
        self.dev = usbservo.usbservo()
        if self.dev.dev >= 0:
            self.update_job = None
            self.root = tk.Tk()
            self.root.title("Haptic Controller Parameter Tool")
            self.root.protocol("WM_DELETE_WINDOW", self.shut_down)
            fm = tk.Frame(self.root)

            tk.Button(fm, text="MODE", command=self.dev.toggle_mode).pack(side=tk.LEFT)
            fm.pack(side=tk.TOP)

            self.mode_status = tk.Label(self.root, text="Current Mode: ?\n")
            self.mode_status.pack(side=tk.TOP)

            tk.Button(fm, text = 'CONNECT', command = self.dev.connect).pack(side = tk.LEFT)
            fm.pack(side = tk.TOP)



            param1_slider = tk.Scale(
                self.root,
                from_=-100,
                to=100,
                orient=tk.HORIZONTAL,
                showvalue=tk.TRUE,
                length=600,
                command=self.set_param1_callback,
            )
            param1_slider.set(0)
            param1_slider.pack(side=tk.TOP)
            self.param1_status = tk.Label(self.root, text="Constant Force\n")
            self.param1_status.pack(side=tk.TOP)

            param2_slider = tk.Scale(
                self.root,
                from_=-100,
                to=100,
                orient=tk.HORIZONTAL,
                showvalue=tk.TRUE,
                length=600,
                command=self.set_param2_callback,
            )
            param2_slider.set(0)
            param2_slider.pack(side=tk.TOP)
            self.param2_status = tk.Label(self.root, text="Spring Constant\n")
            self.param2_status.pack(side=tk.TOP)

            param3_slider = tk.Scale(
                self.root,
                from_=-100,
                to=100,
                orient=tk.HORIZONTAL,
                showvalue=tk.TRUE,
                length=600,
                command=self.set_param3_callback,
            )
            param3_slider.set(0)
            param3_slider.pack(side=tk.TOP)
            self.param3_status = tk.Label(self.root, text="Damping Coefficient\n")
            self.param3_status.pack(side=tk.TOP)

            param4_slider = tk.Scale(
                self.root,
                from_=-100,
                to=100,
                orient=tk.HORIZONTAL,
                showvalue=tk.TRUE,
                length=600,
                command=self.set_param4_callback,
            )
            param4_slider.set(0)
            param4_slider.pack(side=tk.TOP)
            self.param4_status = tk.Label(self.root, text="Wall Distance\n")
            self.param4_status.pack(side=tk.TOP)

            param5_slider = tk.Scale(
                self.root,
                from_=1,
                to=100,
                orient=tk.HORIZONTAL,
                showvalue=tk.TRUE,
                length=600,
                command=self.set_param5_callback,
            )
            param5_slider.set(50)
            param5_slider.pack(side=tk.TOP)
            self.param5_status = tk.Label(self.root, text="Bump Interval\n")
            self.param5_status.pack(side=tk.TOP)

        # self.update_status()

    def set_param1_callback(self, value):
        self.dev.set_param1(int(value))

    def set_param2_callback(self, value):
        self.dev.set_param2(int(value))

    def set_param3_callback(self, value):
        self.dev.set_param3(int(value))

    def set_param4_callback(self, value):
        self.dev.set_param4(int(value))

    def set_param5_callback(self, value):
        self.dev.set_param5(int(value))

    def toggle_get_mode(self):
        self.dev.toggle_mode()
        self.mode_status.configure(text = 'Current Mode: {!s}'.format(self.dev.read_mode()))
        # self.update_job = self.root.after(50, self.update_status)

    def update_status(self):
        self.mode_status.configure(text = 'Current Mode: {!s}'.format(self.dev.read_mode()))
        # self.update_job = self.root.after(50, self.update_status)

    def shut_down(self):
        # self.root.after_cancel(self.update_job)
        self.root.destroy()
        self.dev.close()


if __name__ == "__main__":
    gui = usbservogui()
    gui.root.mainloop()
