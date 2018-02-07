# simple-webserver-c
This is two simple webservers written in C.

The TCP server runs on localhost and can parse html get request and send data back to the browser. It works as a webserver should.

The UDP server & client is a timeserver which sends the time back when you send an empty datagram. It follows the RFC868 time protocol. 


To compile the TCP server use 'gcc -Wall -o TCPserver TCPserver.c' and then to run it './TCPserver'


To compile the UDP server use 'gcc -Wall -o udpserver UDPserver.c' and then to run it './udpserver'

To compile the UDP client use 'gcc -Wall -o udpclient UDPklient.c' and then to run it './udpklient'
