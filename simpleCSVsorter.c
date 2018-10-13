#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "generalFunctions.h"
#include "columnsorter.h"
#include "movieListData.h"
#include "movieValueList.h"
#include "mergesort.h"
#include "simpleCSVsorter.h"
#include "tokenizer.h"
#define TRUE				1
#define FALSE				0

int main(int argc, char* argv[]) {
	//checking if the input argument is correct or Not
	if(argc != 3) {
		printf("Error! Check your Parameters\n");
		return 0;
	}


		char* sort_By_This_Value = argv[2];
		return sort_The_List(sort_By_This_Value);

}


/*
sort_The_List is the function that
is going to sort the stdin movie_List
file in alphabetical boreds. The column
that is going to be sorted is represented
by sort_By_This_Value.
*/
int sort_The_List(char* sort_By_This_Value) {

	// Buffer for reading from stdin
	BUFFER read_Stdin_File;																	// Buffer is created to read the stdin File
	BUFFER header_Of_File;																// For holding the header line


	FUNCTION_POINTER_CMPDATA pFuncCompare = compare;
	//Initializing Buffer to NULL and then will add data to it
	read_Stdin_File.data = NULL;
	read_Stdin_File.length_Of_Data = 0;

	int output_Result = 1;			//setting the output result to 1 in the beginning

	sorting_Column_Info column_Info;						//this will hold info regarding the column's type and index no.
	column_Info.index = -1;
	column_Info.index_Type_SIF = INDEX_TYPE_INTEGER;
					/*
					the type of column to sort is set to integer First
					and will be changed according later in the function
					if necessary
					*/



	// To hold movie list array
	movie_Record movie_List;

	//Intializing movie_Record named movie_List
	movie_List.pRecArray = (one_Movie_Values**) malloc (1024 * sizeof(one_Movie_Values*));
	movie_List.iCapacity = 1024;
	movie_List.iSize = 0;
	int no_Use = 0;
		// Read the header and get the index for the sorting key
		if (getline(&(read_Stdin_File.data), &(read_Stdin_File.length_Of_Data), stdin) > 0 ) {
			delete_Newline_Char_At_The_End(&read_Stdin_File);						// this will delete the new line character from the end of the string line
			//
			header_Of_File.data = (char*)malloc (sizeof(char) * read_Stdin_File.length_Of_Data);
			header_Of_File.length_Of_Data = read_Stdin_File.length_Of_Data;




	//		initialize_Buffer(&header_Of_File, read_Stdin_File.length_Of_Data);			//Holds a line from the file
			strcpy(header_Of_File.data, read_Stdin_File.data);										// copy of header line before tokenizing
			//

			if (get_Column_Index_To_Sort_List(sort_By_This_Value, &column_Info, read_Stdin_File.data)) {
							printf( "Error! Unable to parse the input column name\n");
						}
						else {
				// Read data line by line. Creates record and put them into array
				while (getline(&(read_Stdin_File.data), &(read_Stdin_File.length_Of_Data), stdin) > 0) {
					delete_Newline_Char_At_The_End(&read_Stdin_File);

					one_Movie_Values* pRecord = make_Movie_Value_List(read_Stdin_File.data, &column_Info);
					if (pRecord) {
					no_Use =	add_One_Movie_To_The_List(&movie_List, pRecord);
					}
					else {
						printf( "Error! Unable to create movie list!\n");
					}
		}
				/*
				merge sort will sort the array of movie list based
				on the index type and column name entered in the
				command prompt
				*/
				mergeSort((void**)movie_List.pRecArray, 0, movie_List.iSize - 1, &column_Info, pFuncCompare);
				// Output sorted records to file
				print_The_List(&header_Of_File, &movie_List);
				 output_Result = 0;
		}
	}
		else {
			printf("Error! Unable to read file through STDIN. Please check file\n");
		}

	freeBuffer(&header_Of_File);
	freeBuffer(&read_Stdin_File);
	if (movie_List.pRecArray != NULL) {
		clearMovieList(&movie_List);
	}


	return output_Result;
}


/*
Writes the data from the movie_Record into stdout
*/
void print_The_List(BUFFER* pHeader, movie_Record* pRecordArray) {
	FILE *output_File = stdout;
	fprintf(output_File, "%s\n", pHeader->data);																										// Print header line

	//print all records
	int xyz;
	for (xyz = 0; xyz < pRecordArray->iSize; xyz++) {
		*((*(pRecordArray->pRecArray + xyz))->sorting_Key_Term) = (*(pRecordArray->pRecArray + xyz))->chHold;		// Put back the char set to '\0' for sorting key
		fprintf(output_File, "%s\n", (*(pRecordArray->pRecArray + xyz))->recordData);													// Print Records in a loop
	}
}
