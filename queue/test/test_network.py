import socket
import threading 
import time 


def connect_ad_send(th_name: str, inp: list) ->None:     
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

 
 
def Main():
    thread1 = threading.Thread(target=connect_ad_send,  
                           args=("thread1", ['INSERT A 0 dog\n', 'INSERT A 1 rat\n', 'INSERT A 2 parrot\n', 'INSERT A 3 perl\n', 'INSERT A 4 zig\n'])) 
    thread2 = threading.Thread(target=connect_ad_send,  
                           args=("thread2", ['INSERT B 1 car', 'INSERT B 3 mouse', 'INSERT B 4 flame', 'INSERT B 5 graph', 'INSERT B 6 zig','INTERSECTION','SYMMETRIC_DIFFERENCE','TRUNCATE A','INTERSECTION'])) 
    thread1.start()
    thread2.start()

    thread1.join()
    thread2.join()
 
if __name__ == '__main__':
    Main()