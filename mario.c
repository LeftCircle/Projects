#include <cs50.h>
#include <stdio.h>

void builder(int n);
int getInt(string prompt);
int checkInt(int n);

int main(void)
{
    //get int
    int blocks = getInt("pick an integer between 1 and 8 inclusive");
    
    //now print blocks
    builder(blocks);
}

void builder(int n)
{
    //building from top down I guess
    for(int i = 1; i < (n + 1); i++)
    {
        //adding spaces to the front
        if(n > 1)
        {
            int frontSpaces = n - i;
            for(int k = 0 ; k < frontSpaces; k++)
            {
                printf(" ");
            }
        }
        
        //printing 2*i+1 for each level (to include spaces)
        //on the last print ensure \n
        int printTimes = 2 * i + 1;
        for(int j = 1; j < printTimes + 1; j++)
        {
            if(j == ((printTimes + 1) / 2 ))
            {
                printf(" ");
            }
            if(j == printTimes)
            {
                printf("\n");
            }
            else
            {
                printf("#");
            }
        }
    }
}

int getInt(string prompt)
{
    
    int n;
    int check;
    do
    {
        n = get_int("%s\n", prompt);
        check = checkInt(n);
    }
    while (check == 0);
    return n;
}

int checkInt(int n)
{
    if(n < 1)
    {
        return 0;
    }
    if(n > 8)
    {
        return 0;
    }
    return 1;
}
