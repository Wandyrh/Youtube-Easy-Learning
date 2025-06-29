import socket
import dht
import machine
from machine import Timer
import time


dht22 = dht.DHT22(machine.Pin(13))
period = 1000

temperature_value = '00'
humidity_value = '00'


tim1 = Timer(1)
tim1.init(period=period, mode=Timer.PERIODIC, callback=lambda t:
    get_dht22_values()
)

def get_dht22_values():
    global temperature_value
    global humidity_value

    dht22.measure()
    temperature = dht22.temperature() # eg. 23.6 (°C)
    humidity = dht22.humidity()    # eg. 41.3 (% RH)

    temperature_value = get_string_value(temperature)   
    humidity_value = get_string_value(humidity)
    



def get_string_value(input: int):    
        return str(round(input,1))  

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
            display: inline-block;
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
    <p>Sensor DHT22</p>
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
                    <meter id="fuel" min="0" max="100" low="10" high="25" optimum="24" value=" @@""" + temperature_value +""" @@">
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
                    <meter id="fuel" min="0" max="100" low="30" high="60" optimum="50" value=" @@""" + humidity_value +""" @@">
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