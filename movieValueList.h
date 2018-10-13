#ifndef MOVIEVALUELIST_H
#define MOVIEVALUELIST_H

// Array of records
typedef struct movie_Record {
	one_Movie_Values** 	pRecArray;				// Array of Record pointer
	int iCapacity;					// Total Size of the Record
	int	iSize;						// Current size
} movie_Record;

/*
Inserts one movie into the movie_List
*/
int add_One_Movie_To_The_List(movie_Record* pmovie_Record, one_Movie_Values* pRecord);

void clearMovieList(movie_Record* pmovie_Record);


#endif
