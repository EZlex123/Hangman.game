#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "hangman.h"

//-------------------------------------------------------------------

int get_word(char secret[])
{
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do
    {
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if (result != EOF)
            break;
    } while (1);

    fclose(fp);

    return 0;
}

//-------------------------------------------------------------------

int is_word_guessed(const char secret[], const char letters_guessed[])
{

    int findFlag = 0;

    for (int i = 0; i < strlen(secret); i++)
    {
        // letter from secret

        for (int j = 0; j < strlen(letters_guessed); j++)
        {

            // guessing letter

            if (secret[i] == letters_guessed[j])
            {

                // setting flag on. out of cycle.

                findFlag = 1;
                break;
            }
        }

        if (findFlag == 0)
        {

            // letter not found. End function
            return 0;
        }

        // reseting flag
        findFlag = 0;
    }
    // all letters are same
    return 1;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[])
{

    for (int i = 0; i < strlen(secret); i++)
    {

        guessed_word[i] = '_';
    }

    for (int i = 0; i < strlen(secret); i++)
    {

        for (int j = 0; j < strlen(letters_guessed); j++)
        {

            // guessing letter

            if (secret[i] == letters_guessed[j])
            {

                // setting letter on pos. out of cycle.
                guessed_word[i] = secret[i];

                break;
            }
        }
    }

    guessed_word[strlen(secret)] = '\0';
}

void get_available_letters(const char letters_guessed[], char available_letters[])
{

    char alp[40] = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < strlen(alp); i++)
    {
        available_letters[i] = alp[i];
    }

    int len = strlen(available_letters);

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < strlen(letters_guessed); j++)
        {
            if (available_letters[i] == letters_guessed[j])
            {
                int num = i;

                while (num < len)
                {
                    available_letters[num] = available_letters[num + 1];
                    num++;
                }

                len--;
                i--;
            }
        }
    }

    available_letters[len] = '\0';
}

int checkLetterInWord(const char secret[], const char letter)
{

    for (int i = 0; i < strlen(secret); i++)
    {

        if (secret[i] == letter)
        {
            return 1;
        }
    }

    return 0;
}

void hangman(const char secret[])
{
    int secretLen = strlen(secret);
    int attempts = 8;
    char available_letters[40] = "";
    char letters_guessed[40] = "";
    int letters_guessed_index = 0;
    char guessed_word[40] = "";

    int FindLetter = 0;

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %d letters long.\n", secretLen);
    printf("-------------\n");

    while (attempts > 0)
    {
        printf("You have %d guesses left.\n", attempts);
        get_available_letters(letters_guessed, available_letters);
        printf("Available letters: %s\n", available_letters);
        printf("Please guess a letter: ");
        char tmpInput[20];
        fgets(tmpInput, 20, stdin);
        if (strlen(tmpInput) - 1 == 1)
        {
            // is a symbol
            if (tmpInput[0] >= 97 && tmpInput[0] <= 122)
            { // lowercase

                if (checkLetterInWord(letters_guessed, tmpInput[0]))
                {
                    FindLetter = 1;
                    printf("Oops! You've already guessed that letter:");
                }
                else
                {
                    letters_guessed[letters_guessed_index] = tmpInput[0];
                    letters_guessed_index++;
                }
            }

            else if (tmpInput[0] >= 65 && tmpInput[0] <= 90)
            { // uppercase => upper to lower

                if (checkLetterInWord(letters_guessed, tmpInput[0]))
                {
                    FindLetter = 1;
                    printf("Oops! You've already guessed that letter:");
                }
                else
                {
                    tmpInput[0] += 32;
                    letters_guessed[letters_guessed_index] = tmpInput[0];
                    letters_guessed_index++;
                }
            }

            else
            { // unknown symbol
                printf("Oops! '%c' is not a valid letter: ", tmpInput[0]);
            }

            if (!FindLetter)
            {
                if (checkLetterInWord(secret, tmpInput[0]))
                {
                    printf("Good guess:");
                }
                else
                {
                    printf("Oops! That letter is not in my word:");
                    attempts--;
                }
            }
            FindLetter = 0;

            get_guessed_word(secret, letters_guessed, guessed_word);

            for (int i = 0; i < strlen(guessed_word); i++)
            {
                printf("%c ", guessed_word[i]);
            }
            printf("\n");
            printf("-------------\n");

            if (is_word_guessed(secret, letters_guessed))
            {
                printf("Congratulations, you won!\n");
                break;
            }
        }
        else
        {
            if (is_word_guessed(secret, tmpInput))
            {
                printf("Congratulations, you won!\n");
                break;
            }
            else
            {
                printf("Sorry, bad guess. The word was %s.\n", secret);
                break;
            }
        }
    }
    if (attempts == 0)
    {
        printf("Sorry, you ran out of guesses. The word was undeserved.\n");
    }
}
