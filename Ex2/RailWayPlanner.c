#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

#define OFFSET 4 // 4 lines in the file are guaranteed
#define ALLOC_SIZE 10 // memory block size for initializing array of parts
#define LINE_LEN 1024 // maximum length of a line
/**
 * object of type rail-way part, which has 4 traits:
 * -starting connection
 * -ending connection
 * -price
 * -length
 */
typedef struct RailWayPart
{
	char start, end;
	int length, price;
	int idx;
	// this index will be used to get S_i in O(1) (and not O(n), iterating over array of size K.
} RailWayPart;

FILE *gOutFile; // output file with results
FILE *gInFile; // input file with data

int gLineCount = 1;
int gNumOfParts = 0;
int gRailLength = 0;

char *gArgumentErrors = "Usage: RailwayPlanner <InputFile>";
char *gNoFileError = "File doesn't exists.";
char *gEmptyFileError = "File is empty.";
char *gGeneralError = "Invalid input in line: ";

char *gAllowedParts = NULL; // all parts allowed according to line 3 in file.
RailWayPart *gPartsArray = NULL; // array of all parts in file (line 4 onwards).
int **gDataStruct = NULL; // table of size (L+1)*K for the dynamic algorithm.

/**
 * exits input and output files, and assigns NULL pointers
 */
void exitFiles()
{
	fclose(gOutFile);
	fclose(gInFile);
	gOutFile = NULL;
	gInFile = NULL;
}

/**
 * exit protocol when argc!=2
 */
void argExit()
{
	fprintf(gOutFile, "%s", gArgumentErrors);
	fclose(gOutFile);
	gOutFile = NULL;
}

/**
 * exit protocol when InFile = NULL
 * @param error - some informative error string
 */
void noInFileError(char *error)
{
	fprintf(gOutFile, "%s", error);
	fclose(gOutFile);
	gOutFile = NULL;
}

/**
 * exit protocol for an error regarding files
 * @param error - some informative error string
 */
void fileError(char *error)
{
	fprintf(gOutFile, "%s", error);
	exitFiles();
}

/**
 * exit protocol for code preceding any memory initializaion
 * @param error  some informative error string
 * @param lineCount current line to print
 */
void beforeMallocError(char *error, int lineCount)
{
	fprintf(gOutFile, "%s%d.", error, lineCount);
	exitFiles();
}

/**
 * exit protocol regarding allowed parts array(gAllowedParts)
 */
void allowedPartsExit()
{
	int currLine = gLineCount - 1;
	fprintf(gOutFile, "%s%d.", gGeneralError, currLine);
	free(gAllowedParts);
	exitFiles();
}

/**
 * exit protocol regarding all parts array (gPartsArray)
 */
void partsArrayExit()
{
	free(gAllowedParts);
	gAllowedParts = NULL;
	exitFiles();
}

/**
 * exit protocol regarding realloc failures
 */
void reallocExit()
{
	free(gPartsArray);
	gPartsArray = NULL;
	partsArrayExit();
}

/**
 * exit protocol regarding failure in initializing a part struct
 */
void singlePartExit()
{
	fprintf(gOutFile, "%s%d.", gGeneralError, gLineCount);
	free(gPartsArray);
	gPartsArray = NULL;
	partsArrayExit();
}

/**
 * exit protocol regarding null pointer of gDataStructure
 */
void noDastExit()
{
	free(gPartsArray);
	gPartsArray = NULL;
	partsArrayExit();
}

/**
 * exit protocol regarding gDataStructure initialization failure
 * @param idxOfFail
 */
void dastExit(int idxOfFail)
{
	noDastExit();
	for (int i = 0; i < idxOfFail; i++)
	{
		free(gDataStruct[i]);
		gDataStruct[i] = NULL;
	}
	free(gDataStruct);
	gDataStruct = NULL;
}

/**
 * frees all memory from system
 */
void freeAll()
{
	dastExit(gRailLength + 1);
}

/**
 * checks if the length provided is legit
 * @param lengthNewLine - a string containing some length  + "\n"
 * @return 1 if is INT and >= 0. otherwise returns 0.
 */
int badLength(char *length)
{
	float lengthFloat = (float) strtod(length, NULL);
	if (floorf(lengthFloat) == lengthFloat && lengthFloat >= 0)
	{ //lengthFloat is valid (not invalid)
		gRailLength = (int) lengthFloat;
		return false;
	}
	return true;
}

/**
 * checking if the price of a certain value or its length are valid
 * @param value
 * @return
 */

int notInt(const char *value)
{
	for (int i = 0; value[i] != '\0'; i++)
	{
		if (isdigit(value[i]) == 0)
		{ // value is not a number
			return true;
		}
	}
	return false;
}

/**
 * checks if the num of connections provided is legit
 * @param partsNewLine - a string containing some number of connections  + "\n"
 * @return 1 if is INT and >= 0. otherwise returns 0.
 */
int badNumOfParts(char *parts)
{
	float partsFloat = (float) strtod(parts, NULL);
	if (floorf(partsFloat) == partsFloat && partsFloat > 0)
	{ //partsFloat is valid (not invalid)
		gNumOfParts = (int) partsFloat;
		return false;
	}
	return true;
}

/**
 * adding the parts allowed to the global array gAllowedParts
 * @param parts parts as specified in line 3 of the file
 */
void initializeParts(char *parts)
{
	char *part;
	int count = 0;
	part = strtok(strtok(parts, "\n"), ",");
	while (part != NULL)
	{
		if (strlen(part) != 1) //invalid part
		{
			allowedPartsExit();
			exit(EXIT_FAILURE);
		}
		gAllowedParts[count] = *part;
		count++;
		part = strtok(NULL, ",");
	}
}

/**
 * checks if some string is in an array
 * @param n length of array
 * @param array
 * @param str
 * @return 1 if in, 0 otherwise
 */
int strInArray(int n, char *array, char *str)
{
	int i;
	if (strlen(str) != 1)
	{
		return false;
	}
	else
	{
		for (i = 0; i < n; i++)
		{
			if (array[i] == str[0])
			{
				return true;
			}
		}
	}
	return false;

}


/**
 * returns the index corresponding to start in the array gAllowedParts
 * @param start start part of some railway part
 * @return index of said start connection
 */
int getIdxOfPart(char start)
{
	int idx = 0;
	for (int i = 0; i < gNumOfParts; i++)
	{
		if (gAllowedParts[i] == start)
		{
			return i;
		}
	}
	return idx;
}


/**
 * Main function #2 - for every line in the code regarding parts (4 onwards)
 *checks if the content is ok and
 * @param line
 */
void parseParts(char line[])
{
	char s[LINE_LEN], e[LINE_LEN], l[LINE_LEN], p[LINE_LEN];
	int intL, intP; //p and l as integers (hence the i)
	sscanf(line, "%[^,], %[^,], %[^,], %[^\n]\n", s, e, l, p);
	if (notInt(l) || notInt(p)) //invalid l/p
	{
		singlePartExit();
		exit(EXIT_FAILURE);
	}
	//converting to int
	sscanf(p, "%d", &intP);
	sscanf(l, "%d", &intL);
	if (intP <= 0 || intL == 0)
	{
		singlePartExit();
		exit(EXIT_FAILURE);
	}
	if ((!strInArray(gNumOfParts, gAllowedParts, s)) ||
		(!strInArray(gNumOfParts, gAllowedParts, e)))
	{
		singlePartExit();
		exit(EXIT_FAILURE);
	}
	// input is O-K -> building struct & adding to global array of parts:
	int kIndex = getIdxOfPart(s[0]); // index of s part in the array gAllowedArrays
	RailWayPart rail = {.start = *s, .end = *e, .length = intL, .price = intP, .idx = kIndex};
	int i = gLineCount - OFFSET;
	gPartsArray[i] = rail;
}

/**
 *Main function #1 -parse every line in the code according to specification
 * @param fileName
 */
void parseFile(char const *const fileName)
{
	int size = ALLOC_SIZE;
	gInFile = fopen(fileName, "r");
	if (gInFile == NULL) // no file
	{
		noInFileError(gNoFileError);
		exit(EXIT_FAILURE);
	}
	fseek(gInFile, 0, SEEK_END); // goto end of file
	if (ftell(gInFile) == 0)
	{
		fileError(gEmptyFileError); // file is empty
		exit(EXIT_FAILURE);
	}
	fseek(gInFile, 0, SEEK_SET); // file not empty - return to the beginning
	char line[LINE_LEN];
	fgets(line, (int) sizeof(line), gInFile); //Length
	char *parsedLine; // removing all /r/n ending from the line
	parsedLine = strtok(line, "\n");
	parsedLine = strtok(parsedLine, "\r");
	if (badLength(parsedLine) || notInt(parsedLine))
	{
		beforeMallocError(gGeneralError, gLineCount); // invalid length
		exit(EXIT_FAILURE);
	}
	gLineCount++;

	fgets(line, (int) sizeof(line), gInFile); // Number of connections
	parsedLine = strtok(line, "\n"); // removing all /r/n ending from the line
	parsedLine = strtok(parsedLine, "\r");
	if (badNumOfParts(parsedLine) || notInt(parsedLine))
	{
		beforeMallocError(gGeneralError, gLineCount);
		exit(EXIT_FAILURE);
	}
	gLineCount++;

	// dealing allowed parts (line 3)
	gAllowedParts = (char *) malloc(gNumOfParts * (sizeof(char) + 1));
	if (gAllowedParts == NULL)
	{
		exitFiles();
		exit(EXIT_FAILURE);
	}
	gLineCount++;

	fgets(line, (int) sizeof(line), gInFile); // parts specification
	parsedLine = strtok(line, "\n"); // removing all /r/n ending from the line
	parsedLine = strtok(parsedLine, "\r");
	initializeParts(parsedLine);

	//Now dealing all the parts specification
	gPartsArray = (RailWayPart *) malloc(size * sizeof(RailWayPart));
	if (gPartsArray == NULL)
	{
		partsArrayExit();
		exit(EXIT_FAILURE);
	}
	while (fgets(line, (int) sizeof(line), gInFile)) // each line is some railway part
	{
		parsedLine = strtok(line, "\n");
		parsedLine = strtok(parsedLine, "\r");
		if (size == gLineCount - OFFSET) // not enough space
		{
			size += ALLOC_SIZE;
			gPartsArray = (RailWayPart *) realloc(gPartsArray, size * sizeof(RailWayPart));
			if (gPartsArray == NULL)
			{
				reallocExit();
				exit(EXIT_FAILURE);
			}
		}
		parseParts(parsedLine);
		gLineCount++;
	}
}

/**
 * initialises the table data structure gDataStruct as a table (L+1)*K
 * L is the length of the rail = gRailLength
 * K is the number of parts = gNumOfParts
 * all values are strings (representation of prices)
 */
void initialiseDast()
{
	gDataStruct = (int **) malloc((gRailLength + 1) * sizeof(int *));
	if (gDataStruct == NULL)
	{
		noDastExit();
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < gRailLength + 1; i++)
	{
		gDataStruct[i] = (int *) malloc(gNumOfParts * sizeof(int));
		if (gDataStruct[i] == NULL)
		{
			dastExit(i);
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * helper function that iterates over all possible values
 * (main process of the minimalprice algorithm
 * @param l length
 * @param k parts
 * @param i rails
 * @param currMinVal
 * @param minVal final minimum value
 */
void getMinVal(int l, int k, int i, int *currMinVal, int *minVal)
{
	int price, length, idx;
	char end;
	end = gPartsArray[i].end;
	length = gPartsArray[i].length;
	price = gPartsArray[i].price;
	idx = gPartsArray[i].idx;
	//e_i=k and l-d_i>=0:
	if (end == gAllowedParts[k] && (l - length >= 0))
	{
		*currMinVal = price + gDataStruct[l - length][idx];
		if (*currMinVal < 0) // overflow
		{
			*currMinVal = INT_MAX;
		}
		if (*currMinVal < *minVal)
		{
			*minVal = *currMinVal;
		}
	}
}

/**
 * main function #3 - initializes the dynamic algorithm
 * @return minimum price according to the problems specification
 */

int minimalPrice()
{
	int minVal = INT_MAX, currMinVal = 0;
	int l, k, i;
	for (l = 0; l < gRailLength + 1; l++) // lengths
	{
		for (k = 0; k < gNumOfParts; k++) // connections
		{
			if (l == 0)
			{
				gDataStruct[l][k] = 0; // base case l=0
			}
			else
			{
				for (i = 0; i < gLineCount - OFFSET; i++) //rails
				{
					getMinVal(l, k, i, &currMinVal, &minVal);
				}
				gDataStruct[l][k] = minVal;
				minVal = INT_MAX;
			}
		}
	}
	int returnVal = INT_MAX, currVal;
	// now getting the GLOBAL Min value:
	for (k = 0; k < gNumOfParts; k++)
	{
		currVal = gDataStruct[gRailLength][k];
		if (returnVal > currVal)
		{
			returnVal = currVal;
		}
	}
	if (returnVal == INT_MAX)
	{
		returnVal = -1;
	}
	fprintf(gOutFile, "The minimal price is: %d", returnVal);
	return returnVal;
}

/**
 * main function - calls the main functions in the following order:
 * opens outputfile -> parses file -> initializes table data structure -> finds minimal price
 * -> frees all remaining memory
 * @param argc number of arguments
 * @param argv array of arguments (should contain only name of file)
 * @return 0 upon success. 1 otherwise.
 */
int main(int argc, char *argv[])
{
	// initialize a file to write into:
	gOutFile = fopen("railway_planner_output.txt", "w");
	if (argc != 2)
	{
		argExit();
		exit(EXIT_FAILURE);
	}
	char const *const fileName = argv[1];
	parseFile(fileName);
	initialiseDast();
	minimalPrice();
	freeAll();
	return 0;
}
