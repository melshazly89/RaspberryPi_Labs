import RPi.GPIO as gpio
import time

# Set up GPIO pin 21 as an output
gpio.setmode(gpio.BCM)
#  Use BCM pin numbering
gpio.setup(21, gpio.OUT)

while True:
    # Turn on the LED connected to pin 21, wait for 1 second, then turn it off and wait for another second
    gpio.output(21, gpio.HIGH)
    time.sleep(1)
    gpio.output(21, gpio.LOW)
    time.sleep(1)   