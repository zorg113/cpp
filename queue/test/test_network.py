import socket
import threading 
import time 


def connect_producer(th_name: str, inp: list) ->None:     
    host = '127.0.0.1' 
    port = 1990 
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)     
    s.connect((host,port))    
    print("Producer: connect host %s:%d"%(host,port))
    for message in inp:
        print(th_name,message)
        s.send(message.encode('ascii'))       
        data = s.recv(1024)
        print(f'Producer: Received from the server %s : %s'%(th_name,str(data.decode('ascii'))))
        time.sleep(1) 
             
    s.close()

def connect_consumer(th_name: str, inp: list) ->None:     
    host = '127.0.0.1' 
    port = 1992 
    out =[]
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)     
    s.connect((host,port))    
    print("\t Consumer: connect host %s:%d"%(host,port))
    for message in inp:
        print(f"\t %s %s"%(th_name,message))
        s.send(message.encode('ascii'))       
        data = s.recv(1024)
        out.append(str(data.decode('ascii')))
        print(f'\t Consumer: Received from the server %s : %s'%(th_name,str(data.decode('ascii'))))
        time.sleep(1) 
             
    s.close()
    if out==["kernel","org","data"]:
        print("PASSED")

 
def Main():
    thread1 = threading.Thread(target= connect_producer,  
                           args=("thread1", ["kernel","org","data"])) 
    thread2 = threading.Thread(target=connect_consumer,  
                           args=("thread2", ["get","get","get"])) 
    thread1.start()
    thread2.start()

    thread1.join()
    thread2.join()
    # connect_producer(1,["kernel","org","data"])
    # connect_consumer(1,["get","get","get"])
 
if __name__ == '__main__':
    Main()