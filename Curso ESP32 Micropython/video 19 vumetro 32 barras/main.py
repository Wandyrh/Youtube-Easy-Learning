from machine import Timer, Pin
from machine import ADC

import vumeterMatrix

adc = ADC(Pin(32))
adc.atten(ADC.ATTN_11DB)

adcMin = 4095
adcMax = 0

period = 50
maxDevice = 4

vumeter = vumeterMatrix.vumeterMatrix(maxDevice=maxDevice)


def timerEvent():
    global adcMin
    global adcMax
    barLevel = 0
    adcVal = adc.read()
    print("adc:", adcVal)
    if adcVal < adcMin:
        adcMin = adcVal
    if adcVal > adcMax:
        adcMax = adcVal
    print("adcMin:", adcMin)
    print("adcMax:", adcMax)

    
    barLevel = int(((adcVal - adcMin) / (adcMax - adcMin)) * 32) 
    gain = int(((adcVal - adcMin) / (adcMax - adcMin)) * 100) 
    print("gain:", gain)
    
    range = adcMax - adcMin
    if  range > 50 :
        adcMax = adcMax - 1
        adcMin = adcMin + 1
    print("range:", range)
    if barLevel > 32:
        barLevel = 32
    if barLevel < 0:
        barLevel = 0
    print("barLevel:", barLevel)
    vumeter.updateLevel(barLevel)


tim1 = Timer(1)
tim1.init(period=period, mode=Timer.PERIODIC, callback=lambda t:
          timerEvent()
          )

while True:
    pass
