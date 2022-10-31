//
//            Header declarations
//            -------------------

typedef struct heada
{
	void *pObject; // Pointer to the linked list of objects. 
				   // Objects may be of types Object1, ....Object10.
				   // Declarations of objects are in file Objects.h
	char cBegin;   // The linked list contains objects in which
				   // the first letter of object ID (in case of
	               // Struct1, Struct2 and Struct6) or the first letter
	               // of the second word of ID (in case of Struct3,
				   // STruct4 and Struct5) is cBegin.
	struct heada *pNext;
} HeaderA;


typedef struct headb
{
	HeaderA *pHeaderA; // Pointer to the linked list of headers type A.
	char cBegin;       // The first letter of ID of objects related to
	                   // header is cBegin.
	struct headb *pNext;
} HeaderB;


typedef struct headc
{
	void **ppObjects; // Pointer to the array of pointers to objects. 
				      // Objects may be of types Object1, ....Object10.
					  // Declarations of objects are in file Objects.h
	char cBegin;      // The first letter of ID of objects related to
	                  // header is cBegin.
	struct headc *pNext;
} HeaderC;

typedef struct headd
{
	void *pObject;  // Pointer to the linked list of objects. 
					// Objects may be of types Object1, ....Object10.
					// Declarations of objects are in file Objects.h
	char cBegin;    // The linked list contains objects in which
					// the first letter of object ID is cBegin.
	struct headd *pNext;
	struct headd *pPrior;
} HeaderD;

typedef struct node
{
	void *pObject;		// Pointer to object belonging to this node
						// Objects may be of types Object1, ....Object10.
						// Declarations of objects are in file Objects.h
	struct node *pLeft, // Pointer to the left child node
		*pRight;		// Pointer to the right child node
} Node;

typedef struct stack
{
	void *pObject;
	struct stack *pNext;
} Stack;