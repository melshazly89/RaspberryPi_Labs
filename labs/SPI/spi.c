	#include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
	#include <bits/stdint-uintn.h>
    #include <stdint.h>
	#include <sys/ioctl.h>
	#include <linux/types.h>
	#include <linux/spi/spidev.h>
    #include <unistd.h>

    #define SPI_DEVICE "/dev/spidev0.0"
    #define SPI_MODE SPI_MODE_0
    #define SPI_BITS_PER_WORD 8
    #define SPI_SPEED_HZ 500000

    void pabort(const char *s) {
        perror(s);
        abort();
    }

    int main() {
        int fd = open(SPI_DEVICE, O_RDWR);
        if (fd < 0) {
            perror("Failed to open SPI device");
            return 1;
        }

        uint8_t mode = SPI_MODE;
        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
            pabort("Failed to set SPI mode");
            return 1;
        }

        // Additional SPI configuration (e.g., speed, bits per word) can be set here
        uint8_t bits_per_word = SPI_BITS_PER_WORD;
        if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
            pabort("Failed to set bits per word");
        }

        uint32_t speed_hz = SPI_SPEED_HZ;
        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz) < 0) {
            pabort("Failed to set speed");
        }

        // Example: Write data to the SPI device
        uint8_t tx_buffer[3] = {0x01, 0x02, 0x03}; // Data to send
        struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)tx_buffer,
            .rx_buf = 0,
            .len = sizeof(tx_buffer),
            .speed_hz = 500000,
            .bits_per_word = 8,
        };
        /*Perform the SPI transaction*/
        while (1) { 
        if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
            pabort("Failed to send SPI message");
            return 1;
        }
    }

        close(fd);
        return 0;
    }