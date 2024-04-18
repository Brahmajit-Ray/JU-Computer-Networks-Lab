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

struct sockaddr_in* createIPv4Address(char*ip,int port){
	struct sockaddr_in* address=malloc(sizeof(struct sockaddr_in));
	address->sin_port=htons(port);
	address->sin_family=AF_INET;
	inet_pton(AF_INET,ip,&address->sin_addr.s_addr);
	//address.sin_addr.s_addr=INADDR_ANY;
	return address;
};

int main(){
	int port_numbers[10];
	int idx=0;
	char buffer[1024];
	
	struct sockaddr_in* servaddr;
	struct sockaddr_in cliaddr,sending_addr;
		
	int sockfd=createUDPIPv4Socket(); 
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
		
	servaddr=createIPv4Address("172.16.14.47",8080);
	
	int b=bind(sockfd,(struct sockaddr*)servaddr,sizeof(struct sockaddr_in));
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
		sending_addr=cliaddr;
		
		buffer[n] = '\0';
		bool flag=true;
		char* add=inet_ntoa(cliaddr.sin_addr);
		printf("Client : %s", buffer);
		printf("Address: %s Port:%d\n", add,cliaddr.sin_port);
		
		for(int j=0;j<idx;j++){
			if(port_numbers[j]==cliaddr.sin_port){
				flag=false;
			}
		}
		
		
		if(flag){
			port_numbers[idx]=cliaddr.sin_port;
			idx+=1;
		}
		
		
		for(int k=0;k<idx;k++){
			if(port_numbers[k]!=cliaddr.sin_port){
				sending_addr.sin_port=port_numbers[k];
				ssize_t t=sendto(sockfd,(const char *)buffer, strlen(buffer)-1,MSG_CONFIRM,(const struct sockaddr*)&sending_addr,len);
			}
		}
			
	}
		
	return 0;
}

