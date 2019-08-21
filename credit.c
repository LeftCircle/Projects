// Credit card validator
// Double every digit starting with the second to last and sum
// Sum every non doubled digit
// Add them together and % 10. if it is zero, the card is valid
#include <cs50.h>
#include <stdio.h>

long isValid(long n);
long getNumber(string input);
void printCardType(long n);

int main(void)
{
    // Getting the number
    long x = getNumber("Number: ");
    // Checking what type if the card is valid
    if (isValid(x) == 1)
    {
        printCardType(x);
    }
    
    else
    {
        printf("INVALID\n");
    }
    
}

long getNumber(string input)
{
    long n;
    n = get_long("%s\n", input);
    return n;
}
   
// Doubling every other number starting with the second to last and summing
// With the sum of all non doubled numbers and checking if %10 == 0
// 1 == valid, 0 == invalid
long isValid(long n)
{
    // Figure out last digit, do work with it, subtract it from
    // The number, then divide by 10 to eliminate it
    // Determining the last digit
    bool running = true;
    int evenOrOdd = 0;
    int evenSum = 0;
    int oddSum = 0;
    while (running == true)
    {
        // Determining last digit
        int lastDigit = n % 10;
        
        // Removing it from the card number
        n -= lastDigit;
        // If n - 1 < 0, we have completed all of the checks
        if (n - 1 < 0)
        {
            running = false;
        }
        n /= 10;
        
        evenOrOdd += 1;
        // Doubling if even
        if (evenOrOdd % 2 == 0)
        {
            lastDigit *= 2;
            if (lastDigit > 9)
            {
                int digit1 = lastDigit % 10;
                evenSum += digit1 + 1;
            }
            else
            {
                evenSum += lastDigit;
            }
        }
        else
        {
            oddSum += lastDigit;
        }
    }
    
    // Returning modulus of even and odd sum
    return (evenSum + oddSum) % 10 == 0;
}

void printCardType(long n)
{
    // Last last digit gets the second input
    int lld = 0;
    bool running = true;
    int digitCounter = 0;
    while (running == true)
    {
        // Counting digits
        digitCounter += 1;
        
        // Determining last digit
        int lastDigit = n % 10;
        
        // Removing it from the card number
        n -= lastDigit;
        n /= 10;
        //if n - 1 < 0, we have completed all of the checks
        if (n - 1 < 0)
        {
            if (lastDigit == 4 && digitCounter == 16 && digitCounter == 13 )
            {
                printf("VISA\n");
            }
            
            if (lastDigit == 3 && digitCounter == 15 && (lld == 4 || lld == 5))
            {
               printf("AMEX\n");
            }
            
            if (lastDigit == 5 && digitCounter == 16 && (
                (lld == 1 || lld == 2 || lld == 3 || lld == 4 || lld == 5)))
            {
                printf("MASTERCARD\n");
            }
            
            else
            {
                printf("INVALID\n");
            }
            
            running = false;
        }
        lld = lastDigit;
    }
}

