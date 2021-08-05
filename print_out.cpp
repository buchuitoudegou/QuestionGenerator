#include <fstream>
#include <string>
#include <iterator>
#include <iostream>

using namespace std;

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    exit(0);
  }
  ifstream ifs(argv[1]);
  istream_iterator<char> it(ifs), eof;
  string ss(it, eof);
  cout << ss << endl;
  ifs.close();
  return 0;
}