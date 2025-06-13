#include "key_input.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

/**
 * Print static device information (no events). This information includes
 * version numbers, device name and all bits supported by this device.
 *
 * @param fd The file descriptor to the device.
 * @return 0 on success or 1 otherwise.
 */
static int print_device_info(int fd)
{
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

	printf("Input driver version is %d.%d.%d\n",
		version >> 16, (version >> 8) & 0xff, version & 0xff);

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
			if (type == EV_SYN) continue;
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
		printf("  Repeat type %d (%s)\n", EV_REP, events[EV_REP] ?  events[EV_REP] : "?");
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
static char* scan_devices(void)
{
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

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		int fd = -1;
		char name[256] = "???";

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
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

	asprintf(&filename, "%s/%s%d",
		 DEV_INPUT_EVENT, EVENT_DEV_NAME,
		 devnum);

	return filename;
}

static int do_capture_init(const char *device, int grab_flag) {
  int fd;
  char *filename = NULL;

  if (!device) {
    fprintf(stderr, "No device specified, trying to scan all of %s/%s*\n",
            DEV_INPUT_EVENT, EVENT_DEV_NAME);

    /* check euid if it is root */
    if (geteuid() != 0)
      fprintf(stderr, "Not running as root, no devices may be available.\n");

    filename = scan_devices();
    if (!filename)
      return EXIT_FAILURE;
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

  if (print_device_info(fd))
    goto error;

  printf("Testing ... (interrupt to exit)\n");

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

  signal(SIGINT, interrupt_handler);
  signal(SIGTERM, interrupt_handler);

  free(filename);

  return print_events(fd);

error:
  free(filename);
  return EXIT_FAILURE;
}