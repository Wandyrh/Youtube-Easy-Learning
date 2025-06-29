from machine import Pin
import network
from machine import Timer


p12 = Pin(12, Pin.OUT) 
p14 = Pin(14, Pin.OUT)    
p27 = Pin(27, Pin.OUT)    
  
p12.on() 
p14.on() 
p27.off()



ap = network.WLAN(network.AP_IF) # create access-point interface
ap.config(essid='ESP32', channel=11, authmode = 3, password = '12345678')
ap.config(max_clients=10) # set how many clients can connect to the network
ap.active(True)         # activate the interface

def printClients():
    clients = ap.status('stations')
    print('----------------------------------------------------------')
    print('Clientes conectados: ' + str(len(clients)))
    for item in clients:
        print(str(item))
    print('----------------------------------------------------------')

    if len(clients) > 0:
        p12.on() 
        p14.off() 
        p27.on()
    else:
        p12.on() 
        p14.on() 
        p27.off()

tim1 = Timer(1)
tim1.init(period=2000, mode=Timer.PERIODIC, callback=lambda t:
    printClients()
)
