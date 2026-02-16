#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


#define PIN_NUMBER "21" // GPIO pin number for the LED
#define PIN_DIRECTION "out" // Set the pin as output
#define GPIO_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"
#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio533/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio533/value"
#define PIN_HIGH "1"
#define PIN_LOW "0"

int main()
{
    int fd;
    char pin_path[50];

    // Export the GPIO pin
    /*O_WRONLY ---> is open the file for write only*/
    fd = open(GPIO_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open GPIO export");
        return 1;
    }
    write(fd, PIN_NUMBER, strlen(PIN_NUMBER));
    close(fd);
    sleep(1); // Wait for the system to create the GPIO folder

    // Set the GPIO pin direction
    fd = open(GPIO_DIRECTION_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open GPIO export");
        return 1;
    }
    write(fd, PIN_DIRECTION, strlen(PIN_DIRECTION));
    close(fd);

        // Set the GPIO pin direction
    fd = open(GPIO_VALUE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open GPIO export");
        return 1;
    }
    for (int i = 0; i < 5; i++) {
        // Turn LED on
        write(fd, PIN_HIGH, strlen(PIN_HIGH));
        sleep(1); // LED on for 1 second

        // Turn LED off
        write(fd, PIN_LOW, strlen(PIN_LOW));
        sleep(1); // LED off for 1 second
    }
    
    close(fd);

    // unexport the GPIO pin
    /*O_WRONLY ---> is open the file for write only*/
    fd = open(GPIO_UNEXPORT_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open GPIO unexport");
        return 1;
    }
    write(fd, PIN_NUMBER, strlen(PIN_NUMBER));
    close(fd);
    sleep(1); // Wait for the system to create the GPIO folder
    return 0;
}
