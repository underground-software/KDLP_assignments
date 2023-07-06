// SPDX-License-Identifier: GPL

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#include "asciiart.h"
#include "test.h"

#define BUFFER_SIZE 2048

/* This test file tests the character driver /dev/asciiart. This
driver converts any text written to it into an ASCII art form with
a [modified] dotmatrix font. */


/* Operations to test:

0- open file
1- read file
2- write to file
3- read file
4- ioctl reset
5- read file
6- write to file
7- llseek (all 3)
*/

int main(void)
{
	printf("Initializing test...\n");
	ssize_t ret;
	size_t obtained_length;

	// 0- Test opening the file
	int fd = open(DEV_FILE, O_RDWR);

	// FOR DEBUGGING CONSISTENCY: RESETTING THE DEVICE TO RUN TEST SUCCESSIVELY
	ret = ioctl(fd, IOC_ASCIIART_RESET);

	if (fd < 0)
		err(1, "Unable to open device %s", DEV_FILE);

	char buf[BUFFER_SIZE] = { '\0' };

	// 1- Test an initial read
	ret = read(fd, buf, sizeof(buf));

	if (ret < 0)
		err(1, "Unable to read from device %s", DEV_FILE);
	if (ret != 0)
		warnx("Initial read test failed, expected zero, got %zd", ret);

	// 2- Test writing to the device
	ret = write(fd, TEST_HELLO, sizeof(TEST_HELLO));

	if (ret < 0)
		err(1, "Unable to write to device %s", DEV_FILE);
	if (ret != sizeof(TEST_HELLO))
		warnx("Expected write to consume all %zd bytes, got %zd",
			sizeof(TEST_HELLO), ret);

	// 3- Test reading back after a write()
	memset(buf, '\0', sizeof(buf));
	ret = read(fd, buf, sizeof(buf));
	if (ret < 0)
		err(1, "Unable to read from device %s", DEV_FILE);
	if ((size_t)ret != strlen(TEST_HELLO_ART))
		warnx("Expected to read %zd bytes, got %zd",
				sizeof(TEST_HELLO_ART), ret);
	if (memcmp(buf, TEST_HELLO_ART, sizeof(TEST_HELLO_ART)) != 0)
		warnx("Expected read to return:\n%s\nBut, got:\n%s\n",
				TEST_HELLO_ART, buf);

	// 4- Test ioctl reset
	ret = ioctl(fd, IOC_ASCIIART_RESET);

	if (ret < 0)
		err(1, "Unable to perform ioctl to reset device %s\n",
			DEV_FILE);

	// 5- Test reading back after ioctl reset
	ret = read(fd, buf, sizeof(buf));
	if (ret < 0)
		err(1, "Unable to read from device %s", DEV_FILE);
	if (ret != 0)
		warnx("read test after IOC_ASCIIART_RESET failed, expected zero, got %zd",
				ret);

	// 6- Test ioctl get length
	ret = write(fd, TEST_NUM, sizeof(TEST_NUM));
	if (ret < 0)
		err(1, "Unable to write to device %s", DEV_FILE);
	if (ret != sizeof(TEST_NUM))
		warnx("Expected write to consume all %zd bytes, got %zd",
			sizeof(TEST_NUM), ret);

	ret = ioctl(fd, IOC_ASCIIART_GET_LENGTH, &obtained_length);
	if (ret < 0)
		err(1, "Unable to perform ioctl to get length of device %s\n",
			DEV_FILE);
	if (obtained_length != strlen(TEST_NUM_ART))
		warnx("Expected ioctl to return length %zd, got %zd",
				strlen(TEST_NUM_ART), ret);

	// 7- Test llseek
	ret = write(fd, TEST_HELLO, sizeof(TEST_HELLO));
	if (ret < 0)
		err(1, "Unable to write to device %s", DEV_FILE);
	if (ret != sizeof(TEST_HELLO))
		warnx("Expected write to consume all %zd bytes, got %zd",
			sizeof(TEST_HELLO), ret);

	ret = lseek(fd, 5, SEEK_SET);
	if (ret < 0)
		err(1, "Unable to perform lseek on device %s\n", DEV_FILE);
	if (ret != 5)
		warnx("Expected lseek to return offset 5, got %zd", ret);

	ret = lseek(fd, 4, SEEK_CUR);
	if (ret < 0)
		err(1, "Unable to perform lseek on device %s\n", DEV_FILE);
	if (ret != 9)
		warnx("Expected lseek to return offset 9, got %zd", ret);

	ret = lseek(fd, -2, SEEK_END);
	if (ret < 0)
		err(1, "Unable to perform lseek on device %s\n", DEV_FILE);
	if ((size_t) ret != strlen(TEST_HELLO_ART) - 2)
		warnx("Expected lseek to return offset %zd, got %zd",
				strlen(TEST_HELLO_ART) - 2, ret);

	close(fd);
	printf("Test finished.\n");

	return 0;
}
