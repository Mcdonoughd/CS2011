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
};

void struct_init(int argc, char* argv[]){
	if(argc<3){
			printf("Please Initialize arguments!");
			exit(0);
		}
		for (int i = 2; i < argc; i++) {
			FILE *input = fopen(argv[i], "r");
			if(input == NULL) {
				fclose(input);
				printf("Error in locating file!\n");
				exit(0);
				printf("\n\n");
			}
			//assume there will be no words greater then 40 characters
			char* word = (char *) malloc(40);
			//while file is not ended, read the file per word
			while (!feof(input)) {
				fscanf(input, "%s", word);
				word = (char*)stripPunct(word);
				//printf("%s",word);
				if (strcmp(word, ""))
					//add item to the tree
					root = addItem(root, word);
			}
	}
}

int main(int argc, char* argv[]){
    struct_init(argc, argv);
    printSummary(hits, misses, evictions);
    return 0;
}
