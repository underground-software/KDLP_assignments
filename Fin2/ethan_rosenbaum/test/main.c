#include "../matrix.h"
#include "test.h"
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

static int file_mode(void) {
    struct stat info;
    if (expect_errno(stat("/dev/matrix", &info) == 0))
        return 1;
    if (expect_errno(info.st_mode == 020666)) // crw-rw-rw-
        return 1;
    return 0;
}

static int read_write_1x1(void) {
    int ret = 1;

    int fd = open("/dev/matrix", O_RDWR);
    if (fd < 0) {
        msg_errno("open");
        return 1;
    }

    uint8_t expected[37] = {0};
    uint8_t buf[sizeof expected];

    if (expect_errno(read(fd, &buf, sizeof buf) == sizeof buf))
        goto out_close;
    if (expect(!memcmp(expected, buf, sizeof buf)))
        goto out_close;

    uint8_t one[] = {0, 0, 0, 0, 0, 1};
    if (expect_errno(write(fd, &one, sizeof one) == sizeof one))
        goto out_close;
    if (expect_errno(read(fd, &buf, sizeof buf) == sizeof buf))
        goto out_close;
    for (uint32_t i = 0; i < sizeof buf; i++)
        if (expect(buf[i] == 1))
            goto out_close;

    ret = 0;
out_close:
    close(fd);
    return ret;
}

static int read_write_nxn(void) {
    int ret = 1;

    int fd = open("/dev/matrix", O_RDWR);
    if (fd < 0) {
        msg_errno("open");
        return 1;
    }

    if (expect_errno(!ioctl(fd, MAT_IO_RESIZE, 2)))
        goto out_close;

    uint8_t write_buf[] = {1, 2, 3, 4};
    if (expect_errno(write(fd, write_buf, sizeof write_buf) == sizeof write_buf))
        goto out_close;

    uint8_t read_buf[sizeof write_buf];
    if (expect_errno(read(fd, read_buf, sizeof read_buf) == sizeof read_buf))
        goto out_close;

    if (expect(!memcmp(read_buf, write_buf, sizeof read_buf)))
        goto out_close;

    ret = 0;
out_close:
    close(fd);
    return ret;
}

static int seek_and_rotate(void) {
    int ret = 1;

    int fd = open("/dev/matrix", O_RDWR);
    if (fd < 0) {
        msg_errno("open");
        return 1;
    }

    if (expect_errno(!ioctl(fd, MAT_IO_RESIZE, 3)))
        goto out_close;

    if (expect_errno(lseek(fd, SEEK_VERTICAL, -1) == 6))
        goto out_close;

    uint8_t write_buf[] = {7, 8, 9, 1, 2, 3, 4, 5, 6};
    if (expect_errno(write(fd, write_buf, sizeof write_buf) == sizeof write_buf))
        goto out_close;

    // 1  2  3
    // 4  5  6
    // 7* 8  9

    if (expect_errno(lseek(fd, SEEK_HORIZONTAL, 1) == 7))
        goto out_close;

    if (expect_errno(lseek(fd, SEEK_HORIZONTAL, 3) == 7))
        goto out_close;

    if (expect_errno(ioctl(fd, MAT_IO_ROTATE, 1) == 3))
        goto out_close;

    if (expect_errno(ioctl(fd, MAT_IO_ROTATE, -4) == 3))
        goto out_close;

    if (expect_errno(ioctl(fd, SEEK_VERTICAL, -1) == 0))
        goto out_close;

    uint8_t expected[] = {
        7, 4, 1,
        8, 5, 2,
        9, 6, 3,
    };
    uint8_t read_buf[sizeof expected];
    if (expect_errno(read(fd, read_buf, sizeof read_buf) == sizeof read_buf))
        goto out_close;
    if (expect(!memcmp(read_buf, expected, sizeof expected)))
        goto out_close;

    ret = 0;
out_close:
    close(fd);
    return ret;
}

static int resize(void) {
    int ret = 1;

    int fd = open("/dev/matrix", O_RDWR);
    if (fd < 0) {
        msg_errno("open");
        return 1;
    }

    uint8_t one = 1;
    if (expect_errno(write(fd, &one, sizeof one) == sizeof one))
        goto out_close;

    if (expect_errno(!ioctl(fd, MAT_IO_RESIZE, 2)))
        goto out_close;

    uint8_t expected[4] = {0};
    uint8_t read_buf[4];
    if (expect_errno(read(fd, &read_buf, sizeof read_buf) == sizeof read_buf))
        goto out_close;
    if (expect(!memcmp(read_buf, expected, sizeof read_buf)))
        goto out_close;

    if (expect(lseek(fd, 1, SEEK_HORIZONTAL) == 1))
        goto out_close;

    if (expect_errno(!ioctl(fd, MAT_IO_RESIZE, 2)))
        goto out_close;

    if (expect(lseek(fd, 0, SEEK_VERTICAL) == 0))
        goto out_close;

    ret = 0;
out_close:
    close(fd);
    return ret;
}

static int (*tests[])(void) = {
    file_mode,
    read_write_1x1,
    read_write_nxn,
    seek_and_rotate,
    resize,
};

int main(void) {
    uint32_t pass = 0;
    uint32_t total = sizeof (tests) / sizeof (tests[0]);

    for (uint32_t i = 0; i < total; i++)
        pass += !tests[i]();

    printf("passed %u/%u tests.\n", pass, total);
}
