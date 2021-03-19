First and foremost, thank you for downloading my game!
Here's the details on how to play this game.
Hardware requirements:
Raspberry Pi 2 Model B with 1 GB of Ram (https://www.pishop.us/product/raspberry-pi-2-model-b-armv7-with-1g-ram/)
Snes controller (can buy off Amazon or whatever secondhand store you can find (even third party works))
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
If you have any bugs / issues, please DM or tweet me on twitter @AndrewNReinke, or email me at andrewnereinke@gmail.com!

Gameplay:
Press any button that works (should be A and B) to go into a move on Y axis motion.
After pressing another button, you will move along the forward axis.
Press again and you'll start moving in your specified direction!
See how many times you can jump between the two platforms!