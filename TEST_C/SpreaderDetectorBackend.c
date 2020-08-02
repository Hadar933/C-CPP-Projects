#include <stdio.h>
#include "SpreaderDetectorParams.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define ALLOC_SIZE 10
#define END_FILE 0
#define INPUT_ERROR "Error in input files.\n"

int gNumOfNodes = 0; //global value representing the number of lines in people.in, which
// corresponds to nodes in the graph data structure

//FILE *gPeopleFile;
//FILE *gMeetingFile;

/**
 * stores the data of each person (each line in people.in)
 */
typedef struct Node
{
	float age;
	int ID;
	char name[MAX_LINE_LEN];
	struct Node *next;
} Node;

/**
 * represents the values that are given in meetings.in
 * time: time the src node (person) spent with the dest node (another person)
 * dest: how far a src node (person) was from dest node (another person)
 */
typedef struct Edge
{
	Node *src, dest;
	float time, dist; // from source to dest
} Edge;

typedef struct Graph
{

} Graph;

/**
 * extracts a person data from a line given from people.in
 * @param line - some line in the format <Person name> <Person ID> <Person age>\n
 */
void parsePersonLine(char line[])
{
	char personName[MAX_LINE_LEN], charID[MAX_LINE_LEN], charAge[MAX_LINE_LEN];
	float personAge = 0;
	int personID =0;
	sscanf(line, "%[^ ], %[^ ], %[^\n]\n", personName, charID, charAge);
	strtof(charAge, (char **) &personAge); //converts the age to float value
	strtof(charID, (char **) &personID); //converts the ID to int value
	Node person = {.name=*personName,.age=personAge,.ID=*charID,.next=NULL};
}

/**
 * given a name to people.in file, reads its data and stores it for later use.
 * @param peopleFileName - people.in file
 */
void readPeopleFile(char const *const peopleFileName)
{
	FILE *peopleFile = fopen(peopleFileName, "r");
	// CHECKING VALID INPUT: no such file or file is empty
	if (ftell(peopleFile) == END_FILE || peopleFileName == NULL)
	{
		fprintf(stderr, INPUT_ERROR);
		fclose(peopleFile);
		exit(EXIT_FAILURE);
	}
	fseek(peopleFile, END_FILE, SEEK_SET); // file exists and isn't empty - return to the beginning
	char personLine[MAX_LINE_LEN];
	while (fgets(personLine, (int) sizeof(personLine), peopleFile)) // each line is some person
	{
		gNumOfNodes++;
		parsePersonLine(personLine);
	}


}


int main()
{

}
