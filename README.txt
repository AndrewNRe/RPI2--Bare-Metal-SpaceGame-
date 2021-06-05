Hardware requirements: (NOTE: you surely can use an RPI2 with more ram, but I am running on the 1 gig version, so buy at your own discretion)
Raspberry Pi 2 Model B with 1 GB of Ram (https://www.pishop.us/product/raspberry-pi-2-model-b-armv7-with-1g-ram/)
Micro SD card (i'm using a 4gb Sandisk)
Micro SD card to USB (to get the kernel off your PC, alongside all the required boot files)
Snes controller (can buy off Amazon or whatever secondhand store you can find (even third party works if it works on the actual console))
Male to female GPIO pins, 5 of them (I am using these https://www.amazon.com/gp/product/B00PBZMN7C/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
HDMI cable (for RPI2)
Power Supply for the RPI2 (I am using an old samsung charger cable. I'm not an electrician, but the RPI2 website tells you the requirements to power the RPI2 if you are)

Setup:
Hardware:
I followed this AMAZING diagram from Peter Lemon (aka Khrom) https://github.com/PeterLemon/RaspberryPi/blob/master/Input/SNES/Controller/SNESController.png
You just put the female ends of the GPIO wires on the GPIO slots of the PI. Insert the male pins into the SNES sockets where the diagram specifies.
To be honest, if you're curious about how to program the RPI2, Peter Lemon's repo is one of if not the best repo for such info.
Software:
On your micro SD card, you need to go to the Raspberry Pi github and get the latest firmware (https://github.com/raspberrypi/firmware/tree/master/boot)
The files you will need from the repo are: bootcode.bin, and start.elf
So your SD card's file layout should be-
bootcode.bin (repo)
config.txt (apart of zip)
start.elf (repo)
kernel7.img (apart of zip)
And you're good to go!
If you have any bugs / issues / questions, please email me at andrewnereinke@gmail.com! 
You can also try my twitter, dm or at me @AndrewNReinke
Git also works, but I don't actually know of a way to contact people on it so if you know of a way, please go right on ahead and do so!

Compiling:
I currently am using the Arm Embedded GNU 8-2019-q3-update toolchain to compile the source into a kernel. You can get this compiler on the arm developer website here:
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
I haven't tested any other version besides the one I just wrote about, so i'd only reccomend that you compile using that toolchain.
The actual compilation is just done through a simple batch file in the build folder. 
I have wrote inside that batch file the two things you'll need to change to get it all compiling / linking correctly!
Sorry to all linux users! I have used linux before, but have never compiled any code there. I'd imagine you can figure it out by looking at my Windows batch file, though.
Also, if you're using my code to do some development project or just as reference, i'd highly reccomend you check out the Raspberry Pi bare metal forums as that's where I learned a lot of
"harder to understand things" when getting my project up to the point it is at. https://www.raspberrypi.org/forums/viewforum.php?f=72&sid=634f4b832df8fb7e6aed675abc8d1088
Another extremely helpful thing that is probably old news for older developers, is that I highly reccomend you read the generated assembly when stuff goes wrong as that's where you'll
be able to accurately diagnose where something went wrong.
Also, I can't forget Jaymin Kessler's blog: http://maisonikkoku.com/jaystation2/blog_main.html
And I already mentioned it, but Peter Lemon's git is a goldmine of knowledge to aid you!
Also, if you have any criticisms and or questions about my architectural descisions, please contact me and let me know! I'm always on the path to becoming a better engineer, and or want to offer
ideas to those who are passionate about this path in life too :)

Documentation:
I've linked the documentation to the peripheral sheet, and the CPU architecture reference so you can look up instructions / understand how the hardware works alongside the GPU
Broadcomm: //This depends on the model of Pi you buy, the one I linked is for the Pi2 model B I reccomended you buy. This sheet has included the correct physical address so you can
//access periperhals like the GPIO pins. On the PI2, the base is 0x3F000000 and you just add this to X peripheral's base address.
//An ok example of how you access peripherals would be the startup code that's ran on core 0 in the main.asm of my code.
https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2837/README.md
CPU: //I used this pdf for getting information about X cpu instruction.
https://developer.arm.com/documentation/ddi0464/d/
CPU architecture: //I used this pdf for understanding how the CPU works
https://developer.arm.com/documentation/ddi0406/c/
GPU: //I haven't personally used this (yet), but this is the entire GPU documented. Peter Lemon also has some examples on how to use the GPU on the RPI2 so check there!!!
https://github.com/doe300/VC4C/blob/master/doc/VideoCoreIV-AG100-R.pdf

Gameplay:
Press any button that works (should be A and B) to go into a move on Y axis motion.
After pressing another button, you will move along the forward axis.
Press again and you'll start moving in your specified direction!
See how many times you can jump between the two platforms!