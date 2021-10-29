









#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int main (void)
{
    string text = get_string("Text: ");
    int letters = 0;
    int words = 0;
    int sentences  = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] <=90 && text[i]>=65)||(text[i]<=122 && text[i]>=97))
        {
            letters+=1;
        }
        if (text[i] == 32|| i ==(strlen(text)-1))
        {
            words+=1;
        }
        if (text[i] == 33||text[i] == 46||text[i] == 63)
        {
            sentences+=1;

        }

    }
    printf("%i letter(s)\n%i word(s)\n%i sentence(s)\n",letters,words,sentences);
    float L = (float)letters/words*100;
    float S = (float)sentences/words*100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    if (index<=16 && index>=1)
    {
        printf("Grade %.0f\n",round(index));
    }
    else if (index>=16)
    {
        printf("Grade 16+");
    }
    else if (index <= 1)
    {
        printf("Before Grade 1");
    }
}