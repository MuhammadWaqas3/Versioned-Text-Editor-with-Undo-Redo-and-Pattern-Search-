#include "AdvancedSearch.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

// ==================== AdvancedSearchEngine Implementation ====================

long long AdvancedSearchEngine::power(long long base, int exp) const {
    long long result = 1;
    for (int i = 0; i < exp; i++) {
        result = (result * base) % PRIME;
    }
    return result;
}

long long AdvancedSearchEngine::calculateHash(const string& str, int length) const {
    long long hash = 0;
    for (int i = 0; i < length && i < str.length(); i++) {
        hash = (hash * BASE + static_cast<unsigned char>(str[i])) % PRIME;
    }
    return hash;
}

long long AdvancedSearchEngine::recalculateHash(long long oldHash, char oldChar, char newChar,
                                               long long pow, int length) const {
    long long newHash = (oldHash - (static_cast<unsigned char>(oldChar) * pow) % PRIME + PRIME) % PRIME;
    newHash = (newHash * BASE + static_cast<unsigned char>(newChar)) % PRIME;
    return newHash;
}

string AdvancedSearchEngine::toLowerCase(const string& str) const {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool AdvancedSearchEngine::isWordBoundary(char c) const {
    return !isalnum(c) && c != '_';
}

void AdvancedSearchEngine::getLineAndColumn(const string& text, size_t pos,
                                           size_t& line, size_t& col) const {
    line = 1;
    col = 1;
    
    for (size_t i = 0; i < pos && i < text.length(); i++) {
        if (text[i] == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
    }
}

string AdvancedSearchEngine::getContext(const string& text, size_t pos, int contextLines) const {
    // Find start of context (go back contextLines lines)
    size_t start = pos;
    int linesBack = 0;
    
    while (start > 0 && linesBack < contextLines) {
        start--;
        if (text[start] == '\n') {
            linesBack++;
        }
    }
    
    // Find end of context (go forward contextLines lines)
    size_t end = pos;
    int linesForward = 0;
    
    while (end < text.length() && linesForward < contextLines) {
        if (text[end] == '\n') {
            linesForward++;
        }
        end++;
    }
    
    return text.substr(start, end - start);
}

vector<size_t> AdvancedSearchEngine::rollingHashSearch(const string& text,
                                                       const string& pattern) const {
    vector<size_t> positions;
    
    if (pattern.empty() || pattern.length() > text.length()) {
        return positions;
    }
    // ... Rabin-Karp implementation
    int patternLen = pattern.length();
    long long patternHash = calculateHash(pattern, patternLen);
    long long textHash = calculateHash(text, patternLen);
    long long pow = power(BASE, patternLen - 1);
    
    for (size_t i = 0; i <= text.length() - patternLen; i++) {
        if (textHash == patternHash) {
            if (text.substr(i, patternLen) == pattern) {
                positions.push_back(i);
            }
        }
        
        if (i < text.length() - patternLen) {
            textHash = recalculateHash(textHash, text[i], text[i + patternLen], pow, patternLen);
        }
    }
    
    return positions;
}

vector<int> AdvancedSearchEngine::computeLPSArray(const string& pattern) const {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;
    
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    
    return lps;
}

vector<size_t> AdvancedSearchEngine::kmpSearch(const string& text, const string& pattern) const {
    vector<size_t> positions;
    
    if (pattern.empty() || pattern.length() > text.length()) {
        return positions;
    }
    
    vector<int> lps = computeLPSArray(pattern);
    int n = text.length();
    int m = pattern.length();
    int i = 0;  // index for text
    int j = 0;  // index for pattern
    
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        
        if (j == m) {
            positions.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    return positions;
}

vector<int> AdvancedSearchEngine::computeBadCharTable(const string& pattern) const {
    vector<int> badChar(256, -1);
    
    for (int i = 0; i < pattern.length(); i++) {
        badChar[(unsigned char)pattern[i]] = i;
    }
    
    return badChar;
}

vector<size_t> AdvancedSearchEngine::boyerMooreSearch(const string& text,
                                                      const string& pattern) const {
    vector<size_t> positions;
    
    if (pattern.empty() || pattern.length() > text.length()) {
        return positions;
    }
    
    vector<int> badChar = computeBadCharTable(pattern);
    int n = text.length();
    int m = pattern.length();
    int s = 0;  // shift
    
    while (s <= n - m) {
        int j = m - 1;
        
        while (j >= 0 && pattern[j] == text[s + j]) {
            j--;
        }
        
        if (j < 0) {
            positions.push_back(s);
            s += (s + m < n) ? m - badChar[(unsigned char)text[s + m]] : 1;
        } else {
            s += max(1, j - badChar[(unsigned char)text[s + j]]);
        }
    }
    
    return positions;
}

vector<SearchResult> AdvancedSearchEngine::search(const string& text, const string& pattern,
                                                  const SearchOptions& options) const {
    vector<SearchResult> results;
    
    if (pattern.empty()) {
        return results;
    }
    
    string searchText = text;
    string searchPattern = pattern;
    
    // Handle case sensitivity
    if (!options.caseSensitive) {
        searchText = toLowerCase(text);
        searchPattern = toLowerCase(pattern);
    }
    
    // Use KMP search for better performance
    vector<size_t> positions = kmpSearch(searchText, searchPattern);
    
    // Filter for whole word if needed
    if (options.wholeWord) {
        vector<size_t> filteredPositions;
        for (size_t pos : positions) {
            bool validStart = (pos == 0 || isWordBoundary(text[pos - 1]));
            bool validEnd = (pos + pattern.length() >= text.length() ||
                           isWordBoundary(text[pos + pattern.length()]));
            
            if (validStart && validEnd) {
                filteredPositions.push_back(pos);
            }
        }
        positions = filteredPositions;
    }
    
    // Create SearchResult objects
    for (size_t pos : positions) {
        size_t line, col;
        getLineAndColumn(text, pos, line, col);
        string context = getContext(text, pos, options.contextLines);
        
        results.push_back(SearchResult(pos, line, col, context));
    }
    
    return results;
}


// ==================== SearchHistory Implementation ====================

SearchHistory::SearchEntry::SearchEntry(const string& p, int count) 
    : pattern(p), resultCount(count) {
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    timestamp = buffer;
}

void SearchHistory::addSearch(const string& pattern, int resultCount) {
    history.push_back(SearchEntry(pattern, resultCount));
    
    if (history.size() > MAX_HISTORY) {
        history.erase(history.begin());
    }
}

vector<string> SearchHistory::getRecentPatterns(int count) const {
    vector<string> patterns;
    int start = max(0, (int)history.size() - count);
    
    for (int i = start; i < history.size(); i++) {
        patterns.push_back(history[i].pattern);
    }
    
    return patterns;
}

void SearchHistory::displayHistory() const {
    if (history.empty()) {
        cout << "No search history." << endl;
        return;
    }
    
    cout << "\n=== Search History ===" << endl;
    cout << setw(5) << "#" << " | "
         << setw(30) << left << "Pattern" << " | "
         << setw(19) << "Timestamp" << " | "
         << "Results" << endl;
    cout << string(75, '-') << endl;
    
    for (int i = history.size() - 1; i >= 0 && i >= history.size() - 20; i--) {
        cout << setw(5) << (i + 1) << " | "
             << setw(30) << left << history[i].pattern << " | "
             << setw(19) << history[i].timestamp << " | "
             << history[i].resultCount << endl;
    }
}

void SearchHistory::clear() {
    history.clear();
}