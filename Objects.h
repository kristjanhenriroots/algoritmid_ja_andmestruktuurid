//
//            Object declarations
//            -------------------

typedef struct ob1
{ // formatting string for printf is "%d) %s %lu %s\n", the result is for example "1) Abcde 100 15:52:30"
  // or "10) Abcde Fghij 100 15:52:30".
 	char *pID;
	unsigned long int Code;
	char *pTime; // formatted as GetTimeString output (see file DateTime.h)
	struct ob1 *pNext;
} Object1;

typedef struct ob2
{  // formatting string for printf is "%d) %s %lu %02d:%02d:%02d\n", the result is for example "1) Abcde 100 15:52:07"
   // or "10) Abcde Fghij 100 15:52:07"	
	char *pID;
	unsigned long int Code;
	Time1 *pTime1; // Declaration of Time1 is in file DateTime.h
	struct ob2 *pNext;
} Object2;

typedef struct ob3
{  // formatting string for printf is "%d) %s %lu %02d:%02d:%02d\n", the result is for example "1) Abcde 100 15:52:07"
   // or "10) Abcde Fghij 100 15:52:07"
	char *pID;
	unsigned long int Code;
	Time1 sTime1;  // Declaration of Time1 is in file DateTime.h
	struct ob3 *pNext;
} Object3;

typedef struct ob4
{  // formatting string for printf is "%d) %s %lu %s\n", the result is for example "1) Abcde 100 10-15-2010"
   // or "10) Abcde Fghij 100 10-15-2010"
	char *pID;
	unsigned long int Code;
	char *pDate; // formatted as GetDateString output (see file DateTime.h)
	struct ob4 *pNext;
} Object4;

typedef struct ob5
{  // formatting string for printf is "%d) %s %lu %02d-%02d-%04d\n", the result is for example "1) Abcde 100 01-12-2010"
   // or "10) Abcde Fghij 100 01-12-2010"
	char *pID;
	unsigned long int Code;
	Date1 *pDate1; // Declaration of Date1 is in file DateTime.h
	struct ob5 *pNext;
} Object5;

typedef struct ob6
{  // formatting string for printf is "%d) %s %lu %02d-%02d-%04d\n", the result is for example "1) Abcde 100 01-12-2010"
   // or "10) Abcde Fghij 100 01-12-2010"
	char *pID;
	unsigned long int Code;
	Date1 sDate1; // Declaration of Date1 is in file DateTime.h
	struct ob6 *pNext;
} Object6;

typedef struct ob7
{  // formatting string for printf is "%d) %s %lu %02d %s %04d\n", the result is for example "1) Abcde 100 01 Dec 2010"
   // or "10) Abcde Fghij 100 01 Dec 2010"
	char *pID;
	unsigned long int Code;
	Date2 *pDate2; // Declaration of Date2 is in file DateTime.h
	struct ob7 *pNext;
} Object7;

typedef struct ob8
{  // formatting string for printf is "%d) %s %lu %02d %s %04d\n", the result is for example "1) Abcde 100 01 Dec 2010"
   // or "10) Abcde Fghij 100 01 Dec 2010"
	char *pID;
	unsigned long int Code;
	Date2 sDate2; // Declaration of Date2 is in file DateTime.h
	struct ob8 *pNext;
} Object8;

typedef struct ob9
{  // formatting string for printf is "%d) %s %lu %02d %s %04d\n", the result is for example "1) Abcde 100 01 Detsember 2010"
   // or "10) Abcde Fghij 100 01 Detsember 2010"
	char *pID;
	unsigned long int Code;
	Date3 *pDate3; // Declaration of Date3 is in file DateTime.h
	struct ob9 *pNext;
} Object9;

typedef struct ob10
{  // formatting string for printf is "%d) %s %lu %02d %s %04d\n", the result is for example "1) Abcde 100 01 Detsember 2010"
   // or "10) Abcde Fghij 100 01 Detsember 2010"
	char *pID;
	unsigned long int Code;
	Date3 sDate3; // Declaration of Date3 is in file DateTime.h
	struct ob10 *pNext;
} Object10;
