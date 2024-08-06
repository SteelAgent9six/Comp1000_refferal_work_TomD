#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

class SearchResult {
public:
    int LN;
    int WN;
    string MT;

    SearchResult(int line, int word, const string& text)
        : LN(line), WN(word), MT(text) {}
};

class FileSearcher {
private:
    string fileName;
    string searchTerm;
    bool isRegex;
    vector<SearchResult> results;

    void readContents(string& Contents);
    void searchContents(const string& Contents);
    void saveResultsToCSV(float hit);

public:
    FileSearcher(const string& file, const string& term, bool regex)
        : fileName(file), searchTerm(term), isRegex(regex) {}

    void performSearch();
    void displayResults();
    void displayStatistics(int TW);
};

void FileSearcher::readContents(string& Contents) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        exit(EXIT_FAILURE);
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    Contents = buffer.str();
    cout << "This file reads:" << Contents << endl;
    file.close();
}

void FileSearcher::searchContents(const string& Contents) {
    istringstream stream(Contents);
    string line;
    int LN = 0;
    int TW = 0;
    regex re(searchTerm);
    while (getline(stream, line)) {
        ++LN;
        istringstream lineStream(line);
        string word;
        int WN = 0;
        while (lineStream >> word) {
            ++WN;
            ++TW;
            if ((isRegex && regex_search(word, re)) || (!isRegex && word.find(searchTerm) != string::npos)) {
                results.emplace_back(LN, WN, word);
            }
        }
    }
    float hit = results.size() * 100.0 / TW;
    displayStatistics(TW);
    saveResultsToCSV(hit);
}

void FileSearcher::performSearch() {
    string Contents;
    readContents(Contents);
    searchContents(Contents);
    displayResults();
}

void FileSearcher::displayResults() {
    for (const auto& result : results) {
        cout << "Line " << result.LN << ", Word " << result.WN << ": " << result.MT << endl;
    }
}

void FileSearcher::displayStatistics(int TW) {
    cout << "Total words: " << TW << endl;
    cout << "Search hits: " << results.size() << endl;
    cout << "Hit percentage: " << (results.size() * 100.0 / TW) << "%" << endl;
}

void FileSearcher::saveResultsToCSV(float hit) {
    ofstream csvFile("results.csv", ios::app);
    if (!csvFile.is_open()) {
        cerr << "Error: Could not open results.csv for writing" << endl;
        exit(EXIT_FAILURE);
    }
    csvFile << fileName << "," << searchTerm << "," << hit << endl;
    csvFile.close();
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        cerr << "Usage: TaskA <filename> <search term> [-regex]" << endl;
        return EXIT_FAILURE;
    }

    string fileName(argv[1]);
    string searchTerm(argv[2]);

    bool isRegex = false;
    if (argc == 4 && string(argv[3]) == "-regex") {
        isRegex = true;
    }

    FileSearcher searcher(fileName, searchTerm, isRegex);
    searcher.performSearch();

    return EXIT_SUCCESS;
}
