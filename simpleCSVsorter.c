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
#include <sys/wait.h>
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

	DIR * directory;
	directory = opendir(argv[4]);

		if(directory == NULL) {
			printf("ERROR! Given directory is pointing to NULL\n");
			return -1;		//returning -1 because it's an error
		}

		char* sort_By_This_Value = argv[2];

		return scan_Directory(directory, sort_By_This_Value, argv[4]);

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
	BUFFER read_File;																	// Buffer is created to read the stdin File
	BUFFER header_Of_File;																// For holding the header line


	FUNCTION_POINTER_CMPDATA pFuncCompare = compare;
	//Initializing Buffer to NULL and then will add data to it
	read_File.data = NULL;
	read_File.length_Of_Data = 0;

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
	//int no_Use = 0;
		// Read the header and get the index for the sorting key
		if (getline(&(read_File.data), &(read_File.length_Of_Data), file) > 0 ) {
			delete_Newline_Char_At_The_End(&read_File);						// this will delete the new line character from the end of the string line

			header_Of_File.data = (char*)malloc (sizeof(char) * read_File.length_Of_Data);
			header_Of_File.length_Of_Data = read_File.length_Of_Data;





			strcpy(header_Of_File.data, read_File.data);			// copy of header line before tokenizing
			//head_Of_File keeps record of the first line that is that name of all the columns

			if (get_Column_Index_To_Sort_List(sort_By_This_Value, &column_Info, read_File.data)) {
							printf( "Error! Unable to parse the input column name\n");
						}
						else {
				// Read data line by line. Creates record and put them into array
				while (getline(&(read_File.data), &(read_File.length_Of_Data), file) > 0) {
					delete_Newline_Char_At_The_End(&read_File);

					one_Movie_Values* pRecord = make_Movie_Value_List(read_File.data, &column_Info);
					if (pRecord) {
						add_One_Movie_To_The_List(&movie_List, pRecord);
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
				printf("sorted\n");
				print_The_List(&header_Of_File, &movie_List);
				 output_Result = 0;
		}
	}
		else {
			printf("Error! Unable to read file provided within directory. Please check file or directory\n");
		}

	freeBuffer(&header_Of_File);
	freeBuffer(&read_File);
	if (movie_List.pRecArray != NULL) {
		clearMovieList(&movie_List);
	}


	return output_Result;
}





//this checks if the given char string is directory or not
int is_Directory(const char * name) {
	DIR * temp_dir = opendir(name);
	if (temp_dir != NULL){
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
int scan_Directory(DIR * directory, char * sorting_Column, char * path){

	if(directory == NULL){
		printf("ERROR! Unable to open directory. NULL pointer\n");
		return -1;			//returning -1 because it is an error
	}

	//setting this value to an unusal value so that if later program breaks, we can check if this value changes or not
	int return_Value = -500;
	int PID = -500;		// this PID will store the PID of child going through sub-directory
	int PID_Two = -500;	//this PID will store the PID of child sorting csv file
	struct dirent * directory_Info;
	char current_dir_path[strlen(path) + 1];

	strcpy(current_dir_path, path);

	while ((directory_Info = readdir(directory))!= NULL){

printf("This is the file name part one: %s\n", directory_Info->d_name);

		//this if statement will skip over the first . and .. in the directory. d_name is the name of current file/pointer
		if((strcmp(directory_Info->d_name, ".") == 0) || (strcmp(directory_Info->d_name, "..") == 0)){
			continue;
		}

printf("This is the file name: %s\n", directory_Info->d_name);
		/*
		concatenating path as each file within directory
		will have different path.
		e.g-> directory/file1
			directory/file2
			directory/subdirectory/file3
		*/
		path = strcat(path, "/");
		path = strcat(path, directory_Info->d_name);


		//checks if the current file that file pointer points to is a directory or not
		return_Value = is_Directory(path);
		/*
		if return value is zero, it means this is directory
		and will perform recurssion from this point on that
		sub-directory
		*/
		if(return_Value == 0) {
			//**************this means it is directory, perform recurssion*****************
			PID = fork();
			int status = 0;		// this is just got parents to wait until child returns


			if(PID == 0) {			//this means it's a child Process
				strcpy(current_dir_path, path);	//changing the current path for child process as it is going to be at sub directory level
				directory = opendir(path);	//changing directory to sub directory for child process
			}
			else if(PID > 0) {
				wait(&status);	//parent waiting until child returns going through sub level directory;
			}
		}


		// if the read value is not directory, check if it's CSV file or not
		else {
			return_Value = is_CSV_File(path);
			if(return_Value == 0){
				//************this means it is CSV file, perform sorting******************
				PID = fork();
				int status_Two = 0;

				if(PID == 0) {
					FILE * file = fopen(path, "r");
					return_Value = sort_The_List(sorting_Column, file);
					return return_Value;
				}
				else if (PID > 0) {
					wait(&status_Two);	//parent waiting until child sorts the CSV file.
				}
			}
			else {
				printf("%s is not a csv file or directory\n", path);
				strcpy(path, current_dir_path);
				continue; //continuing to the next loop because read value isn't directory or CSV file
			}
		}
	strcpy(path,current_dir_path);	//changing directory/file1 to directory, because we are at same level but just looping to next file
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
