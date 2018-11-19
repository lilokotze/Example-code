// Program to encipher text using Vigenere's algorithm

//Include all relevant libraries
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[]) //argc stores number of strings in argv
{
    //Initialise all variables used in program
    int c, kj, i, n, p;
    int j = 0;
    int k_count = 0;
    int key_len = 0;

    char c_asc, p_asc;

    string plain_t;
    string k;
    string cipher_t;

    while (argc != 2) //Test whether initial input argument is entered correctly, else return an error message
    {
        printf("You've entered too many arguments. Please try again. \n");
        return (1);
    }

    //Initialize variables
    k = argv[1];
    key_len = strlen(argv[1]);

    int key[key_len];


    for (i = 0, n = strlen(k); i < n; i++) //Loop to test whether key is alphabetic
    {
        k_count = k[i];

        if (isalpha(k_count))
        {
            if (isupper(k_count))
            {
                k_count = k_count - 65; //Change value to alphabetic value - Upper case
            }
            else if (islower(k_count))
            {
                k_count = k_count - 97; //Change value to alphabetic value - Lower case
            }
        }
        else
        {
            printf("Input argument containt non-alphabetic cahracters."); //Error if key containt non-alphabetic character
            return (1);
        }

        key[i] = k_count; //Create key array to use later
    }


    plain_t = get_string("plaintext: ");    //Get string input from user. Prompt user for input

    cipher_t = plain_t;     //Initialize cipher_t

    for (i = 0, n = strlen(plain_t); i < n; i++) //Loop to determine input string length
    {
        p = plain_t[i];

        if (isalpha(p))         //Determine if character is alphabetical
        {
            if (isupper(p))     //Determine if cahracter is upper-case, preserve case
            {
                //Implement Vigenere's algorithm
                kj = key[j];
                p = p - 65;
                c = (p + kj) % 26;

                p_asc = p + 65;
                c_asc = c + 65;

                j++; //Roll on to next key value

                if (j == key_len) //Restart at first key-character
                {
                    j = 0;
                }
            }
            else if (islower(p))    //Determine if cahracter is lower-case, preserve case
            {
                //Implement Vigenere's algorithm
                kj = key[j];
                p = p - 97;
                c = (p + kj) % 26;

                p_asc = p + 97;
                c_asc = c + 97;

                j++; //Roll on to next key value

                if (j == key_len) //Restart at first key-character
                {
                    j = 0;
                }
            }
        }
        else
        {
            //Leave numbers or non-alphabetic characters unchanged
            c_asc = plain_t[i];
        }

        //Add rotated character to ciphertext staring
        cipher_t[i] = c_asc;
    }

    //Print enciphered text
    printf("ciphertext: %s", cipher_t);
    printf("\n");

    return (0);
}
