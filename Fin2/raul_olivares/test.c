#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>

struct message
{
    size_t size;
    char *buff;
};

#define DEV_FILE "/dev/redactor"
#define IOC_RDC_GET_LENGTH _IOR(0x11, 1, size_t)
#define IOC_RDC_REVEAL _IOR(0x11, 2, struct message)
#define IOC_RDC_TO_REDACT _IOW(0x11, 3, struct message)
#define IOC_RDC_REDACTING _IOW(0x11, 4, char)

int main()
{
    int ret;
    ssize_t ret_size;
    size_t size;
    size_t buff_size = 256;
    char buff[buff_size];
    struct message m;

    int fd = open(DEV_FILE, O_RDWR);
    puts("Testing start");

    {
        if (fd < 0)
            err(1, "Open: Unable to open file");

        ret_size = write(fd, "Redact me", 9);
        if (ret_size < 0)
            err(1, "Write: Unable to write to file");
        if (ret_size != 9)
            warnx("Write: Expected 9, got %zd", ret_size);

        ret = ioctl(fd, IOC_RDC_REDACTING, '*');
        if (ret < 0)
            err(1, "Redacting: Unable to ioctl to file");
    }

    {
        m.size = 2;
        m.buff = "me";
        ret = ioctl(fd, IOC_RDC_TO_REDACT, &m);
        if (ret < 0)
            err(1, "To Redact: Unable to ioctl file");

        ret_size = read(fd, buff, buff_size);
        if (ret_size < 0)
            err(1, "Read: Unable to read from device");
        if (ret_size != 9)
            warnx("Read: Expected to have redacted phrase with size 9, got %zd.", ret_size);
        if (memcmp("Redact **", buff, 9) != 0)
            warnx("Read: Expected to receive redacted phrase, got %.*s", (int)ret_size, buff);

        ret = ioctl(fd, IOC_RDC_GET_LENGTH, &size);
        if (ret < 0)
            err(1, "Get Length: Unable to ioctl from device");
        if (size == 0)
            warnx("Get Lenght: Expected 9, got 0 instead.");
    }

    {
        m.size = buff_size;
        m.buff = buff;
        ret = ioctl(fd, IOC_RDC_REVEAL, &m);
        if (ret < 0)
            err(1, "Reveal: Unable to ioctl from device");
        if (memcmp("Redact me", buff, 9) != 0)
            warnx("Reveal: Expected unredacted phrase Redact me, got %s", buff);

        off_t position = lseek(fd, 2, SEEK_SET);
        if (position < 0)
            err(1, "Seek: Unable to lseek file");
        if (position != 2)
            warnx("Seek: Set position expected 2, got %ld", position);
    }

    {
        char c;
        if (read(fd, &c, 1) != 0)
            warnx("Read single: Expected single byte returned");
        if (c != 'e')
            warnx("Read single: Expected e, got %c", c);
    }

    close(fd);
    puts("Testing end");
    return 0;
}