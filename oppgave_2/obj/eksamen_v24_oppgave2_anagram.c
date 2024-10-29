
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* This function takes two pointers to words (char pointers) as arguments and returns true if the words are anagrams, and false if they are not.

 The function initializes two arrays, count1 and count2, to count the occurrences of each letter in the respective words. It then iterates through 
 each character in both words and increments the corresponding count in the respective arrays, ignoring non-alphabetic characters using the isalpha function.

 After counting the occurrences of each letter in both words, the function compares the count of each letter between the two arrays. If any count differs, 
 the function immediately returns false, indicating that the words are not anagrams. If all counts are equal, the function returns true, indicating that the words are anagrams. */

bool isAnagram(const char* word1, const char* word2) {
   int count1[26] = { 0 }; // Array to count the occurrences of each letter in word1
   int count2[26] = { 0 }; // Array to count the occurrences of each letter in word2

   int length1 = strlen(word1);
   int length2 = strlen(word2);

   // If the lengths of the words are not equal, they cannot be anagrams
   if (length1 != length2) {
      return false;
   }

   for (int i = 0; i < length1; i++) {
      if (isalpha(word1[i])) {
         int index = tolower(word1[i]) - 'a';
         count1[index]++;
      }
   }

   for (int i = 0; i < length2; i++) {
      if (isalpha(word2[i])) {
         int index = tolower(word2[i]) - 'a';
         count2[index]++;
      }
   }

   // Compare the count of each letter in both words
   for (int i = 0; i < 26; i++) {
      if (count1[i] != count2[i]) {
         return false;
      }
   }

   return true;
}
