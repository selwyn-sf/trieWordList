/*
 * TrieWordList.cpp
 *
 *  Created on: Feb 19, 2014
 *      Author: Selwyn Yee
 *      Email:  selwyn_yee@yahoo.com
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TrieWordList.h"
using namespace std;

CTrieWordList::CTrieWordList() {
	nodesCreated = 0;
	nodesDeleted = 0;
	nWords = 0;
	trieRoot = makeTrieNode();	// create root of Trie tree
}

CTrieWordList::~CTrieWordList() {
	int i;

	// free each string pWordList
	for (i = 0; i < nWords; i++) {
		if (pWordList[i] != NULL) {
			delete [] pWordList[i];
		}
	}

	// free pWordList (the main handle)
	if (pWordList != NULL) {
		delete pWordList;
	}

	// recursively delete Trie starting from root.
	deleteTrie(trieRoot);

	if (nodesCreated == nodesDeleted) {
		cout << nodesCreated << " nodes created, " << nodesDeleted
				<< " nodes deleted" << endl;
	}
	else
	{
		cout << "ERROR: " << nodesCreated << " nodes created, " << nodesDeleted
				<< " nodes deleted.  There is a memory leak." << endl;
	}
}

// Allocate memory for pWordList array of strings.  Add
// extra character in each string for the '\0' character
// at end.
void CTrieWordList::allocateWordList(int nwords, int len) {
	int i;
	int lenToAllocate;

	// Step 1: Given the number of words (nwords) allocate that
	// many pointers to strings.
	try {
		pWordList = new char*[sizeof(char*) * nwords];
	}
	catch (std::bad_alloc &ba) {
		cerr << "Error: cannot allocate pWordList " << ba.what() << endl;
	}

	// Step 2: for each of the nwords pointers to string, allocate
	// a string of len+1 (to include the '\0' character)
	lenToAllocate = len + 1;
	try {
		for (i = 0; i < nwords; i++) {
			pWordList[i] = new char[sizeof(char) * (lenToAllocate)];
		}
	}
	catch (std::bad_alloc &ba) {
		cerr << "bad_alloc caught: " << ba.what() << endl;
	}


	cout << "allocateWordList: created array of " << nwords << " elements, " <<
			lenToAllocate << " chars each" << endl;
}


// findSubWords(pWord,pSubWords) recursively tests whether pWord is composed of
// smaller words in the list / Trie.  On success, the function will return nSubWords = 2
// to the top level.
//
// Why nSubWords = 2:
//
// If the first subsection of pWord contains a subWord, increment nSubWords.
//
// Now nSubWords = 1.
//
// Step through pWord by recursively entering findSubWords(pWord,pSubWords).
// if all levels of recursion successfully find subWords, increment nSubWords
// again.
//
// Now nSubWords = 2
//
// Else if not successful, at the top level, use the same Trie prefix node,
// keep adding letters until another subWord / prefix found.  Then recursively
// look for subWords in the rest of pWord.
//
// 1. Step through each character in pWord.
// 2. Navigate through the Trie with the characters in pWord.
// 3. If one of the corresponding TrieNodes is NULL, return false.
// 4. Else if a TrieNode pChild[END_OF_WORD] != NULL, then a word
// 		in the Trie matches part of pWord.  Call this a subWord.
// 5. Increment nSubwords by one.
// 6. Go to next character in pWord recursively enter findSubWords(pWord,pSubWords).
//		This starts at the root of the Trie again.
// 7. For each subWord found, recursively enter findSubWords(pWord,pSubWords).
// 8. If you reach the end of pWord and the deepest recursion of
// 		findSubWords(pWord,pSubWords) has found a COMPLETE subWord, return true
//		to the each level up the recursion stack.  At the top level, inrement
// 		nSubWords.
//		subWord in the Trie AND nSubwords > 1, then pWord is composed
//		of other words in the list.  Return true.
// 9. Else if nSubwords <= 1, pWord matched itself only.  So subWords
//		were not found.  Return false.
bool CTrieWordList::findSubWords(char *pWord, int *pSubWords) {
	char c;
	int charIndex;
	int iChar;
	int len;
	int nChars;
	int nSubWords = 0;				// number of smaller words found in pWord
	TrieNode* pNode;
	TrieNode* pPrevious;
	char subWord[SEARCH_WORD_LEN];	// smaller word found in pWord

	pNode = trieRoot;	// start word insertion at root

	nChars = 0;

	try {
		len = strlen(pWord);
		if (len == 0)
			throw 1;
	}
	catch (int e) {
		cerr << "String of p length.  Skip this string" << endl;
		return false;
	}

	#ifdef NDEBUG
		cout << "findSubWords: test word = " << pWord << ", len = " << len << endl;
	#endif

	for (iChar = 0; iChar < len; iChar++) {
		c = *pWord;			// get character

		#ifdef NDEBUG
			cout << "c = " << c << ", pWord = " << *pWord << endl;
		#endif

		// Calculate index of pChild node.
		// If c = 'a', charIndex = 0
		charIndex = c - 'a';

		if (pNode->pChild[charIndex] == NULL) {
			#ifdef NDEBUG
				cout << "Word not found" << endl;
			#endif

			return false;	// pWord does not exist in Trie
		}

		subWord[nChars++] = c;
		subWord[nChars] = '\0';	// terminate subWord
		pNode = pNode->pChild[charIndex];	// follow node to next level down

		// If endOfWordNextNode (child node has END_OF_WORD mark), the next node
		// marks a subWord.  Skip to the next character and attempt to find next
		// subWord.
		if ( endOfWordNextNode(pNode) ) {
			#ifdef NDEBUG
				cout << "EOW: c = " << c << ", pWord = " << *pWord << endl;
			#endif

			if (nChars > 0) {		// subword found in pWord
				nSubWords++;

				// end of pWord, break from loop
				if ( iChar == (len-1) )
					break;


				#ifdef NDEBUG
					cout << "EOW: subWord = " << subWord << endl;
				#endif


				// the next node marks a subWord.  Skip to the next character
				// and attempt to find next subWord.
				pWord++;
				iChar++;

				if (findSubWords(pWord, pSubWords) == true) {
					nSubWords++;
					break;		// break from while loop.  pWord is composed of subWords
				}
				else {			// wrong subWord attempted.  Retry same char in pWord
					pWord--;
					nSubWords--;
					iChar--;
				}
			}
		}

		pWord++;			// then increment pointer
	}

	// Found all letters in pWord in the Trie.  But we want pWord to be
	// composed of two or more whole words to pass our test.  Therefore,
	// we require:
	// 1. nSubWords > 1
	// 2. The current subWord has ended.
	#ifdef NDEBUG
		cout << "after while: subWord = " << subWord << ", nSubWords = " << nSubWords << endl;
	#endif

	*pSubWords = nSubWords;
	if ( endOfWordNextNode(pNode) ) {
		#ifdef NDEBUG
			cout << "found end of word" << endl;
		#endif

		return true;
	}
	else {
		#ifdef NDEBUG
			cout << "did not find end of word" << endl;
		#endif

		return false;
	}

}

// Recursively delete all TrieNodes in Trie.
void CTrieWordList::deleteTrie(TrieNode* pNode) {
	int i;

	if (pNode != NULL) {
		// Delete child nodes
		for (i = 0; i < N_BRANCHES; i++) {
			deleteTrie(pNode->pChild[i]);
		}

		// Delete node.
		delete(pNode);
		nodesDeleted++;
	}
}

// Search for pWord in Trie tree.  Return true if found.
bool CTrieWordList::findWord(char *pWord) {
	char c;
	int charIndex;
	TrieNode* pNode;

	c = *pWord;			// get first character
	pWord++;			// then increment pointer
	pNode = trieRoot;	// start word insertion at root

	// Increment characters until end of word.
	// By allowing only chars between 'a' and 'z',
	// we implicitly filter out the characters at the
	// end of the word: '\n' and '\0'
	while(c >= 'a' && c <= 'z') {
		// Calculate index of pChild node.
		// If c = 'a', charIndex = 0
		charIndex = c - 'a';

		//
		if (pNode->pChild[charIndex] == NULL) {
			return false;	// pWord does not exist in Trie
		}

		pNode = pNode->pChild[charIndex];	// follow node to next level down
		c = *pWord;			// get next character
		pWord++;			// then increment pointer
	}

	// The Trie has all letters in pWord.  But it is not really a word
	// unless the current pNode has pNode->pChild[END_OF_WORD] != NULL.
	if ( endOfWordNextNode(pNode) )
		return true;
	else
		return false;
}

// Find all words in Trie tree.  Return true if found.
void CTrieWordList::findWordsAll() {
	int i;
	int missingWords = 0;

	for (i = 0; i < nWords; i++) {
		if ( findWord(pWordList[i]) ) {
			#ifdef NDEBUG
						cout << "Found " << pWordList[i] << " in Trie" << endl;
			#endif
		}
		else
			missingWords++;
	}

	if (missingWords > 0)
		cout << missingWords << " words not found" << endl;
}

// Find all words in list which are composed of smaller words.
void CTrieWordList::findWordsComposedOfSmallerWords() {
	int i;
	int nSubWords;
	int nWordsComposedOfSmallerWords = 0;
	int len;
	int longestWordLen = 0;
	int secondLongestWordLen = 0;
	char longestWord[SEARCH_WORD_LEN];
	char secondLongestWord[SEARCH_WORD_LEN];

	strcpy(longestWord, "");
	strcpy(secondLongestWord, "");

	for (i = 0; i < nWords; i++) {
		findSubWords(pWordList[i], &nSubWords);
		if ( nSubWords == 2 ) {		// found at least 2 complete subwords
			#ifdef NDEBUG
						cout << pWordList[i] << " IS COMPOSED OF SMALLER WORDS." << endl;
			#endif

			nWordsComposedOfSmallerWords++;

			len = strlen(pWordList[i]);

			// Update the longest and 2nd longest words and lengths
			if (len > longestWordLen) {
				secondLongestWordLen = longestWordLen;
				longestWordLen = len;

				strcpy(secondLongestWord, longestWord);
				strcpy(longestWord, pWordList[i]);
			}
		}
	}

	cout << endl;
	cout << "Longest word composed of smaller words = " << longestWord << endl;
	cout << "Length = " << longestWordLen << endl;
	cout << "Second longest word composed of smaller words = " << secondLongestWord << endl;
	cout << "Length = " << secondLongestWordLen << endl;
	cout << endl;
	cout << "Total number of words composed of smaller words = " << nWordsComposedOfSmallerWords<< endl;
}

// Detect end of word in Trie tree
bool CTrieWordList::endOfWordNextNode(TrieNode* pNode) {
	if (pNode->pChild[END_OF_WORD] != NULL)
		return true;
	else
		return false;
}

void CTrieWordList::insertWordIntoTrie(char *pWord) {
	char c;
	int charIndex;
	int i;
	int len;
	int nChars = 0;		// number of characters in word
	TrieNode* pNode;

	pNode = trieRoot;	// start word insertion at root

	try {
		len = strlen(pWord);
		if (len == 0)
			throw 1;
	}
	catch (int e) {
		cerr << "String of p length.  Skip this string" << endl;
		return;
	}

	#ifdef NDEBUG
		cout << "insert word = " << pWord << ", len = " << len << endl;
	#endif

	for (i = 0; i < len; i++) {
		c = *pWord;			// get character

		// Calculate index of pChild node.
		// If c = 'a', charIndex = 0
		charIndex = c - 'a';

		// Letter does not exist in Trie.  Make new node.
		if (pNode->pChild[charIndex] == NULL) {
			pNode->pChild[charIndex] = makeTrieNode();
		}

		pNode = pNode->pChild[charIndex];	// follow node to next level down
		nChars++;
		pWord++;			// then increment pointer
	}

	// When we reach this point, either we have finished parsing word,
	// or we got a bad word with no characters between 'a' to 'z'.
	// If nChars > 0, we had a valid word.
	//
	// Thus if we had a valid word, in the last node (after the last
	// character in the word), set the END_OF_WORD branch to another
	// TrieNode.
	if (nChars > 0) {		// actual word inserted into Trie
		pNode->pChild[END_OF_WORD] = makeTrieNode();
	}
}


// Read word list from text file.  File contains one word
// per line, all lower case.
void CTrieWordList::readWordList(char *pFileName) {
	FILE *pFile = NULL;
	int i;
	int len = 0;
	char str[SEARCH_WORD_LEN];

	// Read word list text file, count the number of words (nWords),
	// and longest word (maxWordLen).  Use this information to allocate
	// memory for pWordList array.
	cout << "Opening " << pFileName << endl;

	pFile = fopen(pFileName, "r");
	if (pFile == NULL) {
		perror("Error opening word file");
	} else {

		// Read all words in file.
		//
		// Note: fgets() copies the newline character into each
		// string, and appends a NULL.  Therefore, if
		// word = "abc", the output string = "abc\n\0".
		//
		nWords = 0;
		maxWordLen = 0;
		while ( fgets(str, SEARCH_WORD_LEN, pFile) != NULL ) {

			len = strlen(str);

			// remove '\n' from string
			len = strlen(str);
			if (len > 0 && str[len-1] == '\n') {
				str[len-1] = '\0';
				len--;		// removed one character
			}

			maxWordLen = (len > maxWordLen) ? len : maxWordLen;

			if (len > 0)	// filter out empty lines.
				nWords++;
		}

		cout << "readWordList: Found " << nWords << " words in list";
		cout << " with max length = "<< maxWordLen << endl;

		// Allocate memory for pWordList
		allocateWordList(nWords, maxWordLen);

		rewind(pFile);		// start at beginning of file again

		// Read through word list file again, and copy words into pWordList
		for (i = 0; i < nWords; i++) {
			fgets(str, SEARCH_WORD_LEN, pFile);

			// remove '\n' from string
			len = strlen(str);
			if (len > 0 && str[len-1] == '\n') {
				str[len-1] = '\0';
				len--;		// removed one character
			}

			if (len > 0) {
				strcpy(pWordList[i], str);
				insertWordIntoTrie(pWordList[i]);
			}

			#ifdef NDEBUG
				cout << "word " << i << " = " << pWordList[i] << endl;
			#endif
		}

		fclose(pFile);
		cout << "readWordList: Wrote " << i << " words into Trie" << endl;
	}

}

// Allocate memory for new TrieNode struct, and initialize
// values.
struct TrieNode* CTrieWordList::makeTrieNode() {
	int i;
	TrieNode *pNode = NULL;

	try {
		pNode = new TrieNode;
	}
	catch (std::bad_alloc& ba) {
		cerr << "Error:  cannot allocate TrieNode " << ba.what() << endl;
	}
	if (pNode != NULL) {

		// initialize all pChild nodes to NULL
		for (i = 0; i < N_BRANCHES; i++)
			pNode->pChild[i] = NULL;

		nodesCreated++;
		return pNode;
	}
	else
	{
		return NULL;	// return NULL if new operation failed.
	}

}
