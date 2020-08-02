#include <stdio.h>
#include "SpreaderDetectorParams.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define ALLOC_SIZE 10
#define END_FILE 0
#define VALID_ARGS 3
#define INPUT_ERROR "Error in input files.\n"

int gNumOfPeople = 0; //global value representing the number of lines in people.in, which
// corresponds to nodes in the graph data structure
int gNumOfMeetings = 0; //global value representing the number of lines in meeting.in, which
// corresponds to edges in the graph data structure

/**
 * stores the data of each person (each line in people.in)
 */
typedef struct Node
{
	float age, crna;
	char ID[MAX_LINE_LEN];
	char name[MAX_LINE_LEN];

} Node;

/**
 * represents the values that are given in meetings.in
 * time: time the src node (person) spent with the dest node (another person)
 * dest: how far a src node (person) was from dest node (another person)
 */
typedef struct Edge
{
	char srcID[MAX_LINE_LEN], destID[MAX_LINE_LEN];
	float time, dist; // from source to dest
} Edge;

/**
 * when allocation fails, this function is being called to free all memory, close all files
 * and output an error message
 * @param file - some file that should to be closed
 * @param array - some array that should be freed
 */
void peopleExitProtocol(FILE **file, Node **array)
{
	fprintf(stderr, INPUT_ERROR);
	fclose(*file);
	free(array);
	exit(EXIT_FAILURE);
}

void meetingExitProtocol(FILE **file, Edge **array)
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
	char name[MAX_LINE_LEN], id[MAX_LINE_LEN], age[MAX_LINE_LEN];
	sscanf(line, "%s %s %s", name, id, age);
	float ageFloat = strtof(age, NULL);
	Node person;
	strcpy(person.name, name);
	strcpy(person.ID, id);
	person.age = ageFloat;
	person.crna = 0;
	(*peopleArray)[gNumOfPeople] = person;
	gNumOfPeople++;
}

/**
 * given a name to people.in file, reads its data and stores it for later use.
 * @param peopleFileName - people.in file
 */
Node *readPeopleFile(char const *const peopleFileName)
{
	// Memory Allocation:
	int size = ALLOC_SIZE;
	Node *peopleArray = (Node *) malloc(size * sizeof(Node)); //all data in people.in
	if (peopleArray == NULL) // allocation failed
	{
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	// CHECKING VALID INPUT:
	FILE *peopleFile = fopen(peopleFileName, "r");
	fseek(peopleFile, END_FILE, SEEK_END);
	if (ftell(peopleFile) == END_FILE || peopleFileName == NULL) //no such file or file is empty
	{
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	fseek(peopleFile, END_FILE, SEEK_SET); // file exists and isn't empty - return to the beginning

	// Parsing Data:
	char personLine[MAX_LINE_LEN];
	while (fgets(personLine, (int) sizeof(personLine), peopleFile)) // each line is some person
	{
		if (size == gNumOfPeople) // this means we don't have enough space to allocate
		{
			size += ALLOC_SIZE;
			peopleArray = (Node *) realloc(peopleArray, size * sizeof(Node));
			if (peopleArray == NULL) //reallocation failed
			{
				peopleExitProtocol(&peopleFile, &peopleArray);
			}
		}
		// we are now able to parse the line properly:
		parsePersonLine(personLine, &peopleArray);
	}

	return peopleArray;
}

/**
 * extracts a meeting data from a line given from meeting.in
 * @param line - some line in the format <infector id> <infected id> <distance> <time>\n
 */
void parseMeetingLine(char line[], Edge **meetingArray)
{
	char id1[MAX_LINE_LEN], id2[MAX_LINE_LEN], time[MAX_LINE_LEN], dist[MAX_LINE_LEN];
	sscanf(line, "%s %s %s %s", id1, id2, dist, time);
	float timeFloat = strtof(time, NULL);
	float distFloat = strtof(dist, NULL);
	Edge meeting;
	strcpy(meeting.srcID, id1);
	strcpy(meeting.destID, id2);
	meeting.dist = distFloat;
	meeting.time = timeFloat;
	(*meetingArray)[gNumOfMeetings] = meeting;
	gNumOfMeetings++;
}

/**
 * given a name to meeting.in file, reads its data and stores it for later use.
 * @param meetingFileName - meeting.in file
 */
Edge *readMeetingFile(char const *const meetingFileName)
{
	// Memory Allocation:
	int size = ALLOC_SIZE;
	Edge *meetingArray = (Edge *) malloc(size * sizeof(Edge)); //all data in meeting.in
	if (meetingArray == NULL) // allocation failed
	{
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	// CHECKING VALID INPUT:
	FILE *meetingFile = fopen(meetingFileName, "r");
	fseek(meetingFile, END_FILE, SEEK_END);
	if (ftell(meetingFile) == END_FILE || meetingFileName == NULL) //no such file or file is empty
	{
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	fseek(meetingFile, END_FILE, SEEK_SET); // file exists & isn't empty - return to the beginning

	// Parsing Data:
	char meetingLine[MAX_LINE_LEN];
	fgets(meetingLine, (int) sizeof(meetingLine), meetingFile); // first line - Spreader
	char *spreaderID = meetingLine;

	while (fgets(meetingLine, (int) sizeof(meetingLine), meetingFile)) // each line is some meeting
	{
		if (size == gNumOfMeetings) // this means we don't have enough space to allocate
		{
			size += ALLOC_SIZE;
			meetingArray = (Edge *) realloc(meetingArray, size * sizeof(Edge));
			if (meetingArray == NULL) //reallocation failed
			{
				meetingExitProtocol(&meetingFile, &meetingArray);
			}
		}
		// we are now able to parse the line properly:
		parseMeetingLine(meetingLine, &meetingArray);
	}

	return meetingArray;
}

float getInfectionChance(Node person)
{

}

int floatComp(const void* a, const void *b)
{
	if(*(const float*)a < *(const float*)b)
	{
		return -1;
	}
	return *(const float*)a>*(const float*)b;
}
/**
 * based on the basic provided pseudo-code, for each person in people.in,
 * calculates the chance of infection, sorts the results and outputs value in severity order
 */
void calcOutput(Node *peopleArray)
{
	float *resultsArray = (float *) malloc(gNumOfPeople * sizeof(float));
	if(resultsArray==NULL)
	{
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < gNumOfPeople; i++)
	{
		float infectionChance = getInfectionChance(peopleArray[i]);
		resultsArray[i] = infectionChance;
	}
	qsort(resultsArray,gNumOfPeople,sizeof(float),floatComp);

}

int main(int argc, char *argv[])
{
	if (argc != VALID_ARGS)
	{
		fprintf(stderr, "Usage:./SpreaderDetectorBackend <Path to People.in> <Path to Meetings"
						".in>\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		char *peoplePath = argv[1];
		char *meetingPath = argv[2];
		Node *peopleArray = readPeopleFile(peoplePath);
		for (int i = 0; i < gNumOfPeople; i++)
		{
			printf("name = %s, id = %s, age = %f, crna = %f\n", peopleArray[i].name, peopleArray[i]
					.ID, peopleArray[i].age, peopleArray[i].crna);
		}
		printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		Edge *meetingArray = readMeetingFile(meetingPath);
		for (int i = 0; i < gNumOfMeetings; i++)
		{
			printf("src = %s, dest = %s, dist = %f, time= %f\n", meetingArray[i].srcID,
				   meetingArray[i].destID, meetingArray[i].dist, meetingArray[i].time);
		}

	}
}
