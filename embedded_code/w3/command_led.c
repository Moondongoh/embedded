#include <stdio.h>
#include <stdlib.h>  // atoi()
#include <string.h>  // strlen()
#include <fcntl.h>   // open()
#include <unistd.h>  // close(), usleep()

#define BUFFER_SIZE 64

void ledControl(int gpio) {
    int fd;
    char buf[BUFFER_SIZE];

    // GPIO Export
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        perror("Error opening /sys/class/gpio/export");
        return;
    }
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);

    // Set Direction (out)
    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("Error setting direction");
        return;
    }
    write(fd, "out", 3);
    close(fd);

    // Set Value (1)
    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("Error setting value");
        return;
    }
    write(fd, "1", 1);
    fsync(fd);
    close(fd);

    // 1초 대기
    usleep(1000000);

    // GPIO Unexport (해제)
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        perror("Error opening /sys/class/gpio/unexport");
        return;
    }
    sprintf(buf, "%d", gpio);
    write(fd, buf, strlen(buf));
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s GPIO_NUMBER\n", argv[0]);
        return -1;
    }

    int gpio = atoi(argv[1]);  // 입력된 GPIO 번호 가져오기
    ledControl(gpio);

    return 0;
}
