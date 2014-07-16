/*
 * Main.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: Selwyn Yee
 *      Email:  selwyn_yee@yahoo.com
 */

#include "TrieWordList.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main( int argc, char* argv[])
{
	CTrieWordList trieWordList = CTrieWordList();
	char fileName[SEARCH_WORD_LEN];

	// Process command line argument(s)
	if (argc < 2) {
		cout << "Usage: TrieWordList fileName" << endl;
		cout << endl;
		cout << "Please enter a text file containing the word list."  << endl;
		exit(EXIT_SUCCESS);
	} else {
		strcpy(fileName, argv[1]);
	}
	trieWordList.readWordList(fileName);

#ifdef NDEBUG
	trieWordList.findWordsAll();
#endif

	trieWordList.findWordsComposedOfSmallerWords();
}



