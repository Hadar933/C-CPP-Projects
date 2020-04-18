#include <stdio.h>
#include <string.h>
#include <math.h>

#define FIELD_LEN 42
#define LINE_LEN 60
#define LINES 5500
#define ASCII_A 65
#define ASCII_Z 122
#define ASCII_SP 32
#define ASCII_HYP 45
#define ASCII_COMMA 44
#define MIN_AGE 18
#define MAX_AGE 120
#define MIN_GRADE 0
#define MAX_GRADE 100

int gStudCount = 0;
int gLineCount = -1;

/**
 * struct of type student, contains all the required data to initialize a student as requested.
 */
struct Student // initializing a Student struct
{
    float grade, age;
    double ID;
    char name[FIELD_LEN], country[FIELD_LEN], city[FIELD_LEN];
};
struct Student gStudArray[LINES]; // global student array with all students and their data.

/**
 * @param num - some number
 * @return num's first digit
 */
int firstDigit(double num)
{
    while (num >= 10)
    {
        num /= 10;
    }
    return (int) num;
}

/**
 * checks if a number has 10 digits
 * @param num - some number
 * @return 1 if the number doesn't have exactly 10 digits.
 * 0 otherwise
 */
int notTenDigits(double num)
{
    if ((floor(log10((num))) + 1) != 10)
    {
        return 1;
    }
    return 0;
}

/**
 * checks if a name has so called "Bad chars" in it.
 * @param name
 * @return 1 if has bad chars in it. 0 otherwise
 */
int nameBadChard(char *name)
{
    for (int i = 0; i < (int) strlen(name); i++)
    { // checking ASCII values (A-Z: 65-122, '-':45, ' ': 32)
        if ((name[i] < ASCII_A || name[i] > ASCII_Z) &&
            (name[i] != ASCII_SP && name[i] != ASCII_HYP))
        {
            return 1;
        }
    }
    return 0;
}

/**
 * checks for bad chars in some string value (relevant for city or country)
 * @param strVal
 * @return 1 if has bad chars in it. 0 otherwise
 */
int badCharsIn(char *strVal)
{
    for (int i = 0; i < (int) strlen(strVal); i++)
    { // checking ASCII values (A-Z: 65-122, '-':45)
        if ((strVal[i] < ASCII_A || strVal[i] > ASCII_Z) && (strVal[i] != ASCII_HYP))
        {
            return 1;
        }
    }
    return 0;
}

/**
 * prints the current line
 */
void printLine()
{
    if (gLineCount == -1)
    {
        printf("in line %d\n", gLineCount + 1);
        gLineCount++;
    }
    else
    {
        printf("in line %d\n", gLineCount);
    }
}

/**
 * checks if an ID consists of letters only
 * @return 1 if is invalid, 0 otherwise
 */
int noLettersIn(char ID[])
{

    int i;
    for (i = 0; i < (int) strlen(ID); i++)
    {
        if (ID[i] < 48 || ID[i] > 57)
        {
            printf("ERROR: info must match specified format\n");
            printLine();
            gLineCount++;
            return 0;
        }
    }
    return 1;
}

/**
 * prints an error according to specification
 */

int checkValidInput(float ID, char *name, float grade, float age, char *country,
                    char *city)
{
    int valid = 1;
    if (notTenDigits(ID) || firstDigit(ID) == 0) // ID check
    {
        printf("ERROR: id must be a 10 digits number that does not start with 0\n");
        valid = 0;
    }
    else if (nameBadChard(name))
    {
        printf("ERROR: name can only contain alphabetic characters, whitespaces or '-'\n");
        valid = 0;
    }
    else if (grade < MIN_GRADE || grade > MAX_GRADE)
    {
        printf("ERROR: grade must be an integer between 0 and 100\n");
        valid = 0;
    }
    else if (age < MIN_AGE || age > MAX_AGE)
    {
        printf("ERROR: age must be an integer between 18 and 120\n");
        valid = 0;
    }
    else if (badCharsIn(country))
    {
        printf("ERROR: country can only contain alphabetic characters or '-'\n");
        valid = 0;
    }
    else if (badCharsIn(city))
    {
        printf("ERROR: city can only contain alphabetic characters or '-'\n");
        valid = 0;
    }
    if (!valid)
    {
        printf("in line %d\n", gLineCount);

    }
    return valid;

}

/**
 * finds the student with the best ratio grade/age
 */
int best()
{
    float ratio = 0, bestRatio;
    int bestStudent = 0;
    if (gStudCount == 0)
    {
        return 0;
    }
    for (int i = 0; i <= gStudCount; i++)
    {
        bestRatio = gStudArray[i].grade / gStudArray[i].age;
        if (bestRatio > ratio)
        {
            ratio = bestRatio;
            bestStudent = i;
        }
    }
    printf("best student info is: %.0f,%s,%d,%d,%s,%s\n", gStudArray[bestStudent].ID,
           gStudArray[bestStudent].name, (int) gStudArray[bestStudent].grade,
           (int) gStudArray[bestStudent].age, gStudArray[bestStudent].country,
           gStudArray[bestStudent].city);
    return 0;
}

/**
 * finds if a string has 6 fields by checking for 5 commas.
 * @param data some string
 * @return 1 if has 6 fields, 0 otherwise
 */
int hasSixFields(char data[])
{
    int commaCount = 0;
    for (int i = 0; (int) strlen(data) > i; i++)
    {
        if (data[i] == ASCII_COMMA)
        {
            commaCount++;
        }
    }
    if (commaCount == 5)
    {
        return 1;
    }
    printf("ERROR: info must match specified format\n");
    printLine();
    gLineCount++;
    return 0;
}

/**
 * gets data from the user (ID, name, grade, age, country, city)
 */
int getData()
{
    char data[LINE_LEN], strID[LINE_LEN];
    char name[FIELD_LEN], country[FIELD_LEN], city[FIELD_LEN];
    float grade, age;
    double ID;
    do
    {
        printf("Enter student info. To exit press q, then enter\n");
        fgets(data, LINES, stdin);
        if (data[0] != 'q' && hasSixFields(data))
        {
            gLineCount++;
            sscanf(data, "%[^,], %[^,], %f, %f, %[^,], %[^\n]\n ", strID, name, &grade, &age, country,
                   city);
            sscanf(strID, "%lf", &ID);
            if (noLettersIn(strID) && checkValidInput(ID, name, grade, age, country, city))
            {
                //building a student struct
                struct Student s;
                s.ID = ID;
                strcpy(s.name, name);
                s.grade = grade;
                s.age = age;
                strcpy(s.country, country);
                strcpy(s.city, city);
                // adding the students data to a global gStudArray
                gStudArray[gStudCount] = s;
                gStudCount++;
            }
        }
    } while (data[0] != 'q'); // user hasn't typed 'q' to exit
    return 0;
}

/**
 * merges two arrays of type Student
 * @param lst some lst with elements of type Student
 * @param left leftmost index
 * @param start starting index
 * @param right rightmost index
 */
void merge(struct Student lst[], int left, int start, int right)
{
    // initializing two arrays as followed:
    // arrayL= lst[left,left+1,...,start]
    // arrayR = lst[start+1,start+2,...,right]
    int sizeL = start - left + 1;
    int sizeR = right - start;
    struct Student arrayL[LINES], arrayR[LINES];
    // we now add items from lst to arrayL and arrayR
    for (int i = 0; i < sizeL; i++)
    {
        arrayL[i] = lst[left + i];
    }
    for (int j = 0; j < sizeR; j++)
    {
        arrayR[j] = lst[start + 1 + j];
    }
    // initializing indexes for lst(a),arrayL(l),arrayR(r)
    int r = 0, l = 0, a = left;
    // main loop: comparing elements from both sub arrays and adding them to final lst
    // according to their value.
    while (r < sizeR && l < sizeL)
    {
        if (arrayL[l].grade <= arrayR[r].grade)
        {
            lst[a] = arrayL[l];
            l++;
        }
        else
        {
            lst[a] = arrayR[r];
            r++;
        }
        a++;
    }
    //if one of the sub-arrays is longer, we will add its remaining elements to the result
    while (r < sizeR)
    {
        lst[a] = arrayR[r];
        r++;
        a++;
    }
    while (l < sizeL)
    {
        lst[a] = arrayL[l];
        l++;
        a++;
    }
}

/**
 * recursive algorithm using merge to sort an lst of type Student according to grades.
 * @param lst some lst with elements of type Student
 * @param left leftmost index
 * @param right rightmost index
 */
void mergeSort(struct Student lst[], int left, int right)
{
    int mid;
    if (left < right)
    {
        mid = (left + right) / 2;
        mergeSort(lst, left, mid); //1st half
        mergeSort(lst, mid + 1, right); // 2nd half
        merge(lst, left, mid, right);
    }
}

/**
 * sorts an lst according to some pivot index, according to name trait
 * @param lst some lst with elements of type Student
 * @param left leftmost index
 * @param right rightmost index
 * @return relevant index to continue partitioning recursively
 */
int partition(struct Student lst[], int left, int right)
{
    struct Student pivot = lst[right]; //position right is chosen arbitrarily
    struct Student temp;
    int curr = left - 1;

    for (int i = left; right - 1 >= i; i++)
    {
        if (strcmp((lst[i].name), (pivot.name)) <=
            0)
        { // this means lst[i].name should show up before the pivot.name,so we're swapping them.
            curr++;
            temp = lst[curr];
            lst[curr] = lst[i];
            lst[i] = temp;
        }
    }
    // swapping pivot in place
    temp = lst[curr + 1];
    lst[curr + 1] = lst[right];
    lst[right] = temp;
    return curr + 1;

}

/**
 * recursively sorts an lst of type student, according to the name value
 * @param lst some lst with elements of type Student
 * @param left leftmost index
 * @param right rightmost index
 */
void quickSort(struct Student lst[], int left, int right)
{
    if (left < right)
    {
        int idx = partition(lst, left, right);
        quickSort(lst, left, idx - 1);
        quickSort(lst, idx + 1, right);
    }

}

/**
 * according to argv, performs three different operations - best, merge or quick.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("USAGE: sortStudents <action>\n");
        return 1;
    }
    getData();

    if (strcmp(argv[1], "best") == 0)
    {
        best();
    }
    else if (strcmp(argv[1], "merge") == 0)
    {
        mergeSort(gStudArray, 0, gStudCount - 1);
        for (int i = 0; i < gStudCount; i++)
        {
            printf("%.0f,%s,%d,%d,%s,%s\n", gStudArray[i].ID, gStudArray[i].name,
                   (int) gStudArray[i].grade,
                   (int) gStudArray[i].age, gStudArray[i].country, gStudArray[i].city);
        }
    }
    else if ((strcmp(argv[1], "quick") == 0))
    {
        quickSort(gStudArray, 0, gStudCount - 1);
        for (int i = 0; i < gStudCount; i++)
        {
            printf("%.0f,%s,%d,%d,%s,%s\n", gStudArray[i].ID, gStudArray[i].name,
                   (int) gStudArray[i].grade,
                   (int) gStudArray[i].age, gStudArray[i].country, gStudArray[i].city);
        }
    }
    else
    {
        printf("USAGE: either best,merge or quick!");

        return 1;
    }
    return 0;
}
