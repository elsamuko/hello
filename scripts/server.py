#!/usr/bin/env python3
# http://mfukar.github.io/2013/10/31/HTTPS.html

import http.server, ssl

print("https://localhost:4443")

httpd = http.server.HTTPServer(("localhost", 4443), http.server.SimpleHTTPRequestHandler)
httpd.socket = ssl.wrap_socket(httpd.socket,
                               server_side=True,
                               certfile="certificate.pem")
httpd.serve_forever()
