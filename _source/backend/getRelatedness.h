#ifndef H_GET_RELATEDNESS
#define H_GET_RELATEDNESS

typedef struct{
	unsigned int wordid;
	unsigned char relatedness;
	unsigned char rhymeLevel;
}tuple;
//tuples are used to sort while keeping the indexes (first = index) (second = percentage)

typedef struct{
	int releatedID;
	unsigned short totalWords;
} releatedFileHeader;

tuple *getReleatedness(const char *word, unsigned int *elements);

#endif