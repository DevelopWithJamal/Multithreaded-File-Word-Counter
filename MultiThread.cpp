#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <sstream>
#include <mutex>

using namespace std;

mutex mtx;
int totalWords = 0;

// Function to count words in a file
void countWords(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    int localCount = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string word;

        while (ss >> word) {
            localCount++;
        }
    }

    // Safe update using lock_guard
    lock_guard<mutex> lock(mtx);
    totalWords += localCount;

    cout << "Processed " << filename << " | Words: " << localCount << endl;
}

int main() {
    vector<string> files = {
        "file1.txt",
        "file2.txt",
        "file3.txt"
    };

    vector<thread> threads;

    // Create threads
    for (const string& file : files) {
        threads.emplace_back(countWords, file);
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    cout << "\nTotal words across all files: " << totalWords << endl;

    return 0;
}