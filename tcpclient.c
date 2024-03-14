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
};

void listenAndPrint(int socketFD){
	char buffer[1024];
	
	while(true){
		ssize_t amtrecv=recv(socketFD,buffer,1024,0);
		
		if(amtrecv>0){
			buffer[amtrecv]=0;
			printf("Response:%s",buffer);
			
		}
		
		if(amtrecv==0){
			break;
		}
	}
	
	close(socketFD);
}

void listenAndPrintOnSeparateThread(int socketFD){
	pthread_t id;
	pthread_create(&id,NULL,(void *)&listenAndPrint,(void *)(long)socketFD);
}



int main(){
	int socketFD= createTCPIPv4Socket();
	
	struct sockaddr_in* address=createIPv4Address("127.0.0.1",2000);
	
	int result=connect(socketFD,(struct sockaddr*)address,sizeof(struct sockaddr_in));
	
	if(result==0){
		printf("Success\n");
	}	
	
	/*char* name=NULL;
	size_t nameSize=0;
	printf("Please enter your name:\n");
	ssize_t nameCount=getline(&name,&nameSize,stdin);
	name[nameCount-1]=0;*/
	 
	
	char* line=NULL;
	size_t lineSize=0;
	printf("Type to send\n");
	
	listenAndPrintOnSeparateThread(socketFD);
	
	//char buffer[1024];
	
	while(true){
		
		ssize_t charCount=getline(&line,&lineSize,stdin);
		//line[charCount-1]=0;
		
		//sprintf(buffer,"%s:%s",name,line);
		
		if(charCount>0){
			if(strcmp(line,"exit\n")==0){
				break;
			}
			
			ssize_t amtsend=send(socketFD,line,charCount,0);
			
		}
	}
	
	
	
	return 0;	
}
