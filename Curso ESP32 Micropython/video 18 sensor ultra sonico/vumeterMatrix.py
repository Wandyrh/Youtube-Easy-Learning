from machine import Pin, SPI

class vumeterMatrix(object):
    def __init__(self, maxDevice: int ):        
        self.maxDevice = maxDevice
        self.barLevel = 0        
        self.bufferA  = [[0 for i in range(8)] for j in range(self.maxDevice * 2)]
        self.p27 = Pin(27, Pin.OUT)
        self.p2 = Pin(2, Pin.OUT)
        self.p27.off()
        self.p2.on()        
        self.position = 0
        self.hspi = SPI(1, 100000, sck=Pin(14), mosi=Pin(13), miso=Pin(12))
        self.hspi.init()
        self.InitSPI()
        self.Level = []
        self.Level = [
            [  # 0
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000,
                0b00000000
            ],
            [  # 1
                0b10000000,
                0b10000000,
                0b10000000,
                0b10000000,
                0b10000000,
                0b10000000,
                0b10000000,
                0b10000000
            ],
            [  # 2
                0b11000000,
                0b11000000,
                0b11000000,
                0b11000000,
                0b11000000,
                0b11000000,
                0b11000000,
                0b11000000
            ],
            [  # 3
                0b11100000,
                0b11100000,
                0b11100000,
                0b11100000,
                0b11100000,
                0b11100000,
                0b11100000,
                0b11100000
            ],
            [  # 4
                0b11110000,
                0b11110000,
                0b11110000,
                0b11110000,
                0b11110000,
                0b11110000,
                0b11110000,
                0b11110000
            ],
            [  # 5
                0b11111000,
                0b11111000,
                0b11111000,
                0b11111000,
                0b11111000,
                0b11111000,
                0b11111000,
                0b11111000
            ],
            [  # 6
                0b11111100,
                0b11111100,
                0b11111100,
                0b11111100,
                0b11111100,
                0b11111100,
                0b11111100,
                0b11111100
            ],
            [  # 7
                0b11111110,
                0b11111110,
                0b11111110,
                0b11111110,
                0b11111110,
                0b11111110,
                0b11111110,
                0b11111110
            ],
            [  # 8
                0b11111111,
                0b11111111,
                0b11111111,
                0b11111111,
                0b11111111,
                0b11111111,
                0b11111111,
                0b11111111
            ]
        ]

    def getLevel(self):
        return self.Level

    def sendDataSPI(self, reg: int,  data: int):
        cmd = bytearray([reg, data])
        self.hspi.write(cmd)

    def cleanAllMatrix(self):
        for i in range(8):
            self.p27.off()
            for j in range(self.maxDevice):
                self.sendDataSPI(i+1, 0x00)
            self.p27.on()

    def InitSPI(self):
        print("init spi config")
        for j in range(self.maxDevice):
            self.p27.off()
            self.sendDataSPI(0x09, 0x00)  # Decode Mode  ---no decode
            self.p27.on()
        for j in range(self.maxDevice):
            self.p27.off()
            self.sendDataSPI(0x0A, 0x01)  # Intensity  --- low
            self.p27.on()
        for j in range(self.maxDevice):
            self.p27.off()
            self.sendDataSPI(0x0B, 0x07)  # Scan Limit  --- 0-7
            self.p27.on()
        for j in range(self.maxDevice):
            self.p27.off()
            self.sendDataSPI(0x0C, 0x01)  # Shutdown  --- no
            self.p27.on()
        for j in range(self.maxDevice):
            self.p27.off()
            self.sendDataSPI(0x0F, 0x00)  # Display Test  --- no      
            self.p27.on()

        self.cleanAllMatrix()

    def updateLevel(self, newLevel: int):
        self.barLevel = newLevel
        for j in range(8):
            self.p27.off()
            for k in range(self.maxDevice): 
                cal = (k * 8) - self.barLevel
                pos = cal * - 1            
                if(pos  <= 8 and pos  >= 1):
                    self.bufferA [k][j] = int(self.Level[pos][j])
                elif (pos  > 8):
                    self.bufferA [k][j] = int(self.Level[8][j]) 
                else:
                    self.bufferA [k][j] = int(self.Level[0][j])  
            self.p27.on()  

        for j in range(8):
            self.p27.off()
            for k in range(self.maxDevice):            
                self.sendDataSPI(j+1, int(self.bufferA[self.maxDevice - k -1] [j]))
            self.p27.on() 
            pass