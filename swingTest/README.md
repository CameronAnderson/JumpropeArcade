Steps to be taken to run the blink program:
1. Download and install the compiler for your operating system found on http://www.microchip.com/compilers . Make sure to install the XC16 version of the compiler. Accept all defaults when navigating through the installer.
	a. Potential additional installations required: 32 bit comptibility files. These might be necessary if the installer does not load when the downloaded executable file is run. 
2. Download the build tool SCons for your operating system. For Linux type "sudo apt-get install scons" in the command line
3. Make sure usblib and pylib are installed on your computer.
4. Fork and clone the elecanisms repo from  the GitHub account OlinElecanisms to your own computer. 
5. In the elecanisms repo, enter the directory named blink and run scons.
6. Plug the board into a usb port and hold down switch 1 while pressing reset to put the board into bootloader mode. 
7. Enter the directory site_scons and run the file bootloadergui.py with the command 'sudo python bootloadergui.py' to bring up the bootloader GUI. If properly connected the rectangle at the top will be green and will say it is connected to a PIC device.GUI WILL NOT CONNECT TO PIC IF BOARD IS NOT IN BOOTLOADER MODE.
7. Upload the blink code to the gui by clicking File>>Import Hex, slecting blink.hex from the blink directory. 
8. Click the button in the gui labled "write" to upload the code onto the PIC.
9. Once the message "write completed successfully" is shown in the green box, click "Disconnect/Run" to run the code. You should see a blinking red light if everything has been executed correctly. 

Important things to note: scons must be run while in the directory with the blink code after every modification to the blink code. 

Board must be put in bootloader mode again each time you upload new code from the GUI, as it is diconnected each time you run code. 

It is advisable to open two command lines with one in the blink directory to avoid closing the bootloader GUI each time you wish to modify code. 