import threading
import socket

host="127.0.0.1"
port=40000

server=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server.bind((host,port))
server.listen()

clients=[]
usernames=[]

def broadcast(msg):
	for client in clients:
		client.send(msg)
		
		
def handle(client):
	while True:
		try:
			msg=client.recv(1024)
			broadcast(msg)
		except:
			index=clients.index(client)
			clients.remove(client)
			client.close()
			name=usernames[index]
			broadcast(f"{name} left the chat",encode("ascii"))
			usernames.remove(name)
			break


def receive():
	while True:
		client,address=server.accept();
		print(f"Connected with {address}")
		
		client.send("Username".encode("ascii"));
		name=client.recv(1024).decode("ascii")
		clients.append(client)
		usernames.append(name)
		
		print(f"Name of new client is {name}")
		broadcast(f"{name} joined the chat".encode("ascii"))
		client.send("Connected to the server".encode("ascii"))
		
		thread=threading.Thread(target=handle,args=(client,))
		thread.start()

print("Server is on...")		
receive()
		
		
		
		
			


