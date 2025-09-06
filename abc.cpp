#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // for atoi

using namespace std;

int main() {
    ifstream fin("input.txt");
    string s;
    if (fin >> s) {   // read a token
        int x = atoi(s.c_str());
        cout << "Number: " << x << endl;
    } else {
        cerr << "Failed to read input file" << endl;
    }
    return 0;
}
