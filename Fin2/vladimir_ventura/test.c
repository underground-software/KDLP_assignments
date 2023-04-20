#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

#define DEV_FILE "/dev/calc"
#define IOC_CALC_ADD _IO(0x11, 0)
#define IOC_CALC_SUB _IO(0x11, 1)
#define IOC_CALC_MULT _IO(0x11, 2)
#define IOC_CALC_DIV _IO(0x11, 3)
#define IOC_CALC_RESET _IO(0x11, 4)


int main(void)
{
	char buff[1024];

	int fd = open(DEV_FILE, O_RDWR);

	puts("start of testing");

	//Test Opening
	if(0 > fd)
		err(1, "unable to open file");

	//Test Write if 0 entered should return error
	ssize_t ret_write = write(fd, "0 2", 3);
	if(0 > ret_write)
		err(1, "Unable to write to device");

	//Test Write
	ret_write = write(fd, "1 2", 3);
	if(0 > ret_write)
		err(1, "Unable to write to device");

	//Test Read
	ssize_t ret_read = read(fd, buff, sizeof buff);
	if(0 > ret_read)
		err(1, "unable to read from device");
	if(0 != memcmp("1 + 2 = 3", buff, 9))
		warnx("expected to get a result 1 + 2 = 3, got %.*s", (int)ret_read, buff);

	//Test ioctl add
	{
		int ret_ioctl = ioctl(fd, IOC_CALC_RESET, NULL);
		if(0 > ret_ioctl)
			err(1, "unable to preform ioctl on device");
		if(0 != ret_ioctl)
			warnx("expected return of 0 from ioctl got %d", ret_ioctl);

		ret_ioctl = ioctl(fd, IOC_CALC_ADD, NULL);
		if(0 > ret_ioctl)
			err(1, "unable to preform ioctl on device");
		if(0 != ret_ioctl)
			warnx("expected return of 0 from ioctl got %d", ret_ioctl);

		ssize_t ret_write1 = write(fd, "1 2", 3);
		if(0 > ret_write1)
			err(1, "Unable to write to device");

		ssize_t ret_size1 = read(fd, buff, sizeof buff);
		if(0 > ret_size1)
			err(1, "unable to read from device");
		if(0 != memcmp("1 + 2 = 3", buff, 9))
			warnx("expected to get a result 1 + 2 = 3, got %.*s", (int)ret_size1, buff);
	}

	//Test ioctl Sub
	{
		int ret_ioctl = ioctl(fd, IOC_CALC_RESET, NULL);
		if(0 > ret_ioctl)
			err(1, "unable to preform ioctl on device");
		if(0 != ret_ioctl)
			warnx("expected return of 0 from ioctl got %d", ret_ioctl);

		ret_ioctl = ioctl(fd, IOC_CALC_SUB, NULL);
		if(0 > ret_ioctl)
			err(1, "unable to preform ioctl on device");
		if(0 != ret_ioctl)
			warnx("expected return of 0 from ioctl got %d", ret_ioctl);

		ssize_t ret_write1 = write(fd, "1 2", 3);
		if(0 > ret_write1)
			err(1, "Unable to write to device");

		ssize_t ret_size1 = read(fd, buff, sizeof buff);
		if(0 > ret_size1)
			err(1, "unable to read from device");
		if(0 != memcmp("1 - 2 = -1", buff, 9))
			warnx("expected to get a result 1 - 2 = -1, got %.*s", (int)ret_size1, buff);
	}

	//Test ioctl mult
	{
		int ret_ioctl = ioctl(fd, IOC_CALC_RESET, NULL);
		if(0 > ret_ioctl)
			err(1, "unable to preform ioctl on device");
		if(0 != ret_ioctl)
			warnx("expected return of 0 from ioctl got %d", ret_ioctl);

		ret_ioctl = ioctl(fd, IOC_CALC_MULT, NULL);
		if(0 > ret_ioctl)
			err(1, "unable to preform ioctl on device");
		if(0 != ret_ioctl)
			warnx("expected return of 0 from ioctl got %d", ret_ioctl);

		ssize_t ret_write1 = write(fd, "1 2", 3);
		if(0 > ret_write1)
			err(1, "Unable to write to device");

		ssize_t ret_size1 = read(fd, buff, sizeof buff);
		if(0 > ret_size1)
			err(1, "unable to read from device");
		if(0 != memcmp("1 * 2 = 2", buff, 9))
			warnx("expected to get a result 1 * 2 = 2, got %.*s", (int)ret_size1, buff);
	}

	//Test ioctl divide
	{
		int ret_ioctl = ioctl(fd, IOC_CALC_RESET, NULL);
		if(0 > ret_ioctl)
			err(1, "unable to preform ioctl on device");
		if(0 != ret_ioctl)
			warnx("expected return of 0 from ioctl got %d", ret_ioctl);

		ret_ioctl = ioctl(fd, IOC_CALC_MULT, NULL);
		if(0 > ret_ioctl)
			err(1, "unable to preform ioctl on device");
		if(0 != ret_ioctl)
			warnx("expected return of 0 from ioctl got %d", ret_ioctl);

		ssize_t ret_write1 = write(fd, "1 2", 3);
		if(0 > ret_write1)
			err(1, "Unable to write to device");

		ssize_t ret_size1 = read(fd, buff, sizeof buff);
		if(0 > ret_size1)
			err(1, "unable to read from device");
		if(0 != memcmp("1 / 2 = 0", buff, 9))
			warnx("expected to get a result 1 / 2 = 0, got %.*s", (int)ret_size1, buff);
	}

	//Test seek
	off_t pos = lseek(fd, 2, SEEK_SET);
	if(2 != pos)
		warnx("expected 2, got %ld", pos);

	puts("end of testing");
	close(fd);

	return 0;
}
