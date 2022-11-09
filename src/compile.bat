nasm ./asm/boot.asm -f bin -o boot.bin
nasm ./asm/extended.asm -f elf64 -o extended.o
nasm ./asm/bin.asm -f elf64 -o bin.o


wsl $WSLENV/x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "./cpp/memory/heap.cpp" -o "heap.o"
wsl $WSLENV/x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "./cpp/memory/memory.cpp" -o "memory.o"
wsl $WSLENV/x86_64-elf-gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c "./cpp/krnl.cpp" -o "krnl.o"


wsl $WSLENV/x86_64-elf-ld -T"linker.ld"

copy /b boot.bin+krnl.bin image.img

del extended.o
del bin.o

del krnl.o
del heap.o
del memory.o

del boot.bin
del krnl.bin

pause
start config.bxrc