#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/random.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "shared.h"

#define PASSWORD_FILE "/etc/passwd"
#define LOG_FILE "log.txt"

int main(void)
{
	int pw_fd = openat(AT_FDCWD, PASSWORD_FILE, O_RDONLY);
	if(0 > pw_fd)
		ERROR("unable to open " PASSWORD_FILE);

	struct stat pw_stat;
	if(0 > fstat(pw_fd, &pw_stat))
		ERROR("unable to stat " PASSWORD_FILE);
	size_t pw_size = (size_t)pw_stat.st_size;

	char *pw_data = mmap(NULL, pw_size, PROT_READ, MAP_PRIVATE, pw_fd, 0);
	if(MAP_FAILED == pw_data)
		ERROR("unable to map " PASSWORD_FILE);
	if(0 > close(pw_fd))
		ERROR("unable to close " PASSWORD_FILE);
	uid_t user = getuid();
	static char user_info_buffer[256] = {0};
	size_t user_info_len = get_user_data(sizeof user_info_buffer, user_info_buffer, pw_size, pw_data, user);
	if(0 > munmap(pw_data, pw_size))
		ERROR("unable to unmap " PASSWORD_FILE);

	uint8_t rand_byte;
	if(sizeof rand_byte != getrandom(&rand_byte, sizeof rand_byte, 0))
		ERROR("unable to get random byte");

	struct timeval now;
	if(0 > gettimeofday(&now, NULL))
		ERROR("unable to get time of day");
	pid_t pid = getpid();

	static char prog_info_buffer[256] = {0};
	size_t prog_info_len = get_prog_data(sizeof prog_info_buffer, prog_info_buffer, rand_byte, now.tv_sec, pid);

	int log_fd = openat(AT_FDCWD, LOG_FILE, O_RDWR | O_CREAT, 0600);
	if(0 > log_fd)
		ERROR("unable to open " LOG_FILE);
	if(-1 == lseek(log_fd, 0, SEEK_END))
		ERROR("unable to seek " LOG_FILE);
	if((ssize_t)prog_info_len != write(log_fd, prog_info_buffer, prog_info_len))
		ERROR("unable to write prog info into " LOG_FILE);
	if((ssize_t)user_info_len != write(log_fd, user_info_buffer, user_info_len))
		ERROR("unable to write user info into " LOG_FILE);
	if(0 > close(log_fd))
		ERROR("unable to close " LOG_FILE);
	return 0;
}
