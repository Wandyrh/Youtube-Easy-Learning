from machine import Pin
import network
import time



p27 = Pin(27, Pin.OUT)
p12 = Pin(12, Pin.OUT)
p14 = Pin(14, Pin.OUT)


p27.off() 
p12.on() 
p14.on() 


wlan = network.WLAN(network.STA_IF) # create station interface
wlan.active(True)       # activate the interface
wifiList = wlan.scan()             # scan for access points

print('Redes disponibles-----------------------------------------------------------------')
for item in wifiList:
    print('Red:' + str(item[0]) + ' Canal :' + str(item[2]) + ' Señal: ' + str(item[3]) )
print('----------------------------------------------------------------------------------')


def wifiConncect():
    while not wlan.isconnected():
        print('Conectando...')
        print(wlan.status())
        if wlan.status() == 1001: 
            pass           
        else:            
            wlan.connect('easylearning', '12345678') # connect to an AP
            
        time.sleep(1)
        
    p14.off()
    p27.on() 
    p12.on() 
    print('Conexión establecida!')

        
if not wlan.isconnected():      # check if the station is connected to an AP
    wifiConncect()
else:
    p14.off()
    p27.on() 
    p12.on() 
    
wlan.ifconfig()         # get the interface's IP/netmask/gw/DNS addresses