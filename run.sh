echo [Auto Builder] Assembling

#Assemble the Assembly Files
as --32 src/boot/boot.s -o boot.o
as --32 src/kernel/gdt/load_gdt.s -o load_gdt.o
as --32 src/kernel/idt/load_idt.s -o load_idt.o

echo [Auto Builder] Compiling C files
#Compile the C files
gcc -m32 -c src/kernel/kernel.c -o kernel.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/drivers/screen/screen.c -o screen.o -std=gnu99 -ffreestanding -w 
gcc -m32 -c src/kernel/utils.c -o utils.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/drivers/ports/ports.c -o ports.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/drivers/keyboard/keyboard.c -o keyboard.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/shell.c -o shell.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/cpu/cpuid/cpuid.c -o cpuid.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/gdt/gdt.c -o gdt.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/idt/idt.c -o idt.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/idt/isr.c -o isr.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/fs/vfs.c -o vfs.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/fs/initrd/initrd.c -o initrd.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/drivers/serial/serial.c -o serial.o -std=gnu99 -ffreestanding -w
gcc -m32 -c src/kernel/drivers/pci/pci.c -o pci.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/panic.c -o panic.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/rtc/rtc.c -o rtc.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/mouse/mouse.c -o mouse.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/pic.c -o pic.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/ata/ata.c -o ata.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/fs/fs.c -o fs.o -std=gnu99 -ffreestanding 

echo [Auto Builder] Linking
ld -m elf_i386 -T boot/linker.ld kernel.o screen.o utils.o keyboard.o ports.o shell.o cpuid.o gdt.o idt.o isr.o vfs.o initrd.o serial.o panic.o pci.o rtc.o mouse.o pic.o ata.o fs.o boot.o load_gdt.o load_idt.o -o MyOS.bin -nostdlib --allow-multiple-definition

echo [Auto Builder] Building ISO
#check MyOS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot MyOS.bin

#building the iso file
mkdir -p isodir/boot/grub
cp MyOS.bin isodir/boot/MyOS.bin
cp boot/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o MyOS.iso isodir

echo [Auto Builder] Cleaning
rm -rf isodir
rm -rf *.o

echo Done!
