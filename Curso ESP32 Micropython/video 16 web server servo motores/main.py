import socket
import rgb_pwm

rgb = rgb_pwm.rgbpwm(33,25,26,50,True)

rgb.off()

r_value = "0"
g_value = "0"
b_value = "0"

def web_page():   
    
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

                td {
                    padding: 10px;            
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
                    background-color: rgb(255, 30, 30);
                    border: 3px solid #991f1f;
                    text-shadow: 0px 2px 2px #471e1e;
                }

                .Button:active {
                    position: relative;
                    top: 1px;
                }

                .ButtonG {
                    background-color: rgb(30, 255, 30);
                    border: 3px solid #23991f;
                    text-shadow: 0px 2px 2px #1e4723;
                }
                .range {
                    margin: auto;
                    -webkit-appearance: none;
                    position: relative;
                    overflow: hidden;
                    height: 40px;
                    width: 200px;
                    cursor: pointer;
                    border-radius: 0;

                }

                ::-webkit-slider-runnable-track {
                    background: #ddd;
                }

                .range::-webkit-slider-thumb {
                    -webkit-appearance: none;
                    width: 20px;
                    height: 40px;
                    border: 2px solid #999;
                }

                .redrange::-webkit-slider-thumb {
                    background: #fff;
                    box-shadow: -100vw 0 0 100vw rgb(255, 30, 30);
                }

                .greenrange::-webkit-slider-thumb {
                    background: #fff;
                    box-shadow: -100vw 0 0 100vw rgb(30, 255, 30);
                }
                .slider {
                    -webkit-appearance: none;
                    width: 100%;
                    height: 25px;
                    background: #d3d3d3;
                    outline: none;
                    opacity: 0.7;
                    -webkit-transition: .2s;
                    transition: opacity .2s;
                }

                .slider:hover {
                    opacity: 1;
                }

                .slider::-webkit-slider-thumb {
                    -webkit-appearance: none;
                    appearance: none;
                    width: 25px;
                    height: 25px;
                    background: #04AA6D;
                    cursor: pointer;
                }

                .slider::-moz-range-thumb {
                    width: 25px;
                    height: 25px;
                    background: #04AA6D;
                    cursor: pointer;
                }

                .slidecontainer {
                    width: 100%;
                }
            </style>

        </head>

        <body>
            <h1>ESP32 Web Server</h1>
            <p>Servo motor control</p>
            <form>
                <table>
                    <tbody>
                        <tr>
                            <td>
                                <p><a> <button  class="ButtonR Button">X</button></a></p>
                            </td>
                            <td>
                                <input id="rinput" name="rinput" class="range redrange" type="range" max="99" value=" @@"""+ r_value +""" @@">
            
                            </td>
                            <td>
                                <strong id="rvalue"> """+ r_value +""" </strong>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <p><a><button  class="ButtonG Button">Y</button></a></p>
                            </td>
                            <td>
                                <input id="ginput" name="ginput" class="range greenrange" type="range" max="99" value=" @@"""+ g_value +""" @@">
            
                            </td>
                            <td>
                                <strong id="gvalue">"""+ g_value +"""</strong>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="2">
                                <center>
                                    <img src="https://www.dhresource.com/webp/m/0x0/f2/albu/g11/M00/8C/BD/rBNaFl8VOk2AYWozAAItQWIxRa8076.jpg/mitoot-mg996r-engranajes-de-metal-digital.jpg" alt="" width="200" height="200">
                                </center>
                            </td>
                        </tr> 
                        
                    
                    </tbody>
                </table>        
            </form>
            
        </body>

        <script>
            var sliderr = document.getElementById("rinput");
            var outputr = document.getElementById("rvalue");
            outputr.innerHTML = sliderr.value;
            sliderr.oninput = function () {
                outputr.innerHTML = this.value;
            }

            var sliderg = document.getElementById("ginput");
            var outputg = document.getElementById("gvalue");
            outputg.innerHTML = sliderg.value;
            sliderg.oninput = function () {
                outputg.innerHTML = this.value;
            }
        </script>
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
        request = request.decode().split() 
        
        if 'rinput' in request[1]:
            r_value = request[1].split('&')[0].split('=')[1]
            g_value = request[1].split('&')[1].split('=')[1]
            
            print(r_value)
            print(g_value)
             
            #134 max
            #18 min
            rgb.red(int(int(r_value) * 1.1717 + 18)) 
            rgb.green(int(int(g_value) * 1.1717 + 18)) 
               
        
        response = web_page()        
        response = response.replace(" @@", "")
        conn.send('HTTP/1.1 200 OK\n')
        conn.send('Content-Type: text/html\n')
        conn.send('Connection: close\n\n')
        conn.sendall(response)
        conn.close()
    except Exception as e:
        print(e)
