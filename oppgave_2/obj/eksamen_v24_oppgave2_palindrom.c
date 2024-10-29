
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* This function takes a pointer to a word (a char pointer) as an argument and returns true if the word is a palindrome, and false if it is not.

 The function initializes two indices, i and j, pointing to the start and end of the word respectively. It then compares the characters at these 
 indices while moving towards the center of the word. If the characters at the indices are not equal (ignoring case), the function immediately 
 returns false, indicating that the word is not a palindrome. If the loop completes without any unequal characters, the function returns true, 
 indicating that the word is a palindrome. */

bool isPalindrome(const char* word) {
   int length = strlen(word);
   int i = 0;
   int j = length - 1;

   while (i < j) {
      while (!isalpha(word[i])) {
         i++;
      }
      while (!isalpha(word[j])) {
         j--;
      }

      if (tolower(word[i]) != tolower(word[j])) {
         return false;
      }

      i++;
      j--;
   }

   return true;
}
