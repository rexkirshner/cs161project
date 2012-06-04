#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  ifstream in;
  in.open(argv[1]);
  vector<int> expected;
  while (in.good()) {
    int next;
    in >> next;
    if (!in.good()) break;
    expected.push_back(next);
  }
  in.close();
  vector<int> actual;
  while (cin.good()) {
    int next;
    cin >> next;
    if (!cin.good()) break;
    actual.push_back(next);
  }
  if (expected.size() != actual.size()) {
    cout << "Warning: Unexpected number of outputs: " << actual.size() << endl;
  }
  int incorrect = 0;
  for (int i = 0; i < expected.size(); i++) {
    if (i == actual.size()) break;
    if (actual[i] != expected[i]) {
      cerr << "Test case #" << i + 1 << " incorrect. Expected: " << expected[i] << "; actual: " << actual[i] << endl;
      incorrect++;
    }
  }
  if (actual.size() < expected.size()) {
    cerr << "Test cases #" << actual.size() + 1 << "-" << expected.size() << " absent." << endl;
    incorrect += expected.size() - actual.size();
  }
  if (actual.size() > expected.size()) {
    cerr << actual.size() - expected.size() << " extra lines of output (no penalty assessed)" << endl;
  }
  cout << "Total incorrect cases: " << incorrect << endl;
  return 0;
}

