#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getRhymes.h"


rhymeTuple *getRhymes(const char *word, unsigned int *elements)
{
	rhymeFileHeader fh;
	*elements = 0;

	if (strstr(word, "..") != 0)//for security
		return 0;

	char pathBuf[128];
	sprintf(pathBuf, "rhymes/f.%s", word);

	FILE *f = fopen(pathBuf, "rb");
	fread(&fh, sizeof(rhymeFileHeader), 1, f);

	int nwords = fh.totalRhymingWords;

	rhymeTuple *rt = (rhymeTuple*)malloc(nwords*sizeof(rhymeTuple));

	int read = fread(rt, sizeof(rhymeTuple), nwords, f);

	fclose(f);

	for (int i = 0; i < nwords; ++i){
		rt[i].relatedness = 0;
	}
	

	*elements = nwords;
	return rt;
}