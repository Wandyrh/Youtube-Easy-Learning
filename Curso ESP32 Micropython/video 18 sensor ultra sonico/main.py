from machine import Timer

import vumeterMatrix
import HCSR04

period = 100
maxDevice = 4
sensor = HCSR04.HCSR04(trigger_pin=5, echo_pin=18, echo_timeout_us=500*2*30)
vumeter = vumeterMatrix.vumeterMatrix(maxDevice= maxDevice) 

def timerEvent():     
    barLevel = 0
    distance = int(sensor.distance_cm())
    print("Distance: ", distance, " cm")   
    if distance > 0:
        barLevel = int(distance / 2)           
    if barLevel > 32:
        barLevel = 32
    vumeter.updateLevel(barLevel)
 

tim1 = Timer(1)
tim1.init(period=period, mode=Timer.PERIODIC, callback=lambda t:
          timerEvent()
          )

while True:
    pass
