#include <stdio.h>
#include <stdlib.h>

#define INT 0
#define CHAR 1
#define DOUBLE 2
#define SHORT 4
#define LONG 8

#define K 7703
#define P 3011
#define N 5651

union rawData{
	int intData;
	char charData;
	double doubleData;
	short shortData;
	long longData;
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
void addLL(struct node* head, int index, int element);
void addLLEnd(struct node* head, int element);
int llLength(struct node* head);
void removeLL(struct node* head, int index);
struct node* getLLNode(struct node* head, int index);
int getLL(struct node* head, int index);

struct hashTable* createHashTable(int (*hashFunction)(int));
int perfectHashFunction(int x);
void addHT(struct hashTable* hTable, int data);
int checkIfInTable(struct hashTable* hTable, int data);

int main(){
	printf("%d\n", findPrime(30));
	struct hashTable* testHashTable = createHashTable(perfectHashFunction);
	addHT(testHashTable, 5);
	printf("%d\n", checkIfInTable(testHashTable,6));
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

struct node* getLLNode(struct node* head, int index){
	struct node* current = head;
	for(int x = -1; x < index; x++){
		if(current == NULL) return NULL;
		current = current->nextNode;
	}
	return current;
}

void addLL(struct node* head, int element, int index){
	struct node* prevNode = getLLNode(head, index-1);
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	struct tableData newData;
	newData.data.intData = element;
	newData.type = INT;
	newNode->cargo = newData;
	newNode->nextNode = prevNode->nextNode;
	prevNode->nextNode = newNode;
}

void addLLEnd(struct node* head, int element){
	struct node* current = head;
	while(current->nextNode != NULL){
		current = current->nextNode;
	}
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	struct tableData newData;
	newData.data.intData = element;
	newData.type = INT;
	newNode->cargo = newData;
	current->nextNode = newNode;
}

void removeLL(struct node* head, int index){
	struct node* prevNode = getLLNode(head, index-1);
	struct node* toRemove = getLLNode(head, index);
	prevNode->nextNode = toRemove->nextNode;
}

int llLength(struct node* head) {
	struct node* currentNode = head;
	int size = 0;
	while(currentNode != NULL){
		currentNode = currentNode->nextNode;
		size++;
	}

	return size;
};

int getLLInt(struct node* head, int index){
	return getLLNode(head, index)->cargo.data.intData;
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

void addHT(struct hashTable* hTable, int data){
	int hashedData = (*hTable->hashFunction)(data);
	addLLEnd(hTable->rawData[hashedData],data);
}

int checkIfInTable(struct hashTable* hTable, int data){
	int hashedData = (*hTable->hashFunction)(data);
	struct node* head = hTable->rawData[hashedData];
	if(head->nextNode == NULL) return 0;
	else {
		head = head->nextNode;
		while(head != NULL){
			if(head->cargo.data.intData == data) return 1;
		}
		return 0;
	}
}