#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <errno.h>
#include <sys/time.h>

/* Cycle through the data sizes for each test run.
 * Running the make file is 1 test run
 * The makefile will run each program serial mp mt .c 5 times
 * and record the system time and user time and store it in a
 * text file.
 * to change DATASIZE comment out current size and uncomment
 * desired size
*/
// #define DATASIZE  (16777216)  //16MB
// #define DATASIZE (8388608) 	//8MB
// #define DATASIZE  (4194304)   //4MB
// #define DATASIZE (2097152)	//2MB
#define DATASIZE  (1048576)      //1MB
#define MXSIZE (400)


//* * * * * * * * * * * * * * * * * * * * * * * *
// 	PROVIDED GLOBAL VARIABLES
//* * * * * * * * * * * * * * * * * * * * * * * *
double A[MXSIZE][MXSIZE];
double B[MXSIZE][MXSIZE];
double C[MXSIZE][MXSIZE];
int i, j, k, m; 
int outfile, result, count; 
unsigned seed;
struct timeval before, after;
char *buff;
//* * * * * * * * * * * * * * * * * * * * * * * * 

void Multiply(void);
void ToFile(void);

int main()
{	
    buff = (char *)malloc(sizeof(char)*(16<<20));
    int status;
    pid_t pid, w;
    srandom(seed);


    //* Initialize* * * * * * * * * * * * * * * * * 
    for (i=0; i<MXSIZE; i++) {
		for (j=0; j<MXSIZE; j++) {
	    	A[i][j] = random()/100.0;
	    	B[i][j] = random()/100.0;
		}
	}
	//* * * * * * * * * * * * * * * * * * * * * * * * 


    gettimeofday(&before, NULL);

		pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		} else if(pid != 0){
    		/* Computation */
    		Multiply();
    		exit(0);

    	} else { //pid == 0
    		/* I/O */
			w = waitpid(pid, &status);
    		ToFile();
    	}

    //get time of day after is located in Multiply
    return 0;
};

void Multiply(void){
	//printf("%d Mult\n",m);
	int n;
	for(n = 0; n <100; n++){
		
		for (i=0; i<MXSIZE; i++) {
		    for (j=0; j<MXSIZE; j++) {
				C[i][j] = 0;
				for (k=0; k<MXSIZE; k++){
			    	C[i][j] += A[i][k]*B[k][j];
				}
		    }
		}
	}

    free(buff);
    gettimeofday(&after, NULL);

    count = (after.tv_sec - before.tv_sec) * 1e6;
    count += (after.tv_usec - before.tv_usec);
    printf("\n MP Total time in usec: %d\n", count);
}
void ToFile(void){
	//printf("%d IO\n",m);
	outfile = open("testfileMP", O_RDWR|O_CREAT|O_APPEND|O_SYNC, 0777);
	int p;
	for(p = 0; p <100 ; p++){
		
		if (outfile <= 0)
		    perror("Error opening file\n");
		else {
		    result = write(outfile, buff, DATASIZE);
		    if (result <= 0)
			perror("Error writing file\n");
		}
	}
	close(outfile);
}
