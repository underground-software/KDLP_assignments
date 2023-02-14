#ifndef SHARED_H
#define SHARED_H

#include <stdlib.h>

#define ERROR(msg) \
do { \
	write(2, msg "\n", sizeof msg "\n"); \
	exit(1); \
} while(0)

//gets the user data from the password file and formats it into a message stored in msg_buf, details not important
extern size_t get_user_data(size_t msg_len, char *msg_buf, size_t pwsize, const char *pwdata, uid_t user);
//formats a message containing the random byte, timestamp and pid, and puts it into msg_buf, details not important
extern size_t get_prog_data(size_t msg_len, char *msg_buf, uint8_t randbyte, time_t timestamp, pid_t pid);

#endif//SHARED_H
