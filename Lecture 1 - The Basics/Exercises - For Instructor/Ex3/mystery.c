#include <stdio.h>

__attribute__((used)) char *strcpy(char *dest, const char *src)
{
   char *save = dest;
   while((*dest++ = *src++));
   return save;
}

// returns true if `X` and `Y` are the same
__attribute__((always_inline)) static inline int compare(const char *X, const char *Y)
{
    while (*X && *Y)
    {
        if (*X != *Y) {
            return 0;
        }
 
        X++;
        Y++;
    }
 
    return (*Y == '\0');
}
 
// Function to implement `strstr()` function
__attribute__((used)) const char* strstr(const char* X, const char* Y)
{
    while (*X != '\0')
    {
        if ((*X == *Y) && compare(X, Y)) {
            return X;
        }
        X++;
    }
 
    return 0;
}

__attribute__((used)) int atoi(char *str)
{
    if (*str == '\0')
       return 0;
 
    int res = 0;		// Initialize result
    int sign = 1;		// Initialize sign as positive
    int i = 0;			// Initialize index of first digit
 
     if (str[0] == '-')
    {
        sign = -1;
        i++;  			// Also update index of first digit
    }
 
    for (; str[i] != '\0'; ++i)
    {
        if ( str[i] < '0' || str[i] > '9')	// If string contain character it will terminate
            return 0; 
            
        res = res*10 + str[i] - '0';
    }
 
    return sign*res;
}

int main(void)
{
	printf("Describe these functions three: %p, %p, %p\n", &strcpy, &strstr, &atoi);
}
