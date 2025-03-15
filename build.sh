rm build/os.bin

mkdir build

nasm boot.asm -f bin -o build/boot.bin
nasm zeros.asm -f bin -o build/zeros.bin
i386-elf-gcc main.c -nostdlib -ffreestanding -o build/kernel.o -c

i386-elf-ld -T linker.ld -o build/kernel.elf build/kernel.o
objcopy -O binary build/kernel.elf build/kernel.bin
cat build/boot.bin build/kernel.bin build/zeros.bin >> build/os.bin # zeros.bin is to fill the rest of the disk

qemu-system-x86_64 -drive format=raw,file="build/os.bin",index=0,if=floppy, -m 128m
