#include <stdio.h>
#include <cs50.h>

int main (void)
{
    long no = get_long("Credit card no.: ");
    int n = 0;
    long cc = no;
    while (cc>0)
    {
        cc = cc/10;
        n++;
    }
    if (n != 13 && n != 15 && n !=16)
    {
        printf("INVALID\n");
    }
    else
    {
        int sum1 = 0;
        int sum2 = 0;
        int mod1;
        int mod2;
        long x = no;
        int d1;
        int d2;
        int total;

        do
        {
            mod1 = x % 10;
            x = x/10;
            sum1+=mod1;

            mod2 = x % 10;
            x = x/10;
           mod2=mod2 * 2;
           d1 = mod2%10;
           d2 = mod2/10;
           sum2 = sum2 + d1 + d2;
           total = sum1+sum2;
        }
        while (x>0);

        long cc1 = no;
        if (total % 10 != 0)
        {
            printf("INVALID\n");
        }
        else
        {
            do
            {
                cc1 = cc1/10;
            }
            while (cc1>100);
            if (cc1 == 34|| cc1 == 37)
            {
                printf ("AMEX\n");
            }
            else if (cc1 >50 && cc1 <56)
            {
                printf ("MASTERCARD\n");
            }
            else
            {
                int d3 = cc1/10;
                if (d3==4)
                {
                    printf ("VISA\n");
                }
                else
                {
                    printf ("INVALID\n");
                }
            }
        }
    }
}