#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int arr[2048][2048];
string A, B;

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

int main() {
  int T;
  cin >> T;
  for (int tc = 0; tc < T; tc++) {
    cin >> A >> B;
    cout << LCS() << endl;
  }
  return 0;
}

