#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE "/dev/connect4"
#define IOC_CONNECT_RESET _IO(0x11, 0)
#define IOC_CONNECT_AI_MODE _IOW(0x11, 1, int)
#define IOC_CONNECT_GET_LEN _IOR(0x11, 2, size_t)

char full_col[] =
	"2......\n"
	"1......\n"
	"2......\n"
	"1......\n"
	"2......\n"
	"1......\n"
	"Player 1's Turn";
char win2[] =
	".......\n"
	".......\n"
	"...1...\n"
	"..11...\n"
	".112...\n"
	"1222..2\n"
	"Player 1 Won!";
char win1[] =
	".......\n"
	".......\n"
	".......\n"
	".......\n"
	"222....\n"
	"1111...\n"
	"Player 1 Won!";
char win0[] =
	".......\n"
	".......\n"
	"2......\n"
	"2.....1\n"
	"2.2...1\n"
	"2.1.1.1\n"
	"Player 2 Won!";
char turn3[] =
	".......\n"
	".......\n"
	".......\n"
	".......\n"
	"..2....\n"
	"..1.1..\n"
	"Player 2's Turn";

char turn2[] =
	".......\n"
	".......\n"
	".......\n"
	".......\n"
	"..2....\n"
	"..1....\n"
	"Player 1's Turn";
char turn1[] =
	".......\n"
	".......\n"
	".......\n"
	".......\n"
	".......\n"
	"..1....\n"
	"Player 2's Turn";
char blank[] =
	".......\n"
	".......\n"
	".......\n"
	".......\n"
	".......\n"
	".......\n"
	"Player 1's Turn";
char tie[] =
	"2221112\n"
	"1112221\n"
	"2221112\n"
	"1112221\n"
	"2221112\n"
	"1112221\n"
	"Draw!";

int main(void)
{
	printf("beginning tests\n");
	int fd = open(DEVICE, O_RDWR);
	if (fd < 0)
		err(1, "could not open file!");
	char buff[1024];

	// Read file contents, make sure it is the same as `blank`
	{
		ssize_t ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read");
		if ((size_t) ret != strlen(blank))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(blank), ret);
		if (memcmp(blank, buff, strlen(blank)) != 0)
			warnx("Expected to read string \n%s\n got \n%.*s", tie,
				(int)ret, buff);
	}

	// Write the number '3', make sure the 1 byte write works.
	{
		ssize_t ret = write(fd, "3", 1);
		if (ret < 0)
			err(1, "could not write to file");
		if (ret != 1)
			warnx("expected to write 1 byte, got %ld", ret);
	}

	// make sure a 1 is now in the lowest position in col 3 and that it
	// is now player 2's turn. Player 2 then puts a token on top of
	// player 1's token in column 3
	{
		ssize_t ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read");
		if ((size_t) ret != strlen(turn1))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(turn1), ret);
		if (memcmp(turn1, buff, strlen(turn1) != 0))
			warnx("expected to read string \n%s\n, got \n%.*s",
				turn1, (int)ret, buff);

		ret = write(fd, "3", 1);
		if (ret < 0)
			err(1, "could not write to file");
		if (ret != 1)
			warnx("expected to write 1 byte, got %ld", ret);

		ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read");
		if ((size_t) ret != strlen(turn2))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(turn2), ret);
		if (memcmp(turn2, buff, strlen(turn2) != 0))
			warnx("expected to read string \n%s\n, got \n%.*s",
				turn2, (int)ret, buff);

	}

	// try writing a string that starts with something valid
	{
		ssize_t ret = write(fd, "5jdsfkajfkls\n", 13);
		if (ret < 0)
			err(1, "could not write to file");
		if (ret != 13)
			warnx("expected to write 13 bytes, got %ld", ret);

		ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read file!");
		if ((size_t)ret != strlen(turn3))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(turn3), ret);
		if (memcmp(turn3, buff, strlen(turn2) != 0))
			warnx("expected to read string \n%s\n, got \n%.*s",
				turn3, (int)ret, buff);
	}

	// make player 2 win
	{
		ssize_t ret = write(fd, "this should fail", 1);
		if (ret > 0)
			warnx("expected write to fail with negative error"
				" number, got %ld instead", ret);

		ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read file!");
		if ((size_t)ret != strlen(turn3))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(turn3), ret);
		if (memcmp(turn3, buff, strlen(turn2) != 0))
			warnx("expected to read string \n%s\n, got \n%.*s",
				turn3, (int)ret, buff);

		ret = write(fd, "1", 1); // It should be player 2's turn here.
		ret = write(fd, "7", 1);
		ret = write(fd, "1", 1); // just going to blindly hope that
		ret = write(fd, "7", 1); // all of these writes work.
		ret = write(fd, "1", 1);
		ret = write(fd, "7", 1);
		ret = write(fd, "1", 1); // Player 2 should win here
		ret = write(fd, "7", 1); // so this write should fail
		if (ret > 0)
			warnx("expected to recieve a negative error from write "
				"on finished game, got %ld", ret);
		ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read file!");
		if ((size_t)ret != strlen(win0))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(win0), ret);
		if (memcmp(win0, buff, strlen(win0) != 0))
			warnx("expected to read string \n%s\n, got \n%.*s",
				win0, (int)ret, buff);

		ret = write(fd, "1", 1);
		if (ret > 0)
			warnx("expected write on finished game to fail");
	}

	// test out reset
	{
		int ret = ioctl(fd, IOC_CONNECT_RESET);
		ssize_t ret2;
		if (ret < 0)
			err(1, "could not reset device");
		ret2 = read(fd, buff, sizeof(buff));
		if ((size_t)ret2 != strlen(blank))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(blank), ret2);
		if (memcmp(blank, buff, strlen(blank)) != 0)
			warnx("Expected to read string \n%s\n got \n%.*s", tie,
				(int)ret2, buff);
	}

	// check the getlen ioctl
	{
		size_t size = 123;
		int ret = ioctl(fd, IOC_CONNECT_GET_LEN, &size);
		if (ret < 0)
			err(1, "could get length of device");
		if (size != strlen(blank))
			warnx("expected device len to be same as blank example:"
				" expected %lu, got %lu", strlen(blank), size);
	}

	// produce a tie
	{
		ssize_t ret;
		// this (unchecked) sequence of writes _SHOULD_ tie the game
		write(fd, "1", 1);
		write(fd, "1", 1);
		write(fd, "2", 1);
		write(fd, "2", 1);
		write(fd, "3", 1);
		write(fd, "3", 1);

		write(fd, "1", 1);

		write(fd, "4", 1);
		write(fd, "4", 1);
		write(fd, "5", 1);
		write(fd, "5", 1);
		write(fd, "6", 1);
		write(fd, "6", 1);

		write(fd, "1", 1);

		write(fd, "7", 1);
		write(fd, "7", 1);

		write(fd, "2", 1);
		write(fd, "2", 1);
		write(fd, "3", 1);
		write(fd, "3", 1);

		write(fd, "1", 1);

		write(fd, "4", 1);
		write(fd, "4", 1);
		write(fd, "5", 1);
		write(fd, "5", 1);
		write(fd, "6", 1);
		write(fd, "6", 1);

		write(fd, "1", 1);

		write(fd, "7", 1);
		write(fd, "7", 1);

		write(fd, "2", 1);
		write(fd, "2", 1);
		write(fd, "3", 1);
		write(fd, "3", 1);

		write(fd, "7", 1);

		write(fd, "4", 1);
		write(fd, "4", 1);
		write(fd, "5", 1);
		write(fd, "5", 1);
		write(fd, "6", 1);
		write(fd, "6", 1);
		write(fd, "7", 1);
		ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read file!");
		if ((size_t)ret != strlen(tie))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(tie), ret);
		if (memcmp(tie, buff, strlen(tie) != 0))
			warnx("expected to read string \n%s\n, got \n%.*s",
				tie, (int)ret, buff);
	}

	// Checking seek
	{
		// device at this point should be the tied game.
		off_t ret = lseek(fd, 5, SEEK_CUR);
		if (ret < 0)
			err(1, "couldn't seek");
		if (ret != 5)
			warnx("couldn't seek_cur to pos 5, got %ld", ret);
		ret = lseek(fd, 0, SEEK_SET);
		if (ret < 0)
			err(1, "couldnt seek");
		if (ret != 0)
			warnx("Couldnt seek_set to pos 0, got %ld", ret);
	}

	// reset
	{
		int ret = ioctl(fd, IOC_CONNECT_RESET);
		ssize_t ret2;
		if (ret < 0)
			err(1, "could not reset device");
		ret2 = read(fd, buff, sizeof(buff));
		if ((size_t)ret2 != strlen(blank))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(blank), ret2);
		if (memcmp(blank, buff, strlen(blank)) != 0)
			warnx("Expected to read string \n%s\n got \n%.*s", tie,
				(int)ret2, buff);
	}

	// write to a full column, then reset
	{

		ssize_t ret;
		ret = write(fd, "1", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "1", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "1", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "1", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "1", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "1", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "1", 1);
		if (ret > 0)
			warnx("Expected to fail write: column should be full");

		ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read");
		if ((size_t) ret != strlen(full_col))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(full_col), ret);
		if (memcmp(turn2, buff, strlen(full_col) != 0))
			warnx("expected to read string \n%s\n, got \n%.*s",
				full_col, (int)ret, buff);

		ret = ioctl(fd, IOC_CONNECT_RESET);
		ssize_t ret2;
		if (ret < 0)
			err(1, "could not reset device");
		ret2 = read(fd, buff, sizeof(buff));
		if ((size_t)ret2 != strlen(blank))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(blank), ret2);
		if (memcmp(blank, buff, strlen(blank)) != 0)
			warnx("Expected to read string \n%s\n got \n%.*s", tie,
				(int)ret2, buff);
	}

	{
		ssize_t ret;
		ret = write(fd, "1", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "2", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "2", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "3", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "3", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "4", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "3", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "4", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "4", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "7", 1);
		if (ret != 1)
			warnx("could not write single byte");
		ret = write(fd, "4", 1);
		if (ret != 1)
			warnx("could not write single byte");

		ret = read(fd, buff, sizeof(buff));
		if (ret < 0)
			err(1, "could not read");
		if ((size_t) ret != strlen(win2))
			warnx("Expected to read %lu bytes, got %ld",
				strlen(win2), ret);
		if (memcmp(win2, buff, strlen(win2) != 0))
			warnx("expected to read string \n%s\n, got \n%.*s",
				win2, (int)ret, buff);
	}

	// reset after we're done testing
	{
		int ret = ioctl(fd, IOC_CONNECT_RESET);
		if (ret < 0)
			err(1, "could not reset device");
	}
	printf("tests DONE!\n");

	return 0;
}
