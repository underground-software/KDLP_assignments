// This test file is for the Hangman character driver
// This test file depends on the secret word for hangman to be 'kernel'
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define IOC_NEW_GAME _IO(0x11, 0)
#define IOC_SURRENDER _IO(0x11, 1)
int main(void)
{
	char* buf[28];
	const char* guess_1 = "k";
	const char* guess_2 = "e";
	const char* guess_3 = "j";
	const char* guess_4 = "z";
	const char* initial_state = "______\n12 Guesses Remaining";
	const char* state_1 = "k_____\n12 Guesses Remaining";
	const char* state_2 = "ke__e_\n12 Guesses Remaining";
	const char* state_3 = "ke__e_\n11 Guesses Remaining";
	const char* state_4 = "kernel\nGame Over!";
	const char* state_5 = "kernel\nYou Win!";
	int num_tests = 0;
	int passed_tests = 0;
	printf("start of testing\n\n");
	num_tests += 1;
	int fd = open("/dev/hangman", O_RDWR);
	if (fd == -1)
		printf("error opening device\n");
	else
		passed_tests += 1;
	num_tests += 1;
	if(read(fd, buf, sizeof(buf)) != sizeof(buf))
		printf("error reading from device\n");
	else {
		if(!strcmp((const char*)buf, initial_state))
			passed_tests += 1;
		else
			printf("test %d failed\n", num_tests);
	}
	num_tests += 1;
	if(write(fd, guess_1, 1) != 1)
		printf("error writing to device\n");
	else {
		if(read(fd, buf, sizeof(buf)) != sizeof(buf))
			printf("error reading from device\n");
		if(!strcmp((const char*)buf, state_1))
			passed_tests += 1;
		else
			printf("test %d failed\n", num_tests);
	}
	num_tests += 1;
	if(write(fd, guess_2, 1) != 1)
		printf("error writing to device\n");
	else {
		if(read(fd, buf, sizeof(buf)) != sizeof(buf))
			printf("error reading from device\n");
		if(!strcmp((const char*)buf, state_2))
			passed_tests += 1;
		else
			printf("test %d failed\n", num_tests);
	}
	num_tests += 1;
	if(write(fd, guess_3, 1) != 1)
		printf("error writing to device\n");
	else {
		if(read(fd, buf, sizeof(buf)) != sizeof(buf))
			printf("error reading from device\n");
		if(!strcmp((const char*)buf, state_3))
			passed_tests += 1;
		else
			printf("test %d failed\n", num_tests);
	}
	num_tests += 1;
	// make 11 incorrect guesses so we can verify we get game over
	for(int i = 0; i < 11; ++i)
	{
		if(write(fd, guess_4, 1) != 1)
			printf("error writing to device in bad guess loop\n");
	}
	if(read(fd, buf, sizeof(state_4)) != sizeof(state_4))
		printf("error reading from device\n");
	else {
		if(!strcmp((const char*)buf, state_4))
			passed_tests += 1;
		else
			printf("test %d failed\n", num_tests);
	}
	num_tests +=1;
	if(ioctl(fd, IOC_NEW_GAME) != 0) {
		printf("error requesting a new game through ioctl\n");
		goto end_premature;
	}
	else {
		if (read(fd, buf, sizeof(buf)) != sizeof(buf)) {
		
		}
		else
			if(strcmp((const char*)buf, initial_state) == 0)
				passed_tests += 1;
	}
	if(ioctl(fd, IOC_SURRENDER) != 0)
		printf("error surrendering through ioctl\n");
	else
		passed_tests += 1;
	// win the game
	num_tests += 1;
	write(fd, "k", 1);
	write(fd, "e", 1);
	write(fd, "r", 1);
	write(fd, "n", 1);
	write(fd, "l", 1);
	read(fd, buf, sizeof(buf));
	if(strcmp((const char*)buf, state_5) != 0)
		printf("error winning the game\n");
	else
		passed_tests += 1;
end_premature:
	num_tests += 1;
	if(close(fd) != 0)
		printf("error closing device");
	else
		passed_tests += 1;


	printf("\n\nend of testing\n%d/%d passed", passed_tests, num_tests);
	printf(", %d failed\n", num_tests - passed_tests);
	return 0;	
}
