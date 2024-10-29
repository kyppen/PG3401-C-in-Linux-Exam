
#include <stdbool.h>

/* In this function, we iterate through each character of the input string using a while loop. We then manually compare each character's ASCII value to the range of lowercase letters ('a' to 'z').

 The ASCII value of 'a' is 97, and the ASCII value of 'z' is 122. If the character's ASCII value is outside this range, it means the character is not a lowercase letter, and we return false. If 
 all characters pass this check, the function returns true, indicating that the input string contains only lowercase letters. */

bool isLowerCase(const char* str) {
   while (*str != '\0') {
      // Check if the character is a lowercase letter (ASCII values: 97-122)
      if (*str < 'a' || *str > 'z') {
         return false; // Character is not lowercase
      }
      str++;
   }
   return true; // All characters are lowercase
}

