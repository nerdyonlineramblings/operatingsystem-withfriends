build/os.bin:
	# nasm - compile bootloader
	nasm boot.asm -f bin -o build/boot.bin
	nasm zeros.asm -f bin -o build/zeros.bin
	
	# GCC - compile the kernel and the header files
	i386-elf-gcc -c vga_buffer.c -nostdlib -ffreestanding -o build/vga_buffer.o -c
	i386-elf-gcc main.c -nostdlib -ffreestanding -o build/kernel.o -c
	
	# linker
	i386-elf-ld -T linker.ld -o build/kernel.elf build/kernel.o build/vga_buffer.o

	# assembling the final file. this is a process I do not pretend to understand.
	objcopy -O binary build/kernel.elf build/kernel.bin
	cat build/boot.bin build/kernel.bin build/zeros.bin >> build/os.bin # zeros.bin is to fill the rest of the disk
	qemu-system-x86_64 -drive format=raw,file="build/os.bin",index=0,if=floppy, -m 128m
