from cs50 import get_int

def main():

    # Getting the number
    x = get_int("Number: ")
    # Checking what type if the card is valid
    if isValid(x) == 1:
        printCardType(x)

    else:
        print("INVALID")

# Doubling every other number starting with the second to last and summing
# With the sum of all non doubled numbers and checking if %10 == 0
# 1 == valid, 0 == invalid
def isValid(n):
    # Figure out last digit, do work with it, subtract it from
    # The number, then divide by 10 to eliminate it
    # Determining the last digit
    running = True
    evenOrOdd = 0
    evenSum = 0
    oddSum = 0
    while running == True:
        # Determining last digit
        lastDigit = n % 10;

        # Removing it from the card number
        n -= lastDigit;
        # If n - 1 < 0, we have completed all of the checks
        if n - 1 < 0:
            running = False

        n /= 10

        evenOrOdd += 1
        # Doubling if even
        if (evenOrOdd % 2 == 0):
            lastDigit *= 2
            if (lastDigit > 9):
                digit1 = lastDigit % 10
                evenSum += digit1 + 1
            else:
                evenSum += lastDigit


        else:
            oddSum += lastDigit

    # Returning modulus of even and odd sum
    return (evenSum + oddSum) % 10 == 0


def printCardType(n):
    # Last last digit gets the second input
    lld = 0
    running = True
    digitCounter = 0
    while running == True:
        # Counting digits
        digitCounter += 1

        # Determining last digit
        lastDigit = n % 10

        # Removing it from the card number
        n -= lastDigit
        n /= 10;
        #if n - 1 < 0, we have completed all of the checks
        if (n - 1 < 0):
            if lastDigit == 4 and (digitCounter == 16 or digitCounter == 13):
                print("VISA")

            if (lastDigit == 3 and digitCounter == 15 and (lld == 4 or lld == 7)):
               print("AMEX");

            if (lastDigit == 5 and digitCounter == 16 and (
                (lld == 1 or lld == 2 or lld == 3 or lld == 4 or lld == 5))):
                print("MASTERCARD")


            else:
                print("INVALID")


            running = False

        lld = lastDigit

