#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main (int argc, string argv[])

{   const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (argc!=2)
    {
        printf("Please provide only one command line argument.\n");

    }
    int letters[26];
    for (int i=0, n = 26; i<n; i++)
    {
        if (!((argv[1][i]>='a' && argv[1][i]<='z')||(argv[1][i]>='A' && argv[1][i]<='Z')))
        {
            printf("Key must only contain letters.\n");
            break;
        }
        else if (argv[1][i]>='a' && argv[1][i]<='z')
        {
            argv[1][i]=toupper(argv[1][i]);
        }
        for (int j=0; j<26; j++)
        {
            if (argv[1][i]==letters[j])
            {
                printf("Key letters must not be repeated.\n");
                break;
            }

        }
        letters[i] = argv[1][i];
    }

    string text = get_string("plaintext: ");
    int l = strlen(text);
    char cipher[l + 1];
    for (int i = 0, n =strlen(text); i<n; i++)
    {
        if (isupper(text[i])!=0)
        {
            for (int j = 0; j<26; j++)
            {
                if (text[i]==alphabet[j])
                {
                    cipher[i]=argv[1][j];
                    break;
                }
            }
        }
        else if (islower(text[i])!=0)
        {
            for (int j = 0; j<26; j++)
            {
                if (text[i] == tolower(alphabet[j]))
                {
                    cipher[i] = tolower(argv[1][j]);
                    break;
                }
            }
        }
        else
        {
            cipher[i]=text[i];

        }
        cipher[l]='\0';
    
    }
    printf("ciphertext: %s\n", cipher);


}
