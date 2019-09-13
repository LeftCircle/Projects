# Spell Checker

This code was developed in the cs50 ide, and spell checks a document using an imported dictionary.
The spellchecker works by reading the imported dictionary and creating a trie to store all of the words. 
A text file is then checked against the trie, and any mispelled words are printed. A word is assumed to 
be misspelled if it is not found in the dictionary. Once the file is checked, the trie is then removed from 
the heap.
