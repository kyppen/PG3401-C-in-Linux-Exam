#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "eksamen_v24_oppgave2_all_lower.h"
#include "eksamen_v24_oppgave2_all_upper.h"
#include "eksamen_v24_oppgave2_anagram.h"
#include "eksamen_v24_oppgave2_disjoint.h"
#include "eksamen_v24_oppgave2_heterogram.h"
#include "eksamen_v24_oppgave2_palindrom.h"

typedef struct TASK2_WORD_METADATA {
	int iIndex; /* The index of the word, first = 1 */
	bool bIsPalindrom;
	bool bIsHeterogram;
	bool bIsUppercase;
	bool bIsLowercase;
	bool bIsAnagram;
	bool bIsDisjoint;
	int iSize; /* The length word (number ofs chars) */
	char szWord[]; /* The word written «after» the struct */
} sWord;

int checkWord(sWord *current, const char *word, const char *first, int index){
	if(index == 0){
		/*Sets these values for the first word */
		current->bIsAnagram = false;
		current->bIsDisjoint = false;
	}else{
		/*Rest of the words*/
		current->bIsAnagram = isAnagram(word, first);
		current->bIsDisjoint = areDisjoint(word, first);
	}
	current->iIndex = index + 1;
	current->bIsPalindrom = isPalindrome(word);
	current->bIsHeterogram = isHeterogram(word);
	current->bIsUppercase = isUpperCase(word);
	current->bIsLowercase = isLowerCase(word);
	current->iSize = strlen(word);
	
	return 0;
}

void printWord(const sWord *word) {

	printf("Word: %s\n", word->szWord);
	printf("Index: %d\n", word->iIndex);
	printf("Size: %d\n", word->iSize);
	printf("Palindrome: %d\n", word->bIsPalindrom);
	printf("Heterogram: %d\n", word->bIsHeterogram);
	printf("Uppercase: %d\n", word->bIsUppercase);
	printf("Lowercase: %d\n", word->bIsLowercase);
	printf("Anagram: %d\n", word->bIsAnagram);
	printf("Disjoint: %d\n", word->bIsDisjoint);
	printf("\n");
}
void freeStructList(sWord* arr[], int Count){
   int i;
	for (i = 0; i < Count; i++){
		free(arr[i]);
	}
}
int writeToBinaryFile(sWord* arr[], int Count){
	FILE *pBinFile = fopen("BinaryOutput.bin", "wb");
	if(pBinFile == NULL){
		printf("FILE IS NULL");
		return -1;
	}
	int i;
	for(i = 0; i < Count; i++){
		fwrite(arr[i], sizeof(sWord[0]) + arr[i]->iSize, 1, pBinFile);
		fwrite(arr[i]->szWord, sizeof(char), arr[i]->iSize + 1, pBinFile);
	}
	fclose(pBinFile);
}

int main(int argc, char* argv[]){
	printf("Starting\n");
	char line[50]; /*Max word Length*/
	sWord *list[1000]; /*Max amount of words we support.*/
	char *first; /* First word we read, this is what we'll compare the rest too. */
	

	int i;
	FILE *pFile = NULL;
	pFile = fopen("WordList.txt", "r");
	if(pFile == NULL){
		printf("File Not Found");
		return -1;
	}
	int Count = 0;
	while(fgets(line, sizeof(line), pFile) != NULL){
		line[strcspn(line, "\n\r")] = '\0';
		if(Count == 0){
			first = malloc(sizeof(line) + 1 );
			strncpy(first, line, strlen(line) +1);
		}
		sWord *current = malloc(sizeof(sWord) + strlen(line)+1);
		list[Count] = current;
		strncpy(current->szWord, line, strlen(line)+1);		
		checkWord(list[Count], line, first, Count);
		Count++;

	}
	fclose(pFile);
	
	
	for(i = 0; i < Count; i++){
		printWord(list[i]);
	}
	writeToBinaryFile(list, Count);
	freeStructList(list, Count);
	free(first);
	
	
	
} 	
