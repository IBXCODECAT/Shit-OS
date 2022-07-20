nasm ./asm/boot.asm -f bin -o boot.bin
nasm ./asm/extended.asm -f elf64 -o extended.o

wsl $WSLENV/x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "./cpp/krnl.cpp" -o "krnl.o"

wsl $WSLENV/x86_64-elf-ld -T"linker.ld"

copy /b boot.bin+krnl.bin image.img

pause

del krnl.o
del extended.o

del boot.bin
del krnl.bin

pause
start config.bxrc