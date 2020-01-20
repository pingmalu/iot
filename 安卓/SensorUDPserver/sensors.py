import socket
from struct import *

# Prepare the UDP connection
UDP_IP = "0.0.0.0"
print "Receiver IP: ", UDP_IP
UDP_PORT = 9999
print "Port: ", UDP_PORT
sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(2048)
    print 'Received from %s:%s.' % addr
    print 'get: %s' %data
    sock.sendto('Hello, %s!' % data, addr)

# Continuously read from the UDP socket
# while True:
    # print "Waiting for data..."
    # data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    # print "received: " + data
    # print addr
    # print "%7.4f" %unpack_from ('!f', data, 0)
    # print "%f" %data.read(f,4)
    # print "%s" %unpack('!f',data[0])
    # continue
    # The next line just chops a lot of floats from the received data chunk
    # Just activate and deactivate sensors in the S ensor UDP app to locate
    # the values you want
    # print "received message: ",\
    #     "%7.4f" %unpack_from ('!f', data, 0),\
    #     "%7.4f" %unpack_from ('!f', data, 4),\
    #     "%7.4f" %unpack_from ('!f', data, 8),\
    #     "%7.4f" %unpack_from ('!f', data, 12),\
    #     "%7.4f" %unpack_from ('!f', data, 16),\
    #     "%7.4f" %unpack_from ('!f', data, 20),\
    #     "%7.4f" %unpack_from ('!f', data, 24),\
    #     "%7.4f" %unpack_from ('!f', data, 28),\
    #     "%7.4f" %unpack_from ('!f', data, 32),\
    #     "%7.4f" %unpack_from ('!f', data, 36),\
    #     "%7.4f" %unpack_from ('!f', data, 40),\
    #     "%7.4f" %unpack_from ('!f', data, 44),\
    #     "%7.4f" %unpack_from ('!f', data, 48),\
    #     "%7.4f" %unpack_from ('!f', data, 52),\
    #     "%7.4f" %unpack_from ('!f', data, 56),\
    #     "%7.4f" %unpack_from ('!f', data, 60),\
    #     "%7.4f" %unpack_from ('!f', data, 64),\
    #     "%7.4f" %unpack_from ('!f', data, 68),\
    #     "%7.4f" %unpack_from ('!f', data, 72),\
    #     "%7.4f" %unpack_from ('!f', data, 76),\
    #     "%7.4f" %unpack_from ('!f', data, 80),\
    #     "%7.4f" %unpack_from ('!f', data, 84),\
    #     "%7.4f" %unpack_from ('!f', data, 88),\
    #     "%7.4f" %unpack_from ('!f', data, 92)