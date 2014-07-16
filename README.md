## trieWordList
======================

### Programming Problem - Find Longest Word Made of Other Words

Write a program that reads a file containing a sorted list of words (one word per line, no spaces, all lower case), then identifies the longest word in the file that can be constructed by concatenating copies of shorter words also found in the file. The program should then go on to report the second longest word found as well as how many of the words in the list can be constructed of other words in the list.
Please reply to this email with your solution as source code along with the (2) longest words that the program finds and the count of words that can be constructed as an output in the body of the email and any comments you have on the approach you took.
If you have any questions about the problem, please feel free to email us back.
For example, if the file contained:

       cat
       cats
       catsdogcats
       catxdogcatsrat
       dog
       dogcatsdog
       hippopotamuses
       rat
       ratcatdogcat

The longest word would be 'ratcatdogcat' - at 12 letters, it is the longest word made up of other words in the list.  

### Solution

The result of my program is as follows:

Longest word composed of smaller words = **ethylenediaminetetraacetates**
Length = 28
Second longest word composed of smaller words = **electroencephalographically**
Length = 27

Total number of words composed of smaller words = 97107


My approach was to use the Trie data structure, or prefix tree.  Unlike a binary tree which has 2 branches in each node, the Trie can have one for each letter of the alphabet.  There are a couple advantages to using a Trie to store a large list of words:

1. Less storage required.  For words with similar prefixes (i.e. cat, cats, catapult), the Trie will branch off the common prefix "cat".  
2. Faster access to a word.   Let N = the maximum length of a word.  A Trie only requires O(N) operations to find the word.  This is independent of the number of words in the list.
3. Faster access to similar words.  When determining whether word A in the list is composed of other words, you may need to try several similar target words to see if they fit in word A.  The Trie excels at this task, given the way it stores words with the same prefix under the same parent node.

#### Design Choices
 There are many ways to implement a Trie, and I chose to create Trie nodes with 27 children (for the 26 letters in the alphabet + 1 to designate end of word (EOW)).  While consuming more storage than a linked list, it has better speed.  You merely calculate the index to the child for a given letter of the alphabet, effectively hashing the location.  This is faster than walking through a linked list.
Also in the interest of performance and memory, I chose to use basic character arrays to store the word list.  C++ std string objects consume more storage and are perhaps slower.

#### Finding the Words Composed of Smaller Words
The function which finds the words composed of smaller words is: 

> findSubWords(pWord,pSubWords).  

Where pWord is the word being tested.  And pSubWords is the criteria for success.

> findSubWords(pWord,pSubWords) 

recursively tests whether pWord is composed of smaller words in the list / Trie.  On success, the function will return nSubWords = 2 to the top level.  

##### Why nSubWords = 2:
If the first subsection of pWord contains a subWord, increment nSubWords.  Now nSubWords = 1.
Step through pWord by recursively entering findSubWords(pWord,pSubWords).  If all levels of recursion successfully find subWords, increment nSubWords again.  Now nSubWords = 2.  Else if not successful, at the top level, use the same Trie prefix node, keep adding letters until another subWord / prefix found.  Then recursively look for subWords in the rest of pWord.

### Using the Program

The program is called TrieWordList and consists of three files: 
1. TrieWordList.cpp
2. TrieWordList.h
3. Main.cpp

Compile it, and at the command line run:

> TrieWordList fileName

where fileName is the name of the text file containing the word list.  The program prints a minimum amount of information, and prints the final result.  

To get more detailed printout of the search process, edit TrieWordList.h.  Comment out 
> \#undef NDEBUG

and uncomment

> \#define NDEBUG

