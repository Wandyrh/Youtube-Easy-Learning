import socket
from machine import ADC
from machine import Timer

adc1 = ADC(Pin(34))
adc2 = ADC(Pin(39))
adc3 = ADC(Pin(36))

adc1.atten(ADC.ATTN_11DB) 
adc2.atten(ADC.ATTN_11DB) 
adc3.atten(ADC.ATTN_11DB) 

period = 1000


r_value = "0"
g_value = "0"
b_value = "0"

tim1 = Timer(1)
tim1.init(period=period, mode=Timer.PERIODIC, callback=lambda t:
    get_adc_values()
)

def get_adc_values(): 
    global r_value  
    global g_value  
    global b_value  

    adcVal1 = int(adc1.read() / 40)
    adcVal2 = int(adc2.read() / 40)
    adcVal3 = int(adc3.read() / 40)

    if adcVal1 > 100 :
        adcVal1 = 100    

    if adcVal2 > 100 :
        adcVal2 = 100   

    if adcVal3 > 100 :
        adcVal3 = 100

    r_value = get_string_value(adcVal1)   
    g_value = get_string_value(adcVal2)
    b_value = get_string_value(adcVal3)



def get_string_value(input: int):    
        return str(input)  

def web_page():
    
  
    html = """
    <html>
        <head>
            <title>ESP32 Web Server</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
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
                    padding: 7px;           
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
            <p>Medici&oacute;n de sensores</p>
        
        
            <table>
                <tbody>
                    <tr>
                        <td>
                            <p><a href="/update"><button class="ButtonR Button">Sensor 1</button></a></p>
                        </td>
                        <td>
                            <strong> """+ r_value +""" %</strong>
                            <meter id="fuel" min="0" max="100" low="30" high="70" optimum="80" value=" @@"""+ r_value +""" @@">
                                at 50/100
                            </meter>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p><a href="/update"><button class="ButtonG Button">Sensor 2</button></a></p>
                        </td>
                        <td>
                            <strong> """+ g_value +""" %</strong>
                            <meter id="fuel" min="0" max="100" low="30" high="70" optimum="80" value=" @@"""+ g_value +""" @@">
                                at 50/100
                            </meter>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p><a href="/update"><button class="ButtonB Button">Sensor 3</button></a></p>
                        </td>
                        <td>
                            <strong> """+ b_value +""" %</strong>
                            <meter id="fuel" min="0" max="100" low="30" high="70" optimum="80" value=" @@"""+ b_value +""" @@">
                                at 50/100
                            </meter>
                        </td>
                    </tr>
                </tbody>
            </table>
        </body>
        </html>
        """
    return html


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