#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

typedef struct PAIR {
	string word1;
	string word2;
} PAIR;

/*
this is their stuff... I don't really understand why they are using global variables, but
I think its because if you try to move a the arr declaration in to LCS it seg faults, which
may be because it is too big? I don't know.
*/ 
string A, B;
int arr[2048][2048];

int LCS() {
	int m = A.length(), n = B.length();
	
	int i, j;
	for (i = 0; i <= m; i++) arr[i][0] = 0;
	for (j = 0; j <= n; j++) arr[0][j] = 0;
	
	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
			arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
			if (A[i-1] == B[j-1]) arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
		}
	}
	
	return arr[m][n];
	
}

inline string cut(string toCut, int loc){
	return toCut.substr(loc) + toCut.substr(0,loc) ;
}


int parseInput(PAIR **words){
    istream *inf;
    inf = &cin;
    int num;
    *inf >> num;
	PAIR *wordSet = new PAIR[num];
	for (int i = 0; i < num; i++)
		*inf >> wordSet[i].word1 >> wordSet[i].word2;
	
	*words = wordSet;
	return num;
}

int CLCS(string longWord, string shortWord){

	if (longWord.length() < shortWord.length()){
		string hold = shortWord;
		shortWord = longWord;
		longWord = hold;
	}
	
	int currentLCS = -1;
	for (int i = 0; i < shortWord.length(); i++){
		A = longWord;
		B = cut(shortWord,i);
		int nextLCS = LCS();
		if (nextLCS > currentLCS){
			currentLCS = nextLCS;
		}
	}
	
	return currentLCS;

}


int main(int argc, const char* argv[]) {
	
	PAIR *words;
	int pfds[1];
	
	int num = parseInput(&words);
	
	for (int i = 0; i < num; i++){
		cout << CLCS(words[i].word1, words[i].word2) << endl;
	}
	
}

