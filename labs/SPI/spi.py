import spidev
# Create an instance of the SPI device
spi = spidev.SpiDev()
# Open the SPI bus (bus 0, device 0)
spi.open(0, 0)
# Set the maximum speed of the SPI communication (in Hz)
spi.max_speed_hz = 500000  
# Set the SPI mode (0, 1, 2, or 3)     
spi.mode = 0b00
#
msg = list(b'Hello, I am Mahmoud Elshazly!')

# Send the message and receive the response
result = spi.xfer2(msg)
print("Received:", bytes(result).decode())
spi.close()