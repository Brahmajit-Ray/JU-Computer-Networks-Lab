#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

int createTCPIPv4Socket(){
	return socket(AF_INET,SOCK_STREAM,0);
}

struct sockaddr_in* createIPv4Address(char* ip,int port){
	struct sockaddr_in* address=malloc(sizeof(struct sockaddr_in));
	address->sin_port=htons(port);
	address->sin_family=AF_INET;
	inet_pton(AF_INET,ip,&address->sin_addr.s_addr);
	return address;
}

struct AcceptedSocket{
	int AcceptedSocketFD;
	struct sockaddr_in address;
	int error;
	bool success;
};

struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount=0;


struct AcceptedSocket* AcceptIncomingFunction(int socketServerFD){
	struct sockaddr_in clientAddress;
	int clientAddressSize=sizeof(struct sockaddr_in);
	int clientSocketFD=accept(socketServerFD,(struct sockaddr*)&clientAddress,&clientAddressSize);
	
	struct AcceptedSocket* acceptedSocket=malloc(sizeof(struct AcceptedSocket));
	acceptedSocket->address=clientAddress;
	acceptedSocket->AcceptedSocketFD=clientSocketFD;
	acceptedSocket->success=clientSocketFD>0;
	
	if(!acceptedSocket){
		acceptedSocket->error=clientSocketFD;
	}
	
	return acceptedSocket;
}

void sendReceivedData(char* buffer,int socketFD){
	for(int i=0;i<acceptedSocketsCount;i++){
		if(acceptedSockets[i].AcceptedSocketFD!=socketFD){
			send(acceptedSockets[i].AcceptedSocketFD, buffer,strlen(buffer),0);
		}
	}

}

void ReceiveAndPrintData(int socketFD){
	char buffer[1024];
	
	while(true){
		ssize_t amtrecv=recv(socketFD,buffer,1024,0);
		
		if(amtrecv>0){
			buffer[amtrecv]=0;
			printf("Response:%s",buffer);
			
			sendReceivedData(buffer,socketFD);
		}
		
		if(amtrecv==0){
			break;
		}
	}
	
	close(socketFD);
}



void ReceiveAndPrintOnSeparateThread(struct AcceptedSocket* clientSocket){
	pthread_t id;
	pthread_create(&id,NULL,(void *)&ReceiveAndPrintData,(void *)(long)clientSocket->AcceptedSocketFD);
}

void acceptNewConnectionsAndPrint(int SocketServerFD){
	while(true){
		struct AcceptedSocket* clientSocket=AcceptIncomingFunction(SocketServerFD);
		
		ReceiveAndPrintOnSeparateThread(clientSocket);
	}
}

void startAcceptingConnections(int SocketServerFD){
	while(true){
		struct AcceptedSocket* clientSocket=AcceptIncomingFunction(SocketServerFD);
		acceptedSockets[acceptedSocketsCount++]=*clientSocket;
		
		ReceiveAndPrintOnSeparateThread(clientSocket);
	}
	
}



int main(){
	int SocketServerFD=createTCPIPv4Socket();
	
	struct sockaddr_in* serverAddress=createIPv4Address("127.0.0.1",2000);
	
	int res=bind(SocketServerFD,(struct sockaddr*)serverAddress,sizeof(struct sockaddr_in));
	
	if(res==0){
		printf("Bound successfully\n");
	}
	
	listen(SocketServerFD,10);
	
	startAcceptingConnections(SocketServerFD);

	shutdown(SocketServerFD,SHUT_RDWR);
	
	return 0;
}
