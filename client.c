#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define true 1

int main()

{
    char data_tobe_send[1024];
	char data_tobe_receive[1024];
	int recieved_byte;  
        
    struct hostent *host;
    struct sockaddr_in server_address;  

    host = gethostbyname("127.0.0.1");

	int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s <= -1) {
        perror("Socket Error");
		exit(1);
    }

    server_address.sin_family = AF_INET;     
    server_address.sin_port = htons(4000);   
    server_address.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_address.sin_zero),8); 

    if (connect(s, (struct sockaddr *)&server_address,sizeof(struct sockaddr)) <= -1) 
    {
        perror("Connect Error");
        exit(1);
    }

    while(true)
    {
        
        recieved_byte=recv(s,data_tobe_receive,1024,0);
        data_tobe_receive[recieved_byte] = '\0';
 
        if (strcmp(data_tobe_receive, "quit") != 0)
        {
			printf("\nRecieved data:= %s " , data_tobe_receive);  
        }

        else{
			close(s);
			break;
		}
        printf("\nSEND ('quit' to quit) : ");
        gets(data_tobe_send);
           
        if (strcmp(data_tobe_send, "quit") == 0){
			send(s,data_tobe_send,strlen(data_tobe_send), 0); 
			close(s);
			break;
		}
        else
        {
			send(s,data_tobe_send,strlen(data_tobe_send), 0);   
        }
        
    }   
	return 0;
}