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
#define TIMES 5
#define MCAST_INTERVAL 5
#define LEN 128

int main(int argc,char *argv[])
{
	int socketfd,ret = 0,i = 0;
	struct sockaddr_in localaddr;
	int len = sizeof(localaddr);
	char recvbuf[LEN] = {0};

	if(argc < 2){
		printf("command error!(eg:./client 8888)\n");
		return -1;
	}

	if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1){
		perror("socket error");
		return -1;
	}

	memset(&localaddr,0,len);
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(atoi(argv[1]));
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if((bind(socketfd,(struct sockaddr *)&localaddr,len)) < 0){
		perror("bind error");
		return -1;
	}

	for(i = 0;i < TIMES;i++){
		ret = recvfrom(socketfd,recvbuf,LEN,0,(struct sockaddr *)&localaddr,&len);
		if(ret < 0){
			perror("recvfrom error");
			return -1;
		}
		printf("Recv %dst message from server:%s\n",i,recvbuf);
		sleep(MCAST_INTERVAL);
	}	
	return 0;
}

