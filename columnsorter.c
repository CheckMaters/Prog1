#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "columnsorter.h"
#include "tokenizer.h"
#include "generalFunctions.h"
#define TRUE				1
#define FALSE				0

//this function returns 0 if it is able to get the column index of the input_Line column name from parameter
int get_Column_Index_To_Sort_List(char* sorting_Column_Word, sorting_Column_Info* adding_Column_Info, char* input_Line) {
	int outcome = 1;
	adding_Column_Info->index = -1;
	//starting off with invalid index, so if fuction returns without doing anything it will be -1

	if (sorting_Column_Word[0] != '\0' && input_Line[0] != '\0') { // Check input_Line before doing it
		char* token_Pointer;
		Tokenizer tokenizer;
		initialize_Tokenizer(&tokenizer, input_Line, ',', '\"');

		token_Pointer = getNextToken(&tokenizer);
		int status = 0;
		while (token_Pointer != NULL) {
			if (strcicmp(token_Pointer, sorting_Column_Word) == 0) {		// sorting_Column_Word and token are equal or not
				adding_Column_Info->index = status;
				outcome = 0;
				break;
			}
			else {																					// sorting_Column_Word and token are not equal. Continue loop
				status++;
				token_Pointer = getNextToken(&tokenizer);
			}
		}
	}
	return outcome;
}
