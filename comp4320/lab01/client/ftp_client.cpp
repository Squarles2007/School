/* ============================================================================
*	COMP 4320 
*	Lab01
*	FTP Transfer Server
*	AU ports 10026 - 10029
*	GROUP:
*	Drew Hoover , F. Davis Quarles, Kullen Williams
*   compile: make OR g++ -o ftp_client ftp_client.cpp
*   run: ./ftp_client d <num%> l <num%> s <destination IP>
*  ==========================================================================*/

#include "ftp_client.h"

using namespace std;

int fd;
int slen;
socklen_t slt;
struct sockaddr_in serverAddress; 
	    
//Command line arguments
string destIP;
float damagedFloat;    		
float lostFloat ;	     	


int main(int argc, char *argv[])
{
    
    //Socket variables
    struct hostent *hp;
    unsigned char buf[BUFSIZE];
    slen = sizeof(serverAddress);
    slt = sizeof(serverAddress);

	//DEFAULT VARS
    damagedFloat = 0;  //Default values for damage and loss
    lostFloat = 0;	   //If user only provides ip address values are 0
    bool sentBool = false;
    bool gremlinBool = false;

    //Sending variables
    Packet *packet;
    Packet *tempPack = new Packet;

    readCommandArgs(argc, argv);  //import command line args

   
    string in;
    string putFilename;  //COMMAND AND FILENAME
    for(;;) {
        /* ====================================================================
        *  For UDP/IP sockets
        *  AF_INET  = ip address family
        *  SOCK_DGRAM = datagram service
        *  0 = protocol
        *  https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html
        *  ====================================================================*/
		if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            cout << "Unable to open socket. Exiting..." << endl;
	     	return 0;
	 	}

	 	// server address has ip and port info
		bzero(&serverAddress, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(PORT);

		//can't connect to ip
		if (inet_aton(destIP.c_str(), &serverAddress.sin_addr)==0) {
			cout << "There was a problem with the server IP address. Exiting..." << endl;
			return 0;
		}

		/* ====================================================================
		*	MENU: 1 to PUT file 2 to quit
		*  ==================================================================*/
		cout << "Enter (1) to PUT file or (2) to quit.\n\nEnter: ";
		cin.clear();
		cin >> in;
		string command = in;
		if(in.compare("2") == 0) {	break; }
		
		if(in.compare("1") == 0 ) {
			cin.clear();
			cout << "Enter File Name: ";
			cin >> in;
			
			putFilename = "PUT " + in;  //header

			cout << putFilename << endl;
			//Now in is our filename to send
			ifstream putfile;
			putfile.open(in.c_str());   //open in file
			//unsigned char csum = 0x00;
			//unsigned char lost = 0x00;
			char buff[DATASIZE];
			if(putfile.is_open())  {
			
				//Now send a 1 packet overhead for the filename
			
				sentBool = false;
				packet = buildPacket((char*)putFilename.c_str(), strlen(putFilename.c_str()));
				while(!sentBool ) {
					memcpy(tempPack, packet, sizeof( Packet ));
					gremlinBool = gremlin(damagedFloat, lostFloat, tempPack);
					sentBool = sendPacket(tempPack, gremlinBool);
					cout << "Sending put..." << endl;
				}	
			
				while(!putfile.eof()) {

					/*=========================================================
					*  loads buffer and prints sample 
					===========================================================*/
					for(int i = 0; i < DATASIZE; i++) {
						if(!putfile.eof()) {
							if( i < 48 ) {
								cout << buff[i];
							}
							buff[i] = putfile.get();
						}
						else {
							if( i != 0 ) {
								buff[i-1]= '\0';
							}
							buff[i] = '\0';
						}
					}
					cout << endl;
				

					/*=========================================================
					* GREMLIN function:	 
					==========================================================*/
					sentBool = false;
					packet = buildPacket(buff, strlen(buff));
					while( !sentBool ) {
						memcpy(tempPack, packet, sizeof( Packet ));
						gremlinBool = gremlin(damagedFloat, lostFloat, tempPack);
						sentBool = sendPacket(tempPack, gremlinBool); 
					}
				
				
				}

				/* ============================================================
				* sendto - allows us to specify the destination w/out
				* having to connect to the server first.
				*   
				* sendto(int socket, const void *buffer, size_t length, 
				* int flags, const struct sockaddr *dest_addr,socklen_t dest_len)
				*  ============================================================*/			
				sendto(fd,"\0", 1,0,(struct sockaddr*)&serverAddress,slen);

				cout << "Sending Complete!\n\n";
				putfile.close();
			}
			else
			{
				cout << "Could not open file name.\n";
			}

		}
		close(fd);
    }  //END loop forever =====================================================

    return 0;
} //end main


/*=============================================================================
* GREMLIN function:	
* Randomly determines, depending on the damage probability
* whether to corrupt some bits or pass the packets as it is to the server
* receiving function.
* It will also decide whether some packets will be dropped based on the 
* loss probability. 
* (more information in README under GREMLIN FUNCTION)
*=================================================================================*/
bool gremlin(float damagedFloat, float lostFloat, Packet* packet) {
    
    if(lostFloat > (1.0 * rand()) / (1.0 * RAND_MAX)) {
        return true;
    }

    if(damagedFloat > (1.0 * rand()) / (1.0 * RAND_MAX)) {
        int numDamaged = rand() % 10;
	 int byteNum = rand() % BUFSIZE;

	 if(numDamaged == 9) {

	     if(numDamaged > 1) { packet->Data[numDamaged-HEADERSIZE]+= 8; }
	     else if(numDamaged == 1) {  packet->Checksum+= 8; }
	     else {  packet->Sequence+= 8; }
			
            numDamaged = rand() % 10;
	     
	     if(numDamaged > 1) { packet->Data[numDamaged-HEADERSIZE]+= 4; }
	     else if(numDamaged == 1) {  packet->Checksum+= 4;  }
	     else { packet->Sequence+= 4; }

            numDamaged = rand() % 10;

	     if(numDamaged > 1) { packet->Data[numDamaged-HEADERSIZE]+= 2; }
	     else if(numDamaged == 1)  { packet->Checksum+= 2;  }
	     else { packet->Sequence+= 2; }
	}
	else if(numDamaged > 6) {

	    if(numDamaged > 1) { packet->Data[numDamaged-HEADERSIZE]+= 8; }
           else if(numDamaged == 1) { packet->Checksum+= 8; }
	    else{ packet->Sequence+= 8;	}

	    numDamaged = rand() % 10;

	    if(numDamaged > 1) { packet->Data[numDamaged-HEADERSIZE]+= 4; }
	    else if(numDamaged == 1){ packet->Checksum+= 4; }
	    else {	packet->Sequence+= 4; }
	}
	else {

	    if(numDamaged > 1) {  packet->Data[numDamaged-HEADERSIZE]+=8; }
	    else if(numDamaged == 1) { packet->Checksum+=4; }
	    else { packet->Sequence+= 2; }
	}

    }
    return false;
} //END GREMLIN ===============================================================

/* ============================================================================
*  getChecksum-  https://en.wikipedia.org/wiki/IPv4_header_checksum
*  adds every bit from the the packet's data[] together. Which should be full.
*  so, when you invert checksum the result is 0 (0x00) meaning no errors
*  @param Packet*  (char data[], unsigned char Checksum, uint8_t Sequence )
*  ============================================================================*/
unsigned char getChecksum(Packet* packet ) {
    unsigned char checksum = 0x00;

    checksum = packet->Sequence;

    for( int i=0; i < DATASIZE; i++ ) {
        checksum += packet->Data[i];
    }

   checksum = ~checksum;   //invert   ff --> 00

    return checksum;
}


/* ============================================================================
*
*  ============================================================================*/
Packet* buildPacket(char* data, int length) {
    Packet* packet = new Packet;
    static uint8_t sequenceNum = 0;

    packet->Sequence = sequenceNum;

    sequenceNum = 1 - sequenceNum;

    for( int i=0; i < DATASIZE; i++ ) {
        if( i < length )
            packet->Data[i] = data[i];
        else
            packet->Data[i] = '\0';
    }

    packet->Checksum = getChecksum(packet);

    return packet;
}  //end buildPacket


bool sendPacket(const Packet* packet, bool bLost) {
    bool bReturn = false;

	//Send packet
	if(!bLost)
	{
		if (sendto(fd, packet, BUFSIZE, 0, (struct sockaddr*)&serverAddress, slen) == -1) {
			cerr << "Problem sending packet with sequence #" << packet->Sequence << "..." << endl;
			bReturn = false;
		} 
	}


	int recvPollVal = 0;
	int iLength = 0;
	struct pollfd ufds;
	unsigned char recvline[MAXLINE + 1];
	time_t timer;

	//Wait on return
	ufds.fd = fd;
	ufds.events = POLLIN;
	recvPollVal = poll(&ufds, 1, 20);

	if( recvPollVal == -1 ) {            //If error occurs
		cerr << "Error polling socket..." << endl;
		bReturn = false;
	} else if( recvPollVal == 0 ) {        //If timeout occurs
		cerr << "Timeout... Lost Packet, Sequence Number - "  << (int)packet->Sequence << endl;
		bReturn = false;
	} else {
		iLength = recvfrom(fd, recvline, MAXLINE, 0, (struct sockaddr*)&serverAddress, &slt);

		if( recvline[0] == '1') {          //If ACK Received, return true
			cout << "ACK - " << (int)recvline[1] << endl;
			//cout << packet->Data << endl;
		    bReturn = true;
		} else if( recvline[0] == '0' ) {     //Else if NAK, return false
			cout << "NAK - " << (int)recvline[1] << endl;
		    bReturn = false;
		} else {                             //Else bad stuff, return false
		    bReturn = false;
		}
	}
  
	return bReturn;
}  //END sendPacket



/*=============================================================================
*	readCommandArgs - takes in the arg count and the argument
*	interprets th args and sets the corrisponding values
*   s = server , d = damage, l = loss , h = help
*	@param int, char*
*   @return int
*============================================================================*/
int readCommandArgs(int argc, char *argv[]) {
	 /* ============================
     *   user didn't provide args
     * ============================*/
    if(argc == 1){
	help();
	return 0;
    }
    
    /* ==================================================
    *   read in command line args
    *  ==================================================*/
    char *switchState;

    for(int i=1;i < argc; i+= 2) {
	    switchState = argv[i];
        switch (switchState[0]) {
            case 'd': {
                damagedFloat = atof(argv[i+1]);
                cout << "\nPacket Damaged "<< damagedFloat <<endl;
            };
            break;

            case 'l': {
                lostFloat = atof(argv[i+1]);
		  		cout << "\nPacket Loss " << lostFloat << endl;
            };
            break;

            case 's': {
                destIP = argv[i+1];
 		  		cout << "\nDestination IP address " << destIP << endl;
            };
            break;

            case 'h': {   //help  
            	help();
            	return 0;
            };
            break;
	     	default: {  //incorrect response
				commandArg();
              	return 0;
	    	}

        }  //end switch
    } //end loop ===================================================

    cout << "\n" ;   // blank line for output
    return 1;  //success
}



/* ============================================================================
*  HELP!!!!
*  little Extra instructions
*  ============================================================================*/
void help() {
	commandArg();
}

/* ============================================================================
*  COMMAND ARGS !!!!
*  if user trys to run program with no args or runs with ./fileName h
*  displays proper syntax
*  ============================================================================*/
void commandArg(){
	cout << "***************************************" << endl;
    cout << "\nWHEN RUNNING: \nUse one of the following syntax\n\n./fileName l <lost packets> d <damaged packets> s <ip address>" << endl;
    cout << "OR ./fileName d <damaged packets> s <ip address>" << endl;
    cout << "OR ./fileName l <lost packets> s <ip address>"  <<endl;
    cout << "OR ./fileName s <ip address>\n" <<endl;
    cout << "***************************************" << endl;
}


