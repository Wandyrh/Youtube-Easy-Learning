from machine import Timer
from machine import Pin, I2C
import socket
import time
import BME280



i2c = I2C(scl=Pin(22), sda=Pin(21), freq=10000)
bme = BME280.BME280(i2c=i2c)

temperature_value = '00'
humidity_value = '00'
pressure_value = '00'

period = 1000
tim1 = Timer(1)
tim1.init(period=period, mode=Timer.PERIODIC, callback=lambda t:
    get_BME280_values()
)

def get_BME280_values():
    global temperature_value
    global humidity_value
    global pressure_value
    
    temperature_value = bme.temperature
    humidity_value = bme.humidity
    pressure_value = bme.pressure

def web_page(): 
    html = """
    <html>
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
                background-color: #9549ec;
                border: 3px solid #6c1f99;
                text-shadow: 0px 2px 2px #3b1e47;
            }
            .ButtonR:hover {
                background-color: #c816f5;
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
        <p>Sensor BME280</p>
        <table>
            <tbody>
                <tr>
                    <td>
                        <p class="center">
                            <a href="/update"><button class="ButtonR Button">
                                    <i class="fa fa-thermometer-quarter fa-2x" aria-hidden="true"></i> Temp.
                                </button></a>
                        </p>
                    </td>
                    <td>
                        <strong> """+ temperature_value +""" C</strong>                   
                    </td>
                    <td>                    
                        <meter id="fuel" min="0" max="100" low="10" high="30" optimum="25" value=" @@""" + temperature_value +""" @@">
                            at 50/100
                        </meter>
                    </td>
                </tr>
                <tr>
                    <td>
                        <p><a href="/update"><button class="ButtonG Button">
                                    <i class="fa fa-tint fa-2x" aria-hidden="true"></i> Hum.
                                </button></a></p>
                    </td>
                    <td>
                        <strong> """+ humidity_value +""" %</strong>                   
                    </td>
                    <td>                    
                        <meter id="fuel" min="0" max="100" low="30" high="70" optimum="55" value=" @@""" + humidity_value +""" @@">
                            at 50/100
                        </meter>
                    </td>
                </tr>
                <tr>
                    <td>
                        <p><a href="/update"><button class="ButtonB Button">
                                    <i class="fa fa-cloud fa-2x" aria-hidden="true"></i> Pres.
                                </button></a></p>
                    </td>
                    <td>
                        <strong> """+ pressure_value +""" hPa</strong>                    
                    </td>
                    <td>                    
                        <meter id="fuel" min="0" max="1100" low="900" high="1015" optimum="1000" value=" @@""" + pressure_value +""" @@">
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


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)

while True:
    time.sleep_ms(500)
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