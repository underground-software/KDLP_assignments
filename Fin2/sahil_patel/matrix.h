#pragma once

#include <linux/ioctl.h>

#define SEEK_VERTICAL (0)
#define SEEK_HORIZONTAL (1)

#define MAT_IO_RESIZE _IO(0x11, 0)
#define MAT_IO_ROTATE _IO(0x11, 1)
