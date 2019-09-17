from cs50 import get_string
from cs50 import get_int


def main():

    # First prompt for size
    size = getSize()

    # Now build

    builder(size)

# prints out the blocks according to a given height


def builder(size):
    # Printing from the top down
    for i in range(1, size + 1):
        # Prints 2 * height + 2 times
        spaces = size - i

        for i in range(spaces):
            print(" ", end="")

        # Now print blocks
        blocks = size - spaces

        for i in range(blocks):
            print('#', end="")

        # Now print two middle spaces
        for i in range(2):
            print(' ', end="")

        # Now print blocks then spaces

        for i in range(blocks):
            print('#', end="")

        print()


def getSize():
    size = 0
    while size < 1 or size > 8:

        size = get_int("Give me an int between 1 and 8 inclusive  ")

    return size


main()