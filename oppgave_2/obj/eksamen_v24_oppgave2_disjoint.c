
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* This function takes two pointers to words (char pointers) as arguments and returns true if the words are disjoint, and false if they are not.

 The function initializes an array count to count the occurrences of each letter in word1. It then iterates through each character in word1 and 
 increments the corresponding count in the array, ignoring non-alphabetic characters using the isalpha function.

 Next, the function iterates through each character in word2 and checks if any letter from word2 has occurred in word1 by comparing the count in 
 the count array. If any letter from word2 has occurred in word1, the function immediately returns false, indicating that the words are not 
 disjoint. If the loop completes without finding any shared letters, the function returns true, indicating that the words are disjoint. */

bool areDisjoint(const char* word1, const char* word2) {
   int count[26] = { 0 }; // Array to count the occurrences of each letter

   int length1 = strlen(word1);
   int length2 = strlen(word2);

   for (int i = 0; i < length1; i++) {
      if (isalpha(word1[i])) {
         int index = tolower(word1[i]) - 'a';
         count[index]++;
      }
   }

   for (int i = 0; i < length2; i++) {
      if (isalpha(word2[i])) {
         int index = tolower(word2[i]) - 'a';
         // If any letter from word2 has occurred in word1, they are not disjoint
         if (count[index] > 0) {
            return false;
         }
      }
   }

   return true;
}