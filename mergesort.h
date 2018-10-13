#ifndef MERGESORT_H
#define MERGESORT_H


typedef int (*FUNCTION_POINTER_CMPDATA)(void* pData1, void* pData2, void* pHint);
void merge(void* arr[], int l, int m, int r, void* pHint, FUNCTION_POINTER_CMPDATA pFuncCompare);
void mergeSort(void* arr[], int l, int r, void* pHint, FUNCTION_POINTER_CMPDATA pFuncCompare);

#endif
