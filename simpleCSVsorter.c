#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
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


	int return_Val_For_Main = -1; 			//this is return value

	//checking if the input argument is correct or Not
	if(argc == 3) {
		if(strcmp("-c", argv[1]) != 0) {
			fprintf(stderr, "Error! Parameters are not correctly formated\n");
			return -1;    //returning -1 because it's an error
		}


		/*
		since there isn't any -d in the Parameter
		we are going to use the current working
		directory as input and output directory
		*/

		char * directory_Char = (char *) calloc (2 , sizeof(char));
		strcpy(directory_Char,".");
//printf("%s\n", directory_Char)

		/*
		this will create a directory and also
		a struct that keeps all the information
		about directory.
		*/

		DIR * directory = opendir(directory_Char);

		if(directory == NULL) {
			fprintf(stderr, "Error! Given directory is pointing to NULL\n");
			return -1;		//returning -1 because it's an error
		}

		char* sort_By_This_Value = argv[2];
		return_Val_For_Main = scan_Directory(directory, sort_By_This_Value, directory_Char);
		//closedir(directory);
		return return_Val_For_Main;
	}

	//the output file is not given in the parameter
	else if(argc == 5 || argc == 7) {

		if(strcmp("-c", argv[1]) != 0 || strcmp("-d", argv[3]) != 0) {
			fprintf(stderr, "Error! Parameters are not correctly formated\n");
			return -1;				//returning -1 because it's an error
		}

		if(argc == 7) {
			if(strcmp("-o", argv[5]) != 0) {
				fprintf(stderr, "Error! Parameters are not correctly formated\n");
				return -1;		//returning -1 because it's an error
			}
		}

		/*
		this will create a directory and also
		a struct that keeps all the information
		about directory.
		*/
			char * directory_Char = (char *) calloc ((strlen(argv[4]) + 1), sizeof(char));
			strcpy(directory_Char, argv[4]);
			DIR * directory;
			directory = opendir(directory_Char);

		if(directory == NULL) {
			fprintf(stderr, "Error! Given directory is pointing to NULL\n");
			return -1;		//returning -1 because it's an error
		}

		char* sort_By_This_Value = argv[2];
		return_Val_For_Main =  scan_Directory(directory, sort_By_This_Value, directory_Char);
		//closedir(directory);
		return return_Val_For_Main;
	}

	else {
		fprintf(stderr, "Error! Parameters are not correctly formated\n");
		return -1;  			//returning -1 because it's an error
	}

return 0;

}



/*
sort_The_List is the function that
is going to sort the stdin movie_List
file in alphabetical order. The column
that is going to be sorted is represented
by sort_By_This_Value.
*/
int sort_The_List(char* sort_By_This_Value, FILE* file, char * path, char * file_Name) {

							// Buffer for reading from stdin
	BUFFER read_File;					// Buffer is created to read the stdin File
	BUFFER header_Of_File;					// For holding the header line


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
	//	int no_Use = 0;


	// Read the header and get the index for the sorting key
	if (getline(&(read_File.data), &(read_File.length_Of_Data), file) > 0 ) {
		delete_Newline_Char_At_The_End(&read_File);		// this will delete the new line character from the end of the string line

		header_Of_File.data = (char*)malloc (sizeof(char) * read_File.length_Of_Data);
		header_Of_File.length_Of_Data = read_File.length_Of_Data;


		strcpy(header_Of_File.data, read_File.data);			// copy of header line before tokenizing

		//head_Of_File keeps record of the first line that is that name of all the columns

		if (get_Column_Index_To_Sort_List(sort_By_This_Value, &column_Info, read_File.data)) {
				fprintf(stderr, "Error! Unable to parse the input column name\n");
		}
		else {
		// Read data line by line. Creates record and put them into array

				while (getline(&(read_File.data), &(read_File.length_Of_Data), file) > 0) {

					delete_Newline_Char_At_The_End(&read_File);
					one_Movie_Values* pRecord = make_Movie_Value_List(read_File.data, &column_Info);

					if (pRecord) {
						add_One_Movie_To_The_List(&movie_List, pRecord);
						//no_Use =	add_One_Movie_To_The_List(&movie_List, pRecord);
					}
					else {
						fprintf(stderr, "Error! Unable to create movie list!\n");
					}
				}

				/*
				merge sort will sort the array of movie list based
				on the index type and column name entered in the
				command prompt
				*/

				mergeSort((void**)movie_List.pRecArray, 0, movie_List.iSize - 1, &column_Info, pFuncCompare);

				// Output sorted records to file
				print_The_List(&header_Of_File, &movie_List, path, file_Name, sort_By_This_Value);

				//printf("sorted\n");
				output_Result = 0;
		}
	}

	else {
		fprintf(stderr,"Error! Unable to read file provided within directory. Please check file or directory\n");
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
			closedir(temp_dir);
			return 0;	//returns 0 if it's directory, else -1
		}
	return -1;
}







//this checks if the given char string is CSV file or not
int is_CSV_File (const char * name, char * column) {

//printf("CHECKING THIS FILE : %s with this column %s\n\n", name, column);

	char * test_Name = (char *) malloc ((strlen(column) + 5)* sizeof(char));
	strncpy(test_Name, column, strlen(column));
	strcat(test_Name, ".csv");

//printf("this is what we are checking : %s\n\n", test_Name);

	if((strlen(name) > (strlen(column) + 4)) && !(strcmp(name + strlen(name) - (strlen(column) + 4), test_Name))){
		free(test_Name);
		fprintf(stderr, "Error! This file is already sorted by the sorter\n");
		return -1;		//returning -1 because this file is already sorted
	}

	if(strlen(name) > 4 && !(strcmp(name + strlen(name) - 4, ".csv"))){
		free(test_Name);
		return 0;			//returns 0 if it's csv file, else -1
	}

	fprintf(stderr, "Error! This isn't a CSV file!\n");
	free(test_Name);
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
int scan_Directory(DIR * directory, char * sorting_Column, char * dir_path){

	if(directory == NULL){
		fprintf(stderr, "Error! Unable to open directory. NULL pointer\n");
		return -1;			//returning -1 because it is an error
	}


	//setting this value to an unusal value so that if later program breaks, we can check if this value changes or not
	int return_Value = -500;
	int PID = -500;		// this PID will store the PID of child going through sub-directory
	//int PID_Two = -500;	//this PID will store the PID of child sorting csv file
	struct dirent * directory_Info; //this is the struct that will keep info about each the file in each while loop


//char current_dir_path[strlen(dir_path) + 1];
	char * current_dir_path = (char *) calloc ((strlen(dir_path) + 1), sizeof(char));
	strcpy(current_dir_path, dir_path);

//free(dir_path);
	dir_path = (char *) calloc (1 + strlen(current_dir_path), sizeof(char));

	while ((directory_Info = readdir(directory))!= NULL){

//printf("This is the file name part one: %s\n", directory_Info->d_name);

			//this if statement will skip over the first . and .. in the directory. d_name is the name of current file/pointer
			if((strcmp(directory_Info->d_name, ".") == 0) || (strcmp(directory_Info->d_name, "..") == 0)){
				continue;
			}


//printf("%s\n", directory_Info->d_name);
//printf("This is the file name: %s\n", directory_Info->d_name);


				/*
				concatenating dir_path as each file within directory
				will have different dir_path.
				e.g-> directory/file1
				directory/file2
				directory/subdirectory/file3
				*/
//dir_path = (char *) realloc (dir_path, (2 + strlen(directory_Info->d_name)) * sizeof(char));

			space_Needed(dir_path, 2 + strlen(directory_Info->d_name));
			strcat(dir_path, "/");
			strcat(dir_path, directory_Info->d_name);

//printf("%s\n", dir_path);



					//checks if the current file that file pointer points to is a directory or not
			return_Value = is_Directory(dir_path);


				/*
				if return value is zero, it means this is directory
				and will perform recurssion from this point on that
				sub-directory
				*/

			if(return_Value == 0) {
				//**************this means it is directory, perform recursion*****************
				PID = fork();
				int status = 0;		// this is just got parents to wait until child returns

				if(PID == 0) {			//this means it's a child Process
						space_Needed(current_dir_path, 2 + strlen(directory_Info->d_name));
						strcat(current_dir_path, "/");
						strcat(current_dir_path, directory_Info->d_name);
//strcpy(current_dir_path, dir_path);	//changing the current dir_path for child process as it is going to be at sub directory level
						directory = opendir(dir_path);	//changing directory to sub directory for child process
						//free(dir_path);
						dir_path = (char *) calloc (strlen(current_dir_path) + 1, sizeof(char));
						continue;
				}
				else if(PID > 0) {
						wait(&status);	//parent waiting until child returns going through sub level directory;
				}
			}


			// if the read value is not directory, check if it's CSV file or not
			else {
				return_Value = is_CSV_File(dir_path, sorting_Column);

				if(return_Value == 0){
						//************this means it is CSV file, perform sorting******************
						PID = fork();
						int status_Two = 0;

						if(PID == 0) {
							FILE * file = fopen(dir_path, "r");
							return_Value = sort_The_List(sorting_Column, file, dir_path, directory_Info->d_name);
							fclose(file);
							free(dir_path);
							free(current_dir_path);
							return return_Value;
						}
						else if (PID > 0) {
							wait(&status_Two);	//parent waiting until child sorts the CSV file.
						}
				}
				else {
//printf("%s is not a csv file or directory\n", dir_path);
//**************************************************
						//free(dir_path);
						dir_path = (char *) calloc ((1 + strlen(current_dir_path)) , sizeof(char));
						strcpy(dir_path, current_dir_path);
						continue; //continuing to the next loop because read value isn't directory or CSV file
				}
			}
//***************************************************
//					free(dir_path);
//					dir_path = (char *) calloc ((1 + strlen(current_dir_path)) , sizeof(char));
//					strcpy(dir_path,current_dir_path);	//changing directory/file1 to directory, because we are at same level but just looping to next file

	} //while loop ends here

				//closedir(directory);
				//free(dir_path);
				free(current_dir_path);
				return return_Value;
}






//this function will remove the .csv from the end of the char pointer

void remove_CSV (char * path, char * new_Path){
	if(strcmp(path + strlen(path) - 4, ".csv") == 0){
		strncpy(new_Path, path, strlen(path) - 4);
	return;
	}
	return;
}



//this function will reallocate the memory of size size_S and type char if needed.
void space_Needed (char * here, int size_S){
  here = (char *) realloc (here, size_S);
  return;
}


/*
Writes the data from the movie_Record into stdout
*/
void print_The_List(BUFFER* pHeader, movie_Record* pRecordArray, char * path, char * file_Name, char * column) {

	char * output_File = (char * ) malloc ((strlen(path) + strlen(column) + 10) * sizeof(char));
	//char * new_Path;
	char * new_Path = (char * ) malloc ( strlen(path)  * sizeof(char) );
	remove_CSV(path, new_Path);
//printf("Path with out csv : %s\n", new_Path);
	strcpy(output_File, new_Path);
	//strcat(output_File, file_Name);
	strcat(output_File, "-sorted-");
	strcat(output_File, column);
	strcat(output_File, ".csv");
//printf("Output file path : %s\n", output_File);
	FILE * file = fopen(output_File, "w");
if(file == NULL ){
	fprintf(stderr, "Unable to open the output file.\n\n");
}

	fprintf(file, "%s\n", pHeader->data);																										// Print header line

	//print all records
	int xyz;
	for (xyz = 0; xyz < pRecordArray->iSize; xyz++) {
		*((*(pRecordArray->pRecArray + xyz))->sorting_Key_Term) = (*(pRecordArray->pRecArray + xyz))->chHold;		// Put back the char set to '\0' for sorting key
		fprintf(file, "%s\n", (*(pRecordArray->pRecArray + xyz))->recordData);													// Print Records in a loop
	}
free(output_File);
free(new_Path);
	return;
}
