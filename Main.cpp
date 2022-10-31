#include "stdio.h"
#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#pragma warning ( disable : 4996 )

void PrintObjects(HeaderD* pStruct7)		// function to print all data
{
	

	HeaderD* pStructNew = pStruct7;			// copying input data to local variable
	Object3* obj;							// creating struct pointer where data is stored
	int i = 1, j;							// data counters
			
	while (pStructNew) 
	{
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

HeaderD* beginIndex(HeaderD* pStruct7, char start) {
	int loop = 0;
	while (start > pStruct7->cBegin) {						// loop the structure as long as char start is smaller than the current cBegin
		if (!pStruct7->pNext)
			break;
		pStruct7 = pStruct7->pNext;							// and there is a next element, the loop ends when start and cBegin match
		loop++;
	}
	if (start == pStruct7->cBegin)
		return pStruct7;

	HeaderD* pStructNew;

	pStructNew = (HeaderD*)malloc(sizeof(HeaderD));		// give memory to new element
	if (pStructNew == NULL)								// memory allocation error
		return 0;

	printf("%c: created memory for new element\n", start);

	if (start < pStruct7->cBegin && loop == 0) {						// edge case, new cBegin is sooner in the alphabet than the first element in struct
		printf("new element is first in list\n");
		pStructNew->pNext = pStruct7;
		pStructNew->pPrior = NULL;
		pStruct7->pPrior = pStructNew;
	}
	else if (start > pStruct7->cBegin) {			// edge case 2, new cBegin is last in array
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

// function to find if the given pNewID already exists in the array, returns NULL if it is not present
Object3* findObject(Object3* obj, char* pNewID, int* exists) {
	while (obj != NULL && strcmp(pNewID, obj->pID) >= 0) {	// since the list is sorted there is no need to check objects that come later in the alphabet
		//printf("%s and %s\n", pNewID, obj->pID);
		if (!strcmp(pNewID, obj->pID)) {
			*exists = 1;
			//printf("Found in array\n");
			return obj;						// pNewID is already in the array
		}
			
		if (obj->pNext == NULL || strcmp(pNewID, obj->pNext->pID) < 0)
			break;
		obj = obj->pNext;
	}
	if (obj == NULL || strcmp(pNewID, obj->pID) < 0)
		*exists = -1;
	else
		*exists = 0;
	//printf("\nEnd of function object3 %s\n", obj->pID);
	return obj;							// pNewID is not present
}

unsigned long int randomNumber() {
	return rand() * 30;	// RAND_MAX is 32767, so the result is multiplied by 30 to get a 9 digit number as a limit
}

int InsertNewObject(HeaderD** pStruct7, char* pNewID, unsigned long int NewCode) {
	HeaderD* pS7 = *pStruct7;
	
	if (!checkFormat(pNewID))	// check the format of pNewID
		return 0;
	pS7 = beginIndex(pS7, pNewID[0]);	// find cBegin, if none make new
	
	Object3* obj_test;
	obj_test = (Object3*)pS7->pObject;
	
	// check if pNewID exists
	Object3* newObj, *foundObj, *nextObj;
	int exists = -1;
	int* pExists = &exists;
	foundObj = findObject((Object3*)pS7->pObject, pNewID, pExists);
	
	if (exists == 1) {
		printf("ID %s already present\n", pNewID);
		return 0;
	}
	
	newObj = (Object3*)malloc(sizeof(Object3));
	if (newObj == NULL)
		return 0;
	
	newObj->pID = (char*)malloc(sizeof(pNewID));
	strcpy(newObj->pID, pNewID);

	newObj->Code = NewCode;

	time_t RawTime = time(&RawTime);
	if (!GetTime1(RawTime, &newObj->sTime1))
		return 0;
	

	if (exists < 0) {	// added element needs to be first
		if ((Object3*)pS7->pObject != NULL)
			newObj->pNext = (Object3*)pS7->pObject;
		else
			newObj->pNext = NULL;
		pS7->pObject = (void*)newObj;
	}
	else {
		if (foundObj->pNext == NULL)
			newObj->pNext = NULL;
		else
			newObj->pNext = foundObj->pNext;
		foundObj->pNext = newObj;
	}
	// get time
	
	printf("%-15s %-10lu %02d:%02d:%02d\n", newObj->pID, newObj->Code, newObj->sTime1.Hour, newObj->sTime1.Minute, newObj->sTime1.Second);
	
	while (pS7->pPrior)			// rewind list to first element
		pS7 = pS7->pPrior;
	*pStruct7 = pS7;
	return 1;
}

Object3* RemoveExistingObject(HeaderD** pStruct7, char* pExistingID) {
	if (!checkFormat(pExistingID))	// check if input char can even exist
		return 0;
	
	HeaderD* pS7 = *pStruct7;
	
	while (pExistingID[0] > pS7->cBegin)
		pS7 = pS7->pNext;

	if (pExistingID[0] != pS7->cBegin)
		return 0;

	Object3* obj;
	int exists = -1;
	obj = findObject((Object3*)pS7->pObject, pExistingID, &exists);

	if (exists != 1)
		return 0;
	HeaderD* buffer = pS7;

	if ((Object3*)pS7->pObject == obj) {	// found object is first in the list
		if (obj->pNext == NULL) {			// edge case, found object is the only element in list
			pS7->pObject = NULL;

			if (pS7->pPrior != NULL && pS7->pNext != NULL) {	// header is not last and not first
				pS7->pPrior->pNext = pS7->pNext;	// jump the header were trying to delete
				pS7->pNext->pPrior = pS7->pPrior;
				while(pS7->pPrior != NULL)
					pS7 = pS7->pPrior;		// rewind to the first header
			}
			else if (pS7->pPrior != NULL) {							// header is last
				pS7->pPrior->pNext = NULL;
				while (pS7->pPrior != NULL)
					pS7 = pS7->pPrior;		// rewind to the first header
			}
			else if (pS7->pNext != NULL) {							// header is first
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
		if (obj->pNext == NULL)
			prevObj->pNext = NULL;			
		else
			prevObj->pNext = obj->pNext;
	}
	printf("removed: %-15s %-10lu %02d:%02d:%02d\n", obj->pID, obj->Code, obj->sTime1.Hour, obj->sTime1.Minute, obj->sTime1.Second);
	return obj;
}




int main()
{
	srand(time(NULL));   // Initialization, use to generate Code
	// Kirjutage lähtestruktuuri genereeriv lause. See on:
	//printf("max is %ld\n", RAND_MAX);
	// g) Struct7 puhul:
	HeaderD *pStruct = GetStruct7(3, 35);
	if (!pStruct)							// exit if faulty data was given
		return 0;
	int inloop = 1;
	char input;

	printf(	"1. Print data\n"
			"2. Insert new object\n"
			"3. Delete object\n"
			"x  Exit program\n"
			);

	char test[14][3] = {	{ "Dx" }, { "Db" }, { "Dz" }, { "Dk" }, { "Aa" }, { "Wu" }, { "Wa" },
							{ "Zw" }, { "Za" }, { "wk" }, { "Wa" }, { "WW" }, { "W8" }, { "W_" } };
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
			case 'x':
				inloop = 0;
				break;
		}
	}
	
	
	
	
	return 0;
}
