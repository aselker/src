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

import tkinter as tk
import encodertest


class encodertestgui:
    def __init__(self):
        self.dev = encodertest.encodertest()
        if self.dev.dev != 0:
            self.update_job = None
            self.root = tk.Tk()
            self.root.title("Motor Spin-Down Test GUI")
            self.root.protocol("WM_DELETE_WINDOW", self.shut_down)
            fm = tk.Frame(self.root)
            tk.Button(fm, text="Start Motor", command=self.dev.start_motor).pack(
                side=tk.LEFT
            )
            tk.Button(fm, text="Stop Motor", command=self.dev.stop_motor).pack(
                side=tk.LEFT
            )
            tk.Button(fm, text="Start Recording", command=self.dev.write_file).pack(
                side=tk.LEFT
            )
            fm.pack(side=tk.TOP)
            # self.enc_status = tk.Label(self.root, text = 'Encoder reading is ?????')
            # self.enc_status.pack(side = tk.TOP)
            # self.update_status()

    # def update_status(self):
    #     self.enc_status.configure(text = 'Encoder reading is {:05d}'.format(self.dev.get_enc_time()))
    #     self.update_job = self.root.after(50, self.update_status)

    def shut_down(self):
        self.root.after_cancel(self.update_job)
        self.root.destroy()
        self.dev.close()


if __name__ == "__main__":
    gui = encodertestgui()
    gui.root.mainloop()
