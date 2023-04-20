#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct pig_ioctl_args {
	char *buff;
	size_t buff_sz;
};

#define DEV_FILE "/dev/igpay"
#define IOC_PIG_ENCODE_MSG _IO(0x11, 0)
#define IOC_PIG_GET_ORIG   _IOR(0x11, 1, struct pig_ioctl_args)
#define IOC_PIG_RESET      _IO(0x11, 2)
#define IOC_PIG_MSG_LEN    _IOR(0x11, 3, size_t)
#define IOC_PIG_NUM_TRANS  _IOR(0x11, 4, size_t)

#define TEST_STR "a pig latin test string"
#define TEST_STR_ENCODE1 "ayay igpay atinlay esttay ringstay"
#define TEST_STR_ENCODE2 "ayayyay igpayyay atinlayyay ingstayray"

#define TEST_STR2 "another test message"
#define TEST_STR_2_ENCODE1 "anotheryay esttay essagemay"

int main(void)
{
	int fd = open(DEV_FILE, O_RDWR);

	if (fd < 0)
		err(1, "unable to open file");

	printf("Beginning of testing.\n");

	char buff[1024];
	{
		ssize_t ret = read(fd, buff, sizeof(buff));

		if (ret < 0)
			err(1, "unable to read from device");
		if (ret != 0)
			warnx(
				"initial read test failed, expected 0, got %zd",
				ret);
	}
	{
		struct pig_ioctl_args args;
		ssize_t ret = write(fd, TEST_STR, sizeof(TEST_STR));

		if (ret < 0)
			err(1, "Unable to write to device");
		if (ret != sizeof(TEST_STR))
			warnx(
				"expected write to consume all %lu bytes, got %zd",
				sizeof(TEST_STR),
				ret);

		args.buff = NULL;
		ret = ioctl(fd, IOC_PIG_GET_ORIG, &args);
		if (ret == 0)
			err(1, "expected error return from NULL input");

		args.buff_sz = 1024;
		args.buff = (char *) malloc(args.buff_sz);
		if (args.buff == NULL) {
			printf("malloc failed\n");
			exit(-1);
		}

		ret = ioctl(fd, IOC_PIG_GET_ORIG, &args);
		if (ret < 0)
			err(1, "unable to read from device");

		if (memcmp(TEST_STR, args.buff, sizeof(TEST_STR)) != 0)
			warnx(
				"expected %s, got %s",
				TEST_STR,
				args.buff);
		printf("Read out %s\n", args.buff);

	}
	{
		ssize_t ret = read(fd, buff, sizeof(buff));

		if (ret < 0)
			err(1, "unable to read from device");
		if (ret != sizeof(TEST_STR_ENCODE1))
			warnx(
				"read test failed, expected %lu, got %zd",
				sizeof(TEST_STR),
				ret);
		if (memcmp(TEST_STR_ENCODE1, buff, sizeof(TEST_STR_ENCODE1)) != 0)
			warnx(
				"expected pig latin-ized string, got %s",
				buff);
		printf("Read out %s\n", buff);

		ret = ioctl(fd, IOC_PIG_ENCODE_MSG, NULL);

		if (ret < 0)
			err(1, "unable to perform ioctl on device");
		if (ret != 0)
			warnx("expected return of 0 from ioctl got %zd", ret);

		ret = read(fd, buff, sizeof(buff));

		if (ret < 0)
			err(1, "unable to read from device");
		if (ret != sizeof(TEST_STR_ENCODE2))
			warnx(
				"read test failed, expected %lu, got %zd",
				sizeof(TEST_STR_ENCODE2),
				ret);
		if (memcmp(TEST_STR_ENCODE2, buff, sizeof(TEST_STR_ENCODE2)) != 0)
			warnx(
				"expected doubly-encoded string, got %s",
				buff);
		printf("Read out %s\n", buff);
	}
	{
		size_t size = 1337;
		ssize_t ret = ioctl(fd, IOC_PIG_MSG_LEN, &size);

		if (ret < 0)
			err(1, "unable to perform ioctl on device");
		if (ret != 0)
			warnx("expected return of 0 from ioctl got %zd", ret);
		if (size != sizeof(TEST_STR_ENCODE2))
			warnx(
				"expected size of %lu, got %zu",
				sizeof(TEST_STR_ENCODE2),
				size);

		ret = write(fd, TEST_STR2, sizeof(TEST_STR2));

		if (ret < 0)
			err(1, "Unable to write to device");
		if (ret != sizeof(TEST_STR2))
			warnx(
				"expected write to consume all %lu bytes, got %zd",
				sizeof(TEST_STR2),
				ret);

		size = 1337;
		ret = ioctl(fd, IOC_PIG_NUM_TRANS, &size);

		if (ret < 0)
			err(1, "unable to perform ioctl on device");
		if (ret != 0)
			warnx("expected return of 0 from ioctl got %zd", ret);
		if (size != 3)
			warnx(
				"expected 3 translations total, got %zu",
				size);
	}
	{
		int fd2 = open(DEV_FILE, O_RDONLY);
		char c;

		if (fd2 < 0)
			err(1, "unable to open device");

		ssize_t ret = read(fd2, buff, sizeof(buff));

		if (ret < 0)
			err(1, "unable to read into buff");
		if (ret != 7)
			warnx(
				"expected to read %lu bytes, got %zd",
				sizeof(TEST_STR_2_ENCODE1),
				ret);

		off_t pos = lseek(fd2, 2, SEEK_SET);

		if (pos != 2)
			warnx("expected 2, got %ld", pos);

		if (read(fd2, &c, 1) != 1)
			warnx("expected return of 1");
		if (c != 'o')
			warnx("expected o, got %c", c);

		close(fd2);
	}
	{
		ssize_t ret = ioctl(fd, IOC_PIG_RESET, NULL);

		if (ret < 0)
			err(1, "unable to perform ioctl on device");
		if (ret != 0)
			warnx("expected return of 0 from ioctl got %zd", ret);

		ret = read(fd, buff, sizeof(buff));

		if (ret < 0)
			err(1, "unable to read from device");
		if (ret != 0)
			warnx(
				"test failed, expected 0, got %zd",
				ret);
	}
	printf("End of testing.\n");
	close(fd);

	return 0;
}
