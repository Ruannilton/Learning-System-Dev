@echo off
nasm src\boot\boot_sec.asm -f bin -o src\boot\boot_sec.bin
nasm src\boot\main_kernel.asm -f elf -o src\boot\main_kernel.o

gcc -ffreestanding -c src\kernel\kernel.c -o src\kernel\kernel.o
gcc -ffreestanding -c src\kernel\port_io.c -o src\kernel\port_io.o
gcc -ffreestanding -c src\kernel\utils.c -o src\kernel\utils.o
gcc -ffreestanding -c src\drivers\screen.c -o src\drivers\screen.o

ld -Ttext 0x1000 -o src\kernel\kernel.tmp src\boot\main_kernel.o src\kernel\kernel.o src\kernel\port_io.o src\drivers\screen.o src\kernel\utils.o
objcopy -O binary src\kernel\kernel.tmp src\kernel\kernel.bin
copy /b src\boot\boot_sec.bin+src\kernel\kernel.bin os-image.bin

qemu-system-x86_64w os-image.bin
pause