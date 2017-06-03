#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



int main()
{
    char data_tobe_send [1024];
	char data_tobe_receive[1024];
	int true=1;	
	int fixed;
    int recieved_byte;
    int temp;		
		          
	struct sockaddr_in server_address,client_address;    
                
	int s = (socket(AF_INET, SOCK_STREAM, 0));
		
    if (s <= -1) {
        perror("Socket Error");
        exit(1);
    }

    if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int))<=-1) {
        perror("Setsockopt Error");
	    exit(1);
    }
        
    server_address.sin_family = AF_INET;         
    server_address.sin_port = htons(4000);     
    server_address.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_address.sin_zero),8); 

	temp=0;
    if (bind(s,(struct sockaddr *)&server_address, sizeof(struct sockaddr)) <= -1) {
        perror("Unable to bind");
		temp=1;
        exit(1);
    }

    if (listen(s, 4) <=-1) {
        perror("Listen");
        exit(1);
    }
		
	printf("\nTCPServer Waiting for client on port 4000");
    fflush(stdout);

    int socketsize;
    while(true)
    {  

        socketsize = sizeof(struct sockaddr_in);

        fixed = accept(s, (struct sockaddr *)&client_address,&socketsize);

        printf("\n Client Connected at : (%s)", inet_ntoa(client_address.sin_addr));

        while (true)
        {
            printf("\n SEND ('quit' to quit) : ");
            gets(data_tobe_send);
              
            if (strcmp(data_tobe_send , "quit") == 0)
            {
                temp=0;
				send(fixed, data_tobe_send,strlen(data_tobe_send), 0); 
                close(fixed);
                break;
            }
               
            else
                send(fixed, data_tobe_send,strlen(data_tobe_send), 0);  
                temp=1; 
              
			recieved_byte = recv(fixed,data_tobe_receive,1024,0);

            data_tobe_receive[recieved_byte] = '\0';

            if (strcmp(data_tobe_receive , "quit") == 0)
            {
				close(fixed);
                break;
            }

            else 
				printf("\n RECIEVED DATA = %s " , data_tobe_receive);
			fflush(stdout);
        }
    }       

    close(s);
    return 0;
} 