#define _CRT_SECURE_NO_WARNINGS

#include <mysql.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "libmysql.lib")


#define NUM_WORDS 177267

typedef struct{
	char word[32];
} word;


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

void outToFile(word *outArray, int nWords)
{
	FILE *f = fopen("idtoword.h", "wb");
	fprintf(f, "const char idtoword[%d][32] = { \"\", ", nWords+1);

	for (int i = 0; i < nWords; ++i) {
		fprintf(f, "\"%s\",", outArray[i].word);
	}

	fseek(f, -1, SEEK_CUR);
	fprintf(f, "};");
	fclose(f);
}

int main()
{
	MYSQL *conn = init();
	if (!mysql_real_connect(conn, "127.0.0.1", "root",
		"", "poetsthesaurus", 3306, NULL, 0))
		errorMessage(conn);

	//get all words, 177267

	word *wordarray = (word*)malloc(sizeof(word)*NUM_WORDS);

	if (mysql_query(conn, "SELECT `word` FROM `words` ORDER BY `id` ASC"))
		errorMessage(conn);

	MYSQL_RES *resi = mysql_use_result(conn);
	MYSQL_ROW rowi;

	for (int i = 0; (rowi = mysql_fetch_row(resi)); ++i)
	{
		strcpy(wordarray[i].word, rowi[0]);
		
		if (i % 5000 == 0)
			printf("%d\n", i);
	}

	int numrows = (int)mysql_num_rows(resi);
	printf("\ntotal: %d\n", numrows);

	outToFile(wordarray, numrows);
	return 0;
}