from machine import Pin, I2C
import time
import BME280


i2c = I2C(scl=Pin(22), sda=Pin(21), freq=10000)
bme = BME280.BME280(i2c=i2c)

while True:
  time.sleep_ms(500)  
  temperature = bme.temperature
  humidity = bme.humidity
  pressure = bme.pressure
  print('--------------------------------------------')
  print('Temperature: ', temperature, 'C')
  print('Humidity: ', humidity, '%' )
  print('Pressure: ', pressure, 'hPa')
  print('--------------------------------------------')