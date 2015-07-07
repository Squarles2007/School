#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring> 
#include <vector>
#include <sstream>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdint.h>

using namespace std;

#define PORT 10026
#define BUFSIZE 128
#define DATASIZE 126
#define HEADERSIZE 2
#define MAXLINE 1024

typedef struct {
    uint8_t Sequence;
    unsigned char Checksum;
    char Data[DATASIZE];
} Packet;

// GLOBAL DATA
struct sockaddr_in localAddr;      			// address 
struct sockaddr_in remoteAddr;     			// remote address 
socklen_t addrlen = sizeof(remoteAddr);    	// length of addresses 
int recvlen;                    			// # bytes received 
int fd;                         			// our socket 
unsigned char buf[BUFSIZE];    				// receive buffer 
uint8_t seqnum;
char data[BUFSIZE - HEADERSIZE];
unsigned char checksum;

// ACK and NAK constants
char nak[2] = {'0', '0'};
char ack[2] = {'1', '0'};

Packet* packet = new Packet;

class Server {

};