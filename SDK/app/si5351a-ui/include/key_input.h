#ifndef __KEY_INPUT__H_
#define __KEY_INPUT__H_

#include <linux/input.h>

#define PRINTF_DEVICE_IFIO 0

#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

int do_capture_init(const char *device, int grab_flag) ;

#endif