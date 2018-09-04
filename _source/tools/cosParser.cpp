
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINES 57376
#define MAX_COLS 20000

//#define DIAGNOSTICS//comment out this line to disable diagnostics
#define THREADS 4

int linePosition = 19999;
CRITICAL_SECTION linePositionCriticalSection;
#define INCREMENT_SIZE 100


void readFileToBuffer(const char *fileName, char **fileBuffer)
{
	HANDLE file = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);

	DWORD error = GetLastError();

	if (file == INVALID_HANDLE_VALUE)
		MessageBox(NULL,"Input file not found or something..?", "eerrorrrr", 0);

	LARGE_INTEGER size;
	GetFileSizeEx(file, &size);

	long long fsize = size.QuadPart;

	*fileBuffer = (char*)VirtualAlloc(NULL, fsize + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	DWORD br, bytesToRead;

#define MAX_SIZE 4294967295//max size that you can read at once in bytes

	long long fsizecpy = fsize;

	while (fsize > 0) {//read in 4gb chunks
		if (fsize >= MAX_SIZE)
			bytesToRead = MAX_SIZE;
		else
			bytesToRead = (DWORD)fsize;

		ReadFile(file, (*fileBuffer) + br, bytesToRead, &br, 0);
		error = GetLastError();
		fsize -= br;
	}

	(*fileBuffer)[fsizecpy] = 0;
	CloseHandle(file);
}

void fileToFloats(char *fileBuf, float **outBuf)
{
	unsigned int row = 0;

	for (unsigned long long i = 0; fileBuf[i] != '\0'; ++i)
	{
		if (fileBuf[i] == '\n' || fileBuf[i] == '\r')//skip over newline characters
			continue;

		outBuf[row] = (float*)malloc((MAX_COLS+10) * sizeof(float));//allocate another row

		int col = 0;

		while (fileBuf[i] != '\n' //run through the row until one of these is hit
			&& fileBuf[i] != '\r' //each loop is one float value
			&& fileBuf[i] != '\0')
		{
			if (fileBuf[i] != ' ')
			{
				int end = 0;

				for (; fileBuf[i + end] != ' '
					&& fileBuf[i + end] != '\n'
					&& fileBuf[i + end] != '\r'
					&& fileBuf[i + end] != '\0'; ++end);
				//end is now the offset from fileBuf[i]  to the end of the decimal number

				fileBuf[i + end] = '\0';
				float n = (float)atof(fileBuf + i);
				outBuf[row][col] = n;

				//printf("%f\n", (*outBuf)[col][nNumber]);

				fileBuf[i + end] = ' ';//bring back the space to reduce parsing complexity
				i += end;
			}

			for (; fileBuf[i] != '\n'
				&& fileBuf[i] != '\r'; ++i)
			{
				if (fileBuf[i] == ' ') {//increment until we hit a space
					++i;//pass over space
					break;
				}
			}
			++col;
		}
		//as we run through the fileBuffer, free the memory behind us.
		VirtualFree(fileBuf, i - 1024 * 64, MEM_DECOMMIT);

		if (row >= MAX_LINES)
			break;

		++row;
	}
}

//for debug
void outToFile(char *buf, unsigned long long offset, unsigned int length)
{
	HANDLE file = CreateFile("test.txt", GENERIC_WRITE, 0, 0, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 0);

	DWORD br;
	WriteFile(file, buf + offset, length, &br, 0);
	CloseHandle(file);
}

void calSums(float *squareSums, float **outputBuf)
{
	//This functions calculates sqrt(SIGMA(COL^2))) for every row-- to lighten calculations later
	
	for (int i = 0; i < MAX_LINES; ++i)
	{
		float sumOfCol = 0.0;

		for (int j = 0; j < MAX_COLS; ++j)
			sumOfCol += outputBuf[i][j] * outputBuf[i][j];
		
		squareSums[i] = sqrt(sumOfCol);
	}
}

unsigned char findSimilarity(int wordID1, int wordID2, float **outputBuf, float *squareSums)
{
	//functions returns an unsigned char that corresponds to a # between 128 and 228 (0 and 100% similarity)
	//Why between these numbers? In order to avoid char 127 (DEL char) and annoying chars before 33
	/*if(wordID1 == wordID2) //extra check that isn't needed
	return 228; //100% match*/

	float numeratorSum = 0.0;

	for (int columnNumber = 0; columnNumber < MAX_COLS; ++columnNumber)
		numeratorSum += outputBuf[wordID1][columnNumber] * outputBuf[wordID2][columnNumber];

	if (numeratorSum == 0.0) //if the numerator is zero don't bother calculating the value
		return 128;

	//rounds the result to the nearest integer (add .5 and floor) the result is contained between 128 and 228
	return (unsigned char)floor((numeratorSum / (squareSums[wordID1] * squareSums[wordID2])) * 100 + .5) + 128;
}

typedef struct {
	float **inBuf;
	unsigned char **outBuf;
	float *squareSums;
} calc_thread_params;

bool getNextLines(int *start, int *end)
{
	EnterCriticalSection(&linePositionCriticalSection);

	*start = linePosition;

	if (linePosition + INCREMENT_SIZE < MAX_LINES)
	{
		*end = linePosition + INCREMENT_SIZE;
		linePosition += INCREMENT_SIZE;
	}
	else
	{
		*end = MAX_LINES;
		linePosition = MAX_LINES;
	}

	LeaveCriticalSection(&linePositionCriticalSection);

	//each . represents INCREMENT_SIZE lines are parsed
	printf(".");

	return (*start != *end);
}

DWORD WINAPI simularityCalcThread(LPVOID lpParameter)
{	
	calc_thread_params *params = (calc_thread_params*)lpParameter;
	int start, end;


	while (getNextLines(&start, &end))
	{
		for (int i = start; i < end; ++i)
		{
			params->outBuf[i] = (unsigned char*)malloc(MAX_LINES * sizeof(unsigned char));//allocate another line

			for (int k = 0; k <= i; ++k)
				//place in duplicate locations with 'x's (120) or if the same word is checked against the same word
				params->outBuf[i][k] = 'x';

			for (int j = i + 1; j < MAX_LINES; ++j)
				params->outBuf[i][j] = findSimilarity(i, j, params->inBuf, params->squareSums);
		}
	}

	return 0;
}

void initCalcThreads(float **inBuf, unsigned char **outBuf, float *squareSums)
{
	const int threads = THREADS;

	//allocate one param stuct for each thread
	calc_thread_params *params = (calc_thread_params *)malloc(threads * sizeof(calc_thread_params));
	HANDLE *threadHandles = (HANDLE*)malloc((sizeof(HANDLE) * threads));

	int linesPerThread = MAX_LINES / threads;


	for (int i = 0; i < threads; ++i)
	{
		params[i].inBuf = inBuf;
		params[i].outBuf = outBuf;
		params[i].squareSums = squareSums;

		threadHandles[i] = CreateThread(NULL, 0, simularityCalcThread, &(params[i]), 0, NULL);
	}

	WaitForMultipleObjects(threads, threadHandles, TRUE, INFINITE);

	free(params);
	free(threadHandles);
}

void getSimilarityPercentages(char *filename, unsigned char **simBuf)
{
	char *fileBuffer;
	float **floatFileBuf;
	float preComputedSums[MAX_LINES];

	//alocate the pointers for the rows
	floatFileBuf = (float**)malloc(MAX_LINES * sizeof(float*));
	
	printf("Reading file into memory...\n");
	readFileToBuffer(filename, &fileBuffer);

	printf("Parsing file...\n");
	fileToFloats(fileBuffer, floatFileBuf);
	//you now have a 57xxx*20000 2d array of floats in floatFileBuf

#if defined(DIAGNOSTICS)
	for (int i = 0; i < MAX_LINES; ++i)//rows
	{
		for (int j = 0; j < MAX_COLS; ++j)//columns
		{
			if (j % 1000 == 0)//print every 1000th number or the first of a line
				printf("%2.1f ", floatFileBuf[i][j]);
		}
		printf("\n");
	}
#endif

	VirtualFree(fileBuffer, 0, MEM_RELEASE);//free the 4gb file

	printf("Performing precalculations...\n");
	calSums(preComputedSums, floatFileBuf); //fills the preComputedSums Array

#if defined(DIAGNOSTICS)
	//print precalced sums
	for (int i = 0; i < MAX_LINES; ++i)
	{
		printf("%5.2f\n", preComputedSums[i]);
	}
#endif	
	
	printf("Converting cosine to similarity percentage");

	InitializeCriticalSectionAndSpinCount(&linePositionCriticalSection, 0x00000400);
	initCalcThreads(floatFileBuf, simBuf, preComputedSums);

	DeleteCriticalSection(&linePositionCriticalSection);

	for (int i = 0; i < MAX_LINES; ++i)//free floatFile buffer
	{
		free(floatFileBuf[i]);
		floatFileBuf[i] = NULL;
	}
	//free(floatFileBuf);//y u crash ;_;
}

int main()
{
	unsigned char **simBuf;
	simBuf = (unsigned char**)malloc(MAX_LINES * sizeof(unsigned char*));

	printf("Cosine Parser, by Nathan and Nicholas. Parsing %d lines.\n\n", MAX_LINES);

	getSimilarityPercentages("wikipedia.gcm", simBuf);


	printf("\nOutputting results...\n");

	FILE *f = fopen("output.txt", "w");

	for (int i = 0; i < MAX_LINES; ++i) {//rows
		int count = 0;

		for (int j = 0; j < MAX_LINES; ++j) {//columns
			//if (j % 1000 == 0)//print every 1000th number or the first of a line

			if (simBuf[i][j] - 128 == -8) {
				count++;
			}
			else {
				fprintf(f, "%d,", count);
				count = 0;
			}

			if (count == 0)
				fprintf(f, "%d,", (int)simBuf[i][j] - 128);
		}
		fprintf(f, "\n");
	}

	fclose(f);
	printf("\nDone.\n");

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}