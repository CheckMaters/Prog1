#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "generalFunctions.h"
#include "columnsorter.h"
#include "movieListData.h"
#include "mergesort.h"
#define TRUE				1
#define FALSE				0


void merge(void* arr[], int l, int m, int r, void* pHint, FUNCTION_POINTER_CMPDATA pFuncCompare) {
    int n1 = m - l + 1;
    int n2 =  r - m;
    int i, j, k;
    void* temp_Array_One[n1];
	  void* temp_Array_Two[n2];
    for (i = 0; i < n1; i++) {
		temp_Array_One[i] = arr[i + l];
	}
    for (j = 0; j < n2; j++) {
        temp_Array_Two[j] = arr[1+ j + m];
	}
    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (pFuncCompare(temp_Array_One[i], temp_Array_Two[j], pHint) <= 0 ) {
            arr[k] = temp_Array_One[i];
            i++;
        }
		else {
            arr[k] = temp_Array_Two[j];
            j++;
        }
        k++;
    }

    // Checking and copying the remaining elements of temp_Array_One[]
    while (i < n1) {
        arr[k] = temp_Array_One[i];
        i++;
        k++;
    }
	// Checking and copying the remaining elements of temp_Array_Two[]
    while (j < n2) {
		arr[k] = temp_Array_Two[j];
        j++;
        k++;
    }
}



void mergeSort(void* arr[], int l, int r, void* pHint, FUNCTION_POINTER_CMPDATA pFuncCompare) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, pHint, pFuncCompare);
        mergeSort(arr, m + 1, r, pHint, pFuncCompare);

        merge(arr, l, m, r, pHint, pFuncCompare);
    }
}
