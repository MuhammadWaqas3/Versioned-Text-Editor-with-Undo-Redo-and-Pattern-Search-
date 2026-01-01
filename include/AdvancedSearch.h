#ifndef ADVANCED_SEARCH_H
#define ADVANCED_SEARCH_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

// Search result structure
struct SearchResult {
    size_t position;
    size_t line;
    size_t column;
    string context;  // Surrounding text
    
    SearchResult(size_t pos, size_t ln, size_t col, const string& ctx)
        : position(pos), line(ln), column(col), context(ctx) {}
};

// Search options/flags
struct SearchOptions {
    bool caseSensitive;
    bool wholeWord;
    bool useRegex;
    bool searchBackward;
    int contextLines;  // Number of lines to show before/after match
    
    SearchOptions() 
        : caseSensitive(true), wholeWord(false), useRegex(false),
          searchBackward(false), contextLines(1) {}
};

// Advanced search engine with multiple algorithms
class AdvancedSearchEngine {
private:
    static const long long PRIME = 1000000007;
    static const int BASE = 256;
    
    // Rolling hash helpers
    long long calculateHash(const string& str, int length) const;
    long long recalculateHash(long long oldHash, char oldChar, char newChar,
                             long long pow, int length) const;
    long long power(long long base, int exp) const;
    
    // Helper functions
    string toLowerCase(const string& str) const;
    bool isWordBoundary(char c) const;
    void getLineAndColumn(const string& text, size_t pos, size_t& line, size_t& col) const;
    string getContext(const string& text, size_t pos, int contextLines) const;
    
    // Search algorithms
    vector<size_t> naiveSearch(const string& text, const string& pattern) const;
    vector<size_t> kmpSearch(const string& text, const string& pattern) const;
    vector<size_t> boyerMooreSearch(const string& text, const string& pattern) const;
    
    // KMP helpers
    vector<int> computeLPSArray(const string& pattern) const;
    
    // Boyer-Moore helpers
    vector<int> computeBadCharTable(const string& pattern) const;
    
public:
    // Basic rolling hash search (from your existing implementation)
    vector<size_t> rollingHashSearch(const string& text, const string& pattern) const;
    
    // Advanced search with options
    vector<SearchResult> search(const string& text, const string& pattern,
                               const SearchOptions& options = SearchOptions()) const;
    
   
};

// Search history manager
class SearchHistory {
private:
    struct SearchEntry {
        string pattern;
        string timestamp;
        int resultCount;
        
        SearchEntry(const string& p, int count);
    };
    
    vector<SearchEntry> history;
    static const size_t MAX_HISTORY = 50;
    
public:
    void addSearch(const string& pattern, int resultCount);
    vector<string> getRecentPatterns(int count = 10) const;
    void displayHistory() const;
    void clear();
};

#endif // ADVANCED_SEARCH_H