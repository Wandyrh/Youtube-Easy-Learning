
try:
  import usocket as socket
except:
  import socket

from machine import Pin
import network
import time

import esp
esp.osdebug(None)

import gc
gc.collect()

ssid = 'ssid'
password = 'password'

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  time.sleep_ms(500)

print('Connection successful')
print(station.ifconfig())

led = Pin(2, Pin.OUT)
led.on()