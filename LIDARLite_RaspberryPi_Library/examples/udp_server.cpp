// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>   
#include <linux/types.h>
#include <cstdio>
#include <unistd.h>
#include <include/lidarlite_v3.h>

#define PORT     5800
#define MAXLINE 1024

LIDARLite_v3 myLidarLite;

int lidar_init()
{
	myLidarLite.i2c_init();
	myLidarLite.configure(0);
	return 1;
}

int lidar_read()
{
	__u16 distance;

//	myLidarLite.i2c_init();
	
//	myLidarLite.configure(0);

//	while(1)
//	{

		myLidarLite.takeRange();
		distance = myLidarLite.readDistance();
		printf("%4d\n", distance);
		return distance;
//	}
}

   
static void
broadcast(int socket, const char *mess)
{
    struct sockaddr_in s;

    memset(&s, '\0', sizeof(struct sockaddr_in));
    s.sin_family = AF_INET;
    s.sin_port = (in_port_t)htons(PORT);
    s.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    if(sendto(socket, mess, strlen(mess), 0, (struct sockaddr *)&s, sizeof(struct sockaddr_in)) < 0)
        perror("sendto");
}

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    std::string msg;       

    // open a socket
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // enable broadcast
    int broadcastEnable = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, 
		&broadcastEnable, sizeof(broadcastEnable)) < 0) {
        perror("bcast setting failed");
    }

    printf("test\n");
    
    lidar_init();

    while(1)
    {
		msg = std::to_string(lidar_read());
		char const *c_msg = msg.c_str();    	
        broadcast(sockfd, c_msg);
    }

/*
    for(int i = 0; i <100000000; i++) { 
       broadcast(sockfd, "test");   
    }
*/

    return 0;
}
