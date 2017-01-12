/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include "Asst4.h"
#include <semaphore.h>
int c = 0;
pthread_t 				thread;
pthread_t				lThread;
pthread_mutex_t 		addLock;
pthread_mutex_t 		startLock;
sem_t 					semaphore;

void error(char *msg)
{
    perror(msg);
    exit(1);
}
int startSession(char* accountName, int sock)
{
	char message[256];
	int i=0;
	// read(sock, accoutName, 100);
	for(i = 0; i < 20 && AccountList[i] != NULL; i++)
	{
		if(strcmp(AccountList[i]->accountName, accountName) == 0)
		{
			if(AccountList[i]->inUse == 1)
			{
				strcpy(message, "Account already in use\n");
				write(sock, message, strlen(message));
				strcpy(message, "Please press enter\n");
				write(sock, message, strlen(message));
				return -1;
			}
			AccountList[i]->inUse = 1;
			 return i;
		}
	}
	
	
		return -5;
	
}
void openAccount(char * currAccountName, int sock)
{
	char message[256];
	int i;
	for(i = 0; i <20; i++)
	{
		if(AccountList[i] !=NULL)
		{
			if(strcmp(AccountList[i]->accountName, currAccountName) == 0)
			{

				strcpy(message, "Invalid Command\n");
				write(sock, message, strlen(message));
				strcpy(message, "Please press enter\n");
				write(sock, message, strlen(message));
				return;
			}
		}
		if(AccountList[i] == NULL)
		{
			account *currAccount = malloc(sizeof(account));
			strcpy(currAccount->accountName,currAccountName);
			currAccount->balance = 0;
			currAccount->inUse = 0;
			AccountList[i] = currAccount;
			strcpy(message, "New account with the name ");
			write(sock, message, strlen(message));
			strcpy(message,currAccountName);
			write(sock, message, strlen(message));
			strcpy(message, " was opened\n");
			write(sock, message, strlen(message));
			strcpy(message, "Please press enter\n");
			write(sock, message, strlen(message));
			return;
		}
		
	}
	strcpy(message, "Not enough space in bank\n");
	write(sock, message, strlen(message));
	return;
}
void getNextCommand(account *buster,int sock) 
{
	float newAmount;
	char message[256];
	char command[11];
	char ourMessage[200];
	char str[1000]; 
	int read_size;
	float amount;
	while(1)
	{
		strcpy(ourMessage, "Enter 'debit' or 'credit' followed by amount or 'balance', or 'finish'\n");
		write(sock, ourMessage, strlen(ourMessage));
		read_size = read(sock , message , 255);// > 0 );
		sscanf(message, "%s %f", command, &amount);
		tolower(command);
		if(strcmp(command, "credit") == 0)
		{
			strcpy(ourMessage, "In Credit\n");
			write(sock, ourMessage, strlen(ourMessage));
			buster->balance += amount;
			newAmount = buster->balance;
			strcpy(message, "Credit successful. Amount in account is ");

			write(sock , message , strlen(message));
			snprintf(message, 50, "%.2f", buster->balance);

			write(sock, message, strlen(message));
			strcpy(message, "\n");
			write(sock, message, strlen(message));

		}

		if(strcmp(command, "debit")== 0)
		{
			strcpy(ourMessage, "Made it to debit\n");
			write(sock, ourMessage, strlen(ourMessage));
			if(buster->balance >= amount)
			{
				buster->balance -= amount;
				newAmount = buster->balance;
				strcpy(message, "Debit successful. Amount in account is ");

				write(sock , message , strlen(message));
				snprintf(message, 50, "%.2f", buster->balance);

				write(sock, message, strlen(message));
				strcpy(message, "\n");
				write(sock, message, strlen(message));

			}
			else
			{
				newAmount = amount;
				strcpy(message, "You do not have enough money to withdraw\n");

				write(sock , message , strlen(message));

			}
		}
		if(strcmp(command, "balance") == 0)
		{
			strcpy(ourMessage, "In balance\n");
			write(sock, ourMessage, strlen(ourMessage));
			newAmount = buster->balance;
			strcpy(message, "Accounts current balance ");
			write(sock, message, strlen(message));
			// write(sock, &balance, )
			snprintf(message, 50, "%.2f", buster->balance);
				//strcpy(message, ("%f", (char*)buster->balance));
				write(sock, message, strlen(message));
				strcpy(message, "\n");
				write(sock, message, strlen(message));

		}
		if(strcmp(command, "finish") == 0)
		{
			buster->inUse = 0;
			strcpy(message, "Please press enter\n");
			write(sock, message, strlen(message));
			return;
		}
		if(strcmp(command, "exit") ==0)
		{

			printf("WE MADE IT\n");
					
			break;
		}
	}
}
void printEveryTwenty(void *socket_desc)
{
	int sock = *(int*)socket_desc;
	char client_message[256];
	float accBal;
     char accountName[100];
     char message[500];
     char ourMessage[300];
     int i = 0;
	 //char command[10];  
	 for(i = 0; i < 20 && AccountList[i] != NULL; i++)
	 {
	 	strcpy(client_message, "IN SERVICE ");
	 	write(sock, client_message, strlen(client_message));
	 	strcpy(accountName,AccountList[i]->accountName);
	 	write(sock, accountName, strlen(accountName));
	 	strcpy(message, "BALANCE: ");
	 	write(sock, message, strlen(message));
	 	// accBal = AccountList[i]->balance;
	 	// write(sock, accBal, strlen(accBal));
	 }   
}
void alarmHandler()
{
	sem_post(&semaphore);
}
void *printAccounts(void *emptyPtr)
{
	while (1)
	{
		//c = 0;
		sleep(20);
		//c = 1;
		//signal(SIGALRM, alarmHandler);
		//alarm(20);
		//sem_wait(&semaphore);
		
		printf("ACCOUNT LIST\n");
		int i=0;
		printf("---------------------------------------\n");
		for (i=0;AccountList[i]!=NULL;i++)
		{
			printf("%s\n", AccountList[i]->accountName);
			printf("        %.2f\n", AccountList[i]->balance);
			(AccountList[i]->inUse==1) ? printf("        IN SESSION\n"):printf("        NOT IN SESSION\n");
		}
		
		printf("----------------------------------------\n");
		
		
	}
	pthread_exit(0);
}

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
      int sock = *(int*)socket_desc;
      int read_size; 
      char client_message[256];
      char accountName[100];
      char accountName2[100];
      char message[500];
      char ourMessage[300];
	  char command[10];              
	  char command2[10]; 
	  int same = 0;
	  int i = 0;
	  int opening = 0;
     //  Send some messages to the client
    	// message = "Greetings! I am your connection handler\n";
     //  write(sock , message , strlen(message));
                                     
    	// message = "Now type something and i shall repeat what you type \n";
     //  write(sock , message , strlen(message));
                                                   
      //Receive a message from client
      // while(1)
      while( (read_size = read(sock , client_message , 255)) > 0 )
      { 

			strcpy(ourMessage, "Type in 'open' or 'start' followed by account name or 'exit' to quit\n");
			write(sock , ourMessage, strlen(ourMessage));
			//read_size = read(sock , client_message , 255);
/* THIS IS A HACK WAY TO TAKE IN INPUT */
      			int k=0;
      			int j = 0;

			read_size = read(sock , client_message , 255);
			int z=0;
			for(j = 0; j < 99; j++)
			{
				accountName[j] = '\0';
			}
				
			for(i =0; i<strlen(client_message); i++)
				{
					//printf("%c\n",client_message[i]);
					if(k==0)
					{	
						command[i]=client_message[i];
						//z++;
					}
					
					if(client_message[i] == ' ')
					{	
							//printf("Command = %s\n", command);
						command[i]='\0';
							
							//i++;
							//printf("herererere i = %d\n",i);
						k=1;
						//	z++;
					}
					else if(k==1)
					{
						if(client_message[i] == ' ')
						{
							break;
						}
							//printf("got here\n");
						accountName[z]=client_message[i];
						z++;

					}
					else if(client_message[i] == '\n')
					{
							//printf("herererererereadjlfa;slkdfalsekfj;alsdkfj\n");
						break;
					}

				}
				if(strcmp(command, "exit") == 0)
				{
								//printf("GOT HERE \n");
								// close(socket_desc);
					break;
				}

				accountName[z]='\0';
				k = 0;
					//printf("accountName =  %s and command = %s asdfdf\n", accountName, command);
				for(j = 0; j < 99; j++)
				{
					if(accountName[j] == '\n')
					{
						accountName[j] = '\0';
					}	
				}
			
				/*if(k == 0)
				{
					//strcpy(client_message, );
					
				}*/

				if((strcmp(command,"open") == 0) )//&& isalpha(accountName[0]) ) 
				{
				/*	if(c == 1)
					{
						//printf();
						strcpy(ourMessage, "Printing, please wait...\n");
						write(sock, ourMessage, strlen(ourMessage));
						sleep(5);
						strcpy(ourMessage, "opening that account now\n");
						write(sock, ourMessage, strlen(ourMessage));
					}
*/
					strcpy(ourMessage, "Opening Account...\n");

					write(sock, ourMessage, strlen(ourMessage));
					if(opening == 0)
					{
						opening = 1;
						openAccount(accountName, sock);
						opening = 0; 
					}
					else
					{
						strcpy(ourMessage, "Wait one moment, another account is opeing\n");
						write(sock, ourMessage, strlen(ourMessage));
						sleep(2);
						opening = 1;
						openAccount(accountName, sock); 
						opening = 0;
					}

					
				}
				else if(strcmp(command,"start") == 0)
				{
					strcpy(ourMessage, "Starting Sessiong...\n");
					write(sock, ourMessage, strlen(ourMessage));
					int i =startSession(accountName, sock);
					if( i== -5)
					{
						strcpy(message, "Account Not Found\n");
						write(sock, message, strlen(message));
						for(i = 0; i < 99; i++)
						{
				 			accountName[i] = 0;
						}
					}
					else if(i == -1)
					{
						strcpy(ourMessage, "Account currently being used...\n");
					}
					else
					{
						strcpy(ourMessage,("Starting session with account\n"));
						
						write(sock, ourMessage, strlen(ourMessage));
						getNextCommand(AccountList[i], sock);
						memset(accountName,0,strlen(accountName));
					}
				}
				// else if(strcmp(command,"exit") == 0)
				// {
				// 	strcpy(ourMessage, "Made it to exit\n");
				// 	write(sock, ourMessage, strlen(ourMessage));
				// 	break;
				// 	// close(socket_desc);
				// 	//exit(0);

				// }
				
				else if(command[0] != '\0')
				{
					strcpy(ourMessage, "Command Invalid. Enter new command\n");
				
					write(sock, ourMessage, strlen(ourMessage));
				}
				
      	}
                                                                       						        						         
      if(read_size == 0)
      {
      	puts("Client disconnected");
        fflush(stdout);
    	}
      else if(read_size == -1)
      {
      	perror("recv failed");
      }
                                                                       						        						                                                                  
    return 0;
} 
int main(int argc, char *argv[])
{
    //int sockfd, newsockfd, portno, clilen;
    //struct sockaddr_in serv_addr;
	//struct sockaddr_in cli_addr;
	
	int sockfd = -1;	
	int sockfd2 = -1;													// file descriptor for our server socket
	int newsockfd = -1;			
	int newsockfd2 = -1;									// file descriptor for a client socket
	int portno = -1;														// server port to connect to
	int clilen = -1;															// utility variable - size of clientAddressInfo below
	int n = -1;																// utility variable - for monitoring reading/writing from/to the socket
	char buffer[256];													// char array to store data going to and coming from the socket

	struct sockaddr_in serverAddressInfo;				// Super-special secret C struct that holds address info for building our server socket
	struct sockaddr_in clientAddressInfo;					// Super-special secret C struct that holds address info about our client socket
	 
	 
	 
	// If the user didn't enter enough arguments, complain and exit
    if (argc < 2)
	{
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
	 
	 
	 
	/** If the user gave enough arguments, try to use them to get a port number and address **/

	// convert the text representation of the port number given by the user to an int  
	portno = atoi(argv[1]);
	 
	// try to build a socket .. if it doesn't work, complain and exit
	//sockfd2 = socket(AF_INET, SOCK_STREAM, 0); new stuff

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	{
       error("ERROR opening socket");
	}
	

	
	/** We now have the port to build our server socket on .. time to set up the address struct **/

	// zero out the socket address info struct .. always initialize!
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(portno);
	 
	// set a flag to indicate the type of network address we'll be using  
    serverAddressInfo.sin_family = AF_INET;
	
	// set a flag to indicate the type of network address we'll be willing to accept connections from
    serverAddressInfo.sin_addr.s_addr = INADDR_ANY;
     
	 
	 
	 /** We have an address struct and a socket .. time to build up the server socket **/
     
    // bind the server socket to a specific local port, so the client has a target to connect to 
 //    bind(sockfd2, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)); // bind for printing every 20 seconds
 //    listen(sockfd2, 20);
 //    pthread_t thread_id2;
 //    while(newsockfd2 = accept(sockfd2, (struct sockaddr *) &clientAddressInfo, &clilen ))
 //    {
 //    	pthread_create(&thread_id2, NULL, printEveryTwenty, (void*) &newsockfd2);
 //    } more new stuff

    if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0)
	{
		error("ERROR on binding");
	}
	
			  
	// set up the server socket to listen for client connections
    listen(sockfd,20);
	
	// determine the size of a clientAddressInfo struct
    clilen = sizeof(clientAddressInfo);
	
	// block until a client connects, when it does, create a client socket
		pthread_t thread_id;
		pthread_create(&lThread, NULL, printAccounts, NULL);
		pthread_detach(lThread);
		
    while(newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen))	
		{
			// newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen);
			puts("Connetion Accepted");
			if(pthread_create(&thread_id, NULL, connection_handler, (void*) &newsockfd)<0) //make sure sockfd is correct
			{
				perror("could not create thread\n");
				return 1;
			}
			puts("Handler Assigned\n");
		}
	 
	 
	 
	
	
	// if the write to the client below up, complain and exit
    if (n < 0)
	{
		error("ERROR writing to socket");
	}
		
	return 0; 
}
