#include <stdio.h>
#include "SpreaderDetectorParams.h"
#include <stdlib.h>
#include <string.h>


#define SUCCESS 0
#define BASE 10 
#define MAX_CRNA 1
#define MIN_CRNA 0
#define NEG_RETURN -1
#define START_IDX 0
#define NO_SUCH_ITEM -1
#define MAX_LINE_LEN 1024
#define ALLOC_SIZE 10
#define VALID_ARGS 3
#define USAGE_ERR "Usage:./SpreaderDetectorBackend <Path to People.in> <Path to Meetings.in>\n"
#define INPUT_ERR "Error in input file.\n"

int gNumOfPeople = 0; //global value representing the number of lines in people.in, which
// corresponds to nodes in the graph data structure
int gNumOfMeetings = 0; //global value representing the number of lines in meeting.in, which
// corresponds to edges in the graph data structure

/**
 * stores the data of each person (each line in people.in)
 */
typedef struct Person
{
	float age, crna;
	char ID[MAX_LINE_LEN];
	char name[MAX_LINE_LEN];

} Person;

/**
 * represents the values that are given in meetings.in
 * time: time the src person spent with the dest (another person)
 * dest: how far the src (person) was from dest (another person)
 */
typedef struct Edge
{
	char srcID[MAX_LINE_LEN], destID[MAX_LINE_LEN];
	float time, dist; // from source to dest
} Edge;

/**
 * extracts a person data from a line given from people.in
 * @param line - some line in the format <Person name> <Person ID> <Person age>\n
 */
void parsePersonLine(char line[], Person **peopleArray)
{
	char name[MAX_LINE_LEN], id[MAX_LINE_LEN], age[MAX_LINE_LEN];
	sscanf(line, "%s %s %s", name, id, age);
	float ageFloat = strtof(age, NULL);
	Person person;
	strcpy(person.name, name);
	strcpy(person.ID, id);
	person.age = ageFloat;
	person.crna = MIN_CRNA;
	(*peopleArray)[gNumOfPeople] = person;
	gNumOfPeople++;
}

/**
 * given a name to people.in file, reads its data and stores it for later use.
 * @param peopleFileName - people.in file
 */
Person *readPeopleFile(char const *const peopleFileName)
{
	// Memory Allocation:
	int size = ALLOC_SIZE;
	Person *peopleArray = (Person *) malloc(size * sizeof(Person)); //all data in people.in
	if (peopleArray == NULL) // allocation failed
	{
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	// CHECKING VALID INPUT:
	FILE *peopleFile = fopen(peopleFileName, "r");
	if (peopleFile == NULL) //no such file
	{
		fprintf(stderr, INPUT_ERR);
		free(peopleArray);
		exit(EXIT_FAILURE);
	}
	// Parsing Data:
	char personLine[MAX_LINE_LEN];
	while (fgets(personLine, (int) sizeof(personLine), peopleFile)) // each line is some person
	{
		if (size == gNumOfPeople) // this means we don't have enough space to allocate
		{
			size += ALLOC_SIZE;
			peopleArray = (Person *) realloc(peopleArray, size * sizeof(Person));
			if (peopleArray == NULL) //reallocation failed
			{
				fclose(peopleFile);
			}
		}
		// we are now able to parse the line properly:
		parsePersonLine(personLine, &peopleArray);
	}
	if (gNumOfPeople == START_IDX) // reached here -> we've read the first line and its null, so
		// the file is empty.
	{
		fclose(peopleFile);
		free(peopleArray);
		peopleArray = NULL;
		return NULL;
	}
	fclose(peopleFile);
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
 * treating the first line in meeting.in differently:
 * we find the node with the given ID and updates its CRNA to be 1
 * this costs o(n), because we do not sort the array, and only perform the operation once.
 * @param spreaderID
 */
void updateSpreader(char *spreaderID, Person **peopleArray)
{
	long longID1 = 0;
	longID1 = strtol(spreaderID, NULL, BASE);
	for (int i = 0; i < gNumOfPeople; i++)
	{
		long longID2 = 0;
		longID2 = strtol((*peopleArray)[i].ID, NULL, BASE);
		if (longID1 == longID2) //found the person with the spreader id
		{
			(*peopleArray)[i].crna = MAX_CRNA;
			return;
		}
	}
}

/**
 * given a name to meeting.in file, reads its data and stores it for later use.
 * @param meetingFileName - meeting.in file
 */
Edge *readMeetingFile(char const *const meetingFileName, Person **peopleArray)
{
	// Memory Allocation:
	int size = ALLOC_SIZE;
	Edge *meetingArray = (Edge *) malloc(size * sizeof(Edge)); //all data in meeting.in
	if (meetingArray == NULL) // allocation failed
	{
		free(peopleArray);
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	// CHECKING VALID INPUT:
	FILE *meetingFile = fopen(meetingFileName, "r");
	if (meetingFile == NULL) //no such file
	{
		free(peopleArray);
		free(meetingArray);
		fprintf(stderr, INPUT_ERR);
		exit(EXIT_FAILURE);
	}

	// Parsing Data:
	char meetingLine[MAX_LINE_LEN];
	if (fgets(meetingLine, (int) sizeof(meetingLine), meetingFile) != NULL)
	{// first line - Spreader
		char *spreaderID = meetingLine;
		updateSpreader(spreaderID, peopleArray);
	}
	while (fgets(meetingLine, (int) sizeof(meetingLine), meetingFile)) // each line is some meeting
	{
		if (size == gNumOfMeetings) // this means we don't have enough space to allocate
		{
			size += ALLOC_SIZE;
			meetingArray = (Edge *) realloc(meetingArray, size * sizeof(Edge));
			if (meetingArray == NULL) //reallocation failed
			{
				free(peopleArray);
				fclose(meetingFile);
				fprintf(stderr, STANDARD_LIB_ERR_MSG);
				exit(EXIT_FAILURE);
			}
		}
		// we are now able to parse the line properly:
		parseMeetingLine(meetingLine, &meetingArray);
	}
	if (gNumOfMeetings == START_IDX) // if we have reached here that means we've read the first
		// line and its null, so the file is empty.
	{
		free(meetingArray);
		meetingArray = NULL;
		fclose(meetingFile);
		return NULL;
	}
	fclose(meetingFile);
	return meetingArray;
}

/**
 * compare function based on person's ID
 */
int idCompare(const void *a, const void *b)
{
	return strcmp(((Person *) a)->ID, ((Person *) b)->ID);
}

/**
 * compare function based on crna values
 */
int crnaComp(const void *a, const void *b)
{
	if ((*(Person *) a).crna > (*(Person *) b).crna)
	{
		return NEG_RETURN;
	}
	return ((*(Person *) a).crna < (*(Person *) b).crna);
}

/**
 * binary search based on the IDs
 * @param peopleArray  - array containing the data of people.in
 * @param left - left index of array
 * @param right - right index of array
 * @param personID - some ID to be found
 * @return - index of item if found, -1 if no such item exists.
 */
int binarySearch(Person *peopleArray, int left, int right, const char *personID)
{
	if (right >= left)
	{
		int middle = left + ((right - left) / 2);
		if (strcmp(peopleArray[middle].ID, personID) == 0) // found the ID
		{
			return middle;
		}
		if (strcmp(peopleArray[middle].ID, personID) > 0) // current is bigger->search left part
		{
			return binarySearch(peopleArray, left, middle - 1, personID);
		}
		// else, current is smaller -> search right part
		return binarySearch(peopleArray, middle + 1, right, personID);
	}
	// if we didnt find:
	return NO_SUCH_ITEM;
}

/**
 * calculates the value of crna given dist and time
 * @param dist - distance between two people
 * @param time - amount of time two people have been watched together
 * @return crna value
 */
float calcCrna(float dist, float time)
{
	return ((time * MIN_DISTANCE) / (dist * MAX_TIME));
}

/**
 * given a meeting of two people, calculates the crna of the two, based on the previous
 * values of the source person. this is done using binary search for O(log(#num of people)) runtime
 * for searching a person, rather than O(#people)
 * @param meeting some meeting between two people
 * @param sortedPeople - sorted array of people, ordered by their ID's
 */
void updateInfectionData(Edge meeting, Person *sortedPeople)
{
	float crna = calcCrna(meeting.dist, meeting.time);
	char *destID = meeting.destID;
	char *srcID = meeting.srcID;
	int idxOfSrc = binarySearch(sortedPeople, START_IDX, gNumOfPeople, srcID);
	int idxOfDest = binarySearch(sortedPeople, START_IDX, gNumOfPeople, destID);
	sortedPeople[idxOfDest].crna = crna * sortedPeople[idxOfSrc].crna;
}

/**
 * based on the basic provided pseudo-code, for each person in people.in,
 * calculates the chance of infection, sorts the results and outputs value in severity order
 */
void calcOutput(Edge *meetingArray, Person *peopleArray)
{
	float *resultsArray = (float *) malloc(gNumOfPeople * sizeof(float));
	if (resultsArray == NULL)
	{
		free(meetingArray);
		free(peopleArray);
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	qsort(peopleArray, gNumOfPeople, sizeof(Person), idCompare); // sorting based on IDs
	for (int i = 0; i < gNumOfMeetings; i++)
	{ // calculates Crna for each edge
		updateInfectionData(meetingArray[i], peopleArray);
	}
	// writing output to file:
	qsort(peopleArray, gNumOfPeople, sizeof(Person), crnaComp); // sort by crna value
	FILE *outFile = fopen(OUTPUT_FILE, "w");
	if (outFile == NULL)
	{
		free(meetingArray);
		free(peopleArray);
		fprintf(stderr, STANDARD_LIB_ERR_MSG);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < gNumOfPeople; i++)
	{

		if (peopleArray[i].crna >= MEDICAL_SUPERVISION_THRESHOLD)
		{
			fprintf(outFile, MEDICAL_SUPERVISION_THRESHOLD_MSG, peopleArray[i].name,
					strtoul(peopleArray[i].ID, NULL, BASE));
		}
		else if (peopleArray[i].crna >= REGULAR_QUARANTINE_THRESHOLD)
		{
			fprintf(outFile, REGULAR_QUARANTINE_MSG, peopleArray[i].name,
					strtoul(peopleArray[i].ID, NULL, BASE));
		}
		else // crna < medical threshold
		{
			fprintf(outFile, CLEAN_MSG, peopleArray[i].name, strtoul(peopleArray[i].ID,
																	 NULL, BASE));
		}
	}
	free(resultsArray);
	fclose(outFile);
}

int main(int argc, char *argv[])
{
	if (argc != VALID_ARGS)
	{
		fprintf(stderr, USAGE_ERR);
		exit(EXIT_FAILURE);
	}
	else
	{
		char *peoplePath = argv[1];
		char *meetingPath = argv[2];
		Person *peopleArray = readPeopleFile(peoplePath);
		Edge *meetingArray = readMeetingFile(meetingPath, &peopleArray);
		calcOutput(meetingArray, peopleArray);
		free(peopleArray);
		free(meetingArray);
	}

	return SUCCESS;
}
