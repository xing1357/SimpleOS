# SimpleOS
A simple DOS like OS made in Assembly and C. 

## Build
Run `sh run.sh`. 

## Run
To run the OS, you can download VM software.
The iso Image is in the `iso` folder.
Reccomended:
- [Virtualbox](https://www.virtualbox.org/wiki/Downloads)
- [VMware](https://my.vmware.com/web/vmware/downloads/info/slug/desktop_end_user_computing/vmware_fusion/12_0)
- [QEMU](https://www.qemu.org)

### Running with VMWARE/Virtualbox
To run with this, simply insert the iso image, and make sure to chose the `other` option.

### Running with qemu
Run with `qemu-system-x86_64 iso/MyOS.iso`.

## Source
The source is in the `src` Folder

## Goals
Not nessecarily in order
- [ ] Own Bootloader
- [x] Filesystem
  - [ ] ext2
    - [x] Read Superblock
    - [ ] Read Files
    - [ ] Write 
  - [ ] Fat32
- [ ] GUI

## Community
https://discord.gg/ymMAcmVV


