#include "textEditor.h"
#include <iostream>
#include <cassert>
#include <vector>

// Simple test framework
int totalTests = 0;
int passedTests = 0;

void test(const std::string& name, bool condition) {
    totalTests++;
    if (condition) {
        passedTests++;
        std::cout << "✓ " << name << '\n';
    } else {
        std::cout << "✗ " << name << '\n';
    }
}

void testTextBuffer() {
    std::cout << "\n=== Testing TextBuffer ===\n";
    
    TextBuffer buffer;
    test("Empty buffer initialization", buffer.length() == 0);
    
    buffer.insert(0, "Hello");
    test("Insert into empty buffer", buffer.getText() == "Hello" && buffer.length() == 5);
    
    buffer.insert(5, " World");
    test("Insert at end", buffer.getText() == "Hello World" && buffer.length() == 11);
    
    buffer.insert(5, ",");
    test("Insert in middle", buffer.getText() == "Hello, World" && buffer.length() == 12);
    
    buffer.deleteText(5, 2);
    test("Delete text", buffer.getText() == "Hello World" && buffer.length() == 11);
    
    buffer.replace(6, 5, "C++");
    test("Replace text", buffer.getText() == "Hello C++" && buffer.length() == 9);
    
    // Test with large text
    std::string largeText(1000, 'A');
    TextBuffer largeBuffer(largeText);
    test("Large buffer initialization", largeBuffer.length() == 1000);
    
    largeBuffer.insert(500, "BBB");
    test("Insert into large buffer", largeBuffer.length() == 1003);
    
    buffer.clear();
    test("Clear buffer", buffer.length() == 0 && buffer.getText().empty());
}

void testSearchEngine() {
    std::cout << "\n=== Testing SearchEngine ===\n";
    
    SearchEngine search;
    std::string text = "The quick brown fox jumps over the lazy dog. The fox is quick.";
    
    auto results = search.search(text, "fox");
    test("Find pattern - multiple occurrences", results.size() == 2);
    test("First occurrence position", results.size() >= 1 && results[0] == 16);
    test("Second occurrence position", results.size() >= 2 && results[1] == 45);
    
    results = search.search(text, "cat");
    test("Pattern not found", results.empty());
    
    results = search.search(text, "The");
    test("Case sensitive search", results.size() == 2);
    
    results = search.search(text, "");
    test("Empty pattern", results.empty());
    
    std::string testText = "abcabcabc";
    results = search.search(testText, "abc");
    test("Overlapping patterns", results.size() == 3);
    
    // Test search and replace
    std::string replaceText = "Hello world, world!";
    search.searchAndReplace(replaceText, "world", "C++");
    test("Search and replace", replaceText == "Hello C++, C++!");
}

void testVersionHistory() {
    std::cout << "\n=== Testing VersionHistory ===\n";
    
    VersionHistory history;
    
    auto v1 = std::make_shared<Version>("Version 1 content", 1, "First");
    auto v2 = std::make_shared<Version>("Version 2 content", 2, "Second");
    auto v3 = std::make_shared<Version>("Version 3 content", 3, "Third");
    
    history.addVersion(v1);
    history.addVersion(v2);
    history.addVersion(v3);
    
    test("Version count", history.getCount() == 3);
    
    auto current = history.getCurrentVersion();
    test("Current version is latest", current && current->getVersionNumber() == 3);
    
    auto prev = history.getPreviousVersion();
    test("Navigate to previous", prev && prev->getVersionNumber() == 2);
    
    prev = history.getPreviousVersion();
    test("Navigate to first", prev && prev->getVersionNumber() == 1);
    
    auto next = history.getNextVersion();
    test("Navigate forward", next && next->getVersionNumber() == 2);
    
    auto versions = history.getAllVersions();
    test("Get all versions", versions.size() == 3);
}

void testUndoRedo() {
    std::cout << "\n=== Testing Undo/Redo ===\n";
    
    TextEditor editor;
    editor.clear();
    
    editor.insert(0, "Hello");
    test("After insert", editor.getText() == "Hello");
    
    editor.insert(5, " World");
    test("After second insert", editor.getText() == "Hello World");
    
    editor.undo();
    test("Undo last insert", editor.getText() == "Hello");
    
    editor.undo();
    test("Undo first insert", editor.getText() == "");
    
    editor.redo();
    test("Redo first insert", editor.getText() == "Hello");
    
    editor.redo();
    test("Redo second insert", editor.getText() == "Hello World");
    
    // Test that new operations clear redo stack
    editor.undo();
    editor.insert(5, " C++");
    test("New operation after undo", editor.getText() == "Hello C++");
    
    bool canRedo = editor.canRedo();
    test("Redo stack cleared", !canRedo);
}

void testEditorOperations() {
    std::cout << "\n=== Testing Editor Operations ===\n";
    
    TextEditor editor;
    editor.clear();
    
    // Test insert
    editor.insert(0, "First line\n");
    editor.insert(11, "Second line\n");
    test("Multi-line insert", editor.getText() == "First line\nSecond line\n");
    
    // Test delete
    editor.deleteText(0, 6);
    test("Delete from start", editor.getText() == "line\nSecond line\n");
    
    // Test replace
    editor.replace(0, 4, "text");
    test("Replace operation", editor.getText() == "text\nSecond line\n");
    
    // Test search
    auto positions = editor.search("line");
    test("Search in document", positions.size() == 1);
    
    // Test search and replace
    int count = editor.searchAndReplace("text", "code");
    test("Search and replace count", count == 1);
    test("Search and replace result", editor.getText() == "code\nSecond line\n");
}

void testVersionSnapshots() {
    std::cout << "\n=== Testing Version Snapshots ===\n";
    
    TextEditor editor;
    editor.clear();
    
    editor.insert(0, "Version 1");
    editor.createSnapshot("First snapshot");
    
    editor.insert(9, " modified");
    editor.createSnapshot("Second snapshot");
    
    auto versions = editor.getVersions();
    test("Two snapshots created", versions.size() >= 2);
    
    editor.restoreVersion(1);
    test("Restore to first version", editor.getText() == "Version 1");
    
    editor.restoreVersion(2);
    test("Restore to second version", editor.getText() == "Version 1 modified");
}

void testFileOperations() {
    std::cout << "\n=== Testing File Operations ===\n";
    
    TextEditor editor;
    editor.clear();
    
    std::string testContent = "Line 1\nLine 2\nLine 3\n";
    editor.insert(0, testContent);
    
    bool saved = editor.saveToFile("test_file.txt");
    test("Save to file", saved);
    
    TextEditor editor2;
    bool loaded = editor2.loadFromFile("test_file.txt");
    test("Load from file", loaded);
    test("Content matches after load", editor2.getText() == testContent);
    
    // Clean up
    remove("test_file.txt");
}

void testEdgeCases() {
    std::cout << "\n=== Testing Edge Cases ===\n";
    
    TextEditor editor;
    editor.clear();
    
    // Empty operations
    editor.deleteText(0, 100);
    test("Delete from empty buffer", editor.getText().empty());
    
    editor.insert(100, "Test");
    test("Insert at invalid position", editor.length() > 0);
    
    // Undo/redo on empty stacks
    test("Undo empty stack", !editor.undo());
    
    editor.insert(0, "Test");
    editor.undo();
    editor.undo();
    test("Multiple undo beyond limit", editor.getText().empty());
    
    // Search edge cases
    auto results = editor.search("");
    test("Search empty pattern", results.empty());
    
    editor.insert(0, "Test");
    results = editor.search("Testing");
    test("Search pattern longer than text", results.empty());
}

void testPerformance() {
    std::cout << "\n=== Testing Performance ===\n";
    
    TextEditor editor;
    editor.clear();
    
    // Insert large text
    std::string largeText;
    for (int i = 0; i < 1000; i++) {
        largeText += "Line " + std::to_string(i) + "\n";
    }
    
    editor.insert(0, largeText);
    test("Large text insertion", editor.length() > 10000);
    
    // Search in large text
    auto results = editor.search("Line 500");
    test("Search in large text", results.size() >= 1);
    
    // Multiple undo/redo
    for (int i = 0; i < 10; i++) {
        editor.insert(0, "A");
    }
    
    for (int i = 0; i < 10; i++) {
        editor.undo();
    }
    
    test("Multiple undo operations", editor.length() == largeText.length());
}

int main() {
    std::cout << "====================================\n";
    std::cout << "  Text Editor - Unit Test Suite\n";
    std::cout << "====================================\n";
    
    testTextBuffer();
    testSearchEngine();
    testVersionHistory();
    testUndoRedo();
    testEditorOperations();
    testVersionSnapshots();
    testFileOperations();
    testEdgeCases();
    testPerformance();
    
    std::cout << "\n====================================\n";
    std::cout << "Test Results: " << passedTests << "/" << totalTests << " passed\n";
    
    if (passedTests == totalTests) {
        std::cout << "✓ All tests passed!\n";
    } else {
        std::cout << "✗ Some tests failed.\n";
    }
    std::cout << "====================================\n";
    
    return (passedTests == totalTests) ? 0 : 1;
}