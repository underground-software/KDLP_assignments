#include <err.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define DEV_FILE "/dev/hangman"
#define IOC_HANGMAN_RESET _IO(0x11, 0)
#define IOC_HANGMAN_GUESS _IO(0x11, 1)

char start[] =
"  +---+\n"
"  |   |\n"
"  O   |\n"
" /|\  |\n"
" / \  |\n"
"      |\n"
"========\n";
int main(void)
{
    int fd = open(DEV_FILE, O_RDWR);
    puts("start of testing");
    if (fd < 0)
        err(1, "unable to open dev file");
    char buff[2056];
    {
        ssize_t ret = read(fd, buff, sizeof buff);
        if (ret < 0)
            err(1, "unable to read from device");
        if ((size_t)ret != strlen(start))
            warnx("Expected to read %lu bytes, got %ld", strlen(start),ret);
        if (memcmp(start,buff,strlen(start))!=0)
            warnx("Expected to read string \n %s \n got \n%.*s", start, ret, buff);
    }
    {
        ssize_t ret2;
        int ret = ioctl(fd, IOC_HANGMAN_RESET);
        if (ret < 0)
            err(1, "unable to perform reset on device");
        ret2 = read(fd,buff,sizeof(buff));
        if ((size_t)ret2 != strlen(start))
            warnx("Expected to read %lu bytes, got %ld", strlen(start),ret);
        if (memcmp(start,buff,strlen(start))!=0)
            warnx("Expected to read string \n %s \n got \n%.*s", start, ret, buff);
    }

    close(fd);
    puts("end of testing");
    return 0;
}
