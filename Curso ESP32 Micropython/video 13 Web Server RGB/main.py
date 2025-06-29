import socket
import rgb


rgbObj = rgb.rgb(32, 33, 25, False)
rgbObj.off()

r_state = False
g_state = False
b_state = False

r_value = "OFF"
g_value = "OFF"
b_value = "OFF"


def get_string_value(input: bool):
    if input:
        return "ON"
    return "OFF"

def web_page():
    
    r_value = get_string_value(r_state)
    g_value = get_string_value(g_state)
    b_value = get_string_value(b_state)
    html = """
    <html>
        <head>
        <title>ESP32 Web Server</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
        <style>
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
            td{
                padding: 10px ;
            } 
            .Button {           
                border-radius: 31px;           
                display: inline-block;
                cursor: pointer;
                color: #ffffff;
                font-family: Arial;
                font-size: 17px;
                font-weight: bold;
                font-style: italic;
                padding: 17px 19px;
                text-decoration: none;           
            }
            .ButtonR {
                background-color: #ec4949;            
                border: 6px solid #991f1f;           
                text-shadow: 0px 2px 2px #471e1e;
            }
            .ButtonR:hover {
                background-color: #f51616;
            }

            .Button:active {
                position: relative;
                top: 1px;
            }
            .ButtonG {
                background-color: #49ec56;            
                border: 6px solid #23991f;          
                text-shadow: 0px 2px 2px #1e4723;
            }
            .ButtonG:hover {
                background-color: #29f516;
            }  
            .ButtonB {
                background-color: #4974ec;           
                border: 6px solid #1f3599;         
                text-shadow: 0px 2px 2px #1e2447;
            }
            .ButtonB:hover {
                background-color: #165df5;
            }
        
        </style>
        </head>

        <body>
            <h1>ESP32 Web Server</h1>
            <p>RGB Control</p>    

            <table>
                <tbody>
                    <tr>
                        <td>
                            <p><a href="/?led-r"><button class="ButtonR Button">R</button></a></p>
                        </td>
                        <td>
                            <strong> """+ r_value +"""</strong> 
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p><a href="/?led-g"><button class="ButtonG Button">G</button></a></p>
                        </td>
                        <td>
                            <strong> """+ g_value +"""</strong> 
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <p><a href="/?led-b"><button class="ButtonB Button">B</button></a></p>
                        </td>
                        <td>
                        <strong> """+ b_value +""" </strong>  
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
        led_r = request.find('/?led-r')
        led_g = request.find('/?led-g')
        led_b = request.find('/?led-b')
        
        if led_r == 6:
            print('LED R ') 
            r_state = not r_state
            g_state = False
            b_state = False
            if r_state == True:
                rgbObj.red()
            else:
                rgbObj.off()
            
        elif led_g == 6:
            print('LED G ') 
            r_state = False
            g_state = not g_state
            b_state = False
            if g_state == True:
                rgbObj.green()  
            else:
                rgbObj.off()    
        
        elif led_b == 6:
            print('LED B ')  
            r_state = False
            g_state = False
            b_state = not b_state
            if b_state == True:
                rgbObj.blue()  
            else:
                rgbObj.off()   
            
        response = web_page()
        conn.send('HTTP/1.1 200 OK\n')
        conn.send('Content-Type: text/html\n')
        conn.send('Connection: close\n\n')
        conn.sendall(response)
        conn.close()
    except Exception as e:
        print(e)