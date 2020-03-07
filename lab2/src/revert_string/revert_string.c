#include "revert_string.h"
#include <stdlib.h>
#include <string.h>

void RevertString(char *str)
{
	// your code here
    int length = strlen(str);
    char* str_copy = (char*)malloc(sizeof(char) * (length + 1));
    strcpy(str_copy, str);
    for (int i = 0; i < length; i++) {
        str[i] = str_copy[length - i - 1];
    }
    free(str_copy);
}
