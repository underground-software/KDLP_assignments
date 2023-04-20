#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define DEV_FILE "/dev/morse"
#define IOCTL_MORSE_RESET _IO(0x11, 0)
#define IOCTL_MORSE_SET_PLAIN _IO(0x11, 1)
#define IOCTL_MORSE_SET_MORSE _IO(0x11, 2)

int main()
{
	int fd;
	ssize_t ret;
	char message[] = "SOS";
	char buffer[256];

	fd = open(DEV_FILE, O_RDWR);
	if (fd < 0)
		err(1, "unable to open file");


	//performing read on fd
	ret = read(fd, buffer, sizeof(buffer));
	if(0 > ret)
		err(1, "unable to read from device");
	if(0 != ret)
		warnx("initial read test failed, expected zero, got %zd", ret);

	// Send a message to the driver
	ret = write(fd, message, sizeof(message));
	if (ret < 0) {
		perror("Failed to write to device");
		exit(-1);
	}
	if(3 != ret)
		warnx("expected write to consume all 3 bytes, got %zd", ret);

	// Switch to Morse code output mode
	ret = ioctl(fd, IOCTL_MORSE_SET_MORSE, NULL);
	if (ret < 0) {
		perror("Failed to toggle output mode to Morse");
		exit(-1);
	}

	// Read the Morse code output from the driver
	ret = read(fd, buffer, sizeof(buffer));
	if (ret < 0) {
		perror("Failed to read from device");
		exit(-1);
	}
	if(11 != ret)
		warnx("expected write to consume all 11 bytes, got %zd from fd", ret);

	if(0 != memcmp("... --- ...", buffer, 11))
			warnx("expected to read translated morse code, got %.*s", (int)ret, buffer);

	//Checking if lseek fails properly on SET MORSE MODE
	int pos = lseek(fd, 1, SEEK_SET);
        if(EINVAL != pos){
                perror("Expected EINVAL return from calling lseek in IOCTL_MORSE_SET_MORSE");
                exit(-1);
        }
	// Verify that the output is correct
	//printf("Expected Morse code: ... --- ...\n");
	//printf("Actual Morse code:   %s\n", buffer);

	// Switch back to plaintext output mode
	ret = ioctl(fd, IOCTL_MORSE_SET_PLAIN, NULL);
	if (ret < 0) {
		perror("Failed to toggle output mode to plaintext");
		exit(-1);
	}

	//performing lseek on fd
	int pos = lseek(fd, 1, SEEK_SET);
	if(1 != pos)
		warnx("expected 1, got %ld", pos);
	char c;
	if(1 != read(fd, &c, 1))
		warnx("expected return of 1");
	if(c != 'O')
		warnx("expected O");

	// Reset the device to its initial state
	ret = ioctl(fd, IOCTL_MORSE_RESET, NULL);
	if (ret < 0) {
		perror("Failed to reset device");
		exit(-1);
	}

	//performing read on fd
	ret = read(fd, buffer, sizeof(buffer));
	if(0 > ret)
		err(1, "unable to read from device");
	if(0 != ret)
		warnx("IOCTL_MORSE_RESET test failed, expected zero from read, got %zd", ret);


	//testing that the whole alphabet works
	int fd2;
	fd2 = open(DEV_FILE, O_RDWR);
        if (fd2 < 0)
                err(1, "unable to open file");

	// Send a message to the driver
	char alpha[] = "abcdefghijklmnopqrstuvwxyz";
        ret = write(fd2, alpha, sizeof(alpha));
        if (ret < 0) {
                perror("Failed to write to device");
                exit(-1);
        }
        if(26 != ret)
                warnx("expected write to consume all 26 bytes, got %zd", ret);

	ret = read(fd2, buffer, sizeof(buffer));
        if(0 > ret)
                err(1, "unable to read from device");
        if(107 != ret)
                warnx("Expected 107, got %zd", ret);
	if(0 != memcmp(".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --..", buffer, 107))
                        warnx("expected to read translated morse language, got %.*s", (int)ret, buffer);

	close(fd);
	close(fd2);
	return 0;
}
