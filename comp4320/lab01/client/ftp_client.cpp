/* ============================================================================
*	COMP 4320 
*	Lab01
*	FTP Transfer Server
*	AU ports 10026 - 10029
*	GROUP:
*	Drew Hoover , F. Davis Quarles, Kullen Williams
*  */


//compile: make
//run : ./ftp_client d <num> l <num> s <ipaddress>

#include "ftp_client.h"

using namespace std;

void commandArg();
void help();
void put();
char getChecksum(char*, int);
bool gremlin(float, float, Packet*);
char* loadFileToBuffer();
Packet* buildPacket(char*, int);
bool sendPacket(const Packet*, bool); 

int fd;
int slen;
socklen_t slt;
struct sockaddr_in serverAddress; 

int main(int argc, char *argv[])
{
    //cout << "count " << argc << endl;
    //Socket variables
    struct hostent *hp;
    unsigned char buf[BUFSIZE];
    slen = sizeof(serverAddress);
    slt = sizeof(serverAddress);

    //Command line arguments
    string sServerAddress;
    float damagedFloat = 0;    		//Default values for damage and loss
    float lostFloat = 0;	     	//If user only provides ip address values are 0

    bool sentBool = false;
    bool gremlinBool = false;

    //Sending variables
    Packet *packet;
    Packet *tempPack = new Packet;

    //put and filename
    string putFilename;

    /* ============================
     *   user didn't provide args
     * ============================*/
    if(argc == 1){
	help();
	return 0;
    }
    
    /* ========================================================================
    *   read in command line args
    *  =======================================================================*/
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
                sServerAddress = argv[i+1];
 		  		cout << "\nIP address " << sServerAddress << endl;
            };
            break;

            case 'h': {   //help  
            	help();
            	return 0;
            };
            break;
	     	default: {
				commandArg();
              	return 0;
	    	}

        }  //end switch
    } //end loop ==============================================================

    cout << "\n\n" ;   // blank line for output

    /* ========================================================================
    *   Loop forever
    *  ========================================================================*/
    string ln;
    for(;;) {
        //Open socket
		if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            cout << "Unable to open socket. Exiting..." << endl;
	     	return 0;
	 	}

		bzero(&serverAddress, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(PORT);

		if (inet_aton(sServerAddress.c_str(), &serverAddress.sin_addr)==0) {
			cout << "There was a problem with the server IP address. Exiting..." << endl;
			return 0;
		}

		cout << "Enter (1) to transfer file or (2) to quit.\n\nEnter: ";
		cin.clear();
		cin >> ln;
		string command = ln;
		if(ln.compare("2") == 0) {	break; }
		
		if(ln.compare("1") == 0 ) {
			cin.clear();
			cout << "Enter File Name: ";
			cin >> ln;
			
			putFilename = "PUT " + ln;

			cout << putFilename << endl;
			//Now ln is our filename to send
			ifstream putfile;
			putfile.open(ln.c_str());
			unsigned char csum = 0x00;
			unsigned char lost = 0x00;
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
			
				sendto(fd, 								 //socket
					   "\0", 							 //*buf
					   1, 								 //len
					   0, 								 //FLAGS no option
					   (struct sockaddr*)&serverAddress, //to address
					   slen);							 // address length

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
}


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

/* ============================================================================
*  getChecksum-  https://en.wikipedia.org/wiki/IPv4_header_checksum
*  adds every bit from the the packet's data[] together. Which should be full.
*  so, when you invert checksum the result is 0 (0x00) meaning no errors
*  @param Packet*  (char data[], unsigned char Checksum, uint8_t Sequence )
*  ============================================================================*/
unsigned char getChecksum( Packet* packet ) {
    unsigned char checksum = 0x00;

    checksum = packet->Sequence;

    for( int i=0; i < DATASIZE; i++ ) {
        checksum += packet->Data[i];
    }

   checksum = ~checksum;   //invert   ff --> 00

    return checksum;
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

