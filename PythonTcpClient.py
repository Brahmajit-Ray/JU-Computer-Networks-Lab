import threading
import socket

host="127.0.0.1"
port=40000

username=input("Choose username")

client=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
client.connect((host,port))

def receive():
	while True:
		try:
			msg=client.recv(1024).decode("ascii")
			if msg=="Username":
				client.send(username.encode("ascii"))
			
			else:
				print(msg)
		except:
			print("An error occured")
			client.close()
			break
			
def write():
	while True:
		msg=input()
		if msg=="exit":
			client.shutdown(socket.SHUT_RDWR)
			client.close()
		msg=f"{username}:{msg}"
		client.send(msg.encode("ascii"))
		
receive_thread=threading.Thread(target=receive)
receive_thread.start()

write_thread=threading.Thread(target=write)
write_thread.start()
