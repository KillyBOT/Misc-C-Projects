#include <stdio.h>
#include <stdlib.h>

void incByOne(int* p){
	*p += 1;
}

int main(){
	int* p = malloc(sizeof(int));
	*p = 1;
	printf("%d\n", *p);
	incByOne(p);
	printf("%d\n", *p);
}