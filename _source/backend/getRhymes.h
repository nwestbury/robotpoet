#ifndef H_GET_RHYMES
#define H_GET_RHYMES

#define NUM_SYLLABLES 4

typedef struct{
	int rimeeId;
	int totalRhymingWords;
} rhymeFileHeader;

//header is followed by an array of rhymeTuples
typedef struct{
	unsigned int wordid;
	unsigned char rhymeStr;
	unsigned char relatedness;
}rhymeTuple;

rhymeTuple *getRhymes(const char *word, unsigned int *elements);

#endif