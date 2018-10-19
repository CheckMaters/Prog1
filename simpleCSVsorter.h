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


/*
this function will go through the directory
finding .csv files. It will recurse through
the same function if it detects sub-diretories
within provided directory. 
*/
int scan_Directory (DIR * directory, char * sorting_Column);


//this function checks if the given char string is directory or not
int is_Directory (const char * name);


//this function checks if the given char string is CSV file or no
int is_CSV_file (const char * name);


//Suggestion: define a struct that mirrors a record (row) of the data set

//Suggestion: prototype a mergesort function

#endif
