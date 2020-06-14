#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int compile()
{
	printf("Compiling project... \n\t%s\n", COMPILE);
	remove(OUTPUT);
	remove(ERRORS);

	system(COMPILE);

	FILE *output = fopen(OUTPUT, "r");
	FILE *errors = fopen(ERRORS, "r");

	if (!isClear(output) || !isClear(errors))
	{
		fprintf(stderr, "Please make sure there are no compilation errors first. \nexiting.\n");
		return 0;
	}

	fclose(errors);
	fclose(output);
	printf("Compiled successfully.\n\n");
	return 1;
}

int fileExist(char *filename)
{
	return access(filename, F_OK) != -1;
}

int startsWith(const char *str, const char *pre)
{
	while(*pre)
	{
		if(*pre++ != *str++)
			return 0;
	}
	return 1;
}

int isClear(FILE *fp)
{
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		unsigned long len = (unsigned long)ftell(fp);
		if (len != 0)
		{
			rewind(fp);
			printFile(fp);
			return 0;
		}
	}
	return 1;
}

int runTest(char *testName, char *command, char *output)
{
	remove(output);
	char *cmd = (char*)malloc(150 * sizeof(char));
	sprintf(cmd, "%s %s <%s >%s", _NAME, command, testName, output);
	int exitCode = system(cmd);
	free(cmd);
	return WEXITSTATUS(exitCode);
}

void runDiff(char *output, char *expected)
{
	FILE *create = fopen(DEBUG_FILE, "w");
	fclose(create);

	char *command = (char*)malloc(150 * sizeof(char));
	sprintf(command, "diff %s %s > %s", output, expected, DEBUG_FILE);
	system(command);
	free(command);

	FILE *debug = fopen(DEBUG_FILE, "r");
	if (debug == NULL)
	{
		fprintf(stderr, "test encountered an error, trying to show diff");
	}

	printFile(debug);

	fclose(debug);
}

int preformTest(char *output, char *expected)
{
	FILE *outputF = fopen(output, "r");
	FILE *expectedF = fopen(expected, "r");

	unsigned int length = getLength(expectedF);
	if (getLength(outputF) != length)
	{
		fprintf(stderr, "Incorrect number of lines printed, showing diff: \n");
		runDiff(output, expected);

		fclose(outputF);
		fclose(expectedF);
		return 0;
	}

	rewind(outputF);
	rewind(expectedF);
	int res = compare(outputF, expectedF);
	fclose(outputF);
	fclose(expectedF);
	return res;
}

int compare(FILE *output, FILE *fpExpect)
{
	int passed = 1;
	char line[LINE_SIZE], expected[LINE_SIZE];
	int currentLine = 0;

	FILE *debug = fopen(DEBUG_FILE, "w");
	while (fgets(expected, LINE_SIZE, fpExpect) != NULL)
	{
		if (fgets(line, LINE_SIZE, output) == NULL)
		{
			passed = 0;
			fprintf(debug, "UNEXPECTED END OF OUTPUT\n");
		}

		sscanf(expected, " %[^\n] ", expected);
		if (!startsWith(line, expected))
		{
			passed = 0;
			fprintf(debug, "\tFailed: at line #%d expected to find '%s'\n", currentLine, expected);
			fprintf(debug, "\t\t\t Your output was: %s\n\n", line);
		}
		currentLine++;
	}
	fclose(debug);

	FILE *fp = fopen(DEBUG_FILE, "r");
	printFile(fp);
	fclose(fp);
	return passed;
}

unsigned int getLength(FILE *fp)
{
	char line[LINE_SIZE];
	unsigned int length = 0;
	while(fgets(line, LINE_SIZE, fp))
	{
		length++;
	}

	rewind(fp);
	return length;
}

void printFile(FILE *fp)
{
	char ch=0;

	while(!feof(fp))
	{
		ch = (char) getc(fp);
		if (ferror(fp))
		{
			fputs ("File error - Reading Operation",stderr);
			exit (EXIT_FAILURE);
		}

		if (!feof(fp))
		{
			putchar(ch);
		}
	}
	rewind(fp);
}

int checkSortingGrade(int expectedLen)
{
	FILE *output = fopen(OUTPUT, "r");
	char line[LINE_SIZE];
	char id[FIELD_LEN] = {0}, name[FIELD_LEN] = {0}, age[FIELD_LEN] = {0},
			grade[FIELD_LEN] = {0}, city[FIELD_LEN] = {0}, country[FIELD_LEN] = {0};

	long int arr[MAX_LINES];
	int len = 0;
	while (fgets(line, LINE_SIZE, output))
	{
		int varsNum = sscanf(line, FORMAT, id, name, grade, age, country, city);
		if (varsNum == 6)
		{
			arr[len++] = strtol(grade, NULL, 10);
		}
	}
	fclose(output);

	if (len != expectedLen)
	{
		printf("Incorrect number of lines read, you might have rejected a line which is valid. \n");
		return 0;
	}
	return checkSorted(arr, len);
}

int checkSortingName(int expectedLen)
{
	FILE *output = fopen(OUTPUT, "r");
	char line[LINE_SIZE];
	char id[FIELD_LEN] = {0}, name[FIELD_LEN] = {0}, age[FIELD_LEN] = {0},
			grade[FIELD_LEN] = {0}, city[FIELD_LEN] = {0}, country[FIELD_LEN] = {0};

	char arr[MAX_LINES][LINE_SIZE];
	int len = 0;
	while (fgets(line, LINE_SIZE, output))
	{
		int varsNum = sscanf(line, FORMAT, id, arr[len], grade, age, country, city);
		if (varsNum == 6)
		{
			len++;
		}
	}

	fclose(output);
	if (len != expectedLen)
	{
		printf("Incorrect number of lines read, you might have rejected a line which is valid. \n");
		return 0;
	}

	return checkSortedStr(arr, len);
}

int checkSorted(const long int *arr, int len)
{
	for (int i = 1; i < len; i++)
	{
		if (arr[i - 1] > arr[i])
		{
			printf("Failed at input #%d\n", i);
			return 0;
		}
	}
	return 1;
}

int checkSortedStr(char arr[MAX_LINES][LINE_SIZE], int len)
{
	for (int i = 1; i < len; i++)
	{
		if (strcmp(arr[i - 1], arr[i])  > 0)
		{
			printf("Failed at input #%d\n", i);
			return 0;
		}
	}
	return 1;
}

void createSortingTest(int num)
{
	FILE *newTest = fopen(AUTOMATED_TEST, "w");
	char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ- ";
    int charsetLen = 54;

	for (int i = 0; i < num; i++)
	{
		int grade = rand()%101;
		int strLen = rand()%MAX_NAME_SIZE + 1;
		char name[FIELD_LEN];
		int j;
		for (j = 0; j < strLen; j++)
		{
			name[j] = charset[rand()%charsetLen];
		}
		name[j] = '\0';

		fprintf(newTest, "3845354610,%s,%d,18,U,L\n", name, grade);
	}
	fprintf(newTest, "q\n");
	fclose(newTest);
}

void createBestTest(int num)
{
	FILE *newTest = fopen(AUTOMATED_TEST, "w");

	for (int i = 0; i < num; i++)
	{
		int grade = rand()%101;
		int age = rand()%103 + 18;
		fprintf(newTest, "3845354610,doesnt matter,%d,%d,United-Kingdom,London\n", grade, age);
	}
	fprintf(newTest, "q\n");
	fclose(newTest);
}

int testToIntArr(int arr[MAX_LINES][2], char *testfile)
{
	FILE *test = fopen(testfile, "r");
	char line[LINE_SIZE];
	char id[FIELD_LEN] = {0}, name[FIELD_LEN] = {0}, city[FIELD_LEN] = {0}, country[FIELD_LEN] = {0};

	int len = 0;
	while (fgets(line, LINE_SIZE, test))
	{
		int varsNum = sscanf(line, " %"FIELD_S"[^,],%"FIELD_S"[^,],%d,%d,%"FIELD_S"[^,],%"FIELD_S"[^\n] ", id, name, &arr[len][0], &arr[len][1], country, city);
		if (varsNum == 6)
		{
			len++;
		}
	}

	fclose(test);
	return len;
}

double getBestVal()
{
	FILE *output = fopen(OUTPUT, "r");
	char line[LINE_SIZE];
	char id[FIELD_LEN] = {0}, name[FIELD_LEN] = {0}, city[FIELD_LEN] = {0}, country[FIELD_LEN] = {0};

	int bestGrade=-1, bestAge=-1;
	while (fgets(line, LINE_SIZE, output))
	{
		if (startsWith(line, "best student info is:"))
		{
			sscanf(line, "best student info is: %"FIELD_S"[^,],%"FIELD_S"[^,],%d,%d,%"FIELD_S"[^,],%"FIELD_S"[^\n] ", id, name, &bestGrade, &bestAge, country, city);
		}
	}
	fclose(output);

	if (bestGrade == -1 || bestAge == -1)
	{
		fprintf(stderr, "Couldn't find 'best student info'\n");
		return -1.0;
	}
	return (double)bestGrade/(double)bestAge;
}

int checkBest(int expectedLen)
{
	int arr[MAX_LINES][2];
	int len = testToIntArr(arr, AUTOMATED_TEST);
	if (len != expectedLen)
	{
		printf("Incorrect number of lines read, you might have rejected a line which is valid. \n");
		return 0;
	}

	double bestVal = getBestVal();
	for (int i = 0; i < len; i++)
	{
		double tempVal = (double)arr[i][0] / (double)arr[i][1];
		if (bestVal < tempVal)
		{
			fprintf(stderr, "Failed finding best student, printed best value was %f but student #%d has a value of %f\n", bestVal, i, tempVal);
			return 0;
		}
	}
	return 1;
}

void doProgress(char *label, int step, int total)
{
	//progress width
	const int pwidth = 72;

	//minus label len
	int width = pwidth - strlen(label);
	int pos = (step * width) / total;


	int percent = (step * 100) / total;
	printf("%d ", percent);

	//set green text color, only on Windows
	CMD_COLOR_ON(0x06);
	printf("%s[", label);

	//fill progress bar with =
	for (int i = 0; i < pos; i++) printf("%c", '=');

	//fill progress bar with spaces
	printf("% *c", width - pos + 1, ']');
	printf(" %3d%%\r", percent);

	fflush(stdout);
	//reset text color, only on Windows
	CMD_COLOR_OFF(0x07);
}

void openDebugFiles(char *testFile, char *expectedFile, char *outputFile)
{
	#if defined(_WIN32) || defined (__CYGWIN__) || defined (_WIN64)
		printf("Would you like to open "OUTPUT" and the tests file for debug? (press 'y' for yes)\n");
		char ans = (char)getc(stdin);

		if (ans == 'y')
		{
			char cmd[2*LINE_SIZE];
			sprintf(cmd, DEBUG_TXT_EDITOR" %s & "DEBUG_TXT_EDITOR" %s &", testFile, outputFile);
			if (strlen(expectedFile))
			{
				sprintf(cmd, DEBUG_TXT_EDITOR" %s & "DEBUG_TXT_EDITOR" %s & "DEBUG_TXT_EDITOR" %s &", testFile, expectedFile, outputFile);
			}
			system(cmd);
		}
	#else
		printf("This feature works only for windows at the moment.\n");
		printf("You can manualy open %s, %s and %s to debug.\n", testFile, expectedFile, outputFile);
	#endif
}



