import dht
import machine
import time

d = dht.DHT22(machine.Pin(13))
d.measure()
temperature = d.temperature() # eg. 23.6 (°C)
humidity = d.humidity()    # eg. 41.3 (% RH)

print(str(temperature))
print(str(humidity))

while True:
    time.sleep_ms(500)      # sleep for 500 milliseconds
    d.measure()
    temperature = d.temperature() # eg. 23.6 (°C)
    humidity = d.humidity()    # eg. 41.3 (% RH)
    print('------------------------------------------')
    print('temperature: ', str(temperature), '(C)')
    print('humidity: ', str(humidity), '((% RH)')
    print('------------------------------------------')