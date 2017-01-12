#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

pthread_t tid[2];
pthread_mutex_t lock;
int sockfd = -1;

void error(char *msg)
{
    perror(msg);
    exit(0);
}
void ExitHandler()
{
	char input[1000]="exit";
	int len= strlen(input);

	if (sockfd != -1)
	{
		write(sockfd, &len, sizeof(int));
		write(sockfd, input, len);
	}
	exit(0);	
}
void* inputer(void* args){
	
	char buffer[256];
	int n = -1;

	while(1)
	{

		sleep(2);
		pthread_mutex_lock(&lock);
		bzero(buffer,256);
		fgets(buffer,255,stdin);

		if((strcmp(buffer,"exit\n")) == 0){
			write(sockfd,buffer,strlen(buffer));
			exit(0);

			break;
		}
		n = write(sockfd,buffer,strlen(buffer));

		if(n <0){
			error("Didn't Write");
		}

		pthread_mutex_unlock(&lock);
	}

	return NULL;

}


void* outputer(void* args){
	char buffer[256];
	int n = -1;

	while(1)
	{
		

		pthread_mutex_lock(&lock);
		bzero(buffer,256);

		n = read(sockfd,buffer,255);

		if(n<=0)
		{
			printf("Lost Connection to the server :/\n");
			exit(0);
		}

		if(strlen(buffer)>1){
			printf("%s\n",buffer);
		}

		pthread_mutex_unlock(&lock);
	}

	return NULL;
}

int main(int argc, char *argv[])
{

while(1)
	{ 
		printf("Attempting to connect to the server...\n");
		sleep(3);

	// Declare initial vars
    //int sockfd = -1;																// file descriptor for our socket
	int portno = -1;																// server port to connect to
	int n = -1;																		// utility variable - for monitoring reading/writing from/to the socket
	char buffer[256];															// char array to store data going to and coming from the server
    struct sockaddr_in serverAddressInfo;						// Super-special secret C struct that holds address info for building our socket
    struct hostent *serverIPAddress;									// Super-special secret C struct that holds info about a machine's address
    

	
	// If the user didn't enter enough arguments, complain and exit
    if (argc < 3)
	{
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	
	
	
	/** If the user gave enough arguments, try to use them to get a port number and address **/

	// convert the text representation of the port number given by the user to an int
	portno = atoi(argv[2]);
	
	// look up the IP address that matches up with the name given - the name given might
	//    BE an IP address, which is fine, and store it in the 'serverIPAddress' struct
    serverIPAddress = gethostbyname(argv[1]);
    if (serverIPAddress == NULL)
	{
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
				
	// try to build a socket .. if it doesn't work, complain and exit
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
	{
        error("ERROR creating socket");
	}


	
	/** We now have the IP address and port to connect to on the server, we have to get    **/
	/**   that information into C's special address struct for connecting sockets                     **/

	// zero out the socket address info struct .. always initialize!
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set a flag to indicate the type of network address we'll be using 
    serverAddressInfo.sin_family = AF_INET;
	
	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(portno);

	// do a raw copy of the bytes that represent the server's IP address in 
	//   the 'serverIPAddress' struct into our serverIPAddressInfo struct
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);



	/** We now have a blank socket and a fully parameterized address info struct .. time to connect **/
	
	// try to connect to the server using our blank socket and the address info struct 
	//   if it doesn't work, complain and exit
	
		//sleep(3);
    	if (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0) 
		{
       	 printf("ERROR connecting\n");
       	 //free(sockfd);
		}
		else
		{
			break;
		}
	}	
	
	
	
	/** If we're here, we're connected to the server .. w00t!  **/
	printf("WELCOME TO OUR BANK!\n");
	//printf("Type in Open or Start followed by account name\n");
	printf("Please press enter\n");
	pthread_create(&(tid[0]),NULL,&inputer,NULL);
	pthread_create(&(tid[1]),NULL,&outputer,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	pthread_mutex_destroy(&lock);
	pthread_exit(NULL);

    return 0;
}

