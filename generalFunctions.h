#ifndef HELPER_H
#define HELPER_H

/*
This buffer is going to hold the movie list
Buffer[0] will hold the first line of the CSV FILE
and the the next Buffer will hold the next line of the
CSV file with all the values in one line
*/
typedef struct BUFFER {
	size_t	length_Of_Data;
	char*	data;
} BUFFER;

void delete_Newline_Char_At_The_End(BUFFER* pBuf);
void freeBuffer(BUFFER* pBuf);
int strcicmp(char const *input1, char const *input2);

#endif
