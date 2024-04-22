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

struct sockaddr_in* createIPv4Address(char* ip,int port){
	struct sockaddr_in* address=malloc(sizeof(struct sockaddr_in));
	address->sin_port=htons(port);
	address->sin_family=AF_INET;
	inet_pton(AF_INET,ip,&address->sin_addr.s_addr);
	return address;
};

bool compare_address(struct sockaddr_in address1,struct sockaddr_in address2){
	return ((address1.sin_addr.s_addr==address2.sin_addr.s_addr) && (address1.sin_port==address2.sin_port));
}

int main(){
	struct sockaddr_in addresses[10];
	int idx=0;
	char buffer[1024];
	
	struct sockaddr_in* servaddr;
	struct sockaddr_in cliaddr,sending_addr;
		
	int sockfd=createUDPIPv4Socket(); 
		
	servaddr=createIPv4Address("172.16.14.52",8080);
	
	int b=bind(sockfd,(struct sockaddr*)servaddr,sizeof(struct sockaddr_in));
	if(b==0){
		printf("Bound Successfully\n");
		printf("Waiting for incoming messages\n");
	}else{
		printf("Binding Failed:%s",strerror(errno));
		return -1;
	}
	
	while(true){
		int len = sizeof(struct sockaddr_in);
		int n = recvfrom(sockfd,(char *)buffer,1024,MSG_WAITALL,(struct sockaddr*)&cliaddr,&len);
		//sending_addr=cliaddr;
		
		buffer[n] = '\0';
		bool flag=true;
		char* add=inet_ntoa(cliaddr.sin_addr);
		printf("Client : %s", buffer);
		printf("Address: %s Port:%d\n", add,cliaddr.sin_port);
		
		for(int j=0;j<idx;j++){
			if(compare_address(addresses[j],cliaddr)){
				flag=false;
			}
		}
		
		
		if(flag){
			addresses[idx]=cliaddr;
			idx+=1;
		}
		
		
		for(int k=0;k<idx;k++){
			if(!compare_address(addresses[k],cliaddr)){
				sending_addr=addresses[k];
				ssize_t t=sendto(sockfd,(const char *)buffer,strlen(buffer)-1,MSG_CONFIRM,(const struct sockaddr*)&sending_addr,len);
			}
		}
			
	}
		
	return 0;
}

