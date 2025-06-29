from machine import Pin, PWM


class rgbpwm(object):
    def __init__(self, r: int, g: int, b: int, frecuency: int, onValue: bool):
        self._pwm_r = PWM(Pin(r))
        self._pwm_g = PWM(Pin(g))
        self._pwm_b = PWM(Pin(b))

        self._pwm_r.freq(frecuency)
        self._pwm_g.freq(frecuency)
        self._pwm_b.freq(frecuency)
        self._onValue = onValue
        self._offValue = 0
        self._maxValue = 1023
        if self._onValue == False:
            self._offValue = self._maxValue        

    def off(self):        
        self._pwm_r.duty(self._offValue)
        self._pwm_g.duty(self._offValue)
        self._pwm_b.duty(self._offValue)

    def red(self, duty: int):
        if duty > self._maxValue: duty = self._maxValue
        if self._onValue == True:
            self._pwm_r.duty(duty)
        else:
            self._pwm_r.duty(self._maxValue - duty)

    def green(self, duty: int):
        if duty > self._maxValue: duty = self._maxValue
        if self._onValue == True:
            self._pwm_g.duty(duty)
        else:
            self._pwm_g.duty(self._maxValue - duty)

    def blue(self, duty: int):
        if duty > self._maxValue: duty = self._maxValue
        if self._onValue == True:
            self._pwm_b.duty(duty)
        else:
            self._pwm_b.duty(self._maxValue - duty)
