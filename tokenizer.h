#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef struct Tokenizer {
	int 			isEnded;												// end of the line
	char*			word;												// word
	char*			current_pointer;
	int				delimiter;												// Delimiter is '''
	int				quote;													// Quote is '\"'
} Tokenizer;

char* trim(char* input);
void initialize_Tokenizer(Tokenizer* pTokenizer, char* _current_pointer, int _delimiter, int _quote);
char* getNextToken(Tokenizer* pTokenizer);
#endif
