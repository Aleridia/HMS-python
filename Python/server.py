#coding: utf-8
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 2525))
s.listen(5)


# sometimes we can have multiple message on a line
def parse(data):
    data = data.split(';')
    return (data[0], data[1])

try:
    while True:
        (client, addr) = s.accept()
        print("someone connected to me")
        chunks = []
        read = 0
        
        while True:
            chunk = client.recv(128)
            if chunk == b'':
                break
            
            messages = chunk.decode('ascii')
            messages = messages.split('|')
            
            for message in messages[:-1]:
                (value, sensor) = parse(message[:-1])
                print(value, sensor)
                
except Exception as ex:
    print(ex)
finally:
    s.close()   