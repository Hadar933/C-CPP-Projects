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

/**
 * when allocation fails, this function is being called to free all memory, close all files
 * and output an error message
 * @param file - some file that should to be closed
 * @param array - some array that should be freed
 */
void allocationExitProtocol(FILE** file, Node** array)
{
	fprintf(stderr, INPUT_ERROR);
	fclose(*file);
	free(array);
	exit(EXIT_FAILURE);
}
/**
 * extracts a person data from a line given from people.in
 * @param line - some line in the format <Person name> <Person ID> <Person age>\n
 */
void parsePersonLine(char line[], Node **peopleArray)
{
	char personName[MAX_LINE_LEN], charID[MAX_LINE_LEN], charAge[MAX_LINE_LEN];
	sscanf(line, "%[^ ], %[^ ], %[^\n]\n", personName, charID, charAge);
	float personAge = strtof(charAge, NULL); //converts the age to float value
	int personID = (int) strtof(charID, NULL); //converts the ID to int value
	Node person = {.name=*personName, .age=personAge, .ID=personID, .next=NULL};
	peopleArray[gNumOfNodes] = &person;
}

/**
 * given a name to people.in file, reads its data and stores it for later use.
 * @param peopleFileName - people.in file
 */
void readPeopleFile(char const *const peopleFileName)
{
	// Memory Allocation:
	int size = ALLOC_SIZE;
	Node *peopleArray = (Node *) malloc(size*sizeof(Node)); // representing all data in people.in
	if(peopleArray==NULL) // allocation failed
	{
		fprintf(stderr,STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	// CHECKING VALID INPUT:
	FILE *peopleFile = fopen(peopleFileName, "r");
	if (ftell(peopleFile) == END_FILE || peopleFileName == NULL) //no such file or file is empty
	{
		allocationExitProtocol(&peopleFile,&peopleArray);
	}
	fseek(peopleFile, END_FILE, SEEK_SET); // file exists and isn't empty - return to the beginning

	// Parsing Data:
	char personLine[MAX_LINE_LEN];
	while (fgets(personLine, (int) sizeof(personLine), peopleFile)) // each line is some person
	{
		if(size==gNumOfNodes) // this means we don't have enough space to allocate -> get some more
		{
			size+=ALLOC_SIZE;
			peopleArray = (Node*) realloc(peopleArray,size*sizeof(Node));
			if (peopleArray==NULL) //reallocation failed
			{
				allocationExitProtocol(&peopleFile,&peopleArray);
			}
		}
		// we are now able to parse the line properly:
		parsePersonLine(personLine, &peopleArray);
		gNumOfNodes++;
	}


}


int main()
{

}
