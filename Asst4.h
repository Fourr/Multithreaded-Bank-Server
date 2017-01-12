#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct
{
	int sock;
	struct sockaddr address;
	socklen_t addr_len;

} connection_t;

struct sockNode
{
	int sock;
	struct sockNode *next;
}*head;


typedef struct
{
	char accountName[100];
	float balance;
	int inUse;
} account;

account *AccountList[20] = {NULL};