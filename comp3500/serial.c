#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <errno.h>
#include <sys/time.h>

//#define DATASIZE  (16777216)     //16MB
#define DATASIZE  (4194304)        //4MB
//#define DATASIZE  (1048576)      //1MB
#define MXSIZE (400)

double A[MXSIZE][MXSIZE];
double B[MXSIZE][MXSIZE];
double C[MXSIZE][MXSIZE];
int i, j, k, m; 
int outfile, result, count; 
unsigned seed;
struct timeval before, after;
char *buff;

int main()
{
    buff = (char *)malloc(sizeof(char)*(16<<20));

    srandom(seed);

    for (i=0; i<MXSIZE; i++)
	for (j=0; j<MXSIZE; j++) {
	    A[i][j] = random()/100.0;
	    B[i][j] = random()/100.0;
	}

    gettimeofday(&before, NULL);
    for (m=0 ; m < 100; m++) {
	/* Computation */
	for (i=0; i<MXSIZE; i++) {
	    for (j=0; j<MXSIZE; j++) {
			C[i][j] = 0;
			for (k=0; k<MXSIZE; k++){
		    	C[i][j] += A[i][k]*B[k][j];
			}
	    }
	}
	/* I/O */
	outfile = open("testfile", O_RDWR|O_CREAT|O_APPEND|O_SYNC, 0777);
	if (outfile <= 0)
	    perror("Error opening file\n");
	else {
	    result = write(outfile, buff, DATASIZE);
	    if (result <= 0)
		perror("Error writing file\n");
	}
	close(outfile);
    }
    free(buff);
    gettimeofday(&after, NULL);

    count = (after.tv_sec - before.tv_sec) * 1e6;
    count += (after.tv_usec - before.tv_usec);

    printf("\n Total time in usec: %d\n", count);
    return 0;
};
