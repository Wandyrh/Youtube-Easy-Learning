import socket
from max30102 import MAX30102, MAX30105_PULSE_AMP_MEDIUM
import _thread
from machine import sleep, SoftI2C, Pin 
from utime import ticks_diff, ticks_us

led = Pin(2, Pin.OUT)
MAX_HISTORY = 32
history = []
beats_history = []
beat = False
beats = 0
    
i2c = SoftI2C(sda=Pin(21),scl=Pin(22),freq=400000)
sensor = MAX30102(i2c=i2c)  # An I2C instance is required

# Scan I2C bus to ensure that the sensor is connected
if sensor.i2c_address not in i2c.scan():
    print("Sensor not found.")
    
elif not (sensor.check_part_id()):
    # Check that the targeted sensor is compatible
    print("I2C device ID not corresponding to MAX30102 or MAX30105.")
    
else:
    print("Sensor connected and recognized.")


sensor.setup_sensor()
sensor.set_sample_rate(400)
sensor.set_fifo_average(8)
sensor.set_active_leds_amplitude(MAX30105_PULSE_AMP_MEDIUM)
sensor.set_led_mode(2)
sleep(1)

t_start = ticks_us()  # Starting time of the acquisition  


def get_max30102_values():
    while True:
        global history
        global beats_history
        global beat
        global beats
        global t_start

        sensor.check()
        
        # Check if the storage contains available samples
        if sensor.available():
            # Access the storage FIFO and gather the readings (integers)
            red_reading = sensor.pop_red_from_storage()
            ir_reading = sensor.pop_ir_from_storage()
            
            value = red_reading
            history.append(value)
            # Get the tail, up to MAX_HISTORY length
            history = history[-MAX_HISTORY:]
            minima = 0
            maxima = 0
            threshold_on = 0
            threshold_off = 0

            minima, maxima = min(history), max(history)

            threshold_on = (minima + maxima * 3) // 4   # 3/4
            threshold_off = (minima + maxima) // 2      # 1/2
            
            
            if value > 1000:
                if not beat and value > threshold_on:
                    beat = True 
                                    
                    led.on()
                    
                    t_us = ticks_diff(ticks_us(), t_start)
                    t_s = t_us/1000000
                    f = 1/t_s
                
                    bpm = f * 60
                    
                    if bpm < 500:
                        t_start = ticks_us()
                        beats_history.append(bpm)                    
                        beats_history = beats_history[-MAX_HISTORY:] 
                        beats = round(sum(beats_history)/len(beats_history) ,2)  
                                        
                if beat and value< threshold_off:
                    beat = False
                    led.off()
                
            else:
                led.off()
                print('Not finger')
                beats = 0.00

def web_page(): 
    html = """
    <head>
    <title>ESP32 Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
    <link rel="icon" href="data:,">
    <style>
        meter {
            -webkit-writing-mode: horizontal-tb !important;
            appearance: auto;
            box-sizing: border-box;
            display: inline-block;
            height: 3em;
            width: 13em;
            vertical-align: -1.0em;
            -webkit-user-modify: read-only !important;
        }
        html {
            font-family: Helvetica;
            display: inline-block;
            margin: 0px auto;
            text-align: center;
        }
        h1 {
            color: #0F3376;
            padding: 2vh;
        }
        p {
            font-size: 1.5rem;
        }
        table {
            margin: auto;
        }
        td {
            padding: 3px;           
        }
        .Button {
            border-radius: 31px;
            display: inline-block;
            cursor: pointer;
            color: #ffffff;
            font-family: Arial;
            font-size: 10px;
            font-weight: bold;
            font-style: italic;
            padding: 4px 5px;
            text-decoration: none;
        }
        .ButtonR {
            background-color: #ec5449;
            border: 3px solid #991f1f;
            text-shadow: 0px 2px 2px #47231e;
        }
        .ButtonR:hover {
            background-color: #f54a16;
        }
        .Button:active {
            position: relative;
            top: 1px;
        }
        .ButtonG {
            background-color: #49ece4;
            border: 3px solid #1f8b99;
            text-shadow: 0px 2px 2px #1e3b47;
        }
        .ButtonG:hover {
            background-color: #16b6f5;
        }
        .ButtonB {
            background-color: #4974ec;
            border: 3px solid #1f3599;
            text-shadow: 0px 2px 2px #1e2447;
        }
        .ButtonB:hover {
            background-color: #165df5;
        }
    </style>
</head>
<body>
    <h1>ESP32 Web Server</h1>
    <p>Sensor MAX30102</p>
    <table>
        <tbody>
            <tr>
                <td>
                    <p class="center">
                        <a href="/update"><button class="ButtonR Button">
                                <i class="fa fa-heartbeat fa-2x" aria-hidden="true"></i> BPM
                            </button></a>
                    </p>
                </td>
                <td>
                    <strong> """+ str(beats) +""" </strong>                   
                </td>
                <td>                    
                    <meter id="fuel" min="0" max="200" low="59" high="100" optimum="60" value=" @@""" + str(beats) +""" @@">
                        at 50/100
                    </meter>
                </td>
            </tr>
            <tr>
                <td>
                    <p><a href="/update"><button class="ButtonG Button">
                                <i class="fa fa-thermometer-quarter fa-2x" aria-hidden="true"></i> Temp.
                            </button></a></p>
                </td>
                <td>
                    <strong> """+ str(round(sensor.read_temperature(),2)) +""" &#176;C</strong>                   
                </td>
                <td>                    
                    <meter id="fuel" min="0" max="100" low="20" high="40" optimum="30" value=" @@""" + str(round(sensor.read_temperature(),2)) +""" @@">
                        at 50/100
                    </meter>
                </td>
            </tr>            
        </tbody>
    </table>
</body>
<script>
    setInterval(updateValues, 2000);
    function updateValues() {
        location.reload(); 
    }
</script>
</html> 
   
    """
    return html

_thread.start_new_thread(get_max30102_values, ())


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)

while True: 
    try:
        conn, addr = s.accept()
        print('Got a connection from %s' % str(addr))
        request = conn.recv(1024)
        request = str(request)   
        update = request.find('/update')        
        
        if update == 6:
            print('update') 
            
        response = web_page()
        response = response.replace(" @@","")
        conn.send('HTTP/1.1 200 OK\n')
        conn.send('Content-Type: text/html\n')
        conn.send('Connection: close\n\n')
        conn.sendall(response)
        conn.close()
    except Exception as e:
        print(e)