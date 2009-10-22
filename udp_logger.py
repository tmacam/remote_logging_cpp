#!/usr/bin/python
# -*- coding: utf-8 -*-

from socket import socket, AF_INET, SOCK_DGRAM

def create_udp_server_socket(endpoint):
    skt = socket(AF_INET, SOCK_DGRAM)
    skt.bind(endpoint)
    return skt

if __name__ == '__main__':
    ENDPOINT = ("", 3000)       # empty string == INADDR_ANY
    skt = create_udp_server_socket(ENDPOINT)
    while True:
        data, (host, port) = skt.recvfrom(1500)
        print "(%s:%i) %s" % (host, port, data)

