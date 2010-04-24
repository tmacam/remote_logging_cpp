#!/usr/bin/python
# -*- coding: utf-8 -*-

from socket import socket, AF_INET, SOCK_DGRAM
from optparse import OptionParser
from time import time

def create_udp_server_socket(endpoint):
    skt = socket(AF_INET, SOCK_DGRAM)
    skt.bind(endpoint)
    return skt

def MakeOptionParser():
    parser = OptionParser()
    parser.add_option('-a', '--append', dest='append', metavar="FILE",
            help="Append log data to this file")
    return parser

if __name__ == '__main__':
    parser = MakeOptionParser()
    options, args = parser.parse_args()
    append_file = None
    if options.append:
        append_file = open(options.append, 'a', 0)

    ENDPOINT = ("", 3000)       # empty string == INADDR_ANY
    skt = create_udp_server_socket(ENDPOINT)

    while True:
        data, (host, port) = skt.recvfrom(1500)
        log_line = "(%s:%i) %15.2f %s\n" % (host, port, time(), data)
        print log_line,
        if append_file:
            append_file.write(log_line)

