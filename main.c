#include <stdio.h>
#include "hangman.h"
#include <time.h>
#include <stdlib.h>

int main()
{
    srand(time(NULL));
    char secret[16];

    get_word(secret);
    hangman(secret);

    return 0;
}