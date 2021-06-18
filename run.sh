#assemble boot.s file
as --32 boot/boot.s -o boot.o
as --32 kernel/gdt/load_gdt.s -o load_gdt.o
as --32 kernel/idt/load_idt.s -o load_idt.o
#as --32 kernel/idt/load_idt.s -o load_idt.o



#compile kernel.c file
gcc -m32 -c kernel/kernel.c -o kernel.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/screen/screen.c -o screen.o -std=gnu99 -ffreestanding -w 
gcc -m32 -c kernel/utils.c -o utils.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/ports/ports.c -o ports.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/keyboard/keyboard.c -o keyboard.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/shell.c -o shell.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/cpu/cpuid/cpuid.c -o cpuid.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/gdt/gdt.c -o gdt.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/idt/idt.c -o idt.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/idt/isr.c -o isr.o -std=gnu99 -ffreestanding 
gcc -m32 -c kernel/fs/vfs.c -o vfs.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/fs/initrd/initrd.c -o initrd.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/serial/serial.c -o serial.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/pci/pci.c -o pci.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/panic.c -o panic.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/rtc/rtc.c -o rtc.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/mouse/mouse.c -o mouse.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/pic.c -o pic.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/drivers/ata/ata.c -o ata.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/fs/fs.c -o fs.o -std=gnu99 -ffreestanding 
gcc -m32 -c kernel/drivers/acpi/acpi.c -o acpi.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/math/math.c -o math.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/apps/textedit/textedit.c -o textedit.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/apps/calculator/calculator.c -o calculator.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/apps/serialapp/serialapp.c -o serialapp.o -std=gnu99 -ffreestanding -w
gcc -m32 -c kernel/apps/slang/slang.c -o slang.o -std=gnu99 -ffreestanding -w

ld -m elf_i386 -T boot/linker.ld kernel.o screen.o utils.o keyboard.o ports.o shell.o cpuid.o load_idt.o gdt.o idt.o isr.o vfs.o initrd.o serial.o panic.o pci.o rtc.o mouse.o pic.o ata.o fs.o acpi.o textedit.o math.o calculator.o serialapp.o slang.o boot.o load_gdt.o -o MyOS.bin -nostdlib --allow-multiple-definition

#check MyOS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot MyOS.bin

#building the iso file
mkdir -p isodir/boot/grub
cp MyOS.bin isodir/boot/MyOS.bin
cp boot/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o iso/MyOS.iso isodir

rm -rf isodir
rm -rf *.o
rm -rf *.bin
