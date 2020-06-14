#ifndef EX1_TEST_DEF_H
#define EX1_TEST_DEF_H

#endif //EX1_TEST_DEF_H

// output file filename and source file filename
#define _NAME "./manageStudents"
// #define _NAME "~labcc2/www/ex1/school_solution"
#define _SRC "./manageStudents.c"

// files locations
#define DEBUG_FILE "./tmp/debug.txt"
#define OUTPUT "./tmp/output"
#define ERRORS "./tmp/errors"
#define AUTOMATED_TEST "./tmp/test.txt"

// number of students for automated test for sorting
#define BATCH_SIZE 100
#define RERUNS 100
#define MAX_NAME_SIZE 37
#define DEBUG_TXT_EDITOR "notepad"

#define MAX_LINES 5500
#define LINE_SIZE 150
#define FIELD_LEN 43
#define FIELD_S "43"
#define FORMAT " %"FIELD_S"[^,],%"FIELD_S"[^,],%"FIELD_S"[^,],%"FIELD_S"[^,],%"FIELD_S"[^,],%"FIELD_S"[^\n] "

#define COMPILE "gcc -Wextra -Wall -Wvla -std=c99 -lm "_SRC" -o "_NAME" > "OUTPUT" 2> "ERRORS

#if defined(_WIN32) || defined (__CYGWIN__) || defined (_WIN64)
#include <windows.h>
#define CMD_COLOR_ON(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color)
#define CMD_COLOR_OFF(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color)
#else
#define CMD_COLOR_ON(color)
#define CMD_COLOR_OFF(color)
#endif



int sortingTest(char *command, int batchSize);
int bestTest(int batchSize);
int inputTesting();
int usageTest();

int startsWith(const char *str, const char *pre);
int isClear(FILE *fp);
int runTest(char *testName, char *command, char *output);
int preformTest(char *output, char *expected);
unsigned int getLength(FILE *fp);
int compare(FILE *output, FILE *fpExpect);
int fileExist (char *filename);
void printFile(FILE *fp);

int checkSortingGrade(int expectedLen);
int checkSortingName(int expectedLen);
int checkBest(int expectedLen);
double getBestVal();
int testToIntArr(int arr[MAX_LINES][2], char *testfile);
int checkSorted(const long int *arr, int len);
int checkSortedStr(char arr[MAX_LINES][LINE_SIZE], int len);
void createSortingTest(int num);
void createBestTest(int num);

void openDebugFiles(char *testFile, char *expectedFile, char *outputFile);