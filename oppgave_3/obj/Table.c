#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
I assume theres a way to have these only be declared once, but this works
*/

typedef struct _order{ /*Declared here and in table.h*/
	char *Buyer;
	char *Food;
	int iPrice;
	struct _order* pNext;
}order;

typedef struct _table{ /*Declared here and in table.h*/
	int iTableNr;
	int iSeats;
	int iTime;
	struct _table* pNext;
	struct _table* pPrev;
	order *Order;	
	char *Customer;
}table;

typedef struct _bill{
	char *Buyer; 	
	int total;
}bill;


int freeOrder(order* Order);
int CountTables(table *pHead);
void printTable(table* Table);

table* createTable(char *Customer, int iTableNr, int iSeats, int iTime){
	table* temp = malloc(sizeof(table));
	if(temp == NULL){
		printf("Memory Allocation failed");
	}
	temp->Customer = malloc(strlen(Customer) + 1);
        strcpy(temp->Customer, Customer);
        temp->iTableNr = iTableNr;
        temp->iSeats = iSeats;
        temp->iTime = iTime;
        temp->Order = NULL;
        temp->pPrev = NULL;
        temp->pNext = NULL;
        return temp;
}
order* createOrder(char *Buyer, char *Food, int iPrice){
	order* temp = malloc(sizeof(table));
	if(temp == NULL){
		printf("Memory Allocation failed");
	}
	temp->Buyer = malloc(strlen(Buyer) + 1);
	strcpy(temp->Buyer, Buyer);
	temp->Food = malloc(strlen(Food) + 1);
	strcpy(temp->Food, Food);
	temp->iPrice = iPrice;
	temp->pNext = NULL;
	return temp;
}



void DeleteTable(table** ppHead, table* ToBeDeleted){
	if(ToBeDeleted == NULL){
		
	}
	table* current = *ppHead;
	if(current == ToBeDeleted){
		*ppHead = current->pNext;
		if(*ppHead != NULL){
			(*ppHead)->pPrev = NULL;
		}
		free(current->Order);
		free(current->Customer);
		free(current);
		return;
	}
	
	while(current != NULL && current != ToBeDeleted){
		current = current->pNext; 
	}
	if(current == NULL){
		printf("Table not in list\n");
		return;
	}
	table* prev = current->pPrev;
	table* next = current->pNext;
	if(prev != NULL){
		/*We're at the head of the list */
		prev->pNext = next;
		
	}
	if(next != NULL){
		/*we're at the tail of  the list*/
		next->pPrev = prev;
	}
	current->pNext = NULL;
	current->pPrev = NULL;
	freeOrder(current->Order);
	free(current->Customer);
	free(current);
	
	
}
void PrependTable(table** ppHead, table* newTable){
	table* oldHead = *ppHead;
	table* newHead = newTable;
	newHead->pNext = oldHead;
	oldHead->pPrev = newHead;
	*ppHead = newHead;
}

void AppendTable(table** ppHead, table* Table){
	
	table* last = *ppHead;
	table* temp = Table;
        if(*ppHead == NULL){
        	
        	temp->pPrev = NULL;
        	
        	*ppHead = temp;
        	
        	

        }else{
        	while(last->pNext != NULL){
			last = last->pNext;
		}
		last->pNext = temp;
		temp->pPrev = last;
	}
}
void AddAfter(table* Table, table* toBeAdded){
	toBeAdded->pNext = Table->pNext;
	
	if(Table->pNext != NULL){
		Table->pNext->pPrev = toBeAdded;
	}
	Table->pNext = toBeAdded;
	toBeAdded->pPrev = Table;
}

void addTable(table** ppHead, char* Customer, int iTableNr, int iSeats, int iTime){
	table* temp = createTable(Customer, iTableNr, iSeats, iTime);
	table* current = *ppHead;
	if(*ppHead == NULL){
		AppendTable(ppHead, temp);
		return;
	}
	while(current != NULL){
		int comparison = strcasecmp(current->Customer, temp->Customer);
		if(comparison > 0){
			if(current->pPrev == NULL){
				PrependTable(ppHead, temp);
				return;
			}
			AddAfter(current->pPrev, temp);
			return;
		}else if(comparison < 0){
			if(current->pNext == NULL){
				AppendTable(ppHead, temp);
				return;
			}
			current = current->pNext;
		}else if(comparison == 0){
			AddAfter(current, temp);
			return;
		}
	}
}

void printTableNoSeats(table* Table){
	printf("Customer: %s\n", Table->Customer);
	printf("Table Nr: %d\n", Table->iTableNr);
	printf("Time: %d\n", Table->iTime);
	printf("\n");	
}

void printTable(table* Table){
	printf("Customer: %s\n", Table->Customer);
	printf("Table Nr: %d\n", Table->iTableNr);
	printf("Seats: %d\n", Table->iSeats);
	printf("Time: %d\n", Table->iTime);
	printf("\n");
}
void printOrder(order* Order){
	printf("Buyer: %s\n", Order->Buyer);
	printf("Food: %s\n", Order->Food);
	printf("price: %d\n", Order->iPrice);
}

void printOrderWithPrice(order* Order){
	int count = 0;
	int i;
	bill bills[5000];
	order* current = Order;
	while(current != NULL){
		int duplicate = 0;
		for(i = 0; i < count; i++){
			if(strcmp(bills[i].Buyer, current->Buyer)== 0){
				bills[i].total = bills[i].total + current->iPrice;
				duplicate = 1;
				break;
			}
		}
		if(!duplicate){
			bills[i].Buyer = strdup(current->Buyer);
			bills[i].total = current->iPrice;
			count++;
		}
		current = current->pNext;
	}
	for(i = 0; i < count; i++){
		printf("name: %s total: %d\n", bills[i].Buyer, bills[i].total);
	}
}

int printTableOrders(table* Table){
	table* temp = Table;
	int total = 0;
	if(temp->Order != NULL){
		order* currentOrder = temp->Order;
		while(currentOrder != NULL){
			printOrder(currentOrder);
			total = total + currentOrder->iPrice;
			currentOrder = currentOrder->pNext;
		}
		printf("Total: %d\n", total);
	}else{
		printf("No orders has been made\n");
	}
}
void printAllTable(table* pHead){
	table *temp = pHead;
	int Count = 0;
	while(temp != NULL){
		Count++;
		printf("%d: Customer: %s Table Nr: %d Seats: %d Time: %d\n", Count, temp->Customer, temp->iTableNr, temp->iSeats, temp->iTime);

		temp = temp->pNext;
		
	}
	printf("\n");
}

int checkForSameTimeAndTableNr(table** ppHead, int iTableNr, int iTime){
	
	table* current = *ppHead;
	while(current != NULL){
		if(current->iTime == iTime && current->iTableNr == iTableNr){
			return -1;
		}
		current = current->pNext;
	}
	return 1;
}

void AddOrder(table* currentTable, char *Buyer, char *Food, int iPrice){
	//printf("AddOrder()\n");
	order* temp = createOrder(Buyer, Food, iPrice);
	if(currentTable->Order == NULL){
		currentTable->Order = temp;
		return;
	}else{
		order* current = currentTable->Order;
		while(current->pNext != NULL){
			current = current->pNext;
		}
		current->pNext = temp;
	}
}
	
	

int CountTables(table* pHead){
	//printf("CountTables()\n");
        int count = 0;
        if(pHead == NULL){
        	puts("Head is NULL?");
                return count;
        }
        
        table* current = pHead;
        while(current != NULL){
                count++;
                //printf("%s\n", ptr->cBuf);
                current = current->pNext;

        }

        return count;
}
int CountOrders(table* pHead){
	int iCount = 0;
	if(pHead == NULL){
		puts("Head is NULL?");
		return iCount;
	}
	table* current = pHead;
	while(current != NULL){
		order* currentOrder = current->Order;
		while(currentOrder != NULL){
			iCount++;
		}
	}
	return iCount;
	
}
table* GetTableByIndex(table *pHead, int iIndex){
	/*Index starts at 1 */
	int Count = 1;
	table *current = pHead;
	while(iIndex != Count){
		if(current->pNext != NULL){
			current = current->pNext;
			Count = Count + 1; 
		}else{
			printf("Index is too large, returning NULL\n", Count);
			
			return current = NULL;
		}
	}
	return current;
}
void GetTableByName(table *pHead, char *name){
	table *current = pHead;
	
	while(current != NULL){
		
		/*Using strcasecmp to make the search case insensitive*/
		if(strcasecmp(current->Customer, name) == 0){
			printf("Found:\n");
			printTableNoSeats(current);
			
		}
		current = current->pNext;
	}
	
}

int freeOrder(order* Order){
	order* current = Order;
	order* next;
	while(current != NULL){
		next = current->pNext;
		free(current->Buyer);
		free(current->Food);
		free(current);
		current = next;
	}
}

int freeList(table *pHead){
	table* current = pHead;
	table* next;
	while(current != NULL){
		if(current->Order != NULL){
			freeOrder(current->Order);
		}
		next = current->pNext;
		free(current->Customer);
		free(current);
		current = next;
	}
}


