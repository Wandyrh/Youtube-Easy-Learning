from machine import Pin


class rgb(object):
    def __init__(self, r: int, g: int, b: int, onValue: bool):
        self._pinR = Pin(r, Pin.OUT)
        self._pinG = Pin(g, Pin.OUT)
        self._pinB = Pin(b, Pin.OUT)
        self._onValue = onValue
        self._pinR.off()
        self._pinG.off()
        self._pinB.off()

    def off(self):
        self._pinR.value(not self._onValue)
        self._pinG.value(not self._onValue)
        self._pinB.value(not self._onValue)

    def red(self):
        self._pinR.value(self._onValue)
        self._pinG.value(not self._onValue)
        self._pinB.value(not self._onValue)

    def green(self):
        self._pinR.value(not self._onValue)
        self._pinG.value(self._onValue)
        self._pinB.value(not self._onValue)

    def blue(self):
        self._pinR.value(not self._onValue)
        self._pinG.value(not self._onValue)
        self._pinB.value(self._onValue)
