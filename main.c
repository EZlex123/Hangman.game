#include <stdio.h>
#include "hangman.h"
#include <time.h>
#include <stdlib.h>

int main()
{
    srand(time(NULL));
    char secret[16];

    get_word(secret);
    printf("%s\n", secret);

    printf("%d\n", is_word_guessed("secret", "aeiou"));
    // prints: 0
    printf("%d\n", is_word_guessed("hello", "aeihoul"));
    // prints: 1

    char result[30];
    get_guessed_word(secret, "arpstxgoieyu", result);
    printf("%s\n", result);
    // result = "_o_tai_er"

    return 0;
}