//Rex Kirshner, rbk, 05580888
//Taylor Savage, tjsavage, 05575939
//Matthew Alexander, 

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

typedef struct PAIR {
	string word1;
	string word2;
} PAIR;

/* This
*/
typedef struct node {
    int row;
    int column;
} node;

typedef struct Path {
    int left_bounds[2048]; //TJS: This contains, for each column, the lowest row value on the path.  This will make it easier to use the path as a bound, and can be calculated easily when calculating the path.
    int right_bounds[2048]; //TJS: This contains, for each column, the highest row value on the path.
    int value;
    int rows;
} Path;
         
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
/*
END the stuff they gave us
*/

inline string cut(string toCut, int loc){
	return toCut.substr(loc) + toCut.substr(0,loc) ;
}

void resetGrid(int rows, int cols) {
    for (int i = 0; i <= rows; i++) {
        for( int j = 0; j <= cols; j++) {
            arr[i][j] = 0;
        }
    }
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

void printPath(Path &p) {
    for (int row = 0; row <= p.rows; row++) {
        printf("left bound: %d, %d     right bound: %d, %d\n", p.left_bounds[row], row, p.right_bounds[row], row);
    }
    printf("Path value: %d\n", p.value);
}

void printGrid(string A, string B, int low_row, int low_col, int high_row, int high_col) {
    printf("  ");
    for(int i = low_col; i <= high_col; i++) printf(" %c ", B[i]);
    printf("\n");
    for(int i = low_row; i <= high_row; i++) {
        printf("\n");
        for(int j = low_col; j <= high_col; j++) {
            printf(" %d ", arr[i][j]);
        }
        printf("\n%c", A[i]);
    }
}

void resetPath(Path &p) {
    memset(&(p.left_bounds), -1, 2048 * sizeof(int));
    memset(&(p.right_bounds), -1, 2048 * sizeof(int));
    p.value = -1;
}

void singleShortestPath(string A, string B, int m, Path &left_path, Path &right_path, Path &dest) {
    
    //printf("\nLooking for path starting at %d\n", m);
    //printf("Left bound path: \n");
    //printPath(left_path);
    //printf("\nRight bound path:\n");
    //printPath(right_path);
    
    //resetGrid(A.length(), B.length());
    
    for (int i = 0; i <= A.length(); i++) arr[i][m] = 0;
    for (int j = m; j <= m + B.length() / 2; j++) arr[0][j] = 0;
    
    for (int row = 1; row <= A.length(); row++) {
        int left_bound = max(left_path.left_bounds[row], m + 1);
        int right_bound = min(right_path.right_bounds[row], m + (int)(B.length() / 2));
        for (int col = left_bound; col <= right_bound; col++) {
            if (left_path.left_bounds[row] <= col - 1 && right_path.right_bounds[row - 1] >= col) { //Both up and left are in bounds
                arr[row][col] = max(arr[row - 1][col], arr[row][col - 1]);
            } else if (left_path.left_bounds[row] <= col - 1) { //The node to the left is in-bounds
                arr[row][col] = arr[row][col - 1];
            } else if (right_path.right_bounds[row - 1] >= col ) { //The node up is in-bounds
                arr[row][col] = arr[row - 1][col];
            } else {
                //Shit... we have nothing to compare to except the diagonal
                //Setting a sentinel to check if we fucked up.
                arr[row][col] = -2;
            }
            if (A[row - 1] == B[col - 1] && left_path.left_bounds[row - 1] <= col - 1 && right_path.right_bounds[row - 1] >= col - 1) {
                arr[row][col] = max(arr[row][col], arr[row - 1][col - 1] + 1);
            }
            if (arr[row][col] == -2) {
                printf("Messed up creating grid... nothing in bounds!\n");
                printf("Row: %d Col: %d\n", row, col);
                exit(1);
            }
        }
    }
    //printGrid(A, B, 0, 0, A.length(), B.length());
    
    int col = m + B.length() / 2;
    int row = A.length();
    //resetPath(dest);
    dest.left_bounds[row] = col;
    dest.right_bounds[row] = col;
    dest.value = 0;
    dest.rows = row;
    while(col > m|| row > 0) {
        int curr = arr[row][col];
        if (row >= 1 && col >= m + 1 &&  arr[row - 1][col - 1] < curr && left_path.left_bounds[row - 1] <= col - 1 && right_path.right_bounds[row - 1] >= col - 1 && A[row - 1] == B[col - 1]) { //Go diagonal
            dest.left_bounds[row] = col;
            dest.value++;
            dest.right_bounds[row - 1] = col - 1;
            row--;
            col--;
        }  else if (col >= m + 1 && left_path.left_bounds[row] <= col - 1 && arr[row][col - 1] == arr[row][col] ) { //go left
            dest.left_bounds[row] = col - 1;
            col--;
        } else if (row >= 1 && arr[row - 1][col] == arr[row][col] && right_path.right_bounds[row - 1] >= col) { //go up
            dest.left_bounds[row] = col;
            dest.right_bounds[row - 1] = col;
            row--;
        } else {
            printf("Path out of bounds!\n");
            exit(1);
            break;
        }
        //printf("Col: %d, row: %d \n", col, row);
    }
    dest.left_bounds[0] = m;
    /*
    if (col == m + 1 && row == 0) {
        dest.left_bounds[0] = m;
    }
    */
    
    //cout << "final path: " << endl;
    //printPath(dest);
}

void fullShortestPaths(string A, string B, Path &left_dest, Path &right_dest) {
    int m = A.length();
    int n = B.length() / 2;
    
    for (int i = 0; i <= m; i++) arr[i][0] = 0;
    for (int j = 0; j <= n; j++) arr[0][j] = 0;
    
    for (int i = 1; i <=m; i++) {
        for(int j = 1; j <= n; j++) {
            arr[i][j] = max(arr[i-1][j], arr[i][j - 1]);
            if (A[i-1] == B[j-1]) {
                arr[i][j] = max(arr[i][j], arr[i-1][j-1] + 1);
            }
        }
    }
    
    //printGrid(A, B, 0, 0, A.length(), B.length());
    
    int col = B.length() / 2;
    int row = A.length();
    //resetPath(left_dest);
    //resetPath(right_dest);
    left_dest.left_bounds[row] = col; right_dest.left_bounds[row] = col + n;
    left_dest.right_bounds[row] = col; right_dest.right_bounds[row] = col + n;
    left_dest.value = 0; right_dest.value = 0;
    left_dest.rows = row; right_dest.rows = row;
    while(col > 0 || row > 0) {
        int curr = arr[row][col];
        if (row >=1 && col >= 1 && arr[row-1][col-1] < curr && A[row - 1] == B[col - 1] ) { //Go diagonal
            left_dest.left_bounds[row] = col; right_dest.left_bounds[row] = col +n;
            left_dest.value++; right_dest.value++;
            left_dest.right_bounds[row-1] = col - 1; right_dest.right_bounds[row-1] = col + n - 1;
            row--;
            col--;
        }  else if (col >= 1 && arr[row][col - 1] == arr[row][col]) { //Go left
            left_dest.left_bounds[row] = col - 1; right_dest.left_bounds[row] = col + n - 1;
            col--;
        } else if (row >= 1 && arr[row - 1][col] == arr[row][col]) { //Go up
            left_dest.left_bounds[row] = col; right_dest.left_bounds[row] = col + n;
            left_dest.right_bounds[row-1] = col; right_dest.right_bounds[row - 1] = col + n;
            row--;
        } else {
            printf("Shit, there's no path in bounds on the original paths.\n");
            printf("Row: %d Col: %d\n", row, col);
            exit(1);
        }
    }
    left_dest.left_bounds[0] = 0; right_dest.left_bounds[0] = B.length() / 2;
    
    //printGrid(A, B, 0, 0, A.length(), B.length());
    //printPath(left_dest);
    //printPath(right_dest);
}



void findShortestPaths(string A, string B, Path *p, int l, int r) {
    if (r - l <=1) return;
    int m = (l + r) / 2;
    //resetGrid(A.length(), B.length());
    //cout << "ssp: " << m << " " << l << " " << r << endl;
    singleShortestPath(A, B, m, p[l], p[r], p[m]);
    findShortestPaths(A, B, p, l, m);
    findShortestPaths(A, B, p, m, r);
}



int CLCS(const string A, const string B){
    string doubledB = B + B;
    Path *p = (Path *)malloc((B.length() + 1) * sizeof(Path));
    fullShortestPaths(A, doubledB, p[0], p[B.length()]);
    findShortestPaths(A, doubledB, p, 0, B.length());
    
    //now we need to find the best valued path
    int best_value = 0;
    for(int i = 0; i <= B.length(); i++) {
        //cout << "Comparing: " << A << " " << cut(B, i) << endl;
        //printf("Path Value: %d\n", p[i].value);
        if (p[i].value > best_value) {
            best_value = p[i].value;
        }
    }
    return best_value;
}




int main(int argc, const char* argv[]) {
	PAIR *words;
	
	int num = parseInput(&words);

	for (int i = 0; i < num; i++){
	    //cout << words[i].word1 << " " << words[i].word2 << endl;
		cout << CLCS(words[i].word1, words[i].word2) << endl;
	}
}

