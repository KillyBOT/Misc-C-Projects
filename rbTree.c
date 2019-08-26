#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLACK 0
#define RED 1
#define TREE_SIZE 50

struct Node{
	int key;
	int color; // 0 = Black, 1 = Red
	int height;
	int count;
	struct Node* left;
	struct Node* right;
	struct Node* parent;
};

typedef struct Node Node;

Node* createNode(int key, int color);
Node* rbAdd(Node* root, int key);
Node* rbAdd_helper(Node* currentNode, Node* toRBAdd);
Node* fixHeights(Node* root, int rootHeight);
Node* fixViolation(Node* root, Node* rbAdded);
Node* leftRot(Node* z);
Node* rightRot(Node* z);

Node* rbRemove(Node* root, int key);
Node* rbReplace(Node* v,Node* u);

int getHeight(Node* root);
int getSize(Node* root);
int checkBlack(Node* root);
int checkBlackHelper(Node* root, int currentBlackNum);
int checkRedViolated(Node* root);
int checkOrderViolated(Node* root);
int checkValidity(Node* root);
void infixPrint(Node* root);
void infixPrint_helper(Node* root);
void levelOrderPrint(Node* root);
void levelOrderPrintHelper(Node* root);

int main(){
	//Node root = {0, 0, 1,NULL, NULL, NULL};
	Node* root = NULL;
	srand(time(NULL));
	int randVals[TREE_SIZE];
	//root = &root;

	
	/*//int toRBAdd[] = {7,3,18,10,22,8,11,26,2,6,13};
	//int toRBAdd[] = {16,8,20,26,28,24};
	//int toRBAdd[] = {27,28,2,21,25};
	for(int x = 0; x < sizeof(toRBAdd) / sizeof(toRBAdd[0]); x++){
		int currentSize = getSize(root) + 1;
		root = rbAdd(root,toRBAdd[x]);
		infixPrint(root);
		if(!checkValidity(root)) break;
		printf("\n");
	}*/

	for(int x = 0; x < TREE_SIZE; x++){
		randVals[x] = rand() % (TREE_SIZE * 3);
		printf("%d\t%d\n", randVals[x],x);
		int currentSize = getSize(root) + 1;
		root = rbAdd(root, randVals[x]);
		infixPrint(root);
		printf("\n");
		if(!checkValidity(root)){
			for(int n = 0; n < x; n++){
				printf("%d\n", randVals[n]);
			}
			break;
		}
	}
	

	infixPrint(root);
	printf("%d\n", getSize(root));
	
	
	/*//int toRBRemove[] = {18,11,3,10,22};
	//int toRBRemove[] = {20};
	//int toRBRemove[] = {27,28,2,21,25};
	for(int x = 0; x < sizeof(toRBRemove) / sizeof(toRBRemove[0]); x++){
		root = rbRemove(root, toRBRemove[x]);
		infixPrint(root);
		printf("\n");
	}*/

	for(int x = 0; x < TREE_SIZE; x++){
		root = rbRemove(root, randVals[x]);
		infixPrint(root);
		printf("\n");
	}
	

	//infixPrint(root);

	
}

Node* createNode(int key, int color){

	Node* n = (Node*)malloc(sizeof(Node));
	n->key=  key;
	n->color = color;
	n->height = 0;
	n->count = 1;
	n->left = NULL;
	n->right = NULL;
	n->parent = NULL;

	return n;
}

Node* rbAdd(Node* root, int key){
	Node* toRBAdd = createNode(key, RED);

	if(root == NULL){
		toRBAdd->color = BLACK;
		toRBAdd->height = 1;
		return toRBAdd;
	} else {
		root = rbAdd_helper(root, toRBAdd);
		//infixPrint(root);
		if(checkValidity(root) == 1){
			return root;
		}
		root = fixViolation(root, toRBAdd);
		return root;
	}
}

Node* rbAdd_helper(Node* currentNode,Node* toRBAdd){
	if(currentNode == NULL){
		return toRBAdd;
	} else if(currentNode->key > toRBAdd->key){
		currentNode->left = rbAdd_helper(currentNode->left, toRBAdd);
		currentNode->left->parent = currentNode;
		currentNode->left->height = currentNode->height + 1;
	} else if(currentNode->key < toRBAdd->key){
		currentNode->right = rbAdd_helper(currentNode->right, toRBAdd);
		currentNode->right->parent = currentNode;
		currentNode->right->height = currentNode->height + 1;
	} else {
		currentNode->count += 1;
	}

	return currentNode;
}

Node* fixHeights(Node* root, int rootHeight){
	root->height = rootHeight;
	if(root->left != NULL){
		root->left = fixHeights(root->left,rootHeight + 1);
	}
	if(root->right != NULL){
		root->right = fixHeights(root->right,rootHeight + 1);
	}

	return root;
}

Node* fixViolation(Node* root, Node* rbAdded){
	Node* current = rbAdded;

	while(current->parent != NULL && current->parent->color != BLACK){

		Node* currentU = NULL;

		if(current->parent->parent->left != NULL && current->parent->parent->left->key == current->parent->key){
			currentU = current->parent->parent->right;
		} else {
			currentU = current->parent->parent->left;
		}

		int uncleCol = ((currentU == NULL) ? BLACK : currentU->color);

		if(uncleCol == RED){
			//Uncle is red
			//printf("Uncle is RED\n");
			current = current->parent->parent;
			current->color = RED;
			current->left->color = current->right->color = BLACK;
		} else {
			//Uncle is black
			//printf("Uncle is BLACK\n");
			int toSwap = 0;
			Node* toCheck = current;
			current = current->parent->parent;
			//infixPrint(root);

			//Left Cases
			if(current->left != NULL && current->left == toCheck->parent){
				if(current->left->right != NULL && current->left->right == toCheck){
					//Left right case
					//printf("LR Case\n");
					current->left = leftRot(current->left);
					current->left->parent = current;

				} else {
					//printf("LL Case\n");
					//Left left case
				}
				toSwap = 0;
				if(current->parent != NULL){
					Node* oldCurrent = current;
					current = current->parent;
					if(current->key > oldCurrent->key){
						current->left = rightRot(current->left);
						current->left->parent = current;
						current = current->left;
					} else {
						current->right = rightRot(current->right);
						current->right->parent = current;
						current = current->right;
					}
				}
				else{
					current = rightRot(current);
				}

			} else {//Right cases
				//Right left case
				if(current->right->left != NULL && current->right->left == toCheck){
					//Right left case
					//printf("RL Case\n");
					current->right = rightRot(current->right);
					current->right->parent = current;

				} else {
					//printf("RR Case\n");
					//Right right case
				}
				toSwap = 1;
				if(current->parent != NULL){
					Node* oldCurrent = current;
					current = current->parent;
					if(current->key > oldCurrent->key){
						current->left = leftRot(current->left);
						current->left->parent = current;
						current = current->left;
					} else {
						current->right = leftRot(current->right);
						current->right->parent = current;
						current = current->right;
					}
				}
				else{
					current = leftRot(current);
				}
			}
			//Swapping colors
			//infixPrint(current);
			Node* other = ((toSwap == 0) ? current->right : current->left);
			int tempCol = ((other==NULL) ? BLACK : other->color);
			if(other != NULL) other->color = current->color;
			current->color = tempCol;
			//infixPrint(root);

			break;
		}

	}
	
	//printf("Going up now\n");
	while(current->parent != NULL){
		//infixPrint(current);
		Node* nextCurrent = current->parent;
		nextCurrent = fixHeights(nextCurrent, nextCurrent->height);
		//infixPrint(current);
		//infixPrint(nextCurrent);
		//infixPrint(nextCurrent->left);
		//infixPrint(nextCurrent->right);
		//printf("%d\n", (nextCurrent->left != NULL && nextCurrent->left == current));

		if(nextCurrent->left != NULL && nextCurrent->left == current){
			nextCurrent->left = current;
		} else {
			nextCurrent->right = current;
		}

		//infixPrint(nextCurrent);
		//printf("||\n");

		//infixPrint(current);
		current = nextCurrent;
	}

	//infixPrint(current);

	//root = current;
	//infixPrint(root);
	current->color = BLACK;
	fixHeights(current,1);
	return current;
}

Node* leftRot(Node* z){
	int initHeight = z->height;
	int zLOrR = 0;
	Node* zParent = z->parent;

	if(zParent != NULL){
		if(zParent->right != NULL && zParent->right == z){
			zLOrR = 1;
		}
	}
	Node* y = z->right;
	Node* t2 = y->left;

	y->parent = z->parent;
	if(zParent != NULL){
		if(zLOrR) zParent->right = y;
		else zParent->left = y;
	}

	y->left = z;
	z->right = t2;

	z->parent = y;
	if(t2 != NULL) t2->parent = z;

	y = fixHeights(y,initHeight);

	return y;
}

Node* rightRot(Node* z){
	int initHeight = z->height;
	int zLOrR = 0;
	Node* zParent = z->parent;

	if(zParent != NULL){
		if(zParent->right != NULL && zParent->right == z){
			zLOrR = 1;
		}
	}

	Node* y = z->left;
	Node* t3 = y->right;

	y->parent = z->parent;
	if(zParent != NULL){
		if(zLOrR) zParent->right = y;
		else zParent->left = y;
	}

	z->left = t3;
	y->right = z;

	z->parent = y;
	if(t3 != NULL) t3->parent = z;

	y = fixHeights(y,initHeight);

	return y;

}

Node* rbRemove(Node* root, int key){
	printf("Removing %d from ", key);
	infixPrint(root);
	int lOrR = 0;
	Node* v = root;
	Node* vParent = NULL;
	Node* rootParent = root->parent;
	int rootHeight = root->height;

	while(v != NULL && v->key != key){
		if(key < v->key){
			lOrR = 0;
			v = v->left;
		} else if (v->key < key){
			lOrR = 1;
			v = v->right;
		}
	}

	//If you can't find the value
	if(v == NULL) {
		return root;
	}

	//If the node's count is greater than 1
	if(v->count > 1){
		v->count -= 1;
		return root;
	}

	//printf("v: ");
	//infixPrint(v);

	Node* u = NULL;
	//This is just for the iterating up
	Node* uParent = NULL;

	if(v->left != NULL && v->right != NULL){
		Node* currentNode = v->right;
		while(currentNode->left != NULL){
			currentNode = currentNode->left;
		}
		int inOrdSuc = currentNode->key;
		int inOrdCount = currentNode->count;
		int inOrdColor = currentNode->color;
		currentNode->count = 1;
		//printf("Recursively removing %d...\n",inOrdSuc);
		/*infixPrint(v);
		infixPrint(v->left);
		infixPrint(v->right);*/
		Node* replacementLeft = v->left;
		//printf("\n");
		Node* replacement = rbRemove(v->right, inOrdSuc);
		if(replacement != NULL)replacement->parent = v;
		//infixPrint(replacement);
		//infixPrint(v->right);
		v->right = replacement;
		v->left = replacementLeft;
		//infixPrint(replacementLeft);
		//infixPrint(v);
		v->key = inOrdSuc;
		v->count = inOrdCount;
		v->color = inOrdColor;
		//u = v;
		//uParent = u->parent;
		//v->right = rbRemove(v->right, inOrdSuc);
	} else {

		if(v->left != NULL){
			u = v->left;
		} else {
			u = v->right;
		}

		int vCol = v->color;
		int uCol = ((u == NULL) ? BLACK : u->color);

		//Replace v with u

		if(v->parent != NULL){
				if(!lOrR){
					v->parent->left = u;
				} else {
					v->parent->right = u;
				}
				uParent = v->parent;
		} else {
			if(u != NULL)u = fixHeights(u, rootHeight);
			if(u != NULL)u->color = BLACK;
			return u;
		}

		if(v == root) root = u;
		if(u != NULL) {
			u->parent = v->parent;
		}
		uParent = v->parent;


		//printf("u: ");
		//infixPrint(u);

		//If either u or v is red
		if(vCol + uCol > 0){
			//Replace v with u, and color u red
			//printf("U or V is red\n");
			if(u != NULL)u->color = BLACK;
		} else {

			//printf("U and V are BLACK, getting rid of the double black\n");

			Node* sibling = NULL;

			while(uParent != rootParent){

				int sibRight = 0;

				if(uParent->left == u) {
					sibling = uParent->right;
					sibRight = 1;
				}
				else sibling = uParent->left;

				//printf("Sibling: ");
				//infixPrint(sibling);

				int sibCol = ((sibling == NULL) ? BLACK : sibling->color);

				//Sibling is BLACK
				if(sibling == NULL){
					//printf("Sibling is NULL\n");
					u = uParent;
					uParent = u->parent;
				} else if(sibCol == BLACK){
					//printf("Sibling color is BLACK\n");

					Node* sLeft = sibling->left;
					Node* sRight = sibling->right;
					int sLeftCol = ((sibling->left == NULL) ? BLACK : sLeft->color);
					int sRightCol = ((sibling->right == NULL) ? BLACK : sRight->color);

					if(sLeftCol + sRightCol > 0){
						//Sibling has at least one red child
						printf("Sibling has red child\n");
						//infixPrint(uParent);
						u = uParent;
						infixPrint(u);
						if(u->left == sibling){
							//S is left child
							if(sRightCol == 1 && sLeftCol == 0){
								//printf("LR case\n");
								u->left = leftRot(u->left);
								u->left->parent = u;
								u->left->left->color = u->color;
							} else {
								//printf("LL case\n");
								u->left->left->color = u->left->color;
								u->left->color = u->color;
							}
							//infixPrint(u);
							u = rightRot(u);
						} else {
							if(sLeftCol == 1 && sRightCol == 0){
								//printf("RL case\n");
								u->right = rightRot(u->right);
								u->right->parent = u;
								u->right->right->color = u->color;
							} else {
								//printf("RR case\n");
								u->right->right->color = u->right->color;
								u->right->color = u->color;
							}
							//infixPrint(u);
							u = leftRot(u);
						}
						u->color = BLACK;
						uParent = u->parent;
						infixPrint(u);
						//if(u->height == rootHeight) root = u;

						break;
					} else {
						//Sibling has only black children
						printf("Sibling has only black children\n");
						if(u != NULL) uParent = u->parent;
						else break;
						/*if(sibRight){
							uParent->right->color = RED;
						} else {
							uParent->left->color = RED;
						}*/
						sibling->color = RED;

						int tempCol = ((uParent != NULL) ? uParent->color : BLACK);
						if(uParent != NULL) uParent->color = ((u != NULL) ? u->color : BLACK);
						if(u != NULL) u->color = tempCol;
						//u = uParent;
						if(uParent != NULL && uParent->color == RED){

							//uParent->color = BLACK;
							break;
						} else {
							if(u != NULL) u = u->parent;
							else u = uParent;
							uParent = u->parent;
						}
					}

				} else {
					printf("Sibling color is RED\n");
					infixPrint(uParent);
					infixPrint(sibling);
					infixPrint(u);
					if(uParent->left == sibling){
						printf("L Case\n");
						uParent = rightRot(uParent);
						//sibling = uParent->right;
						//u = uParent->right->right;
						//uParent = uParent->right;
					} else {
						printf("R Case\n");
						uParent = leftRot(uParent);
						//sibling = uParent->left;
						//infixPrint(uParent);
						//u = uParent->left->left;
						//uParent = uParent->left;
					}
					uParent->color = RED;
					sibling->color = BLACK;
					infixPrint(uParent->parent);
					//infixPrint(u);
					//infixPrint(sibling);
				}
				//printf("u: ");
				//infixPrint(u);
			}
			//infixPrint(u);

		}
	}

	/*while(uParent != rootParent){
		infixPrint(u);
		infixPrint(uParent);
		if(u->key < uParent->key){
			uParent->left = u;
			uParent->left->parent = uParent;
		} else {
			uParent->right = u;
			uParent->right->parent = uParent;
		}

		printf("test\n");

		u = uParent;
		if(u == NULL) break;
		uParent = u->parent;

	}*/
	if(root != NULL && root->parent != rootParent){
		printf("Resetting root...\n");
		while(root->parent != rootParent){
			root = root->parent;
			infixPrint(root);
		}
	}
	if(root != NULL)fixHeights(root, rootHeight);
	if(root != NULL && root->parent == NULL) root->color = BLACK;

	//printf("Returning: ");
	//infixPrint(root);
	return root;
}

int getHeight(Node* root){
	if(root->left == NULL && root->right == NULL){
		return root->height;
	} else if(root->left != NULL && root->right == NULL){
		return getHeight(root->left);
	} else if(root->left == NULL && root->right != NULL){
		return getHeight(root->right);
	} else {
		int leftHeight = getHeight(root->left);
		int rightHeight = getHeight(root->right);
		return ((leftHeight > rightHeight) ? leftHeight : rightHeight);
	}
}

int getSize(Node* root){
	if(root == NULL){
		return 0;
	}
	else{
		return 1 + getSize(root->left) + getSize(root->right);
	}
}

int checkBlack(Node* root){
	return checkBlackHelper(root, 0) >= 0;
}

int checkBlackHelper(Node* root, int currentBlackNum){
	if(root->color == BLACK){
		currentBlackNum += 1;
	}

	if(root->left == NULL || root->right == NULL){
		return currentBlackNum;
	} else {
		return ((checkBlackHelper(root->left, currentBlackNum) == checkBlackHelper(root->right, currentBlackNum)) ? checkBlackHelper(root->left, currentBlackNum) : -1);
	}
}

int checkRedViolated(Node* root){
	if(root == NULL){
		return 1;
	} else {
		int leftVal = 0;
		int rightVal = 0;
		if(root->color == RED){
			if(root->left != NULL && root->left->color == RED){
				leftVal = 1;
			}
			if(root->right != NULL && root->right->color == RED){
				rightVal = 1;
			}
		}
		return (leftVal == rightVal) && checkRedViolated(root->left) && checkRedViolated(root->right);
	}
}

int checkOrderViolated(Node* root){
	if(root == NULL){
		return 1;
	} else {
		int leftVal = root->key - 1;
		int rightVal = root->key + 1;
		if(root->left != NULL){
			leftVal = root->left->key;
		}
		if(root->right != NULL){
			rightVal = root->right->key;
		}
		return checkOrderViolated(root->left) && checkOrderViolated(root->right) && (leftVal < root->key <= rightVal);
	}
}

int checkValidity(Node* root){
	return (root->color == BLACK) && checkBlack(root) && checkRedViolated(root) && checkOrderViolated(root);
}

void infixPrint(Node* root){
	if(root == NULL){
		printf("NULL\n");
	} else {
		infixPrint_helper(root);
		printf(" | Root: %d\t Root Height: %d\t Valid Tree: %d\n",root->key,root->height,checkValidity(root));
	}

}

void infixPrint_helper(Node* root){
	if(root == NULL) return;

	infixPrint_helper(root->left);
	//printf("| %d | Height=%d | ",root->key,root->height);
	printf("%d ",root->key);
	if(root->count > 1){
		printf("(%d) ", root->count);
	}
	if(root->color == BLACK){
		printf("BLACK ");
	} else {
		printf("RED ");
	}

	printf("%d, ",root->height);
	infixPrint_helper(root->right);
}