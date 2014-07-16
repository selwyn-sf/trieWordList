/*
 * TrieWordList.h
 *
 *  Created on: Feb 20, 2014
 *      Author: Selwyn Yee
 *      Email:  selwyn_yee@yahoo.com
 */

// For debug, #define NDEBUG
// For no debug, #undef NDEBUG
#undef NDEBUG
//#define NDEBUG
#define SEARCH_WORD_LEN	100			// maximum search word length

// N_BRANCHES = number of letters in alphabet + 1.  Branches 0 to 25
// are for characters 'a' to 'z'.  The 26th branch is to mark end of words.
#define N_BRANCHES		(26 + 1)
#define END_OF_WORD		26			// index for the End-Of-Word branch of TrieNode

// TrieNode data structure.  Unlike a binary tree node, a TrieNode
// contains 27 children, one for each letter of alphabet, plus one
// more to mark the end of a word.
//
// TrieNode T: 	pChild[0] = pointer to 'a' TrieNode = NULL
// 				pChild[1] = pointer to 'b' TrieNode = NULL
//				...
//				...
//				pChild[25] = pointer to 'z' TrieNode = NULL
//				pChild[26] = pointer to 'EndOfWord' TrieNode = NULL
//
// Note: the words or letters are not stored in a TrieNode struct.
// Only the POSITION OF THE LETTER is stored.
//
// Example: let T0 = root node of Trie tree.  To insert the
// word "cab":
//
// 1. Create T1 = new TrieNode.
// 2. Set T0.pChild[2] = T1 ('c' pointer in T0 points to T1)
// 3. Create T2 = new TrieNode.
// 4. Set T1.pChild[0] = T2 ('a' pointer in T1 points to T2)
// 5. Create T3 = new TrieNode.
// 6. Set T2.pChild[1] = T3 ('b' pointer in T2 points to T3)
// 7. Create T4 = new TrieNode.
// 8. Set T3.pChild[26] = T4 ('EndOfWord' pointer in T3 points to T4).
//
// To summarize, the word "cab" is represented by:
//
// The T0 'c' pointer points to T1.
// The T1 'a' pointer points to T2.
// The T2 'b' pointer points to T3.
// The T3 'EndOfWord' pointer points to T4.
//
// The root node T0 holds the beginnings of all words in the list.
// Therefore if you want to add a word beginning with 'j', then
// the pChild node for 'j' will be set to non-NULL.

struct TrieNode {
	// child = array children nodes, one for each letter of alphabet,
	// plus one more to mark end of words.
	// Initialize all child nodes to NULL.
	TrieNode* pChild[N_BRANCHES];
};


class CTrieWordList {
private:

	int maxWordLen;		// maximum word length found in word list.
	int nodesCreated;	// number of TrieNodes created.  Track for memory leaks
	int nodesDeleted;	// number of TrieNodes deleted.  Track for memory leaks
	int nWords;				// number of words in Trie
	char **pWordList;		// pointer to array of strings holding word list
	TrieNode* trieRoot;		// root of Trie tree

public:
	CTrieWordList();
	~CTrieWordList();

	// Find all words in list which are composed of smaller words.
	void findWordsComposedOfSmallerWords();

	// Find all words in Trie tree.  Return true if found.
	void findWordsAll();

	// Read word list from text file.  File contains one word
	// per line, all lower case.
	void readWordList(char *pFileName);

private:

	void allocateWordList(int nwords, int len);

	// Test whether pWord is composed of other words in the list.
	bool findSubWords(char *pWord, int *pSubWords);

	// Search for pWord in Trie tree.  Return true if found.
	void deleteTrie(TrieNode* pNode);

	// Traverse Trie, find prefix or subWord from pWord.  Write result
	// into pPrefix.
	bool findPrefixOrSubword(char *pWord, char *pPrefix);

	bool findWord(char *pWord);

	// At a given TrieNode (pNode) containing prefix, find any words which have
	// this prefix.  Return them in pWordFromPrefix.
	bool findWordsFromPrefix(TrieNode* pNode, char* pPrefix, char* pWordFromPrefix, int branchNum);
	bool endOfWordNextNode(TrieNode* pNode);
	void insertWordIntoTrie(char *pWord);

	// Allocate memory for new TrieNode struct, and initialize
	// values.
	struct TrieNode* makeTrieNode();
};




