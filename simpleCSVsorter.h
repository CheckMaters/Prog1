#ifndef SORTER_H
#define SORTER_H
/*
Define structures and function prototypes for your sorter
*/

void print_The_List(BUFFER* pHeader, movie_Record* pRecordArray); //helps to print out the result
/*
this will use the word provided in the parameter and will sort
the input file alphabetically based on that column
*/
int sort_The_List(char* sort_By_This_Value);

//Suggestion: define a struct that mirrors a record (row) of the data set

//Suggestion: prototype a mergesort function

#endif
