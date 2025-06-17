#include "key_input.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include "multi_button.h"

static uint8_t btn1_state, btn2_state, btn3_state;

#if (PRINTF_DEVICE_IFIO == 1)
/**
 * Print static device information (no events). This information includes
 * version numbers, device name and all bits supported by this device.
 *
 * @param fd The file descriptor to the device.
 * @return 0 on success or 1 otherwise.
 */
static int print_device_info(int fd) {
  unsigned int type, code;
  int version;
  unsigned short id[4];
  char name[256] = "Unknown";
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
#ifdef INPUT_PROP_SEMI_MT
  unsigned int prop;
  unsigned long propbits[INPUT_PROP_MAX];
#endif

  if (ioctl(fd, EVIOCGVERSION, &version)) {
    perror("evtest: can't get version");
    return 1;
  }

  printf("Input driver version is %d.%d.%d\n", version >> 16,
         (version >> 8) & 0xff, version & 0xff);

  ioctl(fd, EVIOCGID, id);
  printf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
         id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);

  ioctl(fd, EVIOCGNAME(sizeof(name)), name);
  printf("Input device name: \"%s\"\n", name);

  memset(bit, 0, sizeof(bit));
  ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);
  printf("Supported events:\n");

  for (type = 0; type < EV_MAX; type++) {
    if (test_bit(type, bit[0]) && type != EV_REP) {
      printf("  Event type %d (%s)\n", type, typename(type));
      if (type == EV_SYN)
        continue;
      ioctl(fd, EVIOCGBIT(type, KEY_MAX), bit[type]);
      for (code = 0; code < KEY_MAX; code++)
        if (test_bit(code, bit[type])) {
          printf("    Event code %d (%s)\n", code, codename(type, code));
          if (type == EV_ABS)
            print_absdata(fd, code);
        }
    }
  }

  if (test_bit(EV_REP, bit[0])) {
    printf("Key repeat handling:\n");
    printf("  Repeat type %d (%s)\n", EV_REP,
           events[EV_REP] ? events[EV_REP] : "?");
    print_repdata(fd);
  }
#ifdef INPUT_PROP_SEMI_MT
  memset(propbits, 0, sizeof(propbits));
  ioctl(fd, EVIOCGPROP(sizeof(propbits)), propbits);
  printf("Properties:\n");
  for (prop = 0; prop < INPUT_PROP_MAX; prop++) {
    if (test_bit(prop, propbits))
      printf("  Property type %d (%s)\n", prop, propname(prop));
  }
#endif

  return 0;
}
#endif /* PRINTF_DEVICE_IFIO */

#if (AUTO_FIND_DEVICE == 1)
/**
 * Filter for the AutoDevProbe scandir on /dev/input.
 *
 * @param dir The current directory entry provided by scandir.
 *
 * @return Non-zero if the given directory entry starts with "event", or zero
 * otherwise.
 */
static int is_event_device(const struct dirent *dir) {
  return strncmp(EVENT_DEV_NAME, dir->d_name, 5) == 0;
}

/**
 * Scans all /dev/input/event*, display them and ask the user which one to
 * open.
 *
 * @return The event device file name of the device file selected. This
 * string is allocated and must be freed by the caller.
 */
static char *scan_devices(void) {
  struct dirent **namelist;
  int i, ndev, devnum;
  char *filename;
  int max_device = 0;
  /* is_event_device, 过滤函数
   * versionsort, 排序函数
   */
  ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
  if (ndev <= 0)
    return NULL;

  fprintf(stderr, "Available devices:\n");

  for (i = 0; i < ndev; i++) {
    char fname[64];
    int fd = -1;
    char name[256] = "???";

    snprintf(fname, sizeof(fname), "%s/%s", DEV_INPUT_EVENT,
             namelist[i]->d_name);
    fd = open(fname, O_RDONLY);
    if (fd < 0)
      continue;
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);

    fprintf(stderr, "%s:	%s\n", fname, name);
    close(fd);

    sscanf(namelist[i]->d_name, "event%d", &devnum);
    if (devnum > max_device)
      max_device = devnum;

    free(namelist[i]);
  }

  fprintf(stderr, "Select the device event number [0-%d]: ", max_device);
  scanf("%d", &devnum);

  if (devnum > max_device || devnum < 0)
    return NULL;

  asprintf(&filename, "%s/%s%d", DEV_INPUT_EVENT, EVENT_DEV_NAME, devnum);

  return filename;
}
#endif /* AUTO_FIND_DEVICE */

void get_key_value(int fd)
{
	struct input_event ev[64];
	int i, rd;
	rd = read(fd, ev, sizeof(ev));

    if (rd < (int)sizeof(struct input_event)) {
    //   printf("expected %d bytes, got %d\n", (int)sizeof(struct input_event),
    //          rd);
    //   perror("\nevtest: error reading");
      return 1;
    }

    for (i = 0; i < rd / sizeof(struct input_event); i++) {
      unsigned int type, code;

      type = ev[i].type;
      code = ev[i].code;

      printf("Event: time %ld.%06ld, ", ev[i].time.tv_sec, ev[i].time.tv_usec);

      if (type == EV_KEY) {
        printf("++++++++++++++ KEY:%d,%d ++++++++++++\n", ev[i].code,
               ev[i].value);
			   if(ev[i].code == 106 && ev[i].value <= 1)
			   {
					btn1_state = ev[i].value;
			   }else if(ev[i].code == 108)
			   {
					btn2_state = ev[i].value;
			   }else if(ev[i].code == 105)
			   {
					btn3_state = ev[i].value;
			   }
      } else {
        // do nothing
        printf("++++++++++++++ %d,%d,%d ++++++++++++\n", ev[i].type, ev[i].code,
               ev[i].value);
      }
    }
}

/**
 * Print device events as they come in.
 *
 * @param fd The file descriptor to the device.
 * @return 0 on success or 1 otherwise.
 */
static int print_events(int fd) {
  struct input_event ev[64];
  int i, rd;
  fd_set rdfs;

  FD_ZERO(&rdfs);
  FD_SET(fd, &rdfs);

  while (1) {
    select(fd + 1, &rdfs, NULL, NULL, NULL);
    rd = read(fd, ev, sizeof(ev));

    if (rd < (int)sizeof(struct input_event)) {
      printf("expected %d bytes, got %d\n", (int)sizeof(struct input_event),
             rd);
      perror("\nevtest: error reading");
      return 1;
    }

    for (i = 0; i < rd / sizeof(struct input_event); i++) {
      unsigned int type, code;

      type = ev[i].type;
      code = ev[i].code;

      printf("Event: time %ld.%06ld, ", ev[i].time.tv_sec, ev[i].time.tv_usec);

      if (type == EV_KEY) {
        printf("++++++++++++++ KEY:%d,%d ++++++++++++\n", ev[i].code,
               ev[i].value);
			   if(ev[i].code == 106 && ev[i].value <= 1)
			   {
					btn1_state = ev[i].value;
			   }else if(ev[i].code == 108)
			   {
					btn2_state = ev[i].value;
			   }else if(ev[i].code == 105)
			   {
					btn3_state = ev[i].value;
			   }
      } else {
        // do nothing
        printf("++++++++++++++ %d,%d,%d ++++++++++++\n", ev[i].type, ev[i].code,
               ev[i].value);
      }
    }
  }

  ioctl(fd, EVIOCGRAB, (void *)0);
  return EXIT_SUCCESS;
}

int do_capture_init(const char *device, int grab_flag) {
  int fd;
  char *filename = NULL;

  if (!device) {
    fprintf(stderr, "No device specified, trying to scan all of %s/%s*\n",
            DEV_INPUT_EVENT, EVENT_DEV_NAME);

    /* check euid if it is root */
    if (geteuid() != 0)
      fprintf(stderr, "Not running as root, no devices may be available.\n");
#if AUTO_FIND_DEVICE == 1
    filename = scan_devices();
    if (!filename)
      return EXIT_FAILURE;
#endif
  } else
    filename = strdup(device);

  if (!filename)
    return EXIT_FAILURE;

  if ((fd = open(filename, O_RDONLY)) < 0) {
    perror("evtest");
    if (errno == EACCES && getuid() != 0)
      fprintf(stderr,
              "You do not have access to %s. Try "
              "running as root instead.\n",
              filename);
    goto error;
  }

  if (!isatty(fileno(stdout)))
    setbuf(stdout, NULL);

#if (PRINTF_DEVICE_IFIO == 1)
  if (print_device_info(fd))
    goto error;
#endif

  printf("Testing ... (interrupt to exit)\n");
  /*
    if (test_grab(fd, grab_flag)) // 测试是否是独占设备
    {
      printf("***********************************************\n");
      printf("  This device is grabbed by another process.\n");
      printf("  No events are available to evtest while the\n"
             "  other grab is active.\n");
      printf("  In most cases, this is caused by an X driver,\n"
             "  try VT-switching and re-run evtest again.\n");
      printf("  Run the following command to see processes with\n"
             "  an open fd on this device\n"
             " \"fuser -v %s\"\n",
             filename);
      printf("***********************************************\n");
    }
  */
  //   signal(SIGINT, interrupt_handler);
  //   signal(SIGTERM, interrupt_handler);

  free(filename);

  return print_events(fd);

error:
  free(filename);
  return EXIT_FAILURE;
}
static Button btn1, btn2, btn3;
void *mypthread(void *arg) {
  printf("mypthread \n");
  pthread_t tid = pthread_self(); // 获取本线程tid
  printf("tid=%lld\n", tid);
  while (1) // 进入死循环防止线程退出
  {
    do_capture_init("/dev/input/event0", 0); // Change the device path as needed
  }
  pthread_exit(NULL); // 结束线程
}

uint8_t read_button_value(uint8_t button_id)
{
    switch (button_id) {
        case 1:
            return btn1_state;
        case 2:
            return btn2_state;
		case 3:
            return btn3_state;
        default:
            return 0;
    }
}
void btn_single_click_handler(Button* btn)
{
    (void)btn;  // suppress unused parameter warning
    printf("🔘 Button %d: Single Click\n", btn->button_id);
}

void init_btn(void)
{

	 // Initialize button 1 (active high for simulation)
	 button_init(&btn1, read_button_value, 1, 1);

	 // Attach event handlers for button 1
	 button_attach(&btn1, BTN_SINGLE_CLICK, btn_single_click_handler);
   
	 // Initialize button 2 (active high for simulation)
	 button_init(&btn2, read_button_value, 1, 2);
   
	 // Attach event handlers for button 2
	 button_attach(&btn2, BTN_SINGLE_CLICK, btn_single_click_handler);
   
	   // Initialize button 2 (active high for simulation)
	   button_init(&btn3, read_button_value, 1, 3);
   
	   // Attach event handlers for button 2
	   button_attach(&btn3, BTN_SINGLE_CLICK, btn_single_click_handler);
   
	 // Start button processing
	 button_start(&btn1);
	 button_start(&btn2);
	 button_start(&btn3);
}

void *btn_pthread(void *arg) {
  printf("mypthread \n");
  pthread_t tid = pthread_self(); // 获取本线程tid
  printf("tid=%lld\n", tid);

  // Initialize button 1 (active high for simulation)
  button_init(&btn1, read_button_value, 1, 1);

  // Attach event handlers for button 1
  button_attach(&btn1, BTN_SINGLE_CLICK, btn_single_click_handler);

  // Initialize button 2 (active high for simulation)
  button_init(&btn2, read_button_value, 1, 2);

  // Attach event handlers for button 2
  button_attach(&btn2, BTN_SINGLE_CLICK, btn_single_click_handler);

    // Initialize button 2 (active high for simulation)
	button_init(&btn3, read_button_value, 1, 3);

	// Attach event handlers for button 2
	button_attach(&btn3, BTN_SINGLE_CLICK, btn_single_click_handler);

  // Start button processing
  button_start(&btn1);
  button_start(&btn2);
  button_start(&btn3);
  while (1) // 进入死循环防止线程退出
  {
	button_ticks();
	usleep(5000); // 5ms sleep to simulate a timer tick
	// printf("btn1_state=%d, btn2_state=%d, btn3_state=%d\n", btn1_state, btn2_state, btn3_state);
	// printf("btn1_event=%d, btn2_event=%d, btn3_event=%d\n", button_get_event(&btn1), button_get_event(&btn2), button_get_event(&btn3));
  }
  pthread_exit(NULL); // 结束线程
}

void do_init_test(void) {
  pthread_t tid1;
  pthread_t tid2;
  pthread_attr_t attr;
  if(pthread_attr_init(&attr)) /*初始化线程属性*/
  {
	perror("pthread_attr_init err.");
	return;
  }

  if(pthread_attr_setstacksize(&attr,2 * 1024))
  {
	perror("pthread_attr_setstacksize err.");
	return;
  }

  if (pthread_create(&tid1, &attr, mypthread, NULL) != 0) {
    perror("pthread_create mypthread err.");
    return -1;
  }
  if (pthread_create(&tid2, &attr, btn_pthread, NULL) != 0) {
    perror("pthread_create btn_pthread err.");
    return -1;
  }
  return 0;
}