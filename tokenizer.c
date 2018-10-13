#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tokenizer.h"
#define TRUE				1
#define FALSE				0

/*
Initilizes the tokenizer
*/
void initialize_Tokenizer(Tokenizer* pTokenizer, char* _current_pointer, int _delimiter, int _quote) {
	pTokenizer->current_pointer = _current_pointer;
	//
	pTokenizer->word = NULL;
	pTokenizer->delimiter = _delimiter;
	pTokenizer->quote = _quote;
	pTokenizer->isEnded = FALSE;
}

/*
this function breaks the line into each words
*/
char* getNextToken(Tokenizer* pTokenizer) {
	char* pOutput;

	// Checks for either NULL Tokenizer pointer or no more tokens
	if (!pTokenizer) {																												// Null Tokenizer
		return NULL;
	}
	else if (pTokenizer->isEnded) {																							// No more token
		return NULL;
	}

	// Trim leading space
	while (isspace((unsigned char)*(pTokenizer->current_pointer))) {
		pTokenizer->current_pointer++;
	}

	// Handle quoted token. First check if it is a quoted token (token inside double quotes)
	if (*(pTokenizer->current_pointer) == pTokenizer->quote ) {												// If quoted token
		pTokenizer->current_pointer++;																						// Go pass the open quote
		//
		char* pQuote = strchr(pTokenizer->current_pointer, pTokenizer->quote);							// Find the closing quote
		//
		if (pQuote != NULL) {
			int length = pQuote - pTokenizer->current_pointer;														// Length of token: between two quotes. Could be 0
			//
			pTokenizer->word = pTokenizer->current_pointer;													// Get token
			*(pTokenizer->word + length) = '\0';																		// NULL  terminated the token
			//
			pTokenizer->current_pointer = pQuote + 1;																// Go past closing quote
			if (pTokenizer->current_pointer != '\0') {
				char* pDelimiter = strchr(pTokenizer->current_pointer, pTokenizer->delimiter);		// Find the delimiter
				if (pDelimiter) {
					pTokenizer->current_pointer = pDelimiter + 1;													// Go past delimiter
				}
				else {
					pTokenizer->isEnded = TRUE;																			// No more token
				}
			}
			else {
				pTokenizer->isEnded = TRUE;																				// no more token
			}
		}
		else {																															// If pQuote is NULL (end quote not found)
			pTokenizer->word = pTokenizer->current_pointer;													// The remaining will be treat as one token
			pTokenizer->isEnded = TRUE;
		}
	}
	// Last token is empty token
	else if (*(pTokenizer->current_pointer) == '\0') {																// Empty token
			pTokenizer->word = pTokenizer->current_pointer;;
			pTokenizer->isEnded = TRUE;
	}
	// Token not quoted
	else {																																// Normal token
		char* pDelimiter = strchr(pTokenizer->current_pointer, pTokenizer->delimiter);				// Find the next delimiter
		if (pDelimiter) {
			int length = pDelimiter - pTokenizer->current_pointer;
			//
			// Length could be 0
			pTokenizer->word = pTokenizer->current_pointer;													// Get token
			*(pTokenizer->word + length) = '\0';																		// NULL  terminated the token
			//
			pTokenizer->current_pointer = pDelimiter + 1;															// Go past delimiter
		}
		else {
			pTokenizer->word = pTokenizer->current_pointer;													// The remaining will be treated as one token
			pTokenizer->isEnded = TRUE;
		}
	}

	//
	pOutput = trim(pTokenizer->word);
	pTokenizer->word = pOutput;

	//
	return pOutput;
}


//this function deletes the unnecessary
char* trim(char* input_String) {
		if (*input_String == '\0') {
		}
		else {
			while (isspace((unsigned char)*input_String)) {
				input_String++;
			}
			if (*input_String == '\0') {
				//
			}
			else {
				char *end;
				//deleting the first spaces from the string, because they are not needed
				end = input_String + strlen(input_String) - 1;
				//
				while (end > input_String && isspace((unsigned char)*end)) {
					end--;
				}
				*(end+1) = 0;
			}
		}
	return input_String;
}
