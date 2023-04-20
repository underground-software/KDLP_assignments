#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEV_FILE "/dev/calc"
#define IOC_CALC_ADDITION _IO(0x11, 0)
#define IOC_CALC_SUBTRACT _IO(0x11, 1)
#define IOC_CALC_MULTIPLICATION _IO(0x11, 2)
#define IOC_CALC_DIVISION _IO(0x11, 3)

int main(void) {
  char buff[1024];
  int fd, fd2, reti;
  ssize_t ret;

  // Open file descriptor
  fd = open(DEV_FILE, O_RDWR);
  puts("Test Reverse Driver");
  if (fd < 0) {
    err(1, "unable to open device");
  }

  // Read test 1
  // Make sure the first read returns 0
  {
    ret = read(fd, buff, sizeof buff);
    if (ret < 0) {
      err(1, "unable to read from device");
    }
    if (ret != 0) {
      warnx("TEST FAIL:  initial read test failed, expected 0  got %zd", ret);
    } else {
      puts("TEST PASS: initial read test passed");
    }
  }

  // ioctl tests
  // Make sure the the ioctl for each operation works
  {
    reti = ioctl(fd, IOC_CALC_ADDITION);
    if (reti < 0) {
      err(1, "Unable to preform ioctl on device");
    }
    if (reti != 0) {
      warnx("TEST FAIL: ioctl addition test failed");
    } else {
      puts("TEST PASS: ioctl addition test passed");
    }

    reti = ioctl(fd, IOC_CALC_SUBTRACT);
    if (reti < 0) {
      err(1, "Unable to preform ioctl on device");
    }
    if (reti != 0) {
      warnx("TEST FAIL: ioctl subtraction test failed");
    } else {
      puts("TEST PASS: ioctl subtraction test passed");
    }

    reti = ioctl(fd, IOC_CALC_MULTIPLICATION);
    if (reti < 0) {
      err(1, "Unable to preform ioctl on device");
    }
    if (reti != 0) {
      warnx("TEST FAIL: ioctl multiplication test failed");
    } else {
      puts("TEST PASS: ioctl multiplication test passed");
    }

    reti = ioctl(fd, IOC_CALC_DIVISION);
    if (reti < 0) {
      err(1, "Unable to preform  ioctl on device");
    }
    if (reti != 0) {
      warnx("TEST FAIL: ioctl division test failed");
    } else {
      puts("TEST PASS: ioctl division test passed");
    }
  }

  // write tests
  // write "10" and "25" into the buffer and make sure 0 is returned each time
  {
    ret = write(fd, "10", 13);
    if (ret < 0) {
      err(1, "Unable to preform write on device!");
    }
    if (ret != 0) {
      warnx("TEST FAIL: write test failed, expected 0  got %zd", ret);
    } else {
      puts("TEST PASS: write test 1 passed");
    }

    ret = write(fd, "25", 13);
    if (ret < 0) {
      err(1, "Unable to preform write on device!");
    }
    if (ret != 0) {
      warnx("TEST FAIL: write test 2 failed, expected 0  got %zd", ret);
    } else {
      puts("TEST PASS: write test 2 passed");
    }
  }

  // read tests
  // make sure each operation returns the appropriate value
  {
    // change the calculator to addition mode
    reti = ioctl(fd, IOC_CALC_ADDITION);
    if (ret < 0) {
      err(1, "Unable to preform ioctl on device");
    }
    if (ret != 0) {
      warnx("TEST FAIL: read test ioctl to addition unsuccessful");
    }

    ret = read(fd, buff, sizeof buff);
    if (ret < 0) {
      err(1, "unable to read from device");
    }
    if (ret != 2) {
      warnx("TEST FAIL: read return value addition, expected 2  got %zd", ret);
    } else {
      puts("TEST PASS: read return value addition");
    }

    if (memcmp("35", buff, 2) != 0) {
      warnx("TEST FAIL: read buffer value addition");
    } else {
      puts("TEST PASS: read buffer value addition\n");
    }

    // change the calculator to subtraction mode
    reti = ioctl(fd, IOC_CALC_SUBTRACT);
    if (ret < 0) {
      err(1, "Unable to preform ioctl on device");
    }
    if (ret != 0) {
      warnx("TEST FAIL: read test ioctl to subtraction unsuccessful");
    }

    ret = read(fd, buff, sizeof buff);
    if (ret < 0) {
      err(1, "unable to read from device");
    }
    if (ret != 3) {
      warnx("TEST FAIL: read return value subtraction, expected 3  got %zd",
            ret);
    } else {
      puts("TEST PASS: read return value subtraction");
    }

    if (memcmp("-15", buff, 3) != 0) {
      warnx("TEST FAIL: read buffer value subtraction");
    } else {
      puts("TEST PASS: read buffer value subtraction\n");
    }

    // change the calculator to multiplication mode
    reti = ioctl(fd, IOC_CALC_MULTIPLICATION);
    if (ret < 0) {
      err(1, "Unable to preform ioctl on device");
    }
    if (ret != 0) {
      warnx("TEST FAIL: read test ioctl to multiplication unsuccessful");
    }

    ret = read(fd, buff, sizeof buff);
    if (ret < 0) {
      err(1, "unable to read from device");
    }
    if (ret != 3) {
      warnx("TEST FAIL: read return value multiplication, expected 3  got %zd",
            ret);
    } else {
      puts("TEST PASS: read return value multiplication");
    }

    if (memcmp("250", buff, 2) != 0) {
      warnx("TEST FAIL: read buffer value multiplication");
    } else {
      puts("TEST PASS: read buffer value multiplication\n");
    }

    // change the calculator to division mode
    reti = ioctl(fd, IOC_CALC_DIVISION);
    if (ret < 0) {
      err(1, "Unable to preform ioctl on device");
    }
    if (ret != 0) {
      warnx("TEST FAIL: read test ioctl to division unsuccessful");
    }

    ret = read(fd, buff, sizeof buff);
    if (ret < 0) {
      err(1, "unable to read from device");
    }
    if (ret != 3) {
      warnx("TEST FAIL: read return value division, expected 3  got %zd", ret);
    } else {
      puts("TEST PASS: read return value division");
    }

    if (memcmp("0.4", buff, 2) != 0) {
      warnx("TEST FAIL: read buffer value division");
    } else {
      puts("TEST PASS: read buffer value division\n");
    }
  }

  // fd 2 tests
  // these tests show that a second file descriptor can be opened and used while
  // the first is still open
  {
    // Open the second file descriptor
    fd2 = open(DEV_FILE, O_RDONLY);
    if (fd2 < 0) {
      err(1, "unable to open device fd2");
    }

    ret = read(fd2, buff, sizeof buff);
    if (ret < 0) {
      err(1, "unable to read from device fd2");
    }
    if (ret != 3) {
      warnx("TEST FAIL: fd2 read return value division, expected 3  got %zd",
            ret);
    } else {
      puts("TEST PASS: fd2 read return value division");
    }

    if (memcmp("0.4", buff, 2) != 0) {
      warnx("TEST FAIL: fd2 read buffer value division");
    } else {
      puts("TEST PASS: fd2 read buffer value division\n");
    }

    close(fd2);
  }

  close(fd);
}
