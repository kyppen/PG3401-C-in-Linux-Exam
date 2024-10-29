
#include <stdbool.h>

/* In this function, we iterate through each character of the input string using a while loop. We then manually compare each character's ASCII value to the range of uppercase letters ('A' to 'Z').

 The ASCII value of 'A' is 65, and the ASCII value of 'Z' is 90. If the character's ASCII value is outside this range, it means the character is not an uppercase letter, and we return false. If 
 all characters pass this check, the function returns true, indicating that the input string contains only uppercase letters. */


bool isUpperCase(const char* str) {
   while (*str != '\0') {
      // Check if the character is an uppercase letter (ASCII values: 65-90)
      if (*str < 'A' || *str > 'Z') {
         return false; // Character is not uppercase
      }
      str++;
   }
   return true; // All characters are uppercase
}
