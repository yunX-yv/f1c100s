#ifndef __KEY_INPUT__H_
#define __KEY_INPUT__H_

#include <linux/input.h>
#include "multi_button.h"
#define PRINTF_DEVICE_IFIO 0

#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

int do_capture_init(const char *device, int grab_flag) ;
void do_init_test(void);
void init_btn(void);
#endif