/* ============================================================================
*	COMP 4320 Lab01
*	ftp_server.cpp
*	FTP transfer service
*	SUMMER 2015 (JULY)
*	GROUP: Drew Hoover, F. Davis Quarles, Kullen Williams
*	PORTS 10026 - 10029
* ===========================================================================*/


#include "ftp_server.h"

unsigned char generateChecksum( Packet*); 
void getData(string);
string dataToString(char[]);

int main()
{    
	
    // create a UDP socket 
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

    /* ========================================================================
    *   waiting/receiving data
    *  ========================================================================*/
    string command;
    string filename;
    string temp;
    for (;;) {
        //http://www.ccplusplus.com/2011/09/recvfrom-example-c-code.html
		recvlen = recvfrom(fd, 								//Socket
						   buf, 							//receiving buffer
						   BUFSIZE, 						//max buf size
						   0, 								//FLAGS: no options
						   (struct sockaddr *)&remoteAddr,  //address
						   &addrlen);						//add len, in and out
	    
	    memcpy(packet, buf, BUFSIZE);

        if (recvlen > 0) {

			//copy buffer into data - header and print
            for( int x = HEADERSIZE; x < recvlen; x++) {
            	data[x - HEADERSIZE] = buf[x];
				cout << buf[x];
            }
			cout << endl;

			//Make checksum
			checksum = generateChecksum(packet);
            if ( checksum != buf[1]) {
				cout << "Checksum invalid - NAK\n";
				nak[1] = seqnum;
            	sendto(fd, nak, 2, 0, (struct sockaddr *)&remoteAddr, addrlen);
            }
            else {
            	temp = dataToString(data);
            	command = temp.substr(0,3);
				
				if ( temp == "PUT" ) {
					filename = temp.substr(4); //remove PUT
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
		} //end if (recvlen > 0)

    } // end for(;;) 

} //end MAIN


/* ============================================================================
*  generateChecksum
*  @param Packet*
*  @return unsigned char
*  =============================================================================*/
unsigned char generateChecksum( Packet* packet ) {
    
    unsigned char checksum = 0x00;

    checksum = packet->Sequence;

    for( int i=0; i < DATASIZE; i++ ) {
        checksum += packet->Data[i];    
    }
    
    checksum = ~checksum;   //invert checksum

    return checksum;
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
		checksum = generateChecksum(packet);
        if ( checksum != buf[1]) {
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
				
				//print sample of the buffer
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

/* ==========================================================================
*	dataToString
* 	takes data array and returns string
*	@param char[]
*	@return string
* ===========================================================================*/
string dataToString(char data[]){
	string dataString = "";
	for(int i = 0 ; i < (sizeof(data)/sizeof(*data)); i++){
		dataString += data[i];
	}
	return dataString;
}
