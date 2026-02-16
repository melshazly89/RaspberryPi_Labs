#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LED_PIN "533"
#define SW_PIN "532"
#define LED_PIN_DIRECTION "out"
#define SW_PIN_DIRECTION "in"
#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_LED_DIRECTION_PATH "/sys/class/gpio/gpio533/direction"
#define GPIO_LED_VALUE_PATH "/sys/class/gpio/gpio533/value"
#define GPIO_SW_DIRECTION_PATH "/sys/class/gpio/gpio532/direction"
#define GPIO_SW_VALUE_PATH "/sys/class/gpio/gpio532/value"

static int writeToFile(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    sleep(1); // Ensure the file is ready for writing
    printf("Writing '%s' to %s\n", value, path);
    ssize_t written = write(fd, value, strlen(value)); // Exclude null terminator
    if (written < 0) {
        perror("Failed to write to file");
        close(fd);
        return -1;
    }
    close(fd);
    sleep(1); // Ensure the system processes the change
    return 0;
}
int main()
{
    char path[80];
    int fd_led, fd_sw;
    char buf[2];
    

    /* Export GPIO Pins and set directions */
    writeToFile(GPIO_EXPORT, LED_PIN);
    writeToFile(GPIO_LED_DIRECTION_PATH, LED_PIN_DIRECTION);

    writeToFile(GPIO_EXPORT, SW_PIN);
    writeToFile(GPIO_SW_DIRECTION_PATH, SW_PIN_DIRECTION);
    

    /* Open value files */
    fd_led = open(GPIO_LED_VALUE_PATH, O_WRONLY);
    if (fd_led < 0) {
        perror("Failed to open LED GPIO value");
        return 1;
    }


    fd_sw = open(GPIO_SW_VALUE_PATH, O_RDONLY);
    if (fd_sw < 0) {
        perror("Failed to open switch GPIO value");
        close(fd_led);
        return 1;
    }

    /* Main loop: read switch and set LED accordingly */
    while (1) {
        if (lseek(fd_sw, 0, SEEK_SET) == -1) {
            perror("lseek");
            break;
        }
        ssize_t r = read(fd_sw, buf, 1);
        if (r == 1) {
            buf[1] = '\0';
            if (buf[0] == '0') { // Assuming active low switch
                if (write(fd_led, "1", 1) < 0) perror("write led");
            } else {
                if (write(fd_led, "0", 1) < 0) perror("write led");
            }
        } else if (r < 0) {
            perror("read");
            break;
        }
        usleep(100000); /* 100 ms debounce/poll interval */
    }

    close(fd_sw);
    close(fd_led);
    return 0;
}
