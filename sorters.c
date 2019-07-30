#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void qSort(void *toSort, size_t elemSize, size_t len, int compare(void*, void*));
void qSortInt(int *toSort, size_t size);
void mSort(void *toSort, size_t elemSize, size_t len, int compare(void*, void*));
void mSortInt(int* toSort, size_t size);
int compareInt(void* x, void* y);
void swapInt(int *x, int *y);
void printArray(int* toPrint, size_t size);

void swapInt(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

void swap(void* x, void* y, size_t len){
	void* temp = malloc(len);
	memcpy(temp, y, len);
	memcpy(y, x, len);
	memcpy(x, temp, len);
	free(temp);
}

int compareInt(void* x, void* y){
	int a = *(int *)x;
	int b = *(int *)y;

	if(a > b) return 1;
	else if(a == b) return 0;
	else return -1;
}

void printArray(int* toPrint, size_t size){
	for(int x = 0; x < size; x++){
		printf("%d ",toPrint[x]);
	}
	printf("\n");
}

void qSort(void *toSort, size_t elemSize, size_t len, int compare(void*, void*)){
	if(len > 1){
		void *pivot = toSort + (len * elemSize) - elemSize;

		int leftPlace = 0;
		int rightPlace = len - 2;

		while(leftPlace <= rightPlace){

			if( compare(toSort + (leftPlace * elemSize), pivot) <= 0) leftPlace++;
			else if( compare(toSort + (rightPlace * elemSize), pivot) > 0) rightPlace--;
			else{
				swap(toSort + (leftPlace * elemSize), toSort + (rightPlace * elemSize), elemSize);
			}
		}

		swap(pivot, toSort + (leftPlace * elemSize), elemSize);
		qSort(toSort, elemSize, leftPlace, compare);
		qSort(toSort + (leftPlace * elemSize), elemSize, len - leftPlace, compare);
	}
}

void qSortInt(int* toSort, size_t size){
	if(size > 1){
		int pivot = size-1;

		int leftPlace = 0;
		int rightPlace = size - 2;

		while(leftPlace <= rightPlace){
			printArray(toSort, size);
			if(*(toSort + leftPlace) <= *(toSort + pivot)) leftPlace++;
			else if(*(toSort + rightPlace) > *(toSort + pivot)) rightPlace--;
			else{
				swapInt(toSort + leftPlace, toSort + rightPlace);
			}
		}

		swapInt(&toSort[leftPlace], &toSort[pivot]);
		qSortInt(toSort, leftPlace);
		qSortInt(toSort + leftPlace, size - leftPlace);
	}
	
	
}

void mSort(void* toSort, size_t elemSize, size_t len, int compare(void*, void*)){
	printArray((int*)toSort,len);

	if(len > 1){
		mSort(toSort, elemSize, (int)len / 2, compare);
		mSort(toSort + ((int)len / 2), elemSize, (int)len - ((int)len / 2), compare);

		void* sortedList = malloc(len * elemSize);

		int leftPlace = 0;
		int rightPlace = len / 2;
		int sortedPlace = 0;

		while(sortedPlace < len){
			if(leftPlace >= len / 2){
				memcpy(sortedList + (sortedPlace * elemSize), toSort + (rightPlace * elemSize), elemSize);
				rightPlace++;
			}
			else if(rightPlace >= len){
				memcpy(sortedList + (sortedPlace * elemSize), toSort + (leftPlace * elemSize), elemSize);
				leftPlace++;
			}
			else if(compare(toSort + (leftPlace * elemSize), toSort + (rightPlace * elemSize)) >= 0){
				memcpy(sortedList + (sortedPlace * elemSize), toSort + (rightPlace * elemSize), elemSize);
				rightPlace++;
			} else {
				memcpy(sortedList + (sortedPlace * elemSize), toSort + (leftPlace * elemSize), elemSize);
				leftPlace++;
			}
		}

		for(int x = 0; x < len; x++){
			memcpy(toSort + (x * elemSize), sortedList + (x * elemSize), elemSize);
		}
		free(sortedList);
	}
}

void mSortInt(int* toSort, size_t size){
	if(size > 1){
		mSortInt(toSort, size/2);
		mSortInt(toSort + (size/2), size - (size/2));

		int* sortedList = malloc(size * sizeof(int));

		int leftPlace = 0;
		int rightPlace = size/2;
		int sortedPlace = 0;

		while(sortedPlace < size){
			if(leftPlace >= size/2){
				sortedList[sortedPlace] = toSort[rightPlace];
				rightPlace++;
			} else if(rightPlace >= size){
				sortedList[sortedPlace] = toSort[leftPlace];
				leftPlace++;
			} else if(toSort[leftPlace] >= toSort[rightPlace]){
				sortedList[sortedPlace] = toSort[rightPlace];
				rightPlace++;
			} else {
				sortedList[sortedPlace] = toSort[leftPlace];
				leftPlace++;
			}
			sortedPlace++;
		}

		for(int x = 0; x < size; x++){
			toSort[x] = sortedList[x];
		}
		free(sortedList);

	}
}

int main(){
	int testArray[20] = {0,19,1,18,2,17,3,12,4,15,5,14,6,13,7,12,8,11,9,10};

	//qSortInt(testArray,sizeof(testArray)/sizeof(testArray[0]));
	mSort(testArray, sizeof(testArray[0]), sizeof(testArray) / sizeof(testArray[0]), compareInt);
	//mSortInt(testArray,sizeof(testArray)/sizeof(testArray[0]));
	printArray(testArray,sizeof(testArray)/sizeof(testArray[0]));
}