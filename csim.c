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
#include <math.h>

typedef struct bStruct{
	int valid;
	int tag;
	int data;
}Block;

typedef struct cacheStruct{
	
	int s;
	int S;
	int b;
	int B;
	int E;

	int numBlocks;
	int Size;
	
	Block* cacheBlock;
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

char* removeCommas(char* address){
 	int addressLength = strlen(address);
	char* newAddress = malloc(addressLength*sizeof(char));

	for(int k = 0; k <= addressLength; k++){
		if(address[k] != ','){
			newAddress[k] = address[k];
		}
		else{
			newAddress[k] = '\0';
			//printf("\n");
			break;
		}
	}
	//printf("%d", atoi(newAddress));
	
	return newAddress;
}
char* getSize(char* address){
	int addressLength = strlen(address);
	char* Size = malloc(addressLength*sizeof(char));

	for(int k = 0; k <= addressLength; k++){
		if(address[k] != ','){
			//not important
		}
		else{
			Size[0] = address[k+1];
			Size[1] = '\0';
			//printf("\n");
			break;
		}
	}
	//printf("%d", atoi(newAddress));
	return Size;
}

int main(int argc, char* argv[]){
	//number of hits	
	int hits = 0;

	//number of misses
	int misses = 0;

	//number of evictions
	int evictions = 0;

	
	char *fileName; //
	int option = 0; //
	int vflag = 0; //Check if vflag is thrown
	
	//if no arguments
	if (argc == 1){
		helpMessage();
	}
	
	//the cache
	Cache myCache;

	//Parse arguments
	while ((option = getopt(argc, argv,"hvs:E:b:t:")) != -1) {
		switch (option) {
		case 'h':
			helpMessage();
			break;
		case 'v':
			vflag = 1;
			//printf("hello\n");
			break;
		case 's':
			myCache.s = (atoi(optarg));
			myCache.S = pow(2, myCache.s);
			printf("Value of s: %d\n", myCache.s);

			printf("Value of S: %d\n", myCache.S);
			break;
		case 'E':
			if (atoi(optarg) <= 0){
				printf("E value can't be <= 0. \n");
				helpMessage();
			}
			if(!(atoi(optarg) % 2) || (atoi(optarg) == 1)){
				myCache.E = atoi(optarg);
				printf("Value of E: %d\n", myCache.E);


			}
			else{
				printf("E value not power of 2. \n");
				helpMessage();
			}
			break;
		case 'b':
			myCache.b = (atoi(optarg));
			myCache.B = pow(2, myCache.b);
			printf("Value of b: %d\n", myCache.b);
			printf("Value of B: %d\n", myCache.B);

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
	char instruction[maxlines][2]; // array of the first letter in the trace file for the entire trace file
	char address[maxlines][3]; //Array of the second part of the trace file for the entire trace file

	//Print the file
	if(vflag == 1){
		while(fscanf(pFile,"%s %s", *instruction, *address)!= EOF ){
			//go through address to obtain the actual address and size
			char* newAddress = removeCommas(*address);
			char* Size = getSize(*address);

			//if string compare is not I
			if(strcmp(*instruction,"I")){
				//put a space
				printf(" %s %s,%s\n",*instruction, newAddress, Size);
			}
			//if instruction starts with I
			else{
				//put no space
				printf("%s %s,%s\n",*instruction, newAddress, Size);
			}
		}
	}
	
	//Calculate the SIze of the Cache
	myCache.Size = (myCache.S * myCache.E * myCache.B);
	printf("Cache SIZE: %d\n", myCache.Size);
	
	//Calculate the Number of Blocks	
	myCache.numBlocks = (myCache.S * myCache.E);
	printf("Cache Blocks: %d\n", myCache.numBlocks);

	
	printf("sizeof a block: %ld\n",sizeof(Block));
	
	//Malloc the size needed
	myCache.cacheBlock = malloc(myCache.numBlocks * sizeof(Block)*myCache.B);
	printf("Cache SIZE: %ld\n", sizeof(myCache.cacheBlock));

	//Initialize cache to all all zeros
	for (int i = 0; i <= myCache.numBlocks; ++i){
			myCache.cacheBlock[i].valid = 0;
			myCache.cacheBlock[i].tag = 0;
			myCache.cacheBlock[i].data = 0;
	}

	for (int i = 0; i < myCache.numBlocks; ++i){
		printf("TAG BIT: %d\n",myCache.cacheBlock[i].tag);
		printf("Valid BIT: %d\n",myCache.cacheBlock[i].valid);
		printf("DATA BIT: %d\n",myCache.cacheBlock[i].data);
			
	}

	free(myCache.cacheBlock); //Free memeory
	printSummary(hits, misses, evictions);

	return 0;
}
