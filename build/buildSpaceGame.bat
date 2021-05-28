@echo off
REM Just change the below set c macro like statement to the folder that your compiler is in. 
set CompilerLocation=C:\ArmFreeToolChain\
REM Also, change the below to the filepath that SpaceGame\ is located in.
set FilePathToGame=C:\

cd %FilePathToGame%SpaceGame\build\
REM Before, I was actually just setting the gcc environment variables via the gccvar batch file that's in bin. But for some reason, I couldn't compile unless I ran the batch file through my code editor 4Coder.
REM Therefore, I have this batch just raw call the executables to compile. If you had the environment variables, you'd just replace what I have for the compile link objcopy like this-
REM %Cpp_Compiler% aka %CompilerLocation%bin\arm-none-eabi-gcc.exe -option
REM with
REM arm-none-eabi-gcc -option
set Definitions=-D SPACEGAME_INTERNAL=1
set Assembler=%CompilerLocation%bin\arm-none-eabi-as.exe
set Cpp_Compiler=%CompilerLocation%bin\arm-none-eabi-gcc.exe %Definitions%
set Linker=%CompilerLocation%bin\arm-none-eabi-ld.exe
set ObjCopy=%CompilerLocation%bin\arm-none-eabi-objcopy.exe

del a.o

del sdk_c.o
del sdk_c.asm

del SpaceGame.o
del SpaceGame.asm

del kernel7.elf
del kernel7.map
del kernel7.img

@echo START BUILD: %time%
REM First, compile the main assembly file for the RPI2, that asm file is where the RPI2 will start code execution after the bootcode is ran.
REM To understand the boot process, I'd reccomend this blog article: https://thekandyancode.wordpress.com/2013/09/21/how-the-raspberry-pi-boots-up/
%Assembler% -I ..\RPI2src\ -march=armv7-a -mfloat-abi=hard -mfpu=neon ..\RPI2src\main.asm -o a.o

REM I do two compile passes on the same c file, one for the assembly, and the second for the output that will be linked with the main's output, and the game's output file (.o)!
REM I couldn't figure out how to generate both in one command, so that's why there's two. If you know how, let me know, please!
%Cpp_Compiler% -I ..\src\ -I ..\RPI2src\ -march=armv7-a -mfloat-abi=hard -mfpu=neon -nostdlib -mno-thumb-interwork -c ..\RPI2src\sdk.c -o sdk_c.o
%Cpp_Compiler% -I ..\src\ -I ..\RPI2src\ -march=armv7-a -mfloat-abi=hard -mfpu=neon -nostdlib -mno-thumb-interwork -S ..\RPI2src\sdk.c -o sdk_c.asm

REM Yet again, the same as above.
%Cpp_Compiler% -I ..\src -I ..\RPI2src\ -fno-exceptions -march=armv7-a -mfloat-abi=hard -mfpu=neon -finput-charset=UTF-8 -fextended-identifiers -nostdinc++ -nostdlib -std=c++11 -c ..\src\SpaceGame_RPI2.cpp -o SpaceGame.o	
%Cpp_Compiler%  -I ..\src -I ..\RPI2src\ -fno-exceptions -march=armv7-a -mfloat-abi=hard -mfpu=neon -finput-charset=UTF-8 -fextended-identifiers -nostdinc++ -nostdlib -std=c++11 -S ..\src\SpaceGame_RPI2.cpp -o SpaceGame.asm

%Linker% -nolibc -nostdlib a.o sdk_c.o SpaceGame.o -o kernel7.elf -Map kernel7.map
REM So the obj copy is interesting. Basically, you're just taking the elf file and formatting it into an img file so the RPI2 will actually run the code since the start.elf file on the usb will look for a kernel.
REM Note that the name of the kernel differs dependant on the type of RPI you're running on, alongside things like the peripheral base addresses. On the 2, it's kernel7!
%ObjCopy% -O binary kernel7.elf kernel7.img
@echo END BUILD: %time%