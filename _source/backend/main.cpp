#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "getRhymes.h"
#include "getRelatedness.h"
#include "arrayIntersection.h"

#include "idtoword.h"


int main(int argc, char **argv)
{
	unsigned int numberOfReleatedWords, nRhymes;
	
	if (argc != 3 && argc != 2) {
		printf("usage: %s [rhymedword] [lexicalword(optional)]\n", argv[0]);
		exit(1);
	}
	
	//clock_t start = clock();


	rhymeTuple *rhymeArray = getRhymes(argv[1], &nRhymes);
	
	if (argc == 3)
	{
		tuple *relatednessArray = getReleatedness(argv[2], &numberOfReleatedWords);
		intersect(relatednessArray, numberOfReleatedWords, rhymeArray, nRhymes);
	}

	//up to here, the program only takes about 0.838ms with cached files

	printf("[");
	for (unsigned int i = 0; i < nRhymes; ++i){
		printf("[\"%s\",%d,%d]", idtoword[rhymeArray[i].wordid], rhymeArray[i].rhymeStr, rhymeArray[i].relatedness);

		if (i != nRhymes - 1)
			printf(",");
	}
	printf("]");

	/*clock_t end = clock();
	double elapsed_time = (end - start) / (double)CLOCKS_PER_SEC;
	printf("Took about %.3lf seconds", elapsed_time);*/

	return 0;
}
