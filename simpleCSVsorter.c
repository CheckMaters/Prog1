#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "generalFunctions.h"
#include "columnsorter.h"
#include "movieListData.h"
#include "movieValueList.h"
#include "mergesort.h"
#include "simpleCSVsorter.h"
#include "tokenizer.h"
#define TRUE				1
#define FALSE				0

//lksakjflkdjflk
int main(int argc, char* argv[]) {
	//checking if the input argument is correct or Not
	
	
	//the output file is not given in the parameter
	if(argc == 5 || argc == 7) {
		if(strcmp("-c", argv[1]) != 0 || strcmp("-d", argv[3]) != 0) {
			printf("Parameters are not correctly formated\n");
			return -1;				//returning -1 because it's an error
		}
		if(argc == 7) {
				if(strcmp("-o", argv[5]) != 0) {
					printf("Parameters are not correctly formated\n");
					return -1;		//returning -1 because it's an error
				}
		}
	} 
	else {
		printf("Parameters are not correctly formated\n");
		return -1;  			//returning -1 because it's an error
	}
	
	/*
	this will create a directory and also
	a struct that keeps all the information
	about directory.
	*/
	DIR * directory = opendir(argv[4]);
		if(directory == NULL) {
			printf("Error! Can't open directory\n");
			return -1;		//returning -1 because it's an error
		}

		char* sort_By_This_Value = argv[2];
		return scan_Directory (directory, sort_By_This_Value);

}


/*
sort_The_List is the function that
is going to sort the stdin movie_List
file in alphabetical boreds. The column
that is going to be sorted is represented
by sort_By_This_Value.
*/
int sort_The_List(char* sort_By_This_Value, FILE* file) {

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
		if (getline(&(read_Stdin_File.data), &(read_Stdin_File.length_Of_Data), file) > 0 ) {
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
			printf("Error! Unable to read file provided within directory. Please check file or directory\n");
		}

	freeBuffer(&header_Of_File);
	freeBuffer(&read_Stdin_File);
	if (movie_List.pRecArray != NULL) {
		clearMovieList(&movie_List);
	}


	return output_Result;
}





//this checks if the given char string is directory or not
int is_Directory (const char * name) {
	DIR * temp = opendir(name);
	if (temp != NULL){
		return 0;	//returns 0 if it's directory, else -1
	}
	return -1;
}


//this checks if the given char string is CSV file or not
int is_CSV_File (const char * name) {
	if(strlen(name) > 4 && !(strcmp(name + strlen(name) - 4, ".csv"))){
		return 0;			//returns 0 if it's csv file, else -1
	}
	   return -1;
}


/*
this function will scan through the directory 
to find .csv files and possibly other 
sub-directories if there's any.
It also performs sorting on finding 
CSV file with sorting_Column, which is the 
column user wants to sort file on.
*/
int scan_Directory(DIR * directory, char * sorting_Column){
	int return_Value = -1;
	if(directory == NULL){
		printf("ERROR! Unable to open directory\n");
		return -1;			//returning -1 because it is an error
	}
	struct dirent * directory_Info;
	while ((directory_Info = readdir(directory))!= NULL){
		
		
		
		//checks if the current file that file pointer points to is a directory or not
		return_Value = is_Directory(directory_Info->d_name);
		/*
		if return value is zero, it means this is directory
		and will perform recurssion from this point on that 
		sub-directory
		*/
		if(return_Value == 0) {
			//**************this means it is directory, perform recurssion
			
		}
		
		
		// if the read value is not directory, check if it's CSV file or not
		else {
			return_Value = is_CSV_File(directory_Info->d_name);
			if(return_Value == 0){
				//************this means it is CSV file, perform sorting
				FILE * file = fopen(directory_Info->d_name, "r");
				return_Value = sort_The_List(sorting_Column, file);
			} 
			else {
				continue; //continuing to the next loop because read value isn't directory or CSV file
			}
		}
		
		
	}
	return return_Value;
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
