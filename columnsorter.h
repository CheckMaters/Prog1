#ifndef COLUMNSORTER_H
#define COLUMNSORTER_H

#define INDEX_TYPE_INTEGER		10
#define INDEX_TYPE_STRING			123
#define INDEX_TYPE_FLOAT			23

/*
this struct keeps the data related to
the column on which sorting is going
to occur. It keeps track of column
number and also the type of that
column i.e., String OR Integer OR
Float.
*/
typedef struct sorting_Column_Info {
	int index_Type_SIF;														// SIF: STRING, INTEGER or FLOAT
	int index;
} sorting_Column_Info;
/*
Gets the index for the column-To-Sort from the Top Line of the file
*/
int get_Column_Index_To_Sort_List(char* key, sorting_Column_Info* pcolumn_Info, char* input);

#endif
