/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#ifndef SHELL_H
#define SHELL_H

#include "types.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/screen/screen.h"
#include "utils.h"
#include "fs/vfs.h"
#include "panic.h"
#include "drivers/rtc/rtc.h"
#include "drivers/mouse/mouse.h"
#include "fs/fs.h"
#include "drivers/acpi/acpi.c"

void launch_shell(int n);
void launch_shell_root(int n);

#endif