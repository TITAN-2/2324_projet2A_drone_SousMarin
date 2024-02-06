# /myflaskapp/streaming/server.py

from http import server
import socketserver
from streaming.camera import StreamingOutput

class StreamingHandler(server.BaseHTTPRequestHandler):
    def do_GET(self):
        # Gérer les requêtes GET
        pass

class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True
