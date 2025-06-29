from machine import Pin

count = 0

p13 = Pin(13, Pin.OUT)   
p12 = Pin(12, Pin.OUT)   
p14 = Pin(14, Pin.OUT)   
p27 = Pin(27, Pin.IN) 

p13.on()   
p12.on()  
p14.on()  

def setLed(pin):
    global count           
    print(count)
    count += 1
    if count > 2:
        count = 0
        
    if  count == 0:
        p13.off()   
        p12.on()  
        p14.on() 
    if count == 1:
        p13.on()   
        p12.off()  
        p14.on() 
    if count == 2:
        p13.on()   
        p12.on()  
        p14.off()   

 
p27.irq(trigger=machine.Pin.IRQ_FALLING  , handler=setLed)