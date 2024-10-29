#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Table.h"
#include "stdbool.h"
#include <ctype.h>
#define MAX_INPUT_LENGTH 100


void printMenu();
int getIntInput();

/*Most of these function are just interfaces for Table.h*/

void addReservation(table** ppHead){
	char Customer[MAX_INPUT_LENGTH];
	int iTableNr;
	int iSeats;
	int iTime;
	printf("Enter Customer Name: ");
	fgets(Customer, MAX_INPUT_LENGTH, stdin);
	Customer[strcspn(Customer, "\n\r")] = '\0';
	if(strlen(Customer) <= 0){
		printf("Name cannot be empty\n");
		return;
	}
	printf("Table Number: ");
	iTableNr = getIntInput();
	printf("Seats: ");
	iSeats = getIntInput();
	printf("Time: ");
	iTime = getIntInput();
	
	if(checkForSameTimeAndTableNr(ppHead, iTableNr, iTime) == -1){
		return;
	}
	addTable(ppHead ,Customer, iTableNr, iSeats, iTime);
	printf("\n");
}
void OrderFood(table** ppHead){
	int selected;
	char buyer[MAX_INPUT_LENGTH];
	char food[MAX_INPUT_LENGTH];
	int price;
	printAllTable(*ppHead);
	selected = getIntInput();
	table* toOrder = GetTableByIndex(*ppHead, selected);
	if(toOrder != NULL){
		printf("Enter Buyer: ");
		fgets(buyer, MAX_INPUT_LENGTH, stdin);
		buyer[strcspn(buyer, "\n\r")] = '\0';
		if(strlen(buyer) <= 1){
			printf("Name cannot be empty\n");
			return;
		}
		printf("Enter Food: ");
		fgets(food, MAX_INPUT_LENGTH, stdin);
		food[strcspn(food, "\n\r")] = '\0';
		if(strlen(food) <= 1){
			printf("Name cannot be empty\n");
			return;
		}
		printf("Enter price: ");
		price = getIntInput();
		AddOrder(toOrder, buyer, food, price);
	}
}

int getIntInput(){
	bool InputAccepted = false;
	char input[MAX_INPUT_LENGTH];
	char *endptr;
	int selected;
	while(!InputAccepted){
		memset(input, 0, strlen(input));
		fgets(input, MAX_INPUT_LENGTH, stdin);
		selected = strtol(input, &endptr, 10);
		if(!(selected <= 0)){
			return selected;
		}else{
			printf("Invalid input\n");
			printf("Retry:");
			
			
		}
	}
}
int getInput(){
	bool InputAccepted = false;
	char input[MAX_INPUT_LENGTH];
	char *endptr;
	long selected;
	while(!InputAccepted){
		printMenu();
		fgets(input, MAX_INPUT_LENGTH, stdin);
		selected = strtol(input, &endptr, 10);
		if(!(selected <= 0)){
			return selected;
		}else{
			puts("Not accepted\n");
		}
	}	
}
void findByIndex(table** ppHead){
	int index;
	index = getIntInput();
	table *Wanted = GetTableByIndex(*ppHead, index);
	if(Wanted != NULL){
		printTable(Wanted);
		printTableOrders(Wanted);
	}
}
void findByName(table** ppHead){
	char Customer[MAX_INPUT_LENGTH];
	
	printf("Enter Customer Name: ");
	fgets(Customer, MAX_INPUT_LENGTH, stdin);
	Customer[strcspn(Customer, "\n\r")] = '\0';
	GetTableByName(*ppHead, Customer);
}
void deleteTableWithSelect(table** ppHead){
	int selected;
	printAllTable(*ppHead);
	selected = getIntInput();
	table* toDelete = GetTableByIndex(*ppHead, selected);
	if(toDelete != NULL){
		DeleteTable(ppHead, toDelete);
	}
}
void printTableBill(table** ppHead){
	int selected;
	printAllTable(*ppHead);
	selected = getIntInput();
	table* Table = GetTableByIndex(*ppHead, selected);
	if(Table != NULL){
		printTableOrders(Table);
	}
}
void printBuyerBill(table** ppHead){
	int selected;
	printAllTable(*ppHead);
	selected = getIntInput();
	table* Table = GetTableByIndex(*ppHead, selected);
	if(Table != NULL){
		printOrderWithPrice(Table->Order);
	}	
}
int listEmptyCheck(table** ppHead){
	if(CountTables(*ppHead) != 0){
		return 1;
	}else{
		return -1;
	}
}


int MenuSwitchCase(table** ppHead,int choice){
	switch(choice){
		case 1:
			puts("Make Reservation");
			addReservation(ppHead);
		break;
		case 2:
			puts("Find table by index");
			if(listEmptyCheck(ppHead) == 1){
				findByIndex(ppHead);
			}else{
				printf("List is currently empty\n");
			}

		break;
		case 3: 
			puts("Find table by name");
			if(listEmptyCheck(ppHead) == 1){
				findByName(ppHead);
			}else{
				printf("List is currently empty\n");
			}

		break;
		case 4:
			puts("Delete reservation");
			if(listEmptyCheck(ppHead) == 1){
				deleteTableWithSelect(ppHead);
			}else{
				printf("List is currently empty\n");
			}
		break;
		case 5:
			puts("Order Food");
			if(listEmptyCheck(ppHead) == 1){
				OrderFood(ppHead);
			}else{
				printf("List is currently empty\n");
			}
		break; 
		case 6:
			puts("Print table bill");
			if(listEmptyCheck(ppHead) == 1){
				printTableBill(ppHead);
			}else{
				printf("List is currently empty\n");
			}
			
		break;
		case 7:
			puts("Print split bill");
			if(listEmptyCheck(ppHead) == 1){
				printBuyerBill(ppHead);
			}else{
				printf("List is currently empty\n");
			}
		break;
		case 8:
			puts("Prints all tables");
			if(listEmptyCheck(ppHead) == 1){
				printAllTable(*ppHead);	
			}else{
				printf("List is currently empty\n");
			}
		break;
			
		case 9:
			puts("Exiting");
			return -1;
		default:
			puts("Please select one of the options");
	}
	printf("\n");
}
int Menu(table **ppHead){
	bool running = true;
	int choice;
	while(running){
		choice = getInput();
		if(MenuSwitchCase(ppHead,choice) == -1){
			running = false;
		}
	}
}
void printMenu(table** ppHead){
	
	printf("1: Make reservation\n");
	printf("2: Find Table by index\n");
	printf("3: Find Table by name\n");
	printf("4: Delete reservation\n");
	printf("5: Order food\n");
	printf("6: Print table bill\n");
	printf("7: Print split bill\n");
	printf("8: Print all ables\n");
	printf("9: Exit\n");	
	
}

int main(int argc, char* argv[]){
	printf("Starting\n");
	table* pHead = NULL;
	int i;
	int j;
	Menu(&pHead);
	
	freeList(pHead);
}


