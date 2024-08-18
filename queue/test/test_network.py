import socket
import threading 
import time 


def connect_producer(th_name: str, inp: list) ->None:     
    host = '127.0.0.1' 
    port = 1990 
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)     
    s.connect((host,port))    
    for message in inp:
        print(th_name,message)
        s.send(message.encode('ascii'))       
        data = s.recv(1024)
        print(f'Received from the server %s : %s'%(th_name,str(data.decode('ascii'))))
        time.sleep(1) 
             
    s.close()

def connect_consumer(th_name: str, inp: list) ->None:     
    host = '127.0.0.1' 
    port = 1992 
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)     
    s.connect((host,port))    
    for message in inp:
        print(th_name,message)
        s.send(message.encode('ascii'))       
        data = s.recv(1024)
        print(f'Received from the server %s : %s'%(th_name,str(data.decode('ascii'))))
        time.sleep(1) 
             
    s.close()
 
 
def Main():
    connect_producer(1,["kernel","org","data"])
    connect_consumer(1,["get","get","get"])
 
if __name__ == '__main__':
    Main()