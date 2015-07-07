/* ============================================================================
*	COMP 4320 Lab01
*	ftp_server.cpp
*	FTP transfer service
*	SUMMER 2015 (JULY)
*	GROUP: Drew Hoover, F. Davis Quarles, Kullen Williams
*	PORTS 10026 - 10029
* ===========================================================================*/


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
struct sockaddr_in localAddr;      		// address 
struct sockaddr_in remoteAddr;     		// remote address 
socklen_t addrlen = sizeof(remoteAddr);    // length of addresses 
int recvlen;                    		// # bytes received 
int fd;                         		// our socket 
unsigned char buf[BUFSIZE];    			// receive buffer 
uint8_t seqnum;
char data[BUFSIZE - HEADERSIZE];

// ACK and NAK constants
char nak[2] = {'0', '0'};
char ack[2] = {'1', '0'};

Packet* packet = new Packet;

unsigned char generateChecksum( Packet*); 
void getData(string);

int main()
{    

    /* create a UDP socket */
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("cannot create socket\n");
            return 0;
    }

    /* bind the socket to any valid IP address and a specific port */

    memset((char *)&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(PORT);



    if (bind(fd, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0) {
            perror("bind failed");
            return 0;
    }
	
	cout<<"\n- Reliable FTP Application - "<< endl;
	cout<<"Waiting on port: "<< PORT << endl; 

	cout << inet_ntoa(localAddr.sin_addr) << endl;

    /* now loop, receiving data */
    for (;;) {
        //http://www.ccplusplus.com/2011/09/recvfrom-example-c-code.html
		recvlen = recvfrom(fd, 						//Socket
						buf, 						//receiving buffer
						BUFSIZE, 					//max buf size
						0, 							//FLAGS: no options
						(struct sockaddr *)&remoteAddr, //address
						&addrlen);					//add len, in and out
	    memcpy(packet, buf, BUFSIZE);

            if (recvlen > 0) {

		//Add data to buffer (minus two byte header)
                for( int x = HEADERSIZE; x < recvlen; x++) {
                	data[x - HEADERSIZE] = buf[x];
					cout << buf[x];
                }
				cout << endl;


	
		//Make checksum
                if ( generateChecksum(packet) != buf[1]) {
					cout << "Checksum invalid - NAK\n";
					nak[1] = seqnum;
                	sendto(fd, nak, 2, 0, (struct sockaddr *)&remoteAddr, addrlen);
                }
                else {
					string command(data);
					if ( command.substr(0,3) == "PUT" ) {
						string filename = command.substr(4); //remove PUT
						cout << "Checksum valid - ACK" << endl;
						seqnum = packet->Sequence;
						ack[1] = seqnum;
                		sendto(fd, ack, 2, 0, (struct sockaddr *)&remoteAddr, addrlen);
						getData(filename);
						cout << "PUT successfully completed" << endl;
					}
					else {
						cout << "Not a PUT command - NAK\n";
						nak[1] = (char)packet->Sequence ^ 30;
				        sendto(fd, nak, 2, 0, (struct sockaddr *)&remoteAddr, addrlen);
					}
				}
			}

    }
    /* never exits */
}


unsigned char generateChecksum( Packet* packet ) {
    unsigned char retVal = 0x00;

    retVal = packet->Sequence;

    for( int i=0; i < DATASIZE; i++ ) {
        retVal += packet->Data[i];
    }

    retVal = ~retVal;

    return retVal;
}


/* ============================================================================
*	getData - copies content from client file (buffer) to server file.
*	creates server file named after client file.	
*	@param string
*  ==========================================================================*/
void getData(string filenameIN) {
	string filename = filenameIN;
	ofstream outFile;
	outFile.open(filename.c_str(),fstream::out | fstream::trunc);
	
	//http://www.ccplusplus.com/2011/09/recvfrom-example-c-code.html
	recvlen = recvfrom(fd, 							//Socket
						buf, 						//receiving buffer
						BUFSIZE, 					//max buf size
						0, 							//FLAGS: no options
						(struct sockaddr *)&remoteAddr, //address
						&addrlen);					//add len, in and out
	
	memcpy(packet, buf, BUFSIZE);
	
	while ( recvlen > 1 ) {
		//Make checksum
		if ( generateChecksum(packet) != buf[1]) {
			cout << "Checksum invalid - NAK - Sequence Num: " << (int)seqnum << "\n";
			nak[1] = seqnum;
			sendto(fd, nak, 2, 0, (struct sockaddr *)&remoteAddr, addrlen);
		}
		else {
			cout << "ACK - Seq Num: " << (int)seqnum << "\n";
			ack[1] = seqnum;
			sendto(fd, ack, 2, 0, (struct sockaddr *)&remoteAddr, addrlen);
			//Add data to buffer (minus two byte header)
			for( int x = HEADERSIZE; x < recvlen; x++) {
				data[x - HEADERSIZE] = buf[x];
				if ( x < 50 ) {
					cout << buf[x];
				}
			}

			cout << endl;
			string buffer(data);

			//copy data to file.
			if ( seqnum != packet->Sequence ) {
				seqnum = packet->Sequence;
				outFile << buffer;
			}
		}

		//http://www.ccplusplus.com/2011/09/recvfrom-example-c-code.html
		recvlen = recvfrom(fd, 						//Socket
						buf, 						//receiving buffer
						BUFSIZE, 					//max buf size
						0, 							//FLAGS: no options
						(struct sockaddr *)&remoteAddr, //address
						&addrlen);					//add len, in and out
		
		memcpy(packet, buf, BUFSIZE);
	}

	ack[1] = seqnum;
	sendto(fd, ack, 2, 0, (struct sockaddr *)&remoteAddr, addrlen);
	outFile.close();
}

