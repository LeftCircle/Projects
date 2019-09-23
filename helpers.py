from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""
    '''
    Given two strings, a and b, it returns a list of the lines that are, identically, in both a and b.
    The list should not contain any duplicates. Assume that lines in a and b will be be separated by \n,
    but the strings in the returned list should not end in \n. If both a and b contain one or more blank
    lines (i.e., a \n immediately preceded by no other characters), the returned list should include an
    empty string (i.e., "").
    '''

    # generating a list of lines in A and lines in B
    linesA = getLines(a)
    linesB = getLines(b)

    return findSame(linesA, linesB)


def sentences(a, b):
    """Return sentences in both a and b"""

    # TODO
    # Generate a list of sentences in A and B
    sentA = sent_tokenize(a)
    sentB = sent_tokenize(b)

    return findSame(sentA, sentB)


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # TODO
    # Determine substrings of length n in a and b
    substrA = findSubStr(a, n)
    substrB = findSubStr(b, n)

    # determine substrings in both

    return findSame(substrA, substrB)

# Looks at two strings, checks thier lengths, then returns if
# they are the same and always returns the first string

def stringChecker(a, b):
    same = False
    if len(a) == len(b):
        for i in range(len(a)):
            if a[i] == b[i]:
                if i == len(b) - 1:
                    same = True
                else:
                    continue
            else:
                same = False
                break

    identicalBits = a

    return same, identicalBits

def numOfLines(string):
    lines = 0
    for i in range(len(string)):
        if string[i] == "/n":
            lines += 1
    return lines

def getLines(string):
    lenS = len(string)
    lines = []
    temp = ""
    for i in range(lenS):
        if string[i] == "\n":
            lines.append(temp)
            temp = ""
        else:
            temp += (string[i])
        if i == lenS - 1:
            lines.append(temp)

    return lines

# finds the parts of two lists of lists that are the same
def findSame(a, b):
    # Create dics
    dicA = dict.fromkeys(a)
    dicB = dict.fromkeys(b)
    # Check items in A and compare them to B
    shared = {i: dicA[i] for i in dicA if i in dicB and dicA[i] == dicB[i]}
    # Return the list
    return list(shared)




def findSubStr(string, length):
    # determine len of string
    strLen = len(string)

    # determine all possible substrings
    substr = []
    for i in range(strLen):
        temp = ""
        added = 0
        for j in range(length):
            if i + length <= strLen:
                temp += string[i + j]
                added = 1

        if added == 1:
            substr.append(temp)

    # Delete duplicates
    substr = list(dict.fromkeys(substr))
    return substr