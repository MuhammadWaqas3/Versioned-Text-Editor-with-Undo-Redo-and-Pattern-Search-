// (Internal) : It prevents this file to get included in multiple cpp file. 
#ifndef ADVANCED_SEARCH_H
#define ADVANCED_SEARCH_H

#include <string>
#include <vector>
#include <memory>

// Search result structure
struct SearchResult
{
    size_t position;
    size_t line;
    size_t column;
    string context; // Surrounding text

    SearchResult(size_t pos, size_t ln, size_t col, const string &ctx) : position(pos), line(ln), column(col), context(ctx) {}
};

// Search options/flags
struct SearchOptions
{
    bool caseSensitive;
    bool wholeWord;
    bool useRegex;
    bool searchBackward;
    int contextLines; // Number of lines to show before/after match

    SearchOptions()
        : caseSensitive(true), wholeWord(false), useRegex(false),
          searchBackward(false), contextLines(1) {}
};

// Search history manager
class SearchHistory
{
private:
    struct SearchEntry
    {
        string pattern;
        string timestamp;
        int resultCount;

        SearchEntry(const string &p, int count);
    };

    vector<SearchEntry> history;
    static const size_t MAX_HISTORY = 50;

public:
    void addSearch(const string &pattern, int resultCount);
    vector<string> getRecentPatterns(int count = 10) const;
    void displayHistory() const;
    void clear();
};

using namespace std;


#endif