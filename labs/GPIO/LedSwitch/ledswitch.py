import RPi.GPIO as gpio
import time

BUTTON_PIN = 20
LED_PIN=21


def InitDevices():
    gpio.setwarnings(False)
    gpio.setmode(gpio.BCM)
    gpio.setup(BUTTON_PIN, gpio.IN, pull_up_down=gpio.PUD_UP)
    gpio.setup(LED_PIN, gpio.OUT)

InitDevices()
while True:
    value = gpio.input(BUTTON_PIN)
    gpio.output(LED_PIN, not value)  # LED on when button is pressed (active low)