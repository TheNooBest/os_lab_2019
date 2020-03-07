#include "swap.h"

void Swap(char *left, char *right)
{
	// ваш код здесь
    char buff = *left;
    *left = *right;
    *right = buff;
}
