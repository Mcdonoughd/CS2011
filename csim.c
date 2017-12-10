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

struct cacheStruct{
	int s; 
	int S;
	int E;
	int b;
	int B;
	int hits;
	int misses;
	int evictions;
}mycache;
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
	//char *fileName;
	int option = 0;
	int vflag = 0;
	//if no arguments
	if (argc == 1){
		helpMessage();
	}
	//


	//Parse arguments
	while ((option = getopt(argc, argv,"hvs:E:b:t")) != -1) {
		switch (option) {
		case 'h':
			helpMessage();
			break;
		case 'v':
			vflag++;
			//printf("hello\n");
			break;
		case 's':
			mycache.s = atoi(optarg);
			//printf("S");
			break;
		case 'E':
			mycache.E = atoi(optarg);
			//printf("E");
			break;
		case 'b':
			mycache.b = atoi(optarg);
			//printf("b");
			break;
		case 't':
			//fileName = optarg;
			break;
		default:
			helpMessage();
			break;
		}

	}
	printSummary(0, 0, 0);
	return 0;
}
