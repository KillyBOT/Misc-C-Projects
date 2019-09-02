#include <stdio.h>
#include <stdlib.h>

#define INT 0
#define CHAR 1
#define DOUBLE 2
#define SHORT 4
#define LONG 8

#define K 5471
#define P 3221
#define N 2153

union rawData{
	int i;
	char c;
	char** s;
	double d;
	short sh;
	long l;
};

struct tableData{
	union rawData data;
	char type;
};

struct node{
	struct tableData cargo;
	//char dataType;
	struct node* nextNode;
};

struct hashTable{
	struct node** rawData;
	int (*hashFunction)(int);
};

int* createSieve(int size);
int findPrime(int location);
void printSieve(int* sieve, int size);

struct node* createLinkedList();
struct tableData createIntData(int data);
struct tableData createDoubleData(double data);
void addLL(struct node* head, int index, struct tableData cargo);
void addLLEnd(struct node* head, struct tableData cargo);
int llLength(struct node* head);
void removeLL(struct node* head, int index);
struct node* getLLNode(struct node* head, int index);
struct tableData getLL(struct node* head, int index);

struct hashTable* createHashTable(int (*hashFunction)(int));
int perfectHashFunction(int x);
void addHT(struct hashTable* hTable, struct tableData data);
int checkIfInTable(struct hashTable* hTable, struct tableData data);
void printHashFunction(int (*hashFunction)(int), int size);

int main(){
	printf("%d\n", findPrime(30));
	union rawData test;
	test.d = 8.6;
	printf("%d\n", test.i);
	struct hashTable* testHashTable = createHashTable(perfectHashFunction);
	addHT(testHashTable, createDoubleData(8.6));
	printf("%d\n", checkIfInTable(testHashTable,createDoubleData(8.6)));
	printHashFunction(&perfectHashFunction, 40);
}

int* createSieve(int size){
	int* sieve = malloc(sizeof(int) * size);
	for(int x = 0; x < size; x++){
		sieve[x] = ((x+1) * 2) + 1;
	}
	return sieve;
}

int findPrime(int location){
	if(location == 0) return 2;
	int* sieve = createSieve(location * location + 5);
	int currentPrime = sieve[0];
	int currentPrimeInc = 1;
	int currentPrimePos = 0;

	int sp;
	while(currentPrimeInc < location){
		sp = currentPrimePos;
		while(sp < location * location + 5){
			sieve[sp] = 0;
			sp += currentPrime;
		}
		currentPrimePos += 1;
		while(sieve[currentPrimePos] == 0){
			currentPrimePos += 1;
		}
		currentPrime = sieve[currentPrimePos];
		currentPrimeInc += 1;

	}
	return currentPrime;
}

void printSieve(int* sieve, int size){
	for(int x = 0; x < size; x++){
		printf("%d ",*(sieve + x));
	}
	printf("\n");
}

struct node* createLinkedList(){
	struct node* head = (struct node*)malloc(sizeof(struct node));
	return head;
}

struct tableData createIntData(int data){
	struct tableData toRet;
	toRet.type = INT;
	toRet.data.i = data;
	return toRet;
}

struct tableData createDoubleData(double data){
	struct tableData toRet;
	toRet.type = DOUBLE;
	toRet.data.d = data;
	return toRet;
}

struct node* getLLNode(struct node* head, int index){
	struct node* current = head;
	for(int x = -1; x < index; x++){
		if(current == NULL) return NULL;
		current = current->nextNode;
	}
	return current;
}

void addLL(struct node* head, int index, struct tableData cargo){
	struct node* prevNode = getLLNode(head, index-1);
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	struct tableData newData;
	newNode->cargo = cargo;
	newNode->nextNode = prevNode->nextNode;
	prevNode->nextNode = newNode;
}

void addLLEnd(struct node* head, struct tableData cargo){
	struct node* current = head;
	while(current->nextNode != NULL){
		current = current->nextNode;
	}
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	newNode->cargo = cargo;
	current->nextNode = newNode;
}

void removeLL(struct node* head, int index){
	struct node* prevNode = getLLNode(head, index-1);
	struct node* toRemove = getLLNode(head, index);
	prevNode->nextNode = toRemove->nextNode;
}

int llLength(struct node* head) {
	struct node* currentNode = head->nextNode;
	int size = 0;
	while(currentNode != NULL){
		currentNode = currentNode->nextNode;
		size++;
	}

	return size;
};

struct tableData getLL(struct node* head, int index){
	return getLLNode(head, index)->cargo;
}

struct hashTable* createHashTable(int (*hashFunction)(int)){
	struct hashTable* hTable = (struct hashTable*)malloc(sizeof(struct hashTable));
	hTable->hashFunction = &perfectHashFunction;
	hTable->rawData = (struct node**)malloc(sizeof(struct node) * N);
	for(int x = 0; x < N; x++){
		hTable->rawData[x] = createLinkedList();
	}
	return hTable;
}

int perfectHashFunction(int x){
	return ( (K * x) % P) % N;
}

void addHT(struct hashTable* hTable, struct tableData data){
	int hashedData = (*hTable->hashFunction)(data.data.i % 4294956296);
	addLLEnd(hTable->rawData[hashedData],data);
}

int checkIfInTable(struct hashTable* hTable, struct tableData data){
	int hashedData = (int)(*hTable->hashFunction)(data.data.i % 4294956296);
	struct node* head = hTable->rawData[hashedData];
	if(head->nextNode == NULL) return 0;
	else {
		head = head->nextNode;
		while(head != NULL){
			if(data.type == INT){
				if(head->cargo.data.i == data.data.i) return 1;
			} else if (data.type == DOUBLE){
				if(head->cargo.data.d == data.data.d) return 1;
			}

			//TODO: Add more types here

		}
		return 0;
	}
}

void printHashFunction(int (*hashFunction)(int), int size){
	for(int x = 0; x < size; x++){
		printf("%d  ", (*hashFunction)(x));
	}
	printf("\n");
}