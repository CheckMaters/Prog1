#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "columnsorter.h"
#include "movieListData.h"
#include "movieValueList.h"
#define TRUE				1
#define FALSE				0


//Inserting one movie to the movie list
int add_One_Movie_To_The_List(movie_Record* pointer_To_MovieList_Array, one_Movie_Values* pointer_To_Movie) {
	int output_Result = 1;
	// checking inputs if they are correct or not
	if (pointer_To_MovieList_Array == NULL || (pointer_To_MovieList_Array->pRecArray) == NULL || pointer_To_Movie==NULL) {
		output_Result = 1;
	}
	else {
		if (pointer_To_MovieList_Array->iCapacity > pointer_To_MovieList_Array->iSize) {
			*(pointer_To_MovieList_Array->pRecArray + pointer_To_MovieList_Array->iSize) = pointer_To_Movie;
			pointer_To_MovieList_Array->iSize++;

			output_Result = 0;
		}
		else {
			pointer_To_MovieList_Array->pRecArray = (one_Movie_Values**)realloc(pointer_To_MovieList_Array->pRecArray, sizeof(one_Movie_Values) * (pointer_To_MovieList_Array->iCapacity + 1024));
			pointer_To_MovieList_Array->iCapacity += 1024;
			*(pointer_To_MovieList_Array->pRecArray + pointer_To_MovieList_Array->iSize) = pointer_To_Movie;
			pointer_To_MovieList_Array->iSize++;

			output_Result = 0;
		}
	}

	return output_Result;
}

void clearMovieList(movie_Record* pointer_To_MovieList_Array) {
	if (pointer_To_MovieList_Array != NULL) {
		int i;
		for (i = 0; i < pointer_To_MovieList_Array->iSize; i++) {
			free( (*(pointer_To_MovieList_Array->pRecArray + i)) );
		}

		free(pointer_To_MovieList_Array->pRecArray);
	}
}
