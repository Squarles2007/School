#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <iostream>   //cout

using namespace std;

/* paddr: print the IP address in a standard decimal dotted format */
void paddr(unsigned char *a)
{
        printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

void runInstructions(){
        cout << "**************************************\n" << endl;
        cout << "To Compile : g++ -o findIP findIP.cpp  "  <<endl;
        cout << "To Run     : ./findIP <<wedaddress>>\n"   << endl;
        cout << "**************************************\n" << endl;
}

int main(int argc, char **argv) {

        if(argc == 0){
                runInstructions();
                return 0;
        }
        struct hostent *hp;
        char *host = argv[1];
        int i;

        hp = gethostbyname(host);
        if (!hp) {
                fprintf(stderr, "could not obtain address of %s\n", host);
                return 0;
        }
        for (i=0; hp->h_addr_list[i] != 0; i++)
                paddr((unsigned char*) hp->h_addr_list[i]);
        
        return 0;
}