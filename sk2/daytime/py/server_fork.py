#!/usr/bin/env python
__author__ = "Dariusz Dwornikowski"

"""
Works with python > 2.7

"""
import socket
import sys
import os
import time

def main(port=1234, hostname=''):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind( (hostname, port) )
    sock.listen(1)
    while True:
        con, addr = sock.accept()
        pid = os.fork()
        if not pid:
            print("Child %d started" % os.getpid())
            print('Connected by', addr)
            localtime = time.asctime( time.localtime( time.time() ) )
            con.sendall( localtime.encode() )
            con.close()
            print("Child %d exiting" % os.getpid())
            sys.exit(0)
    sock.close()

if __name__ == "__main__":
   main()
