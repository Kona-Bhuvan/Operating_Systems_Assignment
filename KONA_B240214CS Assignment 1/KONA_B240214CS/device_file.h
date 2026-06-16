#ifndef DEVICE_FILE_H
#define DEVICE_FILE_H

#include <linux/compiler.h>
#include <linux/types.h>

// Function prototypes to be used in main.c
int register_device(void);
void unregister_device(void);

#endif