#ifndef SHELL_H
#define SHELL_H

#include "types.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/screen/screen.h"
#include "utils.h"
#include "fs/vfs.h"
#include "panic.h"

void launch_shell(int n);

#endif