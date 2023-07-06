/* SPDX-License-Identifier: GPL */

#ifndef ASCIIART_H
#define ASCIIART_H

#ifdef __KERNEL__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h>
#endif

#define DEV_FILE "/dev/asciiart"

#define IOC_ASCIIART_RESET _IO(0x11, 0)
#define IOC_ASCIIART_GET_LENGTH _IOR(0x11, 1, size_t)

#endif // ASCIIART_H
