#define _CRT_SECURE_NO_WARNINGS

#include <mysql.h>
#include <stdio.h>
#include <string.h>

#define NUM_SYLLABLES 4
#define MAX_RHYMES 50000


typedef struct{
	char word[32];
	int wordid;
} wordTuple;


typedef struct{
	int rimeeId;
	int totalRhymingWords;
} rhymeFileHeader;

//header is followed by an array of rhymeTuples
typedef struct{
	unsigned long wordid;
	unsigned char rhymeStr;
}rhymeTuple;


void errorMessage(MYSQL *mysql)
{
	printf("%s\n", mysql_error(mysql));
	exit(1);
}

MYSQL *init()
{
	mysql_library_init(0, NULL, NULL);
	//printf("MySQL client: %s\n", mysql_get_client_info());

	return mysql_init(NULL);
}

void outToFile(char *word, int wordid, rhymeTuple *outArrayIds, int nTuples)
{
	char pathbuf[128];

	//windows does not support files with the name 'aux' for historical reasons
	//so we need to slightly modify the filename with an arbitrary prefix

	sprintf(pathbuf, "rhymefiles/f.%s", _strlwr(word));
	FILE *f = fopen(pathbuf, "wb");

	rhymeFileHeader header = { wordid, nTuples };


	fwrite(&header, sizeof(rhymeFileHeader), 1, f);
	fwrite(outArrayIds, sizeof(rhymeTuple), nTuples, f);

	fclose(f);
}

int isInArray(rhymeTuple *fullArray, int elements, int checkedID)
{
	for (int i = 0; i < elements; ++i)
	if (fullArray[i].wordid == checkedID)
		return (i + 1);
	return 0;
}

int comparator(const void *a, const void *b)
{
	return ((rhymeTuple*)a)->wordid - ((rhymeTuple*)b)->wordid;
}

void getRhymes(char *word, int wordid, MYSQL *conn)
{
	rhymeTuple *fullArray = (rhymeTuple*)malloc(MAX_RHYMES * sizeof(rhymeTuple));

	char cleanword[66];

	mysql_real_escape_string(conn, cleanword, word, strlen(word));

	/* send SQL query */
	char query[512];
	sprintf(query,
"SELECT endSyllable1, endSyllable2, endSyllable3, endSyllable4 \
FROM words WHERE id=%d", wordid);

	if (mysql_query(conn, query))
		errorMessage(conn);

	MYSQL_RES *res = mysql_use_result(conn);
	MYSQL_ROW row = mysql_fetch_row(res);

	if (row == NULL)
		errorMessage(conn);

	int nSyllables = 0; //get the number of syllables returned
	char sylBuffer[4][17];

	for (int i = NUM_SYLLABLES - 1; i >= 0; --i){
		if (nSyllables == 0 && row[i][0] != '\0')
			nSyllables = i + 1;

		strcpy(sylBuffer[i], row[i]); //copy rhyme syllables to array
	}
	mysql_free_result(res);


	int skipWeakRhymes = (nSyllables>1); //skip weak rythmes if there are more than 1 syllable
	unsigned int indexOfRhymeArray = 0;

	for (int sylStrength = nSyllables; sylStrength > skipWeakRhymes; --sylStrength)
	{
		char queryBuffer[70];
		sprintf(queryBuffer, "SELECT id FROM words WHERE endSyllable%d='%s'", sylStrength, sylBuffer[(sylStrength - 1)]);

		if (mysql_query(conn, queryBuffer))
			errorMessage(conn);

		MYSQL_RES *results = mysql_use_result(conn);
		while ((row = mysql_fetch_row(results)) != NULL)
		{
			int iID = atoi(row[0]);

			if (!isInArray(fullArray, indexOfRhymeArray, iID)) {
				fullArray[indexOfRhymeArray].rhymeStr = (char)sylStrength;
				fullArray[indexOfRhymeArray++].wordid = iID; //copy rhyme id to array
			}
		}
		mysql_free_result(results);		
	}
	qsort(fullArray, indexOfRhymeArray, sizeof(rhymeTuple), comparator);
	outToFile(word, wordid, fullArray, indexOfRhymeArray);

	free(fullArray);
}


int main()
{
	MYSQL *conn = init();
	if (!mysql_real_connect(conn, "127.0.0.1", "root",
		"", "poetsthesaurus", 3306, NULL, 0))
		errorMessage(conn);
	
#define COL_WORD 0
#define COL_ID 1

#define CHUNK_SIZE 500

	//get all words, 174798

	char queryBuf[256];
	int offset = 0;


	wordTuple *tempStorage = (wordTuple*)malloc(sizeof(wordTuple)*CHUNK_SIZE);

	while (true)
	{
		sprintf(queryBuf, "SELECT word, id FROM words WHERE endSyllable1 != '' LIMIT %d OFFSET %d", CHUNK_SIZE, offset);

		if (mysql_query(conn, queryBuf))
			errorMessage(conn);

		MYSQL_RES *resi = mysql_use_result(conn);
		MYSQL_ROW rowi;
		
		
		int i = -1;
		while ((rowi = mysql_fetch_row(resi)) != NULL)
		{
			++i;
			strcpy(tempStorage[i].word, rowi[COL_WORD]);
			tempStorage[i].wordid = atoi(rowi[COL_ID]);
		}

		int numrows = (int)mysql_num_rows(resi);
		
		if (numrows == 0)
			break;

		mysql_free_result(resi);


		for (i = 0; i < numrows; ++i) {
			getRhymes(tempStorage[i].word, tempStorage[i].wordid, conn);
			printf("\r%d", offset + i);
		}

		offset += CHUNK_SIZE;
	}

	mysql_close(conn);
	mysql_library_end();
	return 0;
}