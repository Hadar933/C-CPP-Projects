Name: Hadar Sharvit
ID: 208287599
CS-USER:hadar933

SpreaderDetectorBackend
-----------------------------
Please explain how you dealt with the following parts of the exam.

Input processing
----------------
given some file, the process is as followed:
-Allocate memory to a data structure (an array) which will, in time, contain the
entire file's content.
-check that the file exists, and proceed to parse each line. (when reading some
 line, I made sure that the data structure had been allocated with enough memory
to contains said line's content.)-the parsing process of a line extracts the
needed data (for example when dealing with the file people.in, I separate each
 line to name, id, and age) and inserts it into a struct representing the data.
(one of the two-person struct representing each person in the data set, or edge
 struct representing the connection between people in the data set) 
-once each line in the file has been converted to a suitable struct, the struct
 is being added to an array that contains the data for further use.


Data storing
------------
this has been mentioned in the previous part, yet here are some further
clarifications
each line in a file is stored in a struct object. there are two types of structs:
1. Person  - contains the age, id name, and the probability of infection
 (as given by the Crna function). each line in people.in is being converted to
a person struct2. Edge - contains the source Id and the destination id,
representing two people that have came across each other in the video footage
provided. This struct also contains the time and distance specifications,
relevant to the said meeting between some two people.

each line, corresponding to a struct object is being stored in two distinct
arrays:
1. people array - where each element is of type Person
2. meeting array - - where each element is of type Edge

Results sorting
---------------
for the sake of this discussion, we assume
1. size of people array = n (corresponds to n people or n lines in people.in)
2. size of meeting array = m (corresponds to m connections or m lines in meeting.in)
the main runtime issue was that given some Edge, connecting two Persons, in order
to reach the person's struct we have to find that said person, given its ID, in
people array. Therefore, one must iterate over all elements in people array.
a trivial solution would be that for every Edge, find its corresponding two
people that are connected via the edge. this could take up to O(m*n).I have
decided to sort the people array according to the IDs, and binary search the
needed ID instead, all in all, taking O(m*logn) time. more specifically,
the algorithm is as follow-given people array and meeting array:
-sort people array by IDs-for every edge in meeting array:
  -calculate the crna of source person and destination person (those which
  are connected by an edge)
  -binary search people array for the source person id and the destination person id   
  -update the crna of the destination person by multiplying the crna from stage
  1 with the crna of the source person -once done with calculating all crnas,
   sort the people array by crna values and output the needed result using the
  given thresholds.
