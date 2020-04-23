# Installing Drivers / Software Dependencies Needed for the PIC24FJ USB Bootloader Host Software

A *bootloader* is a short program (firmware) that is preinstalled on a microcontroller that, given certain specified conditions, will allow us to reprogram the remainder of that microcontroller's program memory with whatever application code that we have built on the host computer.  The Elecanisms bootloader is set up so that, if SW1 is depressed when the microcontroller is powered on or reset, the bootloader runs.  If SW1 is not depressed when the device is powered or reset, the application code that was last programmed into the microcontroller will start running.

The host software that interfaces with the Elecanisms bootloader is written in Python3 and makes use of the pyusb module with libusb-1.0 as the underlying low-level backend for pyusb.  To successfully run this software to interface with the device, you will need to make sure that you have these two software packages installed on your computer. You should be able to talk to the bootloader from Linux, macOS, or Windows.  The installation process is a little different in each case, as described below.

To run the bootloader host software, which is located in the `software` subfolder in the `bootloader` folder in the Elecanisms github repository, type `python3 bootloadergui.py` in a terminal window or command shell.  There is also a command-line application called `bootloadercmd.py`.  To learn how to use it, run it with the `-h` or `--help` command-line argument.

## macOS

1. Install pyusb via pip: `pip3 install pyusb` or `python3 -m pip install pyusb`

1. Install libusb via homebrew: `brew install libusb` (see <http://macappstore.org/libusb>)

## Linux
1. Install pyusb via pip: `pip3 install pyusb` or `python3 -m pip install pyusb`

1. At this point, you should be able to run the bootloader host software using sudo.  Because our bootloader device is a custom, vendor-specific USB device, Linux does not permit unprivileged access to it.  To allow general access, create a file in `/etc/udev/rules.d/` using your favorite text editor called `usb_prototype_devices.rules`.  Note that you will probably need to do this using sudo.  Type or paste the following text into the file:  
```
# Prototype vendor specific devices    
SUBSYSTEM=="usb", ATTRS{idVendor}=="6666", ATTRS{idProduct}=="4321", MODE="0666"    
SUBSYSTEM=="usb", ATTRS{idVendor}=="6666", ATTRS{idProduct}=="0003", MODE="0666"    
```

1. Log out and log back in to put the new rules into effect.

## Windows

1. Install pyusb via pip: `pip3 install pyusb` or `python3 -m pip install pyusb`

1. Clone or download/unzip the Elecanisms github repository to a convenient location on your computer.

1. Reboot your computer with driver signature enforcement disabled:
   1. Reboot into Advanced Startup Options by pressing and holding the shift key while you select "Restart" from the Power menu.
   1. Select "Troubleshoot" from the menu that comes up.
   1. Select "Advanced Options".
   1. Click on "See more recovery options".
   1. Select "Startup Settings".
   1. Restart by clicking on the "Restart" button.
   1. When the reboot finishes, you will get a "Startup Settings" menu.  Press F7 to select "7) Disable driver signature enforcement".
1. Install the PIC24FJ USB Bootloader device driver:
   1. While holding SW1 down, connect the Elecanisms board to a USB port via the provided cable.
   1. Bring up the Device Manager control panel.  You will probably see "PIC24FJ USB Bootloader" (with a caution sign next to it indicating there is an issue with the device, which is probably that Windows was unable to locate a driver for it automatically) under "Other Devices".
   1. Right click on "PIC24FJ USB Bootloader" and select "Update Driver".
   1. Select "Browse my computer for driver software".
   1. Click on the "Browse" button and select the `driver` subfolder of the `bootloader` folder in your copy of the Elecanisms github repo.
   1. Click on "Next" in the "Update Drivers" dialog.
   1. You will get a warning dialog that says "Windows can't verify the publisher of this driver software".  Click on "Install this driver software anyway".
   1. In the Device Manager control panel, you should now see the "PIC24FJ USB Bootloader" listed under a group called "libusb (WinUSB) devices" with no caution sign.

1. Copy the `libusb-1.0.dll` file from the `driver` folder to `C:\Windows`, which is by default a hidden directory and requires Administrator privileges to do.  This should install the libusb-1.0 backend so that pyusb can find it.
