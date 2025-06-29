from machine import Pin, SPI
import matrix
from machine import Timer

p27 = Pin(27, Pin.OUT)
p2 = Pin(2, Pin.OUT)
p27.off()
p2.on()


period = 100
position = 0

hspi = SPI(1, 100000, sck=Pin(14), mosi=Pin(13), miso=Pin(12))
hspi.init()

matrixObj = matrix.matrix()
IMAGES = matrixObj.getImages()
IMAGES_LEN = len(IMAGES)
maxDevice = 4
printAll = 0

byte = 'A'
count = 0
bufferA  = [[0 for i in range(8)] for j in range(maxDevice * 2)]
buffertemp = [[0 for i in range(8)] for j in range(maxDevice * 2)]
bufferb = [8]
countCol = 0
countCa = 0
array = "     @@@@ SUSCRIBETE AL CANAL Y APRENDE PASO A PASO @@@@          "


def sendDataSPI(reg: int,  data: int):
    cmd = bytearray([reg, data])
    hspi.write(cmd)

def cleanAllMatrix():
    for i in range(8):
        p27.off()
        for j in range(maxDevice):
            sendDataSPI(i+1, 0x00)
        p27.on()

def InitSPI():
    
    for j in range(maxDevice):
        p27.off()
        sendDataSPI(0x09, 0x00)  # Decode Mode  ---no decode
        p27.on()
    for j in range(maxDevice):
        p27.off()
        sendDataSPI(0x0A, 0x01)  # Intensity  --- low
        p27.on()

    for j in range(maxDevice):
        p27.off()
        sendDataSPI(0x0B, 0x07)  # Scan Limit  --- 0-7
        p27.on()
    for j in range(maxDevice):
        p27.off()
        sendDataSPI(0x0C, 0x01)  # Shutdown  --- no
        p27.on()
    for j in range(maxDevice):
        p27.off()
        sendDataSPI(0x0F, 0x00)  # Display Test  --- no      
        p27.on()

    cleanAllMatrix()
    print("init spi config")

def PrintLetter(letter: str):
    pos = 0

    if letter == 'A':
        pos = 0

    elif letter == 'B':
        pos = 1

    elif letter == 'C':
        pos = 2

    elif letter == 'D':
        pos = 3

    elif letter == 'E':
        pos = 4

    elif letter == 'F':
        pos = 5

    elif letter == 'G':
        pos = 6

    elif letter == 'H':
        pos = 7

    elif letter == 'I':
        pos = 8

    elif letter == 'J':
        pos = 9

    elif letter == 'K':
        pos = 10

    elif letter == 'L':
        pos = 11

    elif letter == 'M':
        pos = 12

    elif letter == 'N':
        pos = 13

    elif letter == 'O':
        pos = 14

    elif letter == 'P':
        pos = 15

    elif letter == 'Q':
        pos = 16

    elif letter == 'R':
        pos = 17

    elif letter == 'S':
        pos = 18

    elif letter == 'T':
        pos = 19

    elif letter == 'U':
        pos = 20

    elif letter == 'V':
        pos = 21

    elif letter == 'W':
        pos = 22

    elif letter == 'X':
        pos = 23

    elif letter == 'Y':
        pos = 24

    elif letter == 'Z':
        pos = 25

    elif letter == ' ':
        pos = 26

    elif letter == 'a':
        pos = 27

    elif letter == 'b':
        pos = 28

    elif letter == 'c':
        pos = 29

    elif letter == 'd':
        pos = 30

    elif letter == 'e':
        pos = 31

    elif letter == 'f':
        pos = 32

    elif letter == 'g':
        pos = 33

    elif letter == 'h':
        pos = 34

    elif letter == 'i':
        pos = 35

    elif letter == 'j':
        pos = 36

    elif letter == 'k':
        pos = 37

    elif letter == 'l':
        pos = 38

    elif letter == 'm':
        pos = 39

    elif letter == 'n':
        pos = 40

    elif letter == 'o':
        pos = 41

    elif letter == 'p':
        pos = 42

    elif letter == 'q':
        pos = 43

    elif letter == 'r':
        pos = 44

    elif letter == 's':
        pos = 45

    elif letter == 't':
        pos = 46

    elif letter == 'u':
        pos = 47

    elif letter == 'v':
        pos = 48

    elif letter == 'w':
        pos = 49

    elif letter == 'x':
        pos = 50

    elif letter == 'y':
        pos = 51

    elif letter == 'z':
        pos = 52

    elif letter == 'Ñ':
        pos = 53

    elif letter == 'ñ':
        pos = 54

    elif letter == '1':
        pos = 55

    elif letter == '2':
        pos = 56

    elif letter == '3':
        pos = 57

    elif letter == '4':
        pos = 58

    elif letter == '5':
        pos = 59

    elif letter == '6':
        pos = 60

    elif letter == '7':
        pos = 61

    elif letter == '8':
        pos = 62

    elif letter == '9':
        pos = 63

    elif letter == '0':
        pos = 64

    elif letter == '+':
        pos = 65

    elif letter == '-':
        pos = 66

    elif letter == '*':
        pos = 67

    elif letter == '/':
        pos = 68

    elif letter == '%':
        pos = 69

    elif letter == '=':
        pos = 70
    elif letter == '@':
        pos = 101
    elif letter == '?':
        pos = 86

    else:
        pos = 26
    return pos




buffer = bytearray(2) 

def serialWrite(address, data):
    buffer[0] = address
    buffer[1] = data
    SPI.write(buffer)    

def timerEvent():
    global countCol
    global countCa

    p2.value(not p2.value())
  
    pos = 0
    pos = len(array)  # Get len of array  
    if countCol == 0 and countCa == 0:
        for k in range(maxDevice * 2):
            # pass value to buffers
            for j in range(8):
                position = 0
                if printAll == 1:  
                    bufferA[k][j] = IMAGES[k][j]  # actual print
                else:
                    position = PrintLetter(array[countCa])
                    bufferA[k][j] = IMAGES[position][j]  # actual print
            countCa += 1
            if printAll == 1:
                if countCa >= IMAGES_LEN:
                    countCa = 0
            else:
                if countCa >= pos:
                    countCa = 0
    else:
        for i in range(maxDevice * 2):
            for k in range(8):
                buffertemp[i][k] = bufferA[i][k]

        for j in range(8):
            for k in range(maxDevice+1):
                bufferA[k][j] = (buffertemp[k][j] << 1) | (
                    buffertemp[k+1][j] >> 7)  # scroll caracters
                bufferA[k+1][j] = bufferA[k+1][j] << 1  # shift left

    cleanAllMatrix()
    
    for j in range(8):
        p27.off()
        for k in range(maxDevice):            
            sendDataSPI(j+1, int(bufferA[maxDevice - k -1] [j]))  # Print actual buffer
        p27.on()  

    countCol += 1
    factor = 0
    if maxDevice == 1:
        factor = 1
    else:
        factor = 2
    
    if countCol>=(8*factor):
        countCol=0
        if (maxDevice-2) >0:
            countCa-= (maxDevice-2)
        for k in range(maxDevice):
            # pass value to buffers
            for j in range(8):
                position = 0
                if printAll==1:
                    bufferA[maxDevice+k][j] = IMAGES[countCa][j]  #actual print	
                else:
                    position = PrintLetter(array[countCa])
                    bufferA[maxDevice+k][j] = IMAGES[position][j]  #actual print
            countCa+=1
            if printAll==1:
                if countCa>= IMAGES_LEN:
                    countCa=0
            else:
                if countCa>= pos:
                    countCa=0
            

def toggleLed():
    print("toggleLed")
    p2.value(not p2.value())
    timerEvent()

tim1 = Timer(1)
tim1.init(period=period, mode=Timer.PERIODIC, callback=lambda t:

          toggleLed()
          )

InitSPI()

while True:
    pass
