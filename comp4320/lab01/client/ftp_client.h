//ftp_client.h


#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring> 
#include <vector>
#include <sstream>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdint.h>

#define BUFSIZE 128
#define DATASIZE 126
#define HEADERSIZE 2
#define PORT 10026
#define MAXLINE 1024

typedef struct {
    uint8_t Sequence;
    unsigned char Checksum;
    char Data[DATASIZE];
} Packet;

class Client{


};