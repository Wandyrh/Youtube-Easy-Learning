from machine import Pin
from machine import ADC
from machine import Timer

adc = ADC(Pin(32))
adc.atten(ADC.ATTN_11DB) 

p13 = Pin(13, Pin.OUT)   
p12 = Pin(12, Pin.OUT)   
p14 = Pin(14, Pin.OUT)   

p13.on()   
p12.on()  
p14.on()  

period = 500



tim1 = Timer(1)
tim1.init(period=period, mode=Timer.PERIODIC, callback=lambda t:
    setLed()
)

def setLed():   
    adcVal = adc.read()
    print(adcVal)

    if  adcVal < 1000:
        p13.off()   
        p12.on()  
        p14.on() 
    if adcVal > 1000 and adcVal < 2000:
        p13.on()   
        p12.off()  
        p14.on() 
    if adcVal > 2000:
        p13.on()   
        p12.on()  
        p14.off() 