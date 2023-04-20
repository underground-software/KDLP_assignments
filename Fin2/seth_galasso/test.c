/**************************   Stack State Notation   **************************/
/*                                                                            */
/* The first row represents the state of the file (1 == fd1, 2 == fd2, etc.)  */
/* The second row represents the characters in the stack (growing from left   */
/* to right). Any operation that modifies the stack's state will be marked    */
/* with a similar diagram.                                                    */
/*                                                                            */
/* Example                                                                    */
/* |     1     2 |  <-- fd1's top is 'E', fd2's top is 'O'                    */
/* |   H E L L O |                                                            */
/*                                                                            */
/******************************************************************************/

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEV_FILE "/dev/stack"
#define IOC_STACK_TO_TOP _IO(0x11, 0)
#define IOC_STACK_PEEK _IOR(0x11, 1, char)
#define IOC_STACK_GET_SIZE _IOR(0x11, 2, size_t)

int main(void)
{
	int fd1, fd2, i_ret;
	size_t size;
	ssize_t ss_ret;
	off_t pos;
	char peek, buff[1024], buff2[1024];

	memset(buff, -1, 1024);
	printf("Begin testing of %s\n", DEV_FILE);

	// Open fd1 for reading and writing.
	// | 1 |
	// |   |
	fd1 = open(DEV_FILE, O_RDWR);
	if (fd1 < 0)
		err(1, "Unable to open fd1: %s", DEV_FILE);

	// Test initial read.
	ss_ret = read(fd1, buff, 4);
	if (ss_ret < 0)
		err(1, "Unable to read from fd1");
	if (ss_ret != 0)
		warnx("Expected to read 0 bytes from empty stack. "
		      "Got %zu instead", ss_ret);

	// Test initial ioctl peek.
	peek = 'A';
	i_ret = ioctl(fd1, IOC_STACK_PEEK, &peek);
	if (i_ret < 0)
		err(1, "Unable to perform ioctl IOC_STACK_PEEK "
		    "on empty stack with fd1.");
	if (i_ret != 0)
		warnx("Expected return value of 0 on initial ioctl "
		      "IOC_STACK_PEEK on fd1. Got %d.", i_ret);
	if (peek != '\0')
		warnx("Initial IOC_STACK_PEEK on fd1 return %c, instead of \\0",
		      peek);

	// Test initial ioctl get size.
	size = 1337;
	i_ret = ioctl(fd1, IOC_STACK_GET_SIZE, &size);
	if (i_ret < 0)
		err(1, "Unable to perform ioctl IOC_STACK_GET_SIZE on empty "
		    "stack with fd1.");
	if (i_ret != 0)
		warnx("Expected return value of 0 on initial ioctl "
		      "IOC_STACK_GET_SIZE on fd1. Got %d.", i_ret);
	if (size != 0)
		warnx("Initial IOC_STACK_GET_SIZE on fd1 return %zu, rather "
		      "than 0.", size);

	// Test initial write
	// |         1 |
	// |   K N E L |
	ss_ret = write(fd1, "KNEL", 4);
	if (ss_ret < 0)
		err(1, "Unable to write to fd1.");
	if (ss_ret != 4)
		warn("Expected to write to consume 4 bytes. Got %zu instead",
		     ss_ret);

	// Test that lseek fails when seeking beyond the stack
	pos = lseek(fd1, 1, SEEK_CUR);
	if (pos >= 0)
		warn("Expected lseek to fail when seeking out of stack bounds. "
		     "Offset is now at %ld instead.", pos);

	// Test that lseek succeeds on valid input.
	// |   1       |
	// |   K N E L |
	pos = lseek(fd1, 1, SEEK_SET);
	if (pos != 1)
		warnx("Expected lseek to return 1, got %ld instead.", pos);

	// Test ioctl peek on actual stack element
	peek = 'A';
	i_ret = ioctl(fd1, IOC_STACK_PEEK, &peek);
	if (i_ret < 0)
		err(1, "Unable to perform ioctl IOC_STACK_PEEK on fd1");
	if (i_ret != 0)
		warnx("Expected return value of 0 on second ioctl "
		      "IOC_STACK_PEEK on fd1. Got %d.", i_ret);
	if (peek != 'K')
		warnx("Expected IOC_STACK_PEEK to fill char pointer with 'K', "
		      "got %c instead.", peek);

	// Test write after lseek
	// |       1       |
	// |   K E R N E L |
	ss_ret = write(fd1, "ER", 2);
	if (ss_ret < 0)
		err(1, "Unable to write to fd1.");
	if (ss_ret != 2)
		warn("Expected to write to consume 2 bytes. Got %zu instead",
		     ss_ret);

	// Open fd2 for reading and writing
	// |       1     2 |
	// |   K E R N E L |
	fd2 = open(DEV_FILE, O_RDWR);
	if (fd2 < 0)
		err(1, "Unable to open fd2: %s", DEV_FILE);

	// Verify that fd2's top of stack is correct.
	peek = 'L';
	i_ret = ioctl(fd2, IOC_STACK_PEEK, &peek);
	if (i_ret < 0)
		err(1, "Unable to perform ioctl IOC_STACK_PEEK on fd2");
	if (i_ret != 0)
		warnx("Expected return value of 0 on second ioctl "
		      "IOC_STACK_PEEK on fd2. Got %d.", i_ret);
	if (peek != 'L')
		warnx("Expected IOC_STACK_PEEK to fill char pointer with 'L', "
		      "got %c instead.", peek);

	// Check ioctl get size on a non-empty stack.
	size = 1337;
	i_ret = ioctl(fd2, IOC_STACK_GET_SIZE, &size);
	if (i_ret < 0)
		err(1, "Unable to perform ioctl IOC_STACK_GET_SIZE with fd2.");
	if (i_ret != 0)
		warnx("Expected return value of 0 on ioctl "
		      "IOC_STACK_GET_SIZE on fd2. Got %d.", i_ret);
	if (size != 6)
		warnx("Initial IOC_STACK_GET_SIZE on fd2 return %zu, rather "
		      "than 6.", size);

	// Verify that reading a single character behaves as expected, also
	// invalidating fd2
	// |     1       2 |
	// |   K E N E L   |
	ss_ret = read(fd1, buff, 1);
	if (ss_ret < 0)
		err(1, "Unable to read from fd1");
	if (ss_ret != 1)
		warnx("Expected to read 1 byte, got %zu instead", ss_ret);
	if (memcmp("R", buff, 1) != 0)
		warnx("Expected to read R from fd1, got %1s instead", buff);

	// Verify that reading fails on an invalidated fd.
	ss_ret = read(fd2, buff, 1);
	if (ss_ret >= 0)
		warnx("Read from an invalidated fd, buffer contains %1s.",
		      buff);

	// Verify that writing fails on an invalidated fd.
	ss_ret = write(fd2, "A", 1);
	if (ss_ret >= 0)
		warnx("Wrote %zu bytes to invalid fd.", ss_ret);

	// Verify that ioctl peek fails on an invalidated fd.
	// peek = 'L';
	i_ret = ioctl(fd2, IOC_STACK_PEEK, &peek);
	if (i_ret >= 0)
		warnx("Performed ioctl IOC_STACK_PEEK on invalid fd. "
		      "Got %d as return value. Character ptr contains %c.",
		      i_ret, peek);

	// Test ioctl to top on invalid fd.
	// |     1     2 |
	// |   K E N E L |
	i_ret = ioctl(fd2, IOC_STACK_TO_TOP);
	if (i_ret < 0)
		err(1, "Unable to perform IOC_STACK_TO_TOP on fd2.");
	if (i_ret != 0)
		warnx("Expected return value of 0 from IOC_STACK_TO_TOP on fd2 "
		      ", got %d instead.", i_ret);

	// Verify that reading multiple characters behaves as expected.
	// | 2   1 |
	// |       |
	ss_ret = read(fd2, buff, 5);
	if (ss_ret < 0)
		err(1, "Unable to read from fd2");
	if (ss_ret != 5)
		warnx("Expected to read 5 bytes, got %zu instead", ss_ret);
	if (memcmp("LENEK", buff, 5) != 0)
		warnx("Expected to read LENEK from fd2, got %5s instead", buff);

	// Fill the buffer ('_' represents a space)
	// |     1             2 |
	// |   _ _ _ _ _ ... _ A |
	sprintf(buff2, "%50c", 'A');
	ss_ret = write(fd2, buff2, 50);
	if (ss_ret < 0)
		err(1, "Unable to write to fd2.");
	if (ss_ret != 50)
		warn("Expected to write to consume 50 bytes. Got %zu instead",
		     ss_ret);

	// Verify that writing to a full buffer does not change it
	ss_ret = write(fd2, "A", 1);
	if (ss_ret < 0) {
		err(1, "Unable to write to fd2.");
	}
	if (ss_ret != 0) {
		warnx("Wrote %zu bytes to full stack.", ss_ret);
	}

	// Empty the buffer
	// | 2   1 |
	// |       |
	sprintf(buff2, "%-50c", 'A');
	ss_ret = read(fd2, buff, 50);
	if (ss_ret < 0)
		err(1, "Unable to read from fd2");
	if (ss_ret != 50)
		warnx("Expected to read 50 bytes, got %zu instead", ss_ret);
	if (memcmp(buff2, buff, 50) != 0)
		warnx("Expected to read reverse of input from fd2, got %50s "
		      "instead", buff);

	// Close the files
	i_ret = close(fd1);
	if(i_ret < 0)
		err(1, "Failed to close fd1.");

	i_ret = close(fd2);
	if(i_ret < 0)
		err(1, "Failed to close fd2.");

	printf("End testing of %s.\n", DEV_FILE);
	return 0;
}
