
#ifndef __Table_h__
#define __Table_h__

/*
I assume theres a way to have these only be declared once, but this works
*/
typedef struct _order{ /*Declared here and in table.c*/
	char *Buyer;
	char *Food;
	int iPrice;
	struct _order* pNext;
}order;

typedef struct _table{ /*Declared here and in table.c*/
	int iTableNr;
	int iSeats;
	int iTime;
	struct _table* pNext;
	struct _table* pPrev;
	order *Order;	
	char *Customer;
}table;

/*These are all the function oppgave_3.c need to have access too*/

int CountTables(table*);
int CountOrders(table*);  
int checkForSameTimeAndTableNr(table** , int , int);
void DeleteTable(table** ppHead, table* ToBeDeleted);
void addTable(table** ppHead, char* Customer, int iTableNr, int iSeats, int iTime);
void printTableNoSeats(table* Table);
void printTable(table* Table);
void printOrderWithPrice(order* Order);
int printTableOrders(table* Table);
void printAllTable(table* pHead);
void AddOrder(table* currentTable, char *Buyer, char *Food, int iPrice);
table* GetTableByIndex(table*, int );
table* GetTableByName(table*, char *);
int freeList(table*);

#endif /*__Table_h__*/
