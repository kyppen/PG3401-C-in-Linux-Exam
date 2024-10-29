
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* This function takes a pointer to a word (a char pointer) as an argument and returns true if the word is a heterogram, and false if it is not.

 The function iterates through each character in the word and ignores non-alphabetic characters using the isalpha function. It then converts the 
 letter to lowercase using the tolower function. The index of the letter in the alphabet is calculated by subtracting the ASCII value of 'a'. If 
 the letter has already been counted before (count[index] > 0), it means the letter has already occurred in the word, and the function returns 
 false. If all the letters in the word are checked without any repetitions, the function returns true. */

bool isHeterogram(const char* word) {
   int count[26] = { 0 }; // Array to count the occurrences of each letter

   int length = strlen(word);
   for (int i = 0; i < length; i++) {
      if (isalpha(word[i])) {
         int index = tolower(word[i]) - 'a';

         // If the letter has already been counted before, it's not a heterogram
         if (count[index] > 0) {
            return false;
         }

         count[index]++;
      }
   }

   return true;
}