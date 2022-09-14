#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "mouse.h"
#include "multiboot.h"
#include "gui.h"
#include "ext2.h"
#include "ide.h"
#include "tss.h" // The tss is kinda buggy rn but keep it here for later
#include "pmm.h"
#include "kheap.h"
#include "vesa.h"
#include "bitmap.h"

//@TODO fix the tss loading from restarting machine

KERNEL_MEMORY_MAP g_kmap;

int get_kernel_memory_map(KERNEL_MEMORY_MAP *kmap, MULTIBOOT_INFO *mboot_info) {
    uint32 i;

    if (kmap == NULL) return -1;
    kmap->kernel.k_start_addr = (uint32)&__kernel_section_start;
    kmap->kernel.k_end_addr = (uint32)&__kernel_section_end;
    kmap->kernel.k_len = ((uint32)&__kernel_section_end - (uint32)&__kernel_section_start);

    kmap->kernel.text_start_addr = (uint32)&__kernel_text_section_start;
    kmap->kernel.text_end_addr = (uint32)&__kernel_text_section_end;
    kmap->kernel.text_len = ((uint32)&__kernel_text_section_end - (uint32)&__kernel_text_section_start);

    kmap->kernel.data_start_addr = (uint32)&__kernel_data_section_start;
    kmap->kernel.data_end_addr = (uint32)&__kernel_data_section_end;
    kmap->kernel.data_len = ((uint32)&__kernel_data_section_end - (uint32)&__kernel_data_section_start);

    kmap->kernel.rodata_start_addr = (uint32)&__kernel_rodata_section_start;
    kmap->kernel.rodata_end_addr = (uint32)&__kernel_rodata_section_end;
    kmap->kernel.rodata_len = ((uint32)&__kernel_rodata_section_end - (uint32)&__kernel_rodata_section_start);

    kmap->kernel.bss_start_addr = (uint32)&__kernel_bss_section_start;
    kmap->kernel.bss_end_addr = (uint32)&__kernel_bss_section_end;
    kmap->kernel.bss_len = ((uint32)&__kernel_bss_section_end - (uint32)&__kernel_bss_section_start);

    kmap->system.total_memory = mboot_info->mem_low + mboot_info->mem_high;

    for (i = 0; i < mboot_info->mmap_length; i += sizeof(MULTIBOOT_MEMORY_MAP)) {
        MULTIBOOT_MEMORY_MAP *mmap = (MULTIBOOT_MEMORY_MAP *)(mboot_info->mmap_addr + i);
        if (mmap->type != MULTIBOOT_MEMORY_AVAILABLE) continue;
        // make sure kernel is loaded at 0x100000 by bootloader(see linker.ld)
        if (mmap->addr_low == kmap->kernel.text_start_addr) {
            // set available memory starting from end of our kernel, leaving 1MB size for functions exceution
            kmap->available.start_addr = kmap->kernel.k_end_addr + 1024 * 1024;
            kmap->available.end_addr = mmap->addr_low + mmap->len_low;
            // get availabel memory in bytes
            kmap->available.size = kmap->available.end_addr - kmap->available.start_addr;
            return 0;
        }
    }

    return -1;
}

void display_kernel_memory_map(KERNEL_MEMORY_MAP *kmap) {
    printf("kernel:\n");
    printf("  kernel-start: 0x%x, kernel-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.k_start_addr, kmap->kernel.k_end_addr, kmap->kernel.k_len);
    printf("  text-start: 0x%x, text-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.text_start_addr, kmap->kernel.text_end_addr, kmap->kernel.text_len);
    printf("  data-start: 0x%x, data-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.data_start_addr, kmap->kernel.data_end_addr, kmap->kernel.data_len);
    printf("  rodata-start: 0x%x, rodata-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.rodata_start_addr, kmap->kernel.rodata_end_addr, kmap->kernel.rodata_len);
    printf("  bss-start: 0x%x, bss-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.bss_start_addr, kmap->kernel.bss_end_addr, kmap->kernel.bss_len);

    printf("total_memory: %d KB\n", kmap->system.total_memory);
    printf("available:\n");
    printf("  start_adddr: 0x%x\n  end_addr: 0x%x\n  size: %d\n",
           kmap->available.start_addr, kmap->available.end_addr, kmap->available.size);
}

BOOL is_cd(char *b) {
    if((b[0]=='c')&&(b[1]=='d'))
        if(b[2]==' '||b[2]=='\0')
            return TRUE;
    return FALSE;
}

BOOL is_cat(char *b) {
    if((b[0]=='c')&&(b[1]=='a')&&(b[2]=='t'))
        if(b[3]==' '||b[3]=='\0')
            return TRUE;
    return FALSE;
}

void kmain(unsigned long magic, unsigned long addr) {
    MULTIBOOT_INFO *mboot_info;
    char buffer[255];
    char *shell = "User@SimpleOS ";
    char *cwd = malloc(sizeof(*cwd)); //Current working directory, in it's path
    cwd = "/";
    char *pwd = "/"; //Previous working directory absolute path
    gdt_init();
    idt_init();
    console_init(COLOR_WHITE, COLOR_BLACK);
    vga_disable_cursor();
    printf("Welcome to SimpleOS!\n");
    printf("Getting Memory Map... ");
    mboot_info = (MULTIBOOT_INFO *)addr;
    if (get_kernel_memory_map(&g_kmap, mboot_info) < 0) {
        printf("error: failed to get kernel memory map\n");
        return;
    }
    // put the memory bitmap at the start of the available memory
    printf("Init PMM... ");
    pmm_init(g_kmap.available.start_addr, g_kmap.available.size);
	pmm_init_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 256);

    // initialize heap 20 blocks(81920 bytes)
    void *start = pmm_alloc_blocks(20);
    void *end = start + (pmm_next_free_frame(1) * PMM_BLOCK_SIZE);
	printf("[DONE]\n");
	printf("Init kheap... ");
    kheap_init(start, end);
    printf("[DONE]\n");
    keyboard_init();
    //Disabled for Now
    /*
    int ret = vesa_init(1024, 768, 32);
    bitmap_draw_string("HELLO WORLD", 150, 100, VBE_RGB(255, 255, 255));
    mouse_init();
    */
    ata_init();
    //read superblock
    read_superblock();
    // Dump ext2 fs info
    ext2_init();
    while(1) {
        printf(shell);
        printf(cwd);
        printf(" >");
        memset(buffer, 0, sizeof(buffer));
        getstr_bound(buffer, strlen(shell));
        if (strlen(buffer) == 0)
            continue;
        if(strcmp(buffer, "help") == 0) {
            printf("SimpleOS Terminal\n");
            printf("Commands: help, ls, cd, cat\n");
        }
        else if(strcmp(buffer, "ls") == 0){
            uint32 ino = ext2_path_to_inode(cwd);
            char **names = ext2_ls(ino);
            printf("\n");
        }
        else if(is_cd(buffer)){
            char* dir = malloc(sizeof(*dir));
            dir = buffer + 3;
            strncat(dir, "/", 1);
            strcat(cwd, dir);
            printf("PWD:%s\n", pwd);
        }
        else if(is_cat(buffer)){
            char *filedata = malloc(sizeof(*filedata));
            char *filepath = malloc(sizeof(*filepath));
            filepath = malloc(strlen(buffer + 4) + 1);
            filepath = buffer + 4;
            if(filepath[0] == '/'){ // Is it an absolute path?
                filedata = ext2_read_file(filepath);
                printf("\n%s", filedata);
            }
            else { //No, its a relative path
                char *absolute = kcalloc(255, 1);
                memcpy(absolute, cwd, strlen(cwd));
                strcat(absolute, filepath);
                printf("\nAbsolute: %s", absolute);
                filedata = ext2_read_file(absolute);
                printf("\n%s", filedata);
                cwd = pwd;
            }
        }
        else {
            printf("Command not found: %s\n", buffer);
        }
    }
}



