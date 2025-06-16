#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

unordered_map<char, size_t> buildBadCharTable(const string& pattern) {
    unordered_map<char, size_t> badChar;
    size_t m = pattern.length();
    for (size_t i = 0; i < m; i++) {
        badChar[pattern[i]] = i;
    }
    return badChar;
}

vector<size_t> buildGoodSuffixTable(const string& pattern) {
    size_t m = pattern.size();
    vector<size_t> shift(m + 1, m); 
    vector<size_t> bpos(m + 1, 0);  

    size_t i = m;
    size_t j = m + 1;
    bpos[i] = j;
    while (i > 0) {
        while (j <= m && pattern[i - 1] != pattern[j - 1]) {
            if (shift[j] == m) {
                shift[j] = j - i;
            }
            j = bpos[j];
        }
        i--;
        j--;
        bpos[i] = j;
    }

    j = bpos[0];
    for (i = 0; i <= m; i++) {
        if (shift[i] == m) {
            shift[i] = j;
        }
        if (i == j) {
            j = bpos[j];
        }
    }
    return shift;
}

vector<size_t> boyerMooreSearch(const string& text, const string& pattern) {
    vector<size_t> matches;
    size_t n = text.length();
    size_t m = pattern.length();

    if (m == 0 || n == 0 || m > n) {
        return matches;
    }

    unordered_map<char, size_t> badChar = buildBadCharTable(pattern);
    vector<size_t> goodSuffix = buildGoodSuffixTable(pattern);

    size_t s = 0; 
    while (s <= n - m) {
        int j = m - 1; 

        while (j >= 0 && pattern[j] == text[s + j]) {
            j--;
        }

        if (j < 0) {
            matches.push_back(s);
            s += goodSuffix[0];
        }
        else {
            char bc = text[s + j];
            size_t badCharShift = (badChar.find(bc) != badChar.end())
                ? max(static_cast<size_t>(1), j - badChar.at(bc))
                : max(static_cast<size_t>(1), static_cast<size_t>(j) + 1);

            size_t goodSuffixShift = goodSuffix[j + 1];
            s += max(badCharShift, goodSuffixShift);
        }
    }
    return matches;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string text = "ялюблюбугивуги";
    string pattern = "гивуг";

    vector<size_t> matches = boyerMooreSearch(text, pattern);

    if (matches.empty()) {
        cout << "Подстрока не найдена" << endl;
    }
    else {
        cout << "Подстрока найдена на позициях: ";
        for (size_t pos : matches) {
            cout << pos << " ";
        }
        cout << endl;
    }
    return 0;
}