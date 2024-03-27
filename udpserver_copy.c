#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
	
int createUDPIPv4Socket(){
	return socket(AF_INET,SOCK_DGRAM,0);
}

struct sockaddr_in createIPv4Address(int port){
	struct sockaddr_in address;
	address.sin_port=htons(port);
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=INADDR_ANY;
	return address;
};

/*
struct args{
	int sockfd;
	char** clients;
	char* sender;
};

void broadcast(struct args arg){
	int sockfd=arg.sockfd;
	char** clients=arg.clients;
	char* sender=arg.sender;
	
	char buffer[1024];
}*/

int main(){
	char* clients[10];
	int idx=0;
	char buffer[1024];
	
	struct sockaddr_in servaddr, cliaddr;
		
	int sockfd=createUDPIPv4Socket(); 
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
		
	servaddr=createIPv4Address(8080);
	
	int b=bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(b==0){
		printf("Bound Successfully\n");
		printf("Waiting for incoming messages\n");
	}else{
		printf("Binding Failed:%s",strerror(errno));
		return -1;
	}
	
	while(true){
		int len = sizeof(cliaddr);
		int n = recvfrom(sockfd,(char *)buffer,1024,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
	
		buffer[n] = '\0';
		bool flag=true;
		char* add=inet_ntoa(cliaddr.sin_addr);
		printf("Client : %s", buffer);
		printf("Address: %s\n", add);
		
		for(int j=0;j<idx;j++){
			if(strcmp(clients[j],add)==0){
				flag=false;
			}
		}
		
		
		if(flag){
			clients[idx]=inet_ntoa(cliaddr.sin_addr);
			idx+=1;
		}
		
		ssize_t t=sendto(sockfd,(const char *)buffer, strlen(buffer),MSG_CONFIRM,(const struct sockaddr*) &cliaddr,len);
		
		if(t>0){
			printf("Hello message sent...%ld\n",t);
		}
	}
		
	return 0;
}

