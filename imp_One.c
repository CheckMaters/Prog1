#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<malloc.h>



typedef struct _node
{
char * value;
} node;

typedef struct _linked_List
{
node* movie_Value_List;

}linked_List;



int checkValueType (char* word) {
	if(strcmp(word,"color")==0 || strcmp(word,"director_name")==0 || strcmp(word,"actor_2_name")==0 || strcmp(word,"genres")==0 || strcmp(word,"actor_1_name")==0 || strcmp(word,"movie_title")==0 || strcmp(word,"actor_3_name")==0 || strcmp(word,"plot_keywords")==0 || strcmp(word,"movie_imdb_link")==0 || strcmp(word,"language")==0 || strcmp(word,"counrty")==0 || strcmp(word,"content_rating") ){
	return 1;
	}
	else if (strcmp(word,"imdb_score")==0 || strcmp(word,"ascept_ratio")==0 || strcmp(word,"budget")==0) {
	return 3;
	}
	else {
	return 2;
	}
}



int main(int argc, char * argv[]) {

//this prints out an error if the number of arguments are not correct
		if(argc!=3)
		{
		fprintf(stderr, "%s", "ERROR! Check Arguments!\n");
		return 0;
		}

	char * value_Type_To_Sort_List = argv[2];					//the value type based on which the list is sorted
	int value_Type_Type = checkValueType (value_Type_To_Sort_List);			//1=String, 2=Integer, 3=Float
	FILE* file = stdin;								//FILE pointer to the file entered in stdin


//prints out error if can't open the file or the file is empty
		if(file==NULL)
		{
		fprintf(stderr, "%s", "File doesn't exists or file is empty\n");
		return 0;
		}

	int line_Length = 500;							//assuming the length of line is 128, later it could be reallocated
	char * line = (char*) malloc (line_Length * sizeof(char)); 		//allocating memory to store a line of all value types from the file
	char * line_Two = (char*) malloc (line_Length * sizeof(char));
	char ch = fgetc(file);							//reads ever single character in file line by line
	int count = 0;								//counts the number of integer in line

	while((ch != '\n') && (ch != EOF))
	{
			//if there are more characters in each line, then this will reallocate memory to line
			if(count == line_Length)
			{
				line_Length +=500;
				line = realloc(line, line_Length);
				line_Two = realloc(line_Two, line_Length);

					if(line==NULL)
					{
					fprintf(stderr, "%s", "Error allocating memory!\n");
					return 0;
					}
			}
		line[count] = ch;
		line_Two[count] = ch;
		count++;
		ch = fgetc(file);
	}	//while loop end





//this loop counts the no. of columns given in the file
	char delim[] = ", ";
	char* word = strtok(line, delim);
	int no_Of_Columns = 0;
	while(word != NULL)
	{
		no_Of_Columns++;
		word = strtok(NULL, delim);
	}
free(word);


//making a linked list of columns
	int xyz;
	char * word_One = strtok(line_Two, delim);
	int no_Of_Word_Status = 0;
	node* first_Line_List = (node*) malloc (no_Of_Columns * sizeof(node));
	while(word_One != NULL) {
		char * temp_Word = (char*) malloc (strlen(word_One) * sizeof(char));
		for(xyz = 0; xyz < strlen(word_One); xyz++)
		{
			temp_Word[xyz] = word_One[xyz];
		}

		(first_Line_List + no_Of_Word_Status) -> value = temp_Word;
		word_One = strtok(NULL, delim);
		no_Of_Word_Status++;
	}


free(line);
free(line_Two);

while(ch != EOF){

}



fclose(file);

return 0;
}
