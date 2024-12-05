#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <set>
#include <unordered_set>
using namespace std;

// Clockwise dimensions for movement
const int dxClockwise[] = { 0, 1, 0, -1 }; // x-coordinates change in clockwise direction
const int dyClockwise[] = { 1, 0, -1, 0 }; // y-coordinates change in clockwise direction

// Counter-clockwise dimensions for movement
const int dxCounterClockwise[] = { 0, -1, 0, 1 }; // x-coordinates change in counter-clockwise direction
const int dyCounterClockwise[] = { -1, 0, 1, 0 }; // y-coordinates change in counter-clockwise direction

// Reads the board file and populates the matrix and words
// Returns false if the file format is invalid
bool readBoardFile(string filename, vector<vector<char>>& matrix, vector<string>& words) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    int n;
    file >> n;

    matrix.resize(n, vector<char>(n));
    for (int i = 0; i < n; i++) {
        string row;
        file >> row;
        if (row.size() != n || any_of(row.begin(), row.end(), [](char ch) { return !isupper(ch); })) {
            cout << "Error: Matrix is not in correct format!" << endl;
            return false;
        }
        for (int j = 0; j < n; j++) {
            matrix[i][j] = row[j];
        }
    }

    int wordCount;
    file >> wordCount;
    if (wordCount <= 0) {
        cout << "Error: Invalid word count in file." << endl;
        return false;
    }

    words.resize(wordCount);
    for (int i = 0; i < wordCount; i++) {
        file >> words[i];
    }

    // Debugging output
    /*
    cout << "Matrix Size: " << n << "x" << n << endl;
    cout << "Matrix:" << endl;
    for (const auto& row : matrix) {
        for (char ch : row) {
            cout << ch << " ";
        }
        cout << endl;
    }
    cout << "Words to Search:" << endl;
    for (const auto& word : words) {
        cout << word << endl;
    }
    */
    return true;
}

// Checks if a given position is within bounds and hasn't been visited
bool isSafe(int x, int y, int matrixSize, unordered_set<int>& visited, int uniqueKey) {
    return (x >= 0 && x < matrixSize&& y >= 0 && y < matrixSize&& visited.find(uniqueKey) == visited.end());
}

// Recursive function for spiral search with backtracking
bool spiralSearchBacktracking(const vector<vector<char>>& matrix, const string& word, int wordIdx,
    int x, int y, unordered_set<int>& visited, const int dx[], const int dy[]) {
    if (wordIdx == word.size()) {
        return true;
    }

    int matrixSize = matrix.size();
    int uniqueKey = x * matrixSize + y;

    if (!isSafe(x, y, matrixSize, visited, uniqueKey) || matrix[x][y] != word[wordIdx]) {
        return false;
    }

    visited.insert(uniqueKey);

    for (int dir = 0; dir < 4; dir++) {
        int nextX = x + dx[dir];
        int nextY = y + dy[dir];
        if (spiralSearchBacktracking(matrix, word, wordIdx + 1, nextX, nextY, visited, dx, dy)) {
            return true;
        }
    }

    visited.erase(uniqueKey);
    return false;
}

// Routes the search through clockwise and counter-clockwise paths
bool routerSearch(const vector<vector<char>>& matrix, const string& word, int x, int y) {
    // Debugging output
    /*
    cout << "\n--- Router invoked for \"" << word << "\" starting at (" << x << ", " << y << ") ---\n";
    */

    unordered_set<int> visitedClockwise;
    if (spiralSearchBacktracking(matrix, word, 0, x, y, visitedClockwise, dxClockwise, dyClockwise)) {
        // Debugging output
        /*
        cout << "Word \"" << word << "\" found clockwise starting at (" << x << ", " << y << ")\n";
        */
        return true;
    }
    /*
    else {
        cout << "Clockwise search failed for \"" << word << "\" starting at (" << x << ", " << y << ")\n";
    }
    */

    unordered_set<int> visitedCounterClockwise;
    if (spiralSearchBacktracking(matrix, word, 0, x, y, visitedCounterClockwise, dxCounterClockwise, dyCounterClockwise)) {
        // Debugging output
        /*
        cout << "Word \"" << word << "\" found counter-clockwise starting at (" << x << ", " << y << ")\n";
        */
        return true;
    }
    /*
    else {
        cout << "Counter-clockwise search failed for \"" << word << "\" starting at (" << x << ", " << y << ")\n";
    }
    */

    return false;
}

// Finds all words in the matrix and prints them
void findWords(const vector<vector<char>>& matrix, const vector<string>& words) {
    set<string> foundWords;

    for (const string& word : words) {
        // Debugging output
        /*
        cout << "Searching for word: \"" << word << "\"" << endl;
        */
        bool wordFound = false;
        for (int i = 0; i < matrix.size() && !wordFound; i++) {
            for (int j = 0; j < matrix[i].size() && !wordFound; j++) {
                if (matrix[i][j] == word[0]) {
                    if (routerSearch(matrix, word, i, j)) {
                        foundWords.insert(word);
                        wordFound = true;
                    }
                }
            }
        }
    }

    cout << foundWords.size() << " Words are Found: ";
    for (const string& word : foundWords) {
        cout << word << " ";
    }
    cout << endl;
}

// Main function
int main() {
    vector<vector<char>> matrix;
    vector<string> words;
    string filename;

    cout << "Enter the name of the file: ";
    cin >> filename;

    if (!readBoardFile(filename, matrix, words)) {
        return 0;
    }

    findWords(matrix, words);

    return 0;
}
