/* Project : cSim.c
 * Created by hliang2-jjang
 * Date: April 24th 2017
 * This is a cache simulator. It takes several arguments to run to simulate cache hits, misses and evictions
 * Please take a look at print_usage to learn how to use.
 */
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


// takes the string that contains address comma and size of the data
// and parses them to only return the address.
char *parseAddress(char *address){
    int count = 0;
    int positionComma;
    int totalLength = strlen(address);
    char *parsedAddress = malloc(totalLength*sizeof(char)); // remember to free it at the end

    while(*address != '\0'){ // stepping through each character in the address string to find comma's position
        if(*address == ','){
            positionComma = count;
            break;
        }
        else{
            count = count+1;
        }
        address = address + 1;
    }
    address = address - positionComma;
    for (int i =0; i<positionComma; i++){
        parsedAddress[i] = address[i];
    }
    return parsedAddress;
}

void print_usage(){
	printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
	printf("Options:\n");
	printf("  -h         Print this help message.\n");
	printf("  -v         Optional verbose flag.\n");
	printf("  -s <num>   Number of set index bits.\n");
	printf("  -E <num>   Number of lines per set.\n");
	printf("  -b <num>   Number of block offset bits.\n");
	printf("  -t <file>  Trace file.\n");

	printf("Examples:\n");
			printf("linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
			printf("linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

// each line is a struct comprised of blobkBit, tag bit, and valid bit.
typedef struct{
	int validBit;
	long long int tagBit;
	int accessedCount;
} cacheLine;

// each set is comprised of array of lines
typedef struct{
	cacheLine *cacheLines;
} cacheSet;

// Define cache structure which is comprised of array of sets
typedef struct{
	cacheSet *cacheSets;
} cacheMemory;

struct cacheParameters{
	int setIndexBit;
	int lines; // number of lines per set
	int blockBit;
	int setSize; // number of sets
	int blockSize; // total bytes per line
}p_cache;

int main (int argc, char *argv[]){
	int hitCount = 0;
	int missCount = 0;
	int evictionCount = 0;
	int verbosity = 0;
	int option = 0;
	char *fileName;

	// : in the opstring which is the third argument of getopt refers to optional input after corresponding flag
	while ((option = getopt(argc, argv,"hvs:E:b:t:")) != -1) {
		switch (option) {
			 case 'h' : print_usage();
			 	 break;
			 case 'v' : verbosity = 1;
				 break;
			 case 's' :	p_cache.setIndexBit = atoi(optarg);
	 	 	 			printf("setIndexBit: %d\n",p_cache.setIndexBit);
				 break;
			 case 'E' : p_cache.lines = atoi(optarg);
			 	 	 	printf("associativity: %d\n",p_cache.lines);
				 break;
			 case 'b' : p_cache.blockBit = atoi(optarg);
			 	 	 	printf("blockBit: %d\n", p_cache.blockBit);
				 break;
			 case 't' : fileName = optarg;
			 	 	 	printf("name of the file: %s\n",fileName);
				 break;
			 default: print_usage();
				 	 exit(EXIT_FAILURE);
				 	 break;
		}
	}

	//----------------------------------------------------------------------------------------
	// ------ parsing the input text file and storing the data into local variables-----------
	//----------------------------------------------------------------------------------------
	FILE *pFile = fopen (fileName,"r"); // gets the pointer to the file
	if(!pFile)
		printf("File wasn't opened correctly\n");

	// assume the largest trace file will have 267988 lines (obtained from long.trace)
	int largestLines = 267988;
	char instructions[largestLines][3]; // ?? i somehow cannot have ** for the function fscanf

	char addresses[largestLines][16];
	long long int parsedHexAddresses[largestLines];
	int counter = 0;
	char * pEnd;

	while(fscanf (pFile, "%s %s", instructions[counter], addresses[counter]) != EOF){
	//	printf("%s ",instructions[counter]);

		char *parsedAddress = parseAddress(addresses[counter]); // this gives the parsed address;
		parsedHexAddresses[counter] = strtol(parsedAddress,&pEnd,16);
	//	printf("%llx\n",parsedHexAddresses[counter]);

		counter++;
	}
	fclose(pFile);

	//---------------------------------------------------------------------------------------
	// ---------------------------------CacheSimulation--------------------------------------
	//---------------------------------------------------------------------------------------

	// What should I do for simulation?
		// first get the tagBits from the address
		// do set Selection using set index
		//do tag matching for each line
		//if there is a matching line, HIT!
		//if there isn't, Miss! and then do other stuff
		// the other stuff include: LRU eviction, differentiating between instructions

	p_cache.setSize = 2 << p_cache.setIndexBit;
	p_cache.blockSize = 2 << p_cache.blockBit;

	cacheMemory myCache;
	myCache.cacheSets = malloc(sizeof(cacheSet)*p_cache.setSize); //allocate space for all the sets
	for(int i = 0; i<p_cache.setSize;i++ ){
		myCache.cacheSets[i].cacheLines = malloc(sizeof(cacheLine)*p_cache.lines); // allocate space for each set
	}

	// initialize cache structure data
	for(int i = 0; i<p_cache.setSize;i++ ){
		for(int j = 0; j<p_cache.lines;j++ ){
		myCache.cacheSets[i].cacheLines[j].accessedCount=0;
		myCache.cacheSets[i].cacheLines[j].tagBit= 0;
		myCache.cacheSets[i].cacheLines[j].validBit=0;
	}
	}

	int countUp = 0;
	int Hit = 0; // 0 means miss, 1 means hit
	int evicted = 0;
	while(counter > countUp){
		if((*instructions[countUp]) != 'I'){// ignore I instruction
		//	int realAddress = addresses[countUp]; // somehow long long int to int convert the hex to dec
			long long int tagAddress = (parsedHexAddresses[countUp] >> (p_cache.blockBit + p_cache.setIndexBit));
			long long int cancelTag = tagAddress << (p_cache.setIndexBit); // variable to cancel out some bits for setIndex
			long long int setIndex = ((parsedHexAddresses[countUp] >> p_cache.blockBit) ^ cancelTag);
			Hit = 0;
			evicted = 0;
			if(verbosity == 1){
				printf("%s %s ",instructions[countUp],addresses[countUp]);
			}
		//	printf("address: %llx ",parsedHexAddresses[countUp]);
		//	printf("tag: %llx ",tagAddress);
		//	printf("set: %llx \n",setIndex);
			for(int i =0; i<p_cache.lines; i++){ // match each line with tagbits
				if(myCache.cacheSets[setIndex].cacheLines[i].tagBit == tagAddress
					&& myCache.cacheSets[setIndex].cacheLines[i].validBit == 1){ // if hit

					hitCount++;
					myCache.cacheSets[setIndex].cacheLines[i].accessedCount = countUp;
					Hit = 1;
					if((*instructions[countUp]) == 'M'){
						hitCount++;
						printf("hit ");
					}
					printf("hit\n");
					break;
				}
			} // end of for loop

			if(Hit == 0){ // if missed, load new data and optionally do LRU eviction
				int evictedLine;
				int setFull = 1;
				int minCount = countUp;
				int minIndex = 0;
				// first see whether the cache set has an empty line
				for (int i =0; i< p_cache.lines;i++){
					if(myCache.cacheSets[setIndex].cacheLines[i].validBit != 1){
						evictedLine = i;
						setFull = 0; // set is not full
						break;
					}
				}
				// if the cache set is full, evict LRU line
				if(setFull == 1){
					printf("Full ");
					for (int i =0; i< p_cache.lines;i++){
						//printf("cache: %d, min: %d ",myCache.cacheSets[setIndex].cacheLines[i].accessedCount,minCount);
						if(myCache.cacheSets[setIndex].cacheLines[i].accessedCount < minCount){
							minCount =myCache.cacheSets[setIndex].cacheLines[i].accessedCount;
							minIndex = i;
						}
					}
					evictedLine = minIndex;
					evictionCount++;
					evicted = 1;
				}
				myCache.cacheSets[setIndex].cacheLines[evictedLine].validBit = 1;
				myCache.cacheSets[setIndex].cacheLines[evictedLine].tagBit = tagAddress;
				myCache.cacheSets[setIndex].cacheLines[evictedLine].accessedCount = countUp;
				missCount++;
				printf("miss ");
				if(evicted == 1) // if a line was evicted
					printf("eviction"); // set:%d line:%d\n",evictedLine, minIndex);
				if((*instructions[countUp]) == 'M'){
					hitCount++;
					printf("hit");
				}
				printf("\n");
			}
			}// if valid instructions
			countUp++;
		}// end of while loop to go through every line of text files except 'I' instruction
	printSummary(hitCount, missCount, evictionCount); // hit count, miss count, eviction count
    return 0;
}
