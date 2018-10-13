#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "generalFunctions.h"
#define TRUE				1
#define FALSE				0

int strcicmp(char const *input1, char const *input2) {
	int i;
    for (;; input1++, input2++) {
        i = tolower(*input1) - tolower(*input2);
        if (i != 0 || !*input1) {
            return i;
		}
    }
}

void freeBuffer(BUFFER* pBuf) {
	if (pBuf->data) {
		free(pBuf->data);
		//
		pBuf->data = NULL;
		pBuf->length_Of_Data = 0;
	}
}

void delete_Newline_Char_At_The_End(BUFFER* pBuf) {
	int len = strlen(pBuf->data);
	if ( *(pBuf->data + len - 1) == '\n' ) {
		*(pBuf->data + len - 1)  = '\0';
	}
}
