#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int ledControl(int gpio, int onOff) {
    int fd;
    char buf[128];

    // GPIO export (이미 설정되어 있으면 생략 가능)
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd >= 0) {  // 이미 설정된 경우 에러 발생 가능하므로 체크
        sprintf(buf, "%d", gpio);
        write(fd, buf, strlen(buf));
        close(fd);
    }

    // Set GPIO direction (out)
    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open direction file");
        return -1;
    }
    write(fd, "out", 3);
    close(fd);

    // Set GPIO value (1 or 0)
    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open value file");
        return -1;
    }
    if (onOff == 1)
        write(fd, "1", 1);
    else
        write(fd, "0", 1);
    close(fd);

    return 0;
}

// Usage: ./ledgpio 21 1/0
int main(int argc, char **argv) {
    int gno, onOff;

    if (argc != 3) {
        printf("Usage: %s GPIO_NO 1/0\n", argv[0]);
        return -1;
    }

    gno = atoi(argv[1]);
    onOff = atoi(argv[2]);

    ledControl(gno, onOff);

    return 0;
}
