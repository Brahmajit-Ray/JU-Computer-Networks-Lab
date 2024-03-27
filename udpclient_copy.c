#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
	
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

struct args{
	int sockfd;
	struct sockaddr_in servaddr;
};

void receive(struct args arg){
	int sockfd=arg.sockfd;
	struct sockaddr_in servaddr=arg.servaddr;
	int len=sizeof(servaddr);
	char buffer[1024];
	while(true){
		ssize_t n = recvfrom(sockfd,(char*)buffer,1024,MSG_WAITALL,(struct sockaddr*)&servaddr,&len);
		bool flag=false;
		if(n==-1){
			printf("%s",strerror(errno));
			break;
		}
		if(n>=0){
			buffer[n] = '\0';
			printf("Server:%s\n", buffer);
			flag=true;
			memset(buffer,0,strlen(buffer));
		}
		if(flag){
			printf("Hooray\n");
		}
	}
}

void thread_receive(struct args arg){
	pthread_t id;
	pthread_create(&id,NULL,(void*)&receive,(void*)&arg);
}


int main() {
	char buffer[1024];
	char *msg;
	struct sockaddr_in servaddr;
	
	int sockfd=createUDPIPv4Socket();
	
	memset(&servaddr,0,sizeof(servaddr));
		
	servaddr=createIPv4Address(8080);
	
	struct args arg;
	arg.sockfd=sockfd;
	arg.servaddr=servaddr;
		
	int n, len;
	size_t linesize=0;
	printf("Type to send\n");
	
	//thread_receive(arg);
	
	while(true){
		ssize_t charCount=getline(&msg,&linesize,stdin);
		if(charCount>0){
			if(strcmp(msg,"exit\n")==0){
				close(sockfd);
				return 0;
			}else{
		sendto(sockfd,(const char*)msg,strlen(msg),MSG_CONFIRM,(const struct sockaddr*)&servaddr,sizeof(servaddr));
			}
		}
	}
	
	//printf("Hello message sent.\n");
			
	//n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
	//buffer[n] = '\0';
	//printf("Server : %s\n", buffer);
	
	//close(sockfd);
	//return 0;
}

