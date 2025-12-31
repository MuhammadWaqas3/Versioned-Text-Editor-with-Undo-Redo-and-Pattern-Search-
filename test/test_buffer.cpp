// test_buffer.cpp - Unit tests for TextBuffer and TextEditor
// Compile: g++ -o test_buffer test_buffer.cpp ../src/Buffer.cpp -std=c++11 -I../src
// Run: ./test_buffer

#include "../src/Buffer.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

// Test counter
int tests_passed = 0;
int tests_failed = 0;

// Helper function to print test results
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
// TEST SUITE 1: TextBuffer Basic Operations
// ============================================================================

void test_buffer_insert_basic() {
    TextBuffer buffer;
    buffer.insert(0, "Hello");
    
    bool passed = (buffer.getText() == "Hello" && buffer.length() == 5);
    test_result("TextBuffer::insert - Basic insertion", passed);
}

void test_buffer_insert_middle() {
    TextBuffer buffer("HelloWorld");
    buffer.insert(5, " ");
    
    bool passed = (buffer.getText() == "Hello World");
    test_result("TextBuffer::insert - Middle insertion", passed);
}

void test_buffer_insert_end() {
    TextBuffer buffer("Hello");
    buffer.insert(5, " World");
    
    bool passed = (buffer.getText() == "Hello World");
    test_result("TextBuffer::insert - End insertion", passed);
}

void test_buffer_delete_basic() {
    TextBuffer buffer("Hello World");
    buffer.deleteText(5, 6);
    
    bool passed = (buffer.getText() == "Hello");
    test_result("TextBuffer::deleteText - Basic deletion", passed);
}

void test_buffer_delete_start() {
    TextBuffer buffer("Hello World");
    buffer.deleteText(0, 6);
    
    bool passed = (buffer.getText() == "World");
    test_result("TextBuffer::deleteText - Start deletion", passed);
}

void test_buffer_delete_all() {
    TextBuffer buffer("Hello");
    buffer.deleteText(0, 5);
    
    bool passed = (buffer.getText() == "" && buffer.length() == 0);
    test_result("TextBuffer::deleteText - Delete all", passed);
}

void test_buffer_replace_basic() {
    TextBuffer buffer("Hello World");
    buffer.replace(6, 5, "C++");
    
    bool passed = (buffer.getText() == "Hello C++");
    test_result("TextBuffer::replace - Basic replacement", passed);
}

void test_buffer_replace_longer() {
    TextBuffer buffer("Hi");
    buffer.replace(0, 2, "Hello World");
    
    bool passed = (buffer.getText() == "Hello World");
    test_result("TextBuffer::replace - Replace with longer text", passed);
}

void test_buffer_substring() {
    TextBuffer buffer("Hello World");
    string sub = buffer.getSubstring(0, 5);
    
    bool passed = (sub == "Hello");
    test_result("TextBuffer::getSubstring - Extract substring", passed);
}

void test_buffer_clear() {
    TextBuffer buffer("Hello World");
    buffer.clear();
    
    bool passed = (buffer.getText() == "" && buffer.length() == 0);
    test_result("TextBuffer::clear - Clear buffer", passed);
}

// ============================================================================
// TEST SUITE 2: TextBuffer Chunked Storage (Large Text)
// ============================================================================

void test_buffer_large_text() {
    TextBuffer buffer;
    
    // Insert 1000 characters (multiple chunks)
    string large_text(1000, 'A');
    buffer.insert(0, large_text);
    
    bool passed = (buffer.length() == 1000 && buffer.getText() == large_text);
    test_result("TextBuffer::insert - Large text (multiple chunks)", passed);
}

void test_buffer_chunk_rebalancing() {
    TextBuffer buffer;
    
    // Insert in small pieces to trigger rebalancing
    for (int i = 0; i < 10; i++) {
        buffer.insert(buffer.length(), string(100, 'A' + i));
    }
    
    bool passed = (buffer.length() == 1000);
    test_result("TextBuffer - Chunk rebalancing", passed);
}

// ============================================================================
// TEST SUITE 3: TextEditor with Commands (Undo/Redo)
// ============================================================================

void test_editor_insert_undo() {
    TextEditor editor;
    editor.insert(0, "Hello");
    
    bool insert_ok = (editor.getText() == "Hello");
    
    editor.undo();
    bool undo_ok = (editor.getText() == "");
    
    bool passed = (insert_ok && undo_ok && !editor.canUndo());
    test_result("TextEditor::undo - Undo insert", passed);
}

void test_editor_delete_undo() {
    TextEditor editor;
    editor.insert(0, "Hello World");
    editor.deleteText(5, 6);
    
    bool delete_ok = (editor.getText() == "Hello");
    
    editor.undo();
    bool undo_ok = (editor.getText() == "Hello World");
    
    bool passed = (delete_ok && undo_ok);
    test_result("TextEditor::undo - Undo delete", passed);
}

void test_editor_redo() {
    TextEditor editor;
    editor.insert(0, "Hello");
    editor.undo();
    
    bool undo_ok = (editor.getText() == "");
    
    editor.redo();
    bool redo_ok = (editor.getText() == "Hello");
    
    bool passed = (undo_ok && redo_ok && !editor.canRedo());
    test_result("TextEditor::redo - Redo insert", passed);
}

void test_editor_multiple_undo() {
    TextEditor editor;
    editor.insert(0, "A");
    editor.insert(1, "B");
    editor.insert(2, "C");
    
    editor.undo();  // Remove C
    editor.undo();  // Remove B
    
    bool passed = (editor.getText() == "A");
    test_result("TextEditor::undo - Multiple undo", passed);
}

void test_editor_undo_redo_sequence() {
    TextEditor editor;
    editor.insert(0, "Hello");
    editor.insert(5, " World");
    
    editor.undo();
    editor.undo();
    editor.redo();
    editor.redo();
    
    bool passed = (editor.getText() == "Hello World");
    test_result("TextEditor - Undo/Redo sequence", passed);
}

void test_editor_undo_clears_redo() {
    TextEditor editor;
    editor.insert(0, "Hello");
    editor.undo();
    
    // New operation should clear redo stack
    editor.insert(0, "World");
    
    bool passed = (!editor.canRedo());
    test_result("TextEditor - New operation clears redo stack", passed);
}

// ============================================================================
// TEST SUITE 4: Edge Cases
// ============================================================================

void test_edge_empty_operations() {
    TextBuffer buffer;
    buffer.insert(0, "");
    buffer.deleteText(0, 0);
    
    bool passed = (buffer.getText() == "" && buffer.length() == 0);
    test_result("Edge case - Empty operations", passed);
}

void test_edge_out_of_bounds_insert() {
    TextBuffer buffer("Hello");
    buffer.insert(100, " World");  // Should insert at end
    
    bool passed = (buffer.getText() == "Hello World");
    test_result("Edge case - Out of bounds insert", passed);
}

void test_edge_out_of_bounds_delete() {
    TextBuffer buffer("Hello");
    buffer.deleteText(3, 100);  // Should delete from pos 3 to end
    
    bool passed = (buffer.getText() == "Hel");
    test_result("Edge case - Out of bounds delete", passed);
}

void test_edge_undo_empty_stack() {
    TextEditor editor;
    bool result = editor.undo();  // Should return false
    
    bool passed = (!result && !editor.canUndo());
    test_result("Edge case - Undo on empty stack", passed);
}

void test_edge_redo_empty_stack() {
    TextEditor editor;
    bool result = editor.redo();  // Should return false
    
    bool passed = (!result && !editor.canRedo());
    test_result("Edge case - Redo on empty stack", passed);
}

// ============================================================================
// TEST SUITE 5: SearchEngine (Rolling Hash)
// ============================================================================

void test_search_single_match() {
    SearchEngine engine;
    string text = "Hello World";
    string pattern = "World";
    
    vector<size_t> results = engine.search(text, pattern);
    
    bool passed = (results.size() == 1 && results[0] == 6);
    test_result("SearchEngine::search - Single match", passed);
}

void test_search_multiple_matches() {
    SearchEngine engine;
    string text = "abcabcabc";
    string pattern = "abc";
    
    vector<size_t> results = engine.search(text, pattern);
    
    bool passed = (results.size() == 3 && 
                   results[0] == 0 && 
                   results[1] == 3 && 
                   results[2] == 6);
    test_result("SearchEngine::search - Multiple matches", passed);
}

void test_search_no_match() {
    SearchEngine engine;
    string text = "Hello World";
    string pattern = "xyz";
    
    vector<size_t> results = engine.search(text, pattern);
    
    bool passed = (results.size() == 0);
    test_result("SearchEngine::search - No match", passed);
}

void test_search_overlapping_pattern() {
    SearchEngine engine;
    string text = "aaaa";
    string pattern = "aa";
    
    vector<size_t> results = engine.search(text, pattern);
    
    bool passed = (results.size() == 3);  // Positions 0, 1, 2
    test_result("SearchEngine::search - Overlapping pattern", passed);
}

void test_search_and_replace() {
    SearchEngine engine;
    string text = "Hello World World";
    
    vector<size_t> results = engine.searchAndReplace(text, "World", "C++");
    
    bool passed = (results.size() == 2 && text == "Hello C++ C++");
    test_result("SearchEngine::searchAndReplace - Replace all", passed);
}

// ============================================================================
// TEST SUITE 6: File Operations
// ============================================================================

void test_save_and_load() {
    TextEditor editor1;
    editor1.insert(0, "Test file content");
    
    bool save_ok = editor1.saveToFile("test_output.txt");
    
    TextEditor editor2;
    bool load_ok = editor2.loadFromFile("test_output.txt");
    
    bool content_ok = (editor2.getText() == "Test file content");
    
    bool passed = (save_ok && load_ok && content_ok);
    test_result("File I/O - Save and load", passed);
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main() {
    cout << "=======================================" << endl;
    cout << "   TEXT BUFFER & EDITOR UNIT TESTS" << endl;
    cout << "=======================================" << endl << endl;
    
    cout << "SUITE 1: TextBuffer Basic Operations" << endl;
    cout << "-------------------------------------" << endl;
    test_buffer_insert_basic();
    test_buffer_insert_middle();
    test_buffer_insert_end();
    test_buffer_delete_basic();
    test_buffer_delete_start();
    test_buffer_delete_all();
    test_buffer_replace_basic();
    test_buffer_replace_longer();
    test_buffer_substring();
    test_buffer_clear();
    cout << endl;
    
    cout << "SUITE 2: TextBuffer Chunked Storage" << endl;
    cout << "-------------------------------------" << endl;
    test_buffer_large_text();
    test_buffer_chunk_rebalancing();
    cout << endl;
    
    cout << "SUITE 3: TextEditor Undo/Redo" << endl;
    cout << "-------------------------------------" << endl;
    test_editor_insert_undo();
    test_editor_delete_undo();
    test_editor_redo();
    test_editor_multiple_undo();
    test_editor_undo_redo_sequence();
    test_editor_undo_clears_redo();
    cout << endl;
    
    cout << "SUITE 4: Edge Cases" << endl;
    cout << "-------------------------------------" << endl;
    test_edge_empty_operations();
    test_edge_out_of_bounds_insert();
    test_edge_out_of_bounds_delete();
    test_edge_undo_empty_stack();
    test_edge_redo_empty_stack();
    cout << endl;
    
    cout << "SUITE 5: Search Engine" << endl;
    cout << "-------------------------------------" << endl;
    test_search_single_match();
    test_search_multiple_matches();
    test_search_no_match();
    test_search_overlapping_pattern();
    test_search_and_replace();
    cout << endl;
    
    cout << "SUITE 6: File Operations" << endl;
    cout << "-------------------------------------" << endl;
    test_save_and_load();
    cout << endl;
    
    cout << "=======================================" << endl;
    cout << "RESULTS: " << tests_passed << " passed, " 
         << tests_failed << " failed" << endl;
    cout << "=======================================" << endl;
    
    return (tests_failed == 0) ? 0 : 1;
}