// test_search.cpp - Unit tests for AdvancedSearchEngine
// Compile: g++ -o test_search test_search.cpp ../src/AdvancedSearch.cpp ../src/Buffer.cpp -std=c++11 -I../src
// Run: ./test_search

#include "../src/AdvancedSearch.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

int tests_passed = 0;
int tests_failed = 0;

void test_result(const string& test_name, bool passed) {
    if (passed) {
        cout << "[PASS] " << test_name << endl;
        tests_passed++;
    } else {
        cout << "[FAIL] " << test_name << endl;
        tests_failed++;
    }
}

// ============================================================================
// TEST SUITE 1: Rolling Hash Search (Rabin-Karp)
// ============================================================================

void test_rolling_hash_basic() {
    AdvancedSearchEngine engine;
    string text = "The quick brown fox jumps over the lazy dog";
    string pattern = "fox";
    
    vector<size_t> results = engine.rollingHashSearch(text, pattern);
    
    bool passed = (results.size() == 1 && results[0] == 16);
    test_result("Rolling Hash - Basic search", passed);
}

void test_rolling_hash_multiple() {
    AdvancedSearchEngine engine;
    string text = "to be or not to be";
    string pattern = "to";
    
    vector<size_t> results = engine.rollingHashSearch(text, pattern);
    
    bool passed = (results.size() == 2 && results[0] == 0 && results[1] == 13);
    test_result("Rolling Hash - Multiple matches", passed);
}

void test_rolling_hash_no_match() {
    AdvancedSearchEngine engine;
    string text = "Hello World";
    string pattern = "xyz";
    
    vector<size_t> results = engine.rollingHashSearch(text, pattern);
    
    bool passed = (results.empty());
    test_result("Rolling Hash - No match", passed);
}

// ============================================================================
// TEST SUITE 2: KMP Algorithm
// ============================================================================

void test_kmp_basic() {
    AdvancedSearchEngine engine;
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    
    vector<size_t> results = engine.kmpSearch(text, pattern);
    
    bool passed = (results.size() == 1 && results[0] == 10);
    test_result("KMP - Basic pattern match", passed);
}

void test_kmp_repeating_pattern() {
    AdvancedSearchEngine engine;
    string text = "AAAAAAA";
    string pattern = "AAA";
    
    vector<size_t> results = engine.kmpSearch(text, pattern);
    
    bool passed = (results.size() == 5);  // Overlapping matches
    test_result("KMP - Repeating pattern", passed);
}

void test_kmp_no_match() {
    AdvancedSearchEngine engine;
    string text = "ABCDEFG";
    string pattern = "XYZ";
    
    vector<size_t> results = engine.kmpSearch(text, pattern);
    
    bool passed = (results.empty());
    test_result("KMP - No match", passed);
}

// ============================================================================
// TEST SUITE 3: Boyer-Moore Algorithm
// ============================================================================

void test_boyer_moore_basic() {
    AdvancedSearchEngine engine;
    string text = "GCATCGCAGAGAGTATACAGTACG";
    string pattern = "GCAGAGAG";
    
    vector<size_t> results = engine.boyerMooreSearch(text, pattern);
    
    bool passed = (results.size() == 1 && results[0] == 5);
    test_result("Boyer-Moore - Basic pattern", passed);
}

void test_boyer_moore_multiple() {
    AdvancedSearchEngine engine;
    string text = "ABABABABABAB";
    string pattern = "AB";
    
    vector<size_t> results = engine.boyerMooreSearch(text, pattern);
    
    bool passed = (results.size() == 6);
    test_result("Boyer-Moore - Multiple matches", passed);
}

// ============================================================================
// TEST SUITE 4: Advanced Search with Options
// ============================================================================

void test_search_case_insensitive() {
    AdvancedSearchEngine engine;
    string text = "Hello WORLD world WoRlD";
    string pattern = "world";
    
    SearchOptions opts;
    opts.caseSensitive = false;
    
    vector<SearchResult> results = engine.search(text, pattern, opts);
    
    bool passed = (results.size() == 3);
    test_result("Advanced Search - Case insensitive", passed);
}

void test_search_case_sensitive() {
    AdvancedSearchEngine engine;
    string text = "Hello WORLD world WoRlD";
    string pattern = "world";
    
    SearchOptions opts;
    opts.caseSensitive = true;
    
    vector<SearchResult> results = engine.search(text, pattern, opts);
    
    bool passed = (results.size() == 1);
    test_result("Advanced Search - Case sensitive", passed);
}

void test_search_whole_word() {
    AdvancedSearchEngine engine;
    string text = "the theater is their place";
    string pattern = "the";
    
    SearchOptions opts;
    opts.wholeWord = true;
    opts.caseSensitive = false;
    
    vector<SearchResult> results = engine.search(text, pattern, opts);
    
    // Should match only standalone "the", not "theater" or "their"
    bool passed = (results.size() == 1 && results[0].position == 0);
    test_result("Advanced Search - Whole word only", passed);
}

void test_search_with_context() {
    AdvancedSearchEngine engine;
    string text = "Line1\nLine2 PATTERN here\nLine3";
    string pattern = "PATTERN";
    
    SearchOptions opts;
    opts.contextLines = 1;
    
    vector<SearchResult> results = engine.search(text, pattern, opts);
    
    bool passed = (results.size() == 1 && 
                   results[0].line == 2 &&
                   !results[0].context.empty());
    test_result("Advanced Search - With context", passed);
}

// ============================================================================
// TEST SUITE 5: Replace Operations
// ============================================================================

void test_replace_first() {
    AdvancedSearchEngine engine;
    string text = "cat cat cat";
    
    string result = engine.replaceFirst(text, "cat", "dog");
    
    bool passed = (result == "dog cat cat");
    test_result("Replace - Replace first only", passed);
}

void test_replace_all() {
    AdvancedSearchEngine engine;
    string text = "cat cat cat";
    
    string result = engine.replaceAll(text, "cat", "dog");
    
    bool passed = (result == "dog dog dog");
    test_result("Replace - Replace all occurrences", passed);
}

void test_replace_in_place() {
    AdvancedSearchEngine engine;
    string text = "Hello World World";
    
    int count = engine.replaceAllInPlace(text, "World", "C++");
    
    bool passed = (count == 2 && text == "Hello C++ C++");
    test_result("Replace - In-place replacement", passed);
}

// ============================================================================
// TEST SUITE 6: Fuzzy Search (Edit Distance)
// ============================================================================

void test_edit_distance_identical() {
    AdvancedSearchEngine engine;
    
    int dist = engine.editDistance("hello", "hello");
    
    bool passed = (dist == 0);
    test_result("Edit Distance - Identical strings", passed);
}

void test_edit_distance_insertion() {
    AdvancedSearchEngine engine;
    
    int dist = engine.editDistance("cat", "cats");
    
    bool passed = (dist == 1);  // One insertion
    test_result("Edit Distance - Single insertion", passed);
}

void test_edit_distance_deletion() {
    AdvancedSearchEngine engine;
    
    int dist = engine.editDistance("cats", "cat");
    
    bool passed = (dist == 1);  // One deletion
    test_result("Edit Distance - Single deletion", passed);
}

void test_edit_distance_substitution() {
    AdvancedSearchEngine engine;
    
    int dist = engine.editDistance("cat", "bat");
    
    bool passed = (dist == 1);  // One substitution
    test_result("Edit Distance - Single substitution", passed);
}

void test_edit_distance_multiple() {
    AdvancedSearchEngine engine;
    
    int dist = engine.editDistance("kitten", "sitting");
    
    bool passed = (dist == 3);  // k->s, e->i, insert g
    test_result("Edit Distance - Multiple operations", passed);
}

void test_fuzzy_search() {
    AdvancedSearchEngine engine;
    string text = "The quick brown fox";
    string pattern = "quik";  // Misspelled
    
    vector<size_t> results = engine.fuzzySearch(text, pattern, 1);
    
    // Should find "quick" with edit distance 1
    bool passed = (results.size() == 1);
    test_result("Fuzzy Search - With tolerance", passed);
}

// ============================================================================
// TEST SUITE 7: Utility Functions
// ============================================================================

void test_count_occurrences() {
    AdvancedSearchEngine engine;
    string text = "to be or not to be";
    
    int count = engine.countOccurrences(text, "to");
    
    bool passed = (count == 2);
    test_result("Count Occurrences - Basic count", passed);
}

void test_find_with_context() {
    AdvancedSearchEngine engine;
    string text = "Line1\nLine2\nLine3 PATTERN here\nLine4\nLine5";
    
    vector<SearchResult> results = engine.findWithContext(text, "PATTERN", 2);
    
    bool passed = (results.size() == 1 && results[0].line == 3);
    test_result("Find with Context - Multi-line context", passed);
}

void test_multi_pattern_search() {
    AdvancedSearchEngine engine;
    string text = "The quick brown fox jumps over the lazy dog";
    vector<string> patterns = {"quick", "fox", "dog", "cat"};
    
    auto results = engine.multiPatternSearch(text, patterns);
    
    bool passed = (results.size() == 4 && 
                   results[0].second.size() == 1 &&  // quick found
                   results[1].second.size() == 1 &&  // fox found
                   results[2].second.size() == 1 &&  // dog found
                   results[3].second.empty());       // cat not found
    test_result("Multi-Pattern Search - Search multiple", passed);
}

// ============================================================================
// TEST SUITE 8: Search History
// ============================================================================

void test_search_history_add() {
    SearchHistory history;
    history.addSearch("pattern1", 5);
    history.addSearch("pattern2", 3);
    
    vector<string> recent = history.getRecentPatterns(2);
    
    bool passed = (recent.size() == 2 && 
                   recent[0] == "pattern1" && 
                   recent[1] == "pattern2");
    test_result("Search History - Add and retrieve", passed);
}

void test_search_history_limit() {
    SearchHistory history;
    
    // Add more than max history (50)
    for (int i = 0; i < 60; i++) {
        history.addSearch("pattern" + to_string(i), 1);
    }
    
    vector<string> recent = history.getRecentPatterns(60);
    
    // Should only keep last 50
    bool passed = (recent.size() <= 50);
    test_result("Search History - Max limit enforcement", passed);
}

// ============================================================================
// TEST SUITE 9: Performance Tests
// ============================================================================

void test_performance_large_text() {
    AdvancedSearchEngine engine;
    
    // Create large text (10,000 characters)
    string large_text(10000, 'A');
    large_text += "PATTERN";
    large_text += string(10000, 'B');
    
    string pattern = "PATTERN";
    
    vector<size_t> results = engine.rollingHashSearch(large_text, pattern);
    
    bool passed = (results.size() == 1 && results[0] == 10000);
    test_result("Performance - Large text search", passed);
}

void test_performance_long_pattern() {
    AdvancedSearchEngine engine;
    
    string text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string pattern = "KLMNOPQRS";
    
    vector<size_t> results = engine.kmpSearch(text, pattern);
    
    bool passed = (results.size() == 1 && results[0] == 10);
    test_result("Performance - Long pattern", passed);
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main() {
    cout << "=======================================" << endl;
    cout << "  ADVANCED SEARCH ENGINE UNIT TESTS" << endl;
    cout << "=======================================" << endl << endl;
    
    cout << "SUITE 1: Rolling Hash (Rabin-Karp)" << endl;
    cout << "-------------------------------------" << endl;
    test_rolling_hash_basic();
    test_rolling_hash_multiple();
    test_rolling_hash_no_match();
    cout << endl;
    
    cout << "SUITE 2: KMP Algorithm" << endl;
    cout << "-------------------------------------" << endl;
    test_kmp_basic();
    test_kmp_repeating_pattern();
    test_kmp_no_match();
    cout << endl;
    
    cout << "SUITE 3: Boyer-Moore Algorithm" << endl;
    cout << "-------------------------------------" << endl;
    test_boyer_moore_basic();
    test_boyer_moore_multiple();
    cout << endl;
    
    cout << "SUITE 4: Advanced Search Options" << endl;
    cout << "-------------------------------------" << endl;
    test_search_case_insensitive();
    test_search_case_sensitive();
    test_search_whole_word();
    test_search_with_context();
    cout << endl;
    
    cout << "SUITE 5: Replace Operations" << endl;
    cout << "-------------------------------------" << endl;
    test_replace_first();
    test_replace_all();
    test_replace_in_place();
    cout << endl;
    
    cout << "SUITE 6: Fuzzy Search (Edit Distance)" << endl;
    cout << "-------------------------------------" << endl;
    test_edit_distance_identical();
    test_edit_distance_insertion();
    test_edit_distance_deletion();
    test_edit_distance_substitution();
    test_edit_distance_multiple();
    test_fuzzy_search();
    cout << endl;
    
    cout << "SUITE 7: Utility Functions" << endl;
    cout << "-------------------------------------" << endl;
    test_count_occurrences();
    test_find_with_context();
    test_multi_pattern_search();
    cout << endl;
    
    cout << "SUITE 8: Search History" << endl;
    cout << "-------------------------------------" << endl;
    test_search_history_add();
    test_search_history_limit();
    cout << endl;
    
    cout << "SUITE 9: Performance Tests" << endl;
    cout << "-------------------------------------" << endl;
    test_performance_large_text();
    test_performance_long_pattern();
    cout << endl;
    
    cout << "=======================================" << endl;
    cout << "RESULTS: " << tests_passed << " passed, " 
         << tests_failed << " failed" << endl;
    cout << "=======================================" << endl;
    
    return (tests_failed == 0) ? 0 : 1;
}