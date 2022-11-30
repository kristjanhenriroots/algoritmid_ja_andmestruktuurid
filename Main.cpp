#include "stdio.h"
#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#pragma warning ( disable : 4996 )

void PrintObjects(HeaderD* pStruct7) {		// function to print all data
	
	HeaderD* pStructNew = pStruct7;			// copying input data to local variable
	Object3* obj;							// creating struct pointer where data is stored
	int i = 1, j;							// data counters
			
	while (pStructNew) {
		obj = (Object3*)pStructNew->pObject;			// typecasting void pointer to correct struct
		printf("%2d. %c\n", i, pStructNew->cBegin);
		j = 1;
		while (obj) 
		{	//		"%d) %s %lu %02d:%02d:%02d\n" <- original printf
			printf("\t%d) %-15s %-10lu %02d:%02d:%02d\n", j, obj->pID, obj->Code, obj->sTime1.Hour, obj->sTime1.Minute, obj->sTime1.Second);
			obj = obj->pNext;
			j++;
		}
		pStructNew = pStructNew->pNext;
		i++;
	}
}

// subfunction to check format of input pNewID
int checkFormat(char* name) {
	if (name[0] < 'A' || name[0] > 'Z')	// is the first letter uppercase
		return 0;
	for (int i = 1; name[i] != '\0'; i++) {
		if (name[i] < 'a' || name[i] > 'z')	// return if there is anything except a lowercase letter
			return 0;
	}
	return 1;
}

// function to find the correct headerD element
HeaderD* beginIndex(HeaderD* pStruct7, char start) {
	int loop = 0;
	while (start > pStruct7->cBegin) {						// loop the structure as long as char start is smaller than the current cBegin
		if (!pStruct7->pNext)
			break;
		pStruct7 = pStruct7->pNext;							// and there is a next element, the loop ends when start and cBegin match
		loop++;
	}
	if (start == pStruct7->cBegin)							// if the correct header is found it will be returned
		return pStruct7;

	HeaderD* pStructNew;

	pStructNew = (HeaderD*)malloc(sizeof(HeaderD));		// give memory to new element
	if (pStructNew == NULL)								// memory allocation error
		return NULL;

	//printf("%c: created memory for new element\n", start);

	if (start < pStruct7->cBegin && loop == 0) {		// edge case, new cBegin is sooner in the alphabet than the first element in struct
		//printf("new element is first in list\n");
		pStructNew->pNext = pStruct7;
		pStructNew->pPrior = NULL;
		pStruct7->pPrior = pStructNew;
	}
	else if (start > pStruct7->cBegin) {				// edge case 2, new cBegin is last in array
		pStructNew->pPrior = pStruct7;
		pStruct7->pNext = pStructNew;
		pStructNew->pNext = NULL;
	}

	else {												// given char start comes after cBegin

		pStructNew->pNext = pStruct7;					// create links
		
									
		HeaderD* buffer = pStruct7->pPrior;
		pStructNew->pPrior = buffer;
		buffer->pNext = pStructNew;
		

		pStruct7->pPrior = pStructNew;					// new element linked with neighbours

	}
	pStructNew->pObject = NULL;
	pStructNew->cBegin = start;

	return pStructNew;
}

enum object_states{none, present, first = -1};	// enum to describe the exists variable states in findObject()

// function to find if the given pNewID already exists in the array, returns NULL if it is not present
Object3* findObject(Object3* obj, char* pNewID, int* exists) {
	while (obj && strcmp(pNewID, obj->pID) >= 0) {	// since the list is sorted there is no need to check objects that come later in the alphabet
		//printf("%s and %s\n", pNewID, obj->pID);
		if (!strcmp(pNewID, obj->pID)) {
			*exists = present;
			//printf("Found in array\n");
			return obj;						// pNewID is already in the array
		}
			
		if (!obj->pNext || strcmp(pNewID, obj->pNext->pID) < 0)
			break;
		obj = obj->pNext;
	}
	if (!obj || strcmp(pNewID, obj->pID) < 0)	// object doesn't exist in list and would need to be first
		*exists = first;
	else
		*exists = none;
	//printf("\nEnd of function object3 %s\n", obj->pID);
	return obj;							// pNewID is not present
}

int InsertNewObject(HeaderD** pStruct7, char* pNewID, unsigned long int NewCode) {
	
	HeaderD* pS7 = *pStruct7;
	
	if (!checkFormat(pNewID))	// check the format of pNewID
		return 0;
	pS7 = beginIndex(pS7, pNewID[0]);	// find cBegin, if none make new
	if (pS7 == NULL)
		return 0;


	Object3* obj_test;
	obj_test = (Object3*)pS7->pObject;
	
	// check if pNewID exists
	Object3* newObj, *foundObj;
	int exists = first;
	int* pExists = &exists;
	foundObj = findObject((Object3*)pS7->pObject, pNewID, pExists);
	
	if (exists == present) {
		printf("ID %s already present, ", pNewID);
		return 0;
	}
	
	newObj = (Object3*)malloc(sizeof(Object3));
	if (newObj == NULL)
		return 0;
	
	newObj->pID = (char*)malloc(sizeof(pNewID));
	if (newObj->pID == NULL)
		return 0;
	
	strcpy(newObj->pID, pNewID);		// add pID
	newObj->Code = NewCode;				// add Code

	time_t RawTime = time(&RawTime);	// add Time
	if (!GetTime1(RawTime, &newObj->sTime1))
		return 0;
	

	if (exists == first) {	// added element needs to be first
		if ((Object3*)pS7->pObject)		// checking if added object is the only one in the list
			newObj->pNext = (Object3*)pS7->pObject;
		else
			newObj->pNext = NULL;
		pS7->pObject = (void*)newObj;	// the pointer to the first object in the header must be adjusted
	}
	else {	// added element is somewhere in the middle or last
		if (!foundObj->pNext)
			newObj->pNext = NULL;
		else
			newObj->pNext = foundObj->pNext;
		foundObj->pNext = newObj;
	}
	
	printf("%-15s %-10lu %02d:%02d:%02d\n", newObj->pID, newObj->Code, newObj->sTime1.Hour, newObj->sTime1.Minute, newObj->sTime1.Second);
	
	while (pS7->pPrior)			// rewind list to first element
		pS7 = pS7->pPrior;
	*pStruct7 = pS7;
	return 1;
}

// function for deleting existing objects
Object3* RemoveExistingObject(HeaderD** pStruct7, char* pExistingID) {
	if (!checkFormat(pExistingID))	// check if input char can even exist
		return 0;

	HeaderD* pS7 = *pStruct7;

	while (pS7 && pExistingID[0] > pS7->cBegin)
		pS7 = pS7->pNext;

	if (!pS7 || pExistingID[0] != pS7->cBegin)
		return 0;

	Object3* obj;
	int exists = first;
	obj = findObject((Object3*)pS7->pObject, pExistingID, &exists);

	if (exists != present)	// if the object is not in the list there is nothing to delete
		return 0;
	HeaderD* buffer = pS7;

	if ((Object3*)pS7->pObject == obj) {	// found object is first in the list
		if (!obj->pNext) {					// edge case, found object is the only element in list
			pS7->pObject = NULL;

			if (pS7->pPrior && pS7->pNext) {				// header is not last and not first
				pS7->pPrior->pNext = pS7->pNext;			// jump the header were trying to delete
				pS7->pNext->pPrior = pS7->pPrior;
				while(pS7->pPrior != NULL)
					pS7 = pS7->pPrior;		// rewind to the first header
			}
			else if (pS7->pPrior) {							// header is last
				pS7->pPrior->pNext = NULL;
				while (pS7->pPrior)
					pS7 = pS7->pPrior;		// rewind to the first header
			}
			else if (pS7->pNext) {							// header is first
				pS7->pNext->pPrior = NULL;
				pS7 = pS7->pNext;			// the first header is now ->pNext			
			}
			
			// else there only is one header

			buffer->pNext = NULL;			// clear the original links
			buffer->pPrior = NULL;
			*pStruct7 = pS7;				// first header has changed
		}
		else {	// object is first in the list but not the only element
			pS7->pObject = (void*)obj->pNext;	// the first element has now changed, header will point to that
			obj->pNext = NULL;
		}
	}
	else  {								// object is last in list or in the middle
		Object3* prevObj = (Object3*)pS7->pObject;
		while (prevObj->pNext != obj)	// need to find the previous object to the one we want to remove
			prevObj = prevObj->pNext;
		if (!obj->pNext)
			prevObj->pNext = NULL;			
		else
			prevObj->pNext = obj->pNext;
	}
	printf("removed: %-15s %-10lu %02d:%02d:%02d\n", obj->pID, obj->Code, obj->sTime1.Hour, obj->sTime1.Minute, obj->sTime1.Second);
	return obj;
}

unsigned long int randomNumber() {
	return rand() * 30;	// RAND_MAX is 32767, so the result is multiplied by 30 to get a 9 digit number as a limit
}

Node* CreateBinaryTree(HeaderD* pStruct7) {
	unsigned long int newCode;
	HeaderD* pS7 = pStruct7;
	Object3* obj = (Object3*)pS7->pObject;
	Node* pTree = (Node*)malloc(sizeof(Node)), *p;	// creating root node

	int counter = 1;								// just to number results
	
	if (pTree == NULL)
		return 0;

	pTree->pObject = (void*)pS7->pObject;			// giving values to root node
	pTree->pLeft = pTree->pRight = NULL;
	obj = (Object3*)pTree->pObject;
	pS7 = pS7->pNext;
	int found = 0;
	
	printf("Root is %d\n", obj->Code);
	
	
	while (pS7) {									// go through whole headd
		Node* pNew = (Node*)malloc(sizeof(Node));	// create new node
		if (pNew == NULL)
			return 0;
		p = pTree;

		if (pS7->cBegin == obj->pID[0] && strcmp(((Object3*)pS7->pObject)->pID, obj->pID) != 0)	// new object was taken from same headd, next struct not needed
			pNew->pObject = (void*)obj; 
		else
			pNew->pObject = (void*)pS7->pObject;												// no more objects in head, new head was taken
		
		obj = (Object3*)pNew->pObject;
		pNew->pLeft = pNew->pRight = NULL;														// no leaves
		newCode = obj->Code;

		while (!found) {							// go through tree until a suitable place is found
			if (newCode > ((Object3*)p->pObject)->Code) {		// current node key is bigger, new one goes to the left
				//printf("Moving right\n");
				if (!p->pRight) {
					p->pRight = pNew;
					//printf("Added right of %d\n", ((Object3*)p->pObject)->Code);
					found++;
				}
				else
					p = p->pRight;

			}
			else if (newCode < ((Object3*)p->pObject)->Code) {	// current is smaller, new one goes to right
				//printf("Moving left\n");
				if (!p->pLeft) {
					p->pLeft = pNew;
					//printf("Added left of %d\n", ((Object3*)p->pObject)->Code);
					found++;
				}
				else
					p = p->pLeft;
			}
			else // faulty data, duplicate code
				return 0; 
			//printf("new loop\n");

		}
		counter++;
		found = 0;

		if (obj->pNext)			// if there are more objects in head move to next, otherwise take next from headd
			obj = obj->pNext;
		else
			pS7 = pS7->pNext;


	}
	printf("added %d entries\n", counter);	// print number of entries added to tree
	return pTree;
}

Stack* Push(Stack* pStack, void* p) {	// function for pushing to stack
	if (!p)
		return pStack;
	Stack* pNew;
	pNew = (Stack*)malloc(sizeof(Stack));
	if (pNew == NULL)
		return pStack;
	pNew->pObject = p;
	pNew->pNext = pStack;
	return pNew;
}

Stack* Pop(Stack* pStack, void** pResult) {	// function from popping from stack
	Stack* p;
	if (!pStack)
	{
		*pResult = 0;
		return pStack;
	}
	*pResult = pStack->pObject;
	p = pStack->pNext;
	free(pStack);
	return p;
}

void TreeTraversal(Node* pTree) {	// function 5, prints out organized tree
	int cnt = 1;
	Stack* pStack = 0;
	Node* p1 = pTree, * p2;
	if (!pTree)
		return;
	do
	{
		while (p1)
		{
			pStack = Push(pStack, p1);
			p1 = p1->pLeft;
		}
		pStack = Pop(pStack, (void**)&p2);
		printf("%2d. %-15s %-10lu %02d:%02d:%02d\n", cnt, ((Object3*)p2->pObject)->pID, ((Object3*)p2->pObject)->Code, 
													((Object3*)p2->pObject)->sTime1.Hour, ((Object3*)p2->pObject)->sTime1.Minute, ((Object3*)p2->pObject)->sTime1.Second);
		cnt++;
		p1 = p2->pRight;
	} while (!(!pStack && !p1));
	return;
}

Node* DeleteTreeNode(Node* pTree, unsigned long int Code) {	// function that deletes nodes from tree

	if (!pTree)
		return 0;
	Node* p = pTree, *remove = pTree;
	Stack* pStack = 0;

	while (p && ((Object3*)p->pObject)->Code != Code) { // find if the given code exists in the tree
		pStack = Push(pStack, p);						// stack to keep track of nodes travelled
		if (Code > ((Object3*)p->pObject)->Code)
			p = p->pRight;								// given code is larger than current, go right
		else if (Code < ((Object3*)p->pObject)->Code)
			p = p->pLeft;
	}
	
	// when the loop ends the correct is either found or it doesn't exist
	if (!p || ((Object3*)p->pObject)->Code != Code) {
		printf("%d does not exist\n", Code);
		while (pStack)
			pStack = Pop(pStack, (void**)&p);
		return pTree;
	}

	remove = p;	// node that will be freed

	if (((Object3*)pTree->pObject)->Code == Code) {	// the removed object is the root
		printf("root detected\n");
		remove = pTree;
		if (p->pRight) {					// bigger nodes exist, the smallest of the group will inherit the left branch
			p = p->pRight;
			while (p->pLeft)				// getting the smallest element of right branch
				p = p->pLeft;

			p->pLeft = pTree->pLeft;		// moving the branch
			pTree = pTree->pRight;
		}
		else
			pTree = pTree->pLeft;				// no right branch from root
		//free(pTree);
	}
	else if (!p->pLeft && !p->pRight) {		// no leaves
		printf("No leaves detected\n");
		if (((Node*)pStack->pObject)->pLeft == p)
			((Node*)pStack->pObject)->pLeft = NULL;
		else
			((Node*)pStack->pObject)->pRight = NULL;
	}
	else if (!p->pLeft) {					// leaf on right
		printf("leaf on right\n");
		if (((Object3*)p->pObject)->Code < ((Object3*)pStack->pObject)->Code) { // node being removed is left
			((Node*)pStack->pObject)->pLeft = p->pRight;
		}
		else {
			((Node*)pStack->pObject)->pRight = p->pRight;
		}
	}
	else if (!p->pRight) {					// leaf is left
		printf("leaf on left\n");
		if (((Object3*)p->pObject)->Code < ((Object3*)pStack->pObject)->Code) {
			((Node*)pStack->pObject)->pLeft = p->pLeft;
		}
		else {
			((Node*)pStack->pObject)->pRight = p->pLeft;
		}
	}
	else {									// both leaves present
		Node* t;
		printf("both leaves present\n");
		t = (Node*)pStack->pObject;			// temporary node to extract code

		if (((Object3*)p->pObject)->Code < ((Object3*)t->pObject)->Code)
				t->pLeft = p->pRight; 
		else
			t->pRight = p->pRight;
		t = p->pLeft;
		p = p->pRight;
		while (p->pLeft)				// getting the smallest element of right branch
			p = p->pLeft;
		p->pLeft = t;
		p = p->pLeft;
	}
	printf("Removed: %d\n", Code);
	while (pStack)
		pStack = Pop(pStack, (void**)&p);
	return pTree;
}


int main()
{
	srand((unsigned int)time(NULL));   // Initialization, use to generate Code
	// Kirjutage lähtestruktuuri genereeriv lause. See on:
	// g) Struct7 puhul:
	HeaderD *pStruct = GetStruct7(3, 35);
	Node* pTree = NULL;
	if (!pStruct)							// exit if faulty data was given
		return 0;
	int inloop = 1;
	char input;

	printf(	"1. Print data\n"
			"2. Insert new object\n"
			"3. Delete object\n"
			"4.	Create binary tree\n"
			"5. Print binary tree\n"
			"6. Delete tree node\n"
			"x  Exit program\n"
			);

	char test[14][3] = {	{ "Dx" }, { "Db" }, { "Dz" }, { "Dk" }, { "Aa" }, { "Wu" }, { "Wa" },
							{ "Zw" }, { "Za" }, { "wk" }, { "Wa" }, { "WW" }, { "W8" }, { "W_" } };
	long unsigned int Codes[5] = { 101110100, 316985719, 422218, 4455511, 12345678 };
	while (inloop) {
		scanf("%c", &input);
		switch (input) {
			case '1':
				PrintObjects(pStruct);
				break;
			case '2':
				for (int i = 0; i < 14; i++)
					if (!InsertNewObject(&pStruct, test[i], randomNumber()))
						printf("""%s"" not added!\n", test[i]);
				break;
			case '3':
				for (int i = 0; i < 14; i++)
					if(!RemoveExistingObject(&pStruct, test[i]))
						printf("""%s"" not removed!\n", test[i]);
				break;
			case '4':
				pTree = CreateBinaryTree(pStruct);
				break;
			case '5':
				if(pTree)
					TreeTraversal(pTree);
				break;
			case '6':
				if (pTree) {
					for(int i = 0; i < 5; i++)
						pTree = DeleteTreeNode(pTree, Codes[i]);
				}
					
				break;
			case 'x':
				inloop = 0;
				break;
		}
	}
	
	return 0;
}
