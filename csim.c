/*Cachelab assignment for CS-2011 B'17
 * Written by Surya Vadivazhagu (svadivazhagu) and
 * Daniel McDonough (dmcdonough) 
 *
 * Team dmcdonough-svadivazhagu
 *
 */



/*
 * Includes
 */
#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>



typedef struct bStruct{
	int Size;
	int valid;
	char* tag;
	char* data;
}Block;

typedef struct EStruct{
	int Size;
	Block myblock;

}Line;

typedef struct sStruct{
	int Size;
	Line myLine;
}Set;

typedef struct cacheStruct{
	int s; 
	int S;
	int E;
	int b;
	int B;
	int hits;
	int misses;
	int evictions;
	Set mySet;
}Cache;
/**
 * prints the help message
 */
void helpMessage(){
	printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
	printf("Options:\n");
	printf("  -h         Print this help message.\n");
	printf("  -v         Optional verbose flag.\n");
	printf("  -s <num>   Number of set index bits.\n");
	printf("  -E <num>   Number of lines per set.\n");
	printf("  -b <num>   Number of block offset bits.\n");
	printf("  -t <file>  Trace file.\n");
	printf("Examples:\n");
	printf("linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
	printf("linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
	exit(0);
}



int main(int argc, char* argv[]){
	char *fileName;
	int option = 0;
	int vflag = 0;
	//if no arguments
	if (argc == 1){
		helpMessage();
	}
	//
	//Cache myCache;

	//Parse arguments
	while ((option = getopt(argc, argv,"hvs:E:b:t:")) != -1) {
		switch (option) {
		case 'h':
			helpMessage();
			break;
		case 'v':
			vflag++;
			//printf("hello\n");
			break;
		case 's':
			//myCache.s = atoi(optarg);
			//printf("S");
			break;
		case 'E':
			//myCache.E = atoi(optarg);
			//printf("E");
			break;
		case 'b':
			//myCache.b = atoi(optarg);
			//printf("b");
			break;
		case 't':
			//printf("%s\n",optarg);
			fileName = optarg;
			break;
		default:
			helpMessage();
			break;
		}
	}

	FILE *pFile = fopen (fileName,"r"); //open the file
	//Check if File exists
	if(!pFile){
				printf("No such file or directory\n");
				exit(0);
			}

	int maxlines = 268000; // maximum number of lines to be checked
	char line[maxlines][2]; // array of the first letter in the trace file for the entire trace file
	char address[maxlines][16]; //Array of the second part of the trace file for the entire trace file
	//long long int parsedHexAddresses[maxlines];

	//int counter = 0;
	//char * pEnd;

	while(fscanf(pFile,"%s %s", *line, *address)!= EOF ){
		//if string compare is not I
		if(strcmp(*line,"I")){
			//put a space
			printf(" %s %s \n",*line, *address);
		}
		//if line starts with I
		else{
			//put no space
			printf("%s %s \n",*line, *address);
		}
	}
	printSummary(0, 0, 0);
	return 0;
}
