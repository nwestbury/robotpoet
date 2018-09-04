#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "getRelatedness.h"

bool compare_tup(tuple a, tuple b){
  return a.wordid < b.wordid;
}

tuple *getReleatedness(const char *word, unsigned int *elements)
{
	*elements = 0;
	
	if (strstr(word, "..") != 0)//for security
		return (tuple*)NULL;
		
	char pathBuf[128];
	sprintf(pathBuf, "related/f.%s", word);
	
	FILE *pFile = fopen (pathBuf, "rb");
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
	
	releatedFileHeader header;
	fread(&header, sizeof(releatedFileHeader), 1, pFile);
	
	int numberOfWords = header.totalWords;
  
	tuple *infoArray = (tuple*) malloc(sizeof(tuple)*numberOfWords);
	fread(infoArray, sizeof(tuple), numberOfWords, pFile);
	fclose(pFile);
	
	*elements = numberOfWords;
	std::sort(infoArray, infoArray+numberOfWords, compare_tup);

	return infoArray;
}
