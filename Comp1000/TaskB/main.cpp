#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

void openFile(const std::string& filename, std::string& content) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    content = oss.str();
}

void search(const std::string& content, const std::string& searchTerm, std::vector<std::string>& results, int& hitCount) {
    results.clear();
    hitCount = 0;
    std::istringstream iss(content);
    std::string line;
    int lineNumber = 1;
    while (std::getline(iss, line)) {
        if (line.find(searchTerm) != std::string::npos) {
            results.push_back("Line " + std::to_string(lineNumber) + ": " + line);
            ++hitCount;
        }
        ++lineNumber;
    }
}

void saveResults(const std::string& filename, const std::string& searchTerm, int hitCount) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }
    file << "FileName,SearchTerm,HitCount\n";
    file << "example.txt," << searchTerm << "," << hitCount << "\n";
}

int main() {
    std::string filename;
    std::string content;
    std::string searchTerm;
    std::vector<std::string> results;
    int hitCount;

    std::cout << "Enter file name to open: ";
    std::getline(std::cin, filename);

    openFile(filename, content);

    std::cout << "Enter search term: ";
    std::getline(std::cin, searchTerm);

    search(content, searchTerm, results, hitCount);

    std::cout << "Search results:\n";
    for (const auto& result : results) {
        std::cout << result << std::endl;
    }
    std::cout << "Number of hits: " << hitCount << std::endl;

    std::string saveFileName;
    std::cout << "Enter CSV file name to save results: ";
    std::getline(std::cin, saveFileName);

    saveResults(saveFileName, searchTerm, hitCount);

    return 0;
}
