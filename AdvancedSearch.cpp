#include "AdvancedSearch.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

long long AdvancedSearchEngine::power(long long base, int exp) const
{
    long long result = 1;
    for (int i = 0; i < exp; i++)
    {
        result = (result * base) % PRIME;
    }
    return result;
}

long long AdvancedSearchEngine::calculateHash(const string &str, int length) const
{
    long long hash = 0;
    for (int i = 0; i < length && i < str.length(); i++)
    {
        hash = (hash * BASE + static_cast<unsigned char>(str[i])) % PRIME;
    }
    return hash;
}

long long AdvancedSearchEngine::recalculateHash(long long oldHash, char oldChar, char newChar,long long pow, int length) const
{
    long long newHash = (oldHash - (static_cast<unsigned char>(oldChar) * pow) % PRIME + PRIME) % PRIME;
    newHash = (newHash * BASE + static_cast<unsigned char>(newChar)) % PRIME;
    return newHash;
}

string AdvancedSearchEngine::toLowerCase(const string &str) const
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool AdvancedSearchEngine::isWordBoundary(char c) const
{
    return !isalnum(c) && c != '_';
}

void AdvancedSearchEngine::getLineAndColumn(const string &text, size_t pos, size_t &line, size_t &col) const
{
    line = 1;
    col = 1;

    for (size_t i = 0; i < pos && i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            line++;
            col = 1;
        }
        else
        {
            col++;
        }
    }
}

string AdvancedSearchEngine::getContext(const string &text, size_t pos, int contextLines) const
{
    // Find start of context (go back contextLines lines)
    size_t start = pos;
    int linesBack = 0;

    while (start > 0 && linesBack < contextLines)
    {
        start--;
        if (text[start] == '\n')
        {
            linesBack++;
        }
    }

    // Find end of context (go forward contextLines lines)
    size_t end = pos;
    int linesForward = 0;

    while (end < text.length() && linesForward < contextLines)
    {
        if (text[end] == '\n')
        {
            linesForward++;
        }
        end++;
    }

    return text.substr(start, end - start);
}

