#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>

#define MCAST_DATA "BROADCAST TEST DATA"
#define BCASE_IP "255.255.255.255"
#define MCAST_INTERVAL

int main(int argc,char *argv[])
{
	int socketfd,ret = 0;
	struct sockaddr_in saddr;
	int len = sizeof(saddr);

	if(argc < 2){
		printf("command error!(eg:./server 8888)\n");
		return -1;
	}
	if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1){
		perror("socket fail");
		return -1;
	}

	memset(&saddr,0,len);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = inet_addr(BCASE_IP);
	
	int optval = 1;
	setsockopt(socketfd,SOL_SOCKET,SO_BROADCAST | SO_REUSEADDR,&optval,sizeof(int));
	while(1){
		ret = sendto(socketfd,MCAST_DATA,sizeof(MCAST_DATA),0,(struct sockaddr *)&saddr,len);
		if(ret < 0){
			perror("sendto error!");
			return -1;
		}
	}
}
