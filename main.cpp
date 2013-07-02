#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

struct Lower {
    bool operator() (const string& s1, const string& s2) {
        string one, two;
        auto lower = [](char c) {return tolower(c);};
        transform(s1.begin(), s1.end(), back_inserter(one), lower);
        transform(s2.begin(), s2.end(), back_inserter(two), lower);

        if (one == two) {
            return s1 < s2;
        }
        else return one < two;
    }
};

int longestStringLength = 0;
void handleString(string& line, int& lineNum, map<string, map<int, int>, Lower>& words) {
    auto notDesired = [](char c) {return !(isalpha(c) || isspace(c) || c == '\'');};
    replace_if(line.begin(), line.end(), notDesired, ' ');

    stringstream ss(line);
    while (ss.good()) {
        string word;
        ss >> word;

        if (ss.good()) {
            if (word.length() > longestStringLength)
            longestStringLength = word.length();

            map<int, int> vals = words[word];
            ++vals[lineNum];
            words[word] = vals;
        }
    }
    lineNum++;
}

const int WRAPAT = 9;

int main(int argc, char *argv[])
{
    map<string, map<int, int>, Lower> words;
    int lineNum = 1;

    if (argc > 1) {
        ifstream f(argv[1]);
        if (f.good()) {
            string line;
            while (getline(f, line)) {
                handleString(line, lineNum, words);
            }
        }
    }
    else {
        cout << "No file supplied as command line argument. Enter a string to parse." << endl;
        string s;
        getline(cin, s);
        handleString(s, lineNum, words);
    }

    for (auto x : words) { // For each word
        cout << setw(longestStringLength+1) << left;
        cout << x.first << ": ";
        int printedPairs = 0;
        bool printedFirst = false;
        for (auto y : x.second) { // For each (linenumber : occurrences) pair.
            if (printedFirst == false) {
                cout << y.first << ":" << y.second;
                printedFirst = true;
            }
            else {
                if (printedPairs == WRAPAT) {
                    cout << "," << endl;
                    for (int i = 0; i < longestStringLength+1; ++i)
                        cout << " ";
                    cout << ": " << y.first << ":" << y.second;
                    printedPairs = 0;
                }
                else {
                    cout << ", " << y.first << ":" << y.second;
                }
            }
            printedPairs++;
        }
        cout << endl;
    }
    return 0;
}
