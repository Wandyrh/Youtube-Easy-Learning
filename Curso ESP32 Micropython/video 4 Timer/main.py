from machine import Pin
from machine import Timer

p13 = Pin(13, Pin.OUT)    
p12 = Pin(12, Pin.OUT)   
p14 = Pin(14, Pin.OUT)   


#set pins to 1
p13.on()    
p12.on()   
p14.on()  

#variables
led = 1
period = 1000

#set timer
tim1 = Timer(1)
tim1.init(period=period, mode=Timer.PERIODIC, callback=lambda t:
    setLed()
)

def setLed():
    global led
    
    if led == 1:
        p13.off()    
        p12.on()   
        p14.on() 

    if led == 2:
        p13.on()    
        p12.off()   
        p14.on() 

    if led == 3:
        p13.on()    
        p12.on()   
        p14.off() 

    led += 1
    if led > 3:
        led = 1
    