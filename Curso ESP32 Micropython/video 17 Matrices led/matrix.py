class matrix(object):
    def __init__(self):
        self.IMAGES = []
        self.IMAGES = [
        [  # A 0
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01111110,
        0b01100110,
        0b01100110,
        0b01100110
        ],
        [  # B 1
        0b00000000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01111100
        ],
        [  # C 2
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100000,
        0b01100000,
        0b01100000,
        0b01100110,
        0b00111100
        ],
        [  # D 3
        0b00000000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01111100
        ],
        [  # E 4
        0b00000000,
        0b01111110,
        0b01100000,
        0b01100000,
        0b01111100,
        0b01100000,
        0b01100000,
        0b01111110
        ],
        [  # F 5
        0b00000000,
        0b01111110,
        0b01100000,
        0b01100000,
        0b01111100,
        0b01100000,
        0b01100000,
        0b01100000
        ],
        [  # G 6
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100000,
        0b01100000,
        0b01101110,
        0b01100110,
        0b00111100
        ],
        [  # H 7
        0b00000000,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01111110,
        0b01100110,
        0b01100110,
        0b01100110
        ],
        [  # I 8
        0b00000000,
        0b00111100,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00111100
        ],
        [  # J 9
        0b00000000,
        0b00011110,
        0b00001100,
        0b00001100,
        0b00001100,
        0b01101100,
        0b01101100,
        0b00111000
        ],
        [  # K 10
        0b00000000,
        0b01100110,
        0b01101100,
        0b01111000,
        0b01110000,
        0b01111000,
        0b01101100,
        0b01100110
        ],
        [  # L 11
        0b00000000,
        0b01100000,
        0b01100000,
        0b01100000,
        0b01100000,
        0b01100000,
        0b01100000,
        0b01111110
        ],
        [  # M 12
        0b00000000,
        0b01100011,
        0b01110111,
        0b01111111,
        0b01101011,
        0b01100011,
        0b01100011,
        0b01100011
        ],
        [  # N 13
        0b00000000,
        0b01100011,
        0b01110011,
        0b01111011,
        0b01101111,
        0b01100111,
        0b01100011,
        0b01100011
        ],
        [  # O 14
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100
        ],
        [  # P 15
        0b00000000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01111100,
        0b01100000,
        0b01100000
        ],
        [  # Q 16
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01101110,
        0b00111100,
        0b00000110
        ],
        [  # R 17
        0b00000000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01111100,
        0b01111000,
        0b01101100,
        0b01100110
        ],
        [  # S 18
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100000,
        0b00111100,
        0b00000110,
        0b01100110,
        0b00111100
        ],
        [  # T 19
        0b00000000,
        0b01111110,
        0b01011010,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000
        ],
        [  # U 20
        0b00000000,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111110
        ],
        [  # V 21
        0b00000000,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00011000
        ],
        [  # W 22
        0b00000000,
        0b01100011,
        0b01100011,
        0b01100011,
        0b01101011,
        0b01111111,
        0b01110111,
        0b01100011
        ],
        [  # X 23
        0b00000000,
        0b01100011,
        0b01100011,
        0b00110110,
        0b00011100,
        0b00110110,
        0b01100011,
        0b01100011
        ],
        [  # Y 24
        0b00000000,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00011000,
        0b00011000,
        0b00011000
        ],
        [  # Z 25
        0b00000000,
        0b01111110,
        0b00000110,
        0b00001100,
        0b00011000,
        0b00110000,
        0b01100000,
        0b01111110
        ],
        [  # ' ' 26
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
        ],
        [  # a 27
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b00000110,
        0b00111110,
        0b01100110,
        0b00111110
        ],
        [  # b 28
        0b00000000,
        0b01100000,
        0b01100000,
        0b01100000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01111100
        ],
        [  # c 29
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100000,
        0b01100110,
        0b00111100
        ],
        [  # d 30
        0b00000000,
        0b00000110,
        0b00000110,
        0b00000110,
        0b00111110,
        0b01100110,
        0b01100110,
        0b00111110
        ],
        [  # e 31
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01111110,
        0b01100000,
        0b00111100
        ],
        [  # f 32
        0b00000000,
        0b00011100,
        0b00110110,
        0b00110000,
        0b00110000,
        0b01111100,
        0b00110000,
        0b00110000
        ],
        [  # g 33
        0b00000000,
        0b00000000,
        0b00111110,
        0b01100110,
        0b01100110,
        0b00111110,
        0b00000110,
        0b00111100
        ],
        [  # h 34
        0b00000000,
        0b01100000,
        0b01100000,
        0b01100000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01100110
        ],
        [  # i 35
        0b00000000,
        0b00000000,
        0b00011000,
        0b00000000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00111100
        ],
        [  # j 36
        0b00000000,
        0b00001100,
        0b00000000,
        0b00001100,
        0b00001100,
        0b01101100,
        0b01101100,
        0b00111000
        ],
        [  # k 37
        0b00000000,
        0b01100000,
        0b01100000,
        0b01100110,
        0b01101100,
        0b01111000,
        0b01101100,
        0b01100110
        ],
        [  # l 38
        0b00000000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000
        ],
        [  # m 39
        0b00000000,
        0b00000000,
        0b00000000,
        0b01100011,
        0b01110111,
        0b01111111,
        0b01101011,
        0b01101011
        ],
        [  # n 40
        0b00000000,
        0b00000000,
        0b00000000,
        0b01111100,
        0b01111110,
        0b01100110,
        0b01100110,
        0b01100110
        ],
        [  # o 41
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100
        ],
        [  # p 42
        0b00000000,
        0b00000000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01111100,
        0b01100000,
        0b01100000
        ],
        [  # q 43
        0b00000000,
        0b00000000,
        0b00111100,
        0b01101100,
        0b01101100,
        0b00111100,
        0b00001101,
        0b00001111
        ],
        [  # r 44
        0b00000000,
        0b00000000,
        0b00000000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01100000,
        0b01100000
        ],
        [  # s 45
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111110,
        0b01000000,
        0b00111100,
        0b00000010,
        0b01111100
        ],
        [  # t 46
        0b00000000,
        0b00000000,
        0b00011000,
        0b00011000,
        0b01111110,
        0b00011000,
        0b00011000,
        0b00011000
        ],
        [  # u 47
        0b00000000,
        0b00000000,
        0b00000000,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111110
        ],
        [  # v 48
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00011000
        ],
        [  # w 49
        0b00000000,
        0b00000000,
        0b00000000,
        0b01100011,
        0b01101011,
        0b01101011,
        0b01101011,
        0b00111110
        ],
        [  # x 50
        0b00000000,
        0b00000000,
        0b00000000,
        0b01100110,
        0b00111100,
        0b00011000,
        0b00111100,
        0b01100110
        ],
        [  # y 51
        0b00000000,
        0b00000000,
        0b00000000,
        0b01100110,
        0b01100110,
        0b00111110,
        0b00000110,
        0b00111100
        ],
        [  # z 52
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b00001100,
        0b00011000,
        0b00110000,
        0b00111100
        ],
        [  # Ñ 53
        0b00011100,
        0b01100011,
        0b01110011,
        0b01111011,
        0b01101111,
        0b01100111,
        0b01100011,
        0b01100011
        ],
        [  # ñ 54
        0b00000000,
        0b00111100,
        0b00000000,
        0b01111100,
        0b01111110,
        0b01100110,
        0b01100110,
        0b01100110
        ],
        [  # 1 55
        0b00000000,
        0b00011000,
        0b00011000,
        0b00111000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b01111110
        ],
        [  # 2 56
        0b00000000,
        0b00111100,
        0b01100110,
        0b00000110,
        0b00001100,
        0b00110000,
        0b01100000,
        0b01111110
        ],
        [  # 3 57
        0b00000000,
        0b00111100,
        0b01100110,
        0b00000110,
        0b00011100,
        0b00000110,
        0b01100110,
        0b00111100
        ],
        [  # 4 58
        0b00000000,
        0b00001100,
        0b00011100,
        0b00101100,
        0b01001100,
        0b01111110,
        0b00001100,
        0b00001100
        ],
        [  # 5 59
        0b00000000,
        0b01111110,
        0b01100000,
        0b01111100,
        0b00000110,
        0b00000110,
        0b01100110,
        0b00111100
        ],
        [  # 6 60
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b00111100
        ],
        [  # 7 61
        0b00000000,
        0b01111110,
        0b01100110,
        0b00001100,
        0b00001100,
        0b00011000,
        0b00011000,
        0b00011000
        ],
        [  # 8 62
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b00111100,
        0b01100110,
        0b01100110,
        0b00111100
        ],
        [  # 9 63
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b00111110,
        0b00000110,
        0b01100110,
        0b00111100
        ],
        [  # 0 64
        0b00000000,
        0b00111100,
        0b01100110,
        0b01101110,
        0b01110110,
        0b01100110,
        0b01100110,
        0b00111100
        ],
        [  # + 65
        0b00000000,
        0b00000000,
        0b00001000,
        0b00001000,
        0b00111110,
        0b00001000,
        0b00001000,
        0b00000000
        ],
        [  # - 66
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b00000000,
        0b00000000,
        0b00000000
        ],
        [  # * 67
        0b00000000,
        0b00000000,
        0b00110110,
        0b00011100,
        0b01111111,
        0b00011100,
        0b00110110,
        0b00000000
        ],
        [  # / 68
        0b00000000,
        0b00000000,
        0b00000110,
        0b00001100,
        0b00011000,
        0b00110000,
        0b01100000,
        0b00000000
        ],
        [  # % 69
        0b00000000,
        0b01100000,
        0b01100110,
        0b00001100,
        0b00011000,
        0b00110000,
        0b01100110,
        0b00000110
        ],
        [  # = 70
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b00000000,
        0b00111100,
        0b00000000,
        0b00000000
        ],
        [  # ~ 71
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111010,
        0b01101100,
        0b00000000,
        0b00000000,
        0b00000000
        ],
        [  # ^ 72
        0b00000000,
        0b00001000,
        0b00010100,
        0b00100010,
        0b01000001,
        0b00000000,
        0b00000000,
        0b00000000
        ],
        [  # < 73
        0b00000000,
        0b00000110,
        0b00001100,
        0b00011000,
        0b00110000,
        0b00011000,
        0b00001100,
        0b00000110
        ],
        [  # > 74
        0b00000000,
        0b01100000,
        0b00110000,
        0b00011000,
        0b00001100,
        0b00011000,
        0b00110000,
        0b01100000
        ],
        [  # ( 75
        0b00000000,
        0b00000110,
        0b00001100,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00001100,
        0b00000110
        ],
        [  # 76
        0b00000000,
        0b01100000,
        0b00110000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00110000,
        0b01100000
        ],
        [  # [ 77
        0b00000000,
        0b00011110,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011110
        ],
        [  # ] 78
        0b00000000,
        0b01111000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b01111000
        ],
        [  # [ 79
        0b00000000,
        0b00001110,
        0b00011000,
        0b00011000,
        0b00110000,
        0b00011000,
        0b00011000,
        0b00001110
        ],
        [  # ] 80
        0b00000000,
        0b01110000,
        0b00011000,
        0b00011000,
        0b00001100,
        0b00011000,
        0b00011000,
        0b01110000
        ],
        [  # . 81
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01100000,
        0b01100000
        ],
        [  # : 82
        0b00000000,
        0b00000000,
        0b00011000,
        0b00011000,
        0b00000000,
        0b00011000,
        0b00011000,
        0b00000000
        ],
        [  # ; 83
        0b00000000,
        0b00000000,
        0b00011000,
        0b00011000,
        0b00000000,
        0b00011000,
        0b00011000,
        0b00110000
        ],
        [  # , 84
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00110000,
        0b00110000,
        0b00110000,
        0b01100000
        ],
        [  # ! 85
        0b00000000,
        0b00011000,
        0b00111100,
        0b00111100,
        0b00011000,
        0b00011000,
        0b00000000,
        0b00011000
        ],
        [  # ? 86
        0b00000000,
        0b00111100,
        0b01100110,
        0b00000110,
        0b00011100,
        0b00011000,
        0b00000000,
        0b00011000
        ],
        [  # @ 87
        0b00000000,
        0b00111000,
        0b01000100,
        0b01011100,
        0b01011000,
        0b01000010,
        0b00111100,
        0b00000000
        ],
        [  # & 88
        0b00000000,
        0b00111100,
        0b01100110,
        0b00111100,
        0b00101000,
        0b01100101,
        0b01100110,
        0b00111111
        ],
        [  # $ 89
        0b00000000,
        0b00001000,
        0b00011110,
        0b00100000,
        0b00011100,
        0b00000010,
        0b00111100,
        0b00001000
        ],
        [  # 90
        0b00000000,
        0b00110110,
        0b00110110,
        0b01111111,
        0b00110110,
        0b01111111,
        0b00110110,
        0b00110110
        ],
        [  # row up 91
        0b00000000,
        0b00001000,
        0b00011100,
        0b00111110,
        0b01111111,
        0b00011100,
        0b00011100,
        0b00011100
        ],
        [  # row down 92
        0b00000000,
        0b00011100,
        0b00011100,
        0b00011100,
        0b01111111,
        0b00111110,
        0b00011100,
        0b00001000
        ],
        [  # row right 93
        0b00000000,
        0b00001000,
        0b00001100,
        0b01111110,
        0b01111111,
        0b01111110,
        0b00001100,
        0b00001000
        ],
        [  # row left 94
        0b00000000,
        0b00001000,
        0b00011000,
        0b00111111,
        0b01111111,
        0b00111111,
        0b00011000,
        0b00001000
        ],
        [  # triangle up 95
        0b00000000,
        0b00001000,
        0b00011100,
        0b00011100,
        0b00111110,
        0b00111110,
        0b01111111,
        0b01111111
        ],
        [  # triangle down 96
        0b00000000,
        0b01111111,
        0b01111111,
        0b00111110,
        0b00111110,
        0b00011100,
        0b00011100,
        0b00001000
        ],
        [  # triangle right 97
        0b00000000,
        0b01100000,
        0b01111000,
        0b01111110,
        0b01111111,
        0b01111110,
        0b01111000,
        0b01100000
        ],
        [  # triangle left 98
        0b00000000,
        0b00000011,
        0b00001111,
        0b00111111,
        0b01111111,
        0b00111111,
        0b00001111,
        0b00000011
        ],
        [  # : 99
        0b00000000,
        0b00111110,
        0b01000001,
        0b01010101,
        0b01000001,
        0b01010101,
        0b01001001,
        0b00111110
        ],
        [  # : 100
        0b00000000,
        0b00111110,
        0b01111111,
        0b01101011,
        0b01111111,
        0b01101011,
        0b01110111,
        0b00111110
        ],
        [  # Heart 101
        0b00000000,
        0b00100010,
        0b01110111,
        0b01111111,
        0b01111111,
        0b00111110,
        0b00011100,
        0b00001000
        ],
        [  # Rombo 102
        0b00000000,
        0b00001000,
        0b00011100,
        0b00111110,
        0b01111111,
        0b00111110,
        0b00011100,
        0b00001000
        ],
        [  # cruz 103
        0b00000000,
        0b00001000,
        0b00011100,
        0b00101010,
        0b01111111,
        0b00101010,
        0b00001000,
        0b00011100
        ],
        [  # pi 104
        0b00000000,
        0b00001000,
        0b00011100,
        0b00111110,
        0b01111111,
        0b00111110,
        0b00001000,
        0b00011100
        ],
        [  # full esfera 105
        0b00000000,
        0b00000000,
        0b00011100,
        0b00111110,
        0b00111110,
        0b00111110,
        0b00011100,
        0b00000000
        ],
        [  # cuadro con esfera 106
        0b11111111,
        0b11111111,
        0b11100011,
        0b11000001,
        0b11000001,
        0b11000001,
        0b11100011,
        0b11111111
        ],
        [  # circulo 107
        0b00000000,
        0b00000000,
        0b00011100,
        0b00100010,
        0b00100010,
        0b00100010,
        0b00011100,
        0b00000000
        ],
        [  # cuadro cuadro 108
        0b11111111,
        0b11111111,
        0b11100011,
        0b11011101,
        0b11011101,
        0b11011101,
        0b11100011,
        0b11111111
        ],
        [  # Macho 109
        0b00000000,
        0b00001111,
        0b00000011,
        0b00000101,
        0b00111001,
        0b01001000,
        0b01001000,
        0b00110000
        ],
        [  # Hembra 110
        0b00000000,
        0b00001000,
        0b00111110,
        0b00001000,
        0b00011100,
        0b00100010,
        0b00100010,
        0b00011100
        ],
        [  # music 1 111
        0b00000000,
        0b00011000,
        0b00010100,
        0b00010000,
        0b00010000,
        0b00110000,
        0b01110000,
        0b01100000
        ],
        [  # Music 2 112
        0b00000000,
        0b00001111,
        0b00011001,
        0b00010001,
        0b00010011,
        0b00110111,
        0b01110110,
        0b01100000
        ],
        [  # star * 113
        0b00000000,
        0b00001000,
        0b00101010,
        0b00011100,
        0b01110111,
        0b00011100,
        0b00101010,
        0b00001000
        ],
        [  # doble ancla vertical 114
        0b00000000,
        0b00001000,
        0b00011100,
        0b00101010,
        0b00001000,
        0b00101010,
        0b00011100,
        0b00001000
        ],
        [  # A :: 115
        0b00000000,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00000000,
        0b01100110,
        0b01100110
        ],
        [  # doble ancla horizontal 116
        0b00000000,
        0b00000000,
        0b00010100,
        0b00100010,
        0b01111111,
        0b00100010,
        0b00010100,
        0b00000000
        ],
        [  # '' 117
        0b00000000,
        0b00110110,
        0b00110110,
        0b00010100,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
        ],
        [  # /*   \  118   */
        0b00000000,
        0b00000000,
        0b01100000,
        0b00110000,
        0b00011000,
        0b00001100,
        0b00000110,
        0b00000000
        ],
        [  # ' 119
        0b00000000,
        0b00001100,
        0b00001100,
        0b00000110,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
        ],
        [  # ´ 120
        0b00000000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00110000,
        0b00000000,
        0b00000000,
        0b00000000
        ],
        [  # casita 121
        0b00000000,
        0b00001000,
        0b00011100,
        0b00110110,
        0b01100011,
        0b01000001,
        0b01000001,
        0b01111111
        ]
        ]




    def getImages(self):
        return self.IMAGES
