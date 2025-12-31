// test_history.cpp - Unit tests for VersionHistory and VersionManager
// Compile: g++ -o test_history test_history.cpp ../src/History.cpp ../src/Buffer.cpp -std=c++11 -I../src
// Run: ./test_history

#include "../src/History.h"
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
// TEST SUITE 1: Version Creation and Storage
// ============================================================================

void test_version_creation() {
    Version ver("Hello World", 1, "First version", "test.txt");
    
    bool passed = (ver.getContent() == "Hello World" &&
                   ver.getVersionNumber() == 1 &&
                   ver.getDescription() == "First version" &&
                   ver.getFilename() == "test.txt" &&
                   !ver.getTimestamp().empty());
    
    test_result("Version - Creation and getters", passed);
}

void test_version_serialization() {
    Version ver("Test content", 5, "Description", "file.txt");
    
    string serialized = ver.serialize();
    auto deserialized = Version::deserialize(serialized);
    
    bool passed = (deserialized->getContent() == "Test content" &&
                   deserialized->getVersionNumber() == 5 &&
                   deserialized->getDescription() == "Description" &&
                   deserialized->getFilename() == "file.txt");
    
    test_result("Version - Serialization/Deserialization", passed);
}

// ============================================================================
// TEST SUITE 2: VersionHistory Basic Operations
// ============================================================================

void test_history_add_version() {
    VersionHistory history;
    history.addVersion("Version 1 content", 1, "First", "test.txt");
    
    auto current = history.getCurrentVersion();
    
    bool passed = (current != nullptr &&
                   current->getContent() == "Version 1 content" &&
                   history.getCount() == 1);
    
    test_result("VersionHistory - Add single version", passed);
}

void test_history_multiple_versions() {
    VersionHistory history;
    history.addVersion("V1", 1, "First", "test.txt");
    history.addVersion("V2", 2, "Second", "test.txt");
    history.addVersion("V3", 3, "Third", "test.txt");
    
    bool passed = (history.getCount() == 3 &&
                   history.getCurrentVersion()->getContent() == "V3");
    
    test_result("VersionHistory - Add multiple versions", passed);
}

void test_history_navigation_backward() {
    VersionHistory history;
    history.addVersion("V1", 1, "First", "test.txt");
    history.addVersion("V2", 2, "Second", "test.txt");
    history.addVersion("V3", 3, "Third", "test.txt");
    
    auto prev1 = history.getPreviousVersion();
    auto prev2 = history.getPreviousVersion();
    
    bool passed = (prev1 != nullptr && prev1->getContent() == "V2" &&
                   prev2 != nullptr && prev2->getContent() == "V1");
    
    test_result("VersionHistory - Navigate backward", passed);
}

void test_history_navigation_forward() {
    VersionHistory history;
    history.addVersion("V1", 1, "First", "test.txt");
    history.addVersion("V2", 2, "Second", "test.txt");
    history.addVersion("V3", 3, "Third", "test.txt");
    
    history.getPreviousVersion();
    history.getPreviousVersion();
    
    auto next = history.getNextVersion();
    
    bool passed = (next != nullptr && next->getContent() == "V2");
    
    test_result("VersionHistory - Navigate forward", passed);
}

void test_history_navigation_boundaries() {
    VersionHistory history;
    history.addVersion("V1", 1, "First", "test.txt");
    
    auto prev = history.getPreviousVersion();
    
    bool passed = (prev == nullptr);  // Can't go before first
    
    test_result("VersionHistory - Navigation boundaries", passed);
}

void test_history_get_all_versions() {
    VersionHistory history;
    history.addVersion("V1", 1, "First", "test.txt");
    history.addVersion("V2", 2, "Second", "test.txt");
    history.addVersion("V3", 3, "Third", "test.txt");
    
    auto all = history.getAllVersions();
    
    bool passed = (all.size() == 3 &&
                   all[0]->getContent() == "V1" &&
                   all[1]->getContent() == "V2" &&
                   all[2]->getContent() == "V3");
    
    test_result("VersionHistory - Get all versions", passed);
}

void test_history_clear() {
    VersionHistory history;
    history.addVersion("V1", 1, "First", "test.txt");
    history.addVersion("V2", 2, "Second", "test.txt");
    
    history.clear();
    
    bool passed = (history.getCount() == 0 &&
                   history.getCurrentVersion() == nullptr);
    
    test_result("VersionHistory - Clear all versions", passed);
}

// ============================================================================
// TEST SUITE 3: File Persistence
// ============================================================================

void test_history_save_and_load() {
    // Create and save history
    {
        VersionHistory history1;
        history1.addVersion("V1 content", 1, "Version 1", "test.txt");
        history1.addVersion("V2 content", 2, "Version 2", "test.txt");
        history1.addVersion("V3 content", 3, "Version 3", "test.txt");
        
        bool save_ok = history1.saveToFile();
        
        if (!save_ok) {
            test_result("VersionHistory - Save to file", false);
            return;
        }
    }
    
    // Load history
    {
        VersionHistory history2;
        bool load_ok = history2.loadFromFile();
        
        bool content_ok = (history2.getCount() == 3 &&
                          history2.getCurrentVersion()->getContent() == "V3 content");
        
        bool passed = (load_ok && content_ok);
        test_result("VersionHistory - Save and load from file", passed);
    }
}

void test_history_persistence_navigation() {
    // Save with navigation state
    {
        VersionHistory history1;
        history1.addVersion("V1", 1, "First", "test.txt");
        history1.addVersion("V2", 2, "Second", "test.txt");
        history1.addVersion("V3", 3, "Third", "test.txt");
        
        history1.getPreviousVersion();  // Move to V2
        history1.saveToFile();
    }
    
    // Load and check navigation state
    {
        VersionHistory history2;
        history2.loadFromFile();
        
        bool passed = (history2.getCurrentVersion()->getContent() == "V2");
        test_result("VersionHistory - Persist navigation state", passed);
    }
}

// ============================================================================
// TEST SUITE 4: Doubly Linked List Integrity
// ============================================================================

void test_linked_list_forward_traversal() {
    VersionHistory history;
    history.addVersion("V1", 1, "First", "test.txt");
    history.addVersion("V2", 2, "Second", "test.txt");
    history.addVersion("V3", 3, "Third", "test.txt");
    
    auto all = history.getAllVersions();
    
    bool passed = true;
    for (size_t i = 0; i < all.size(); i++) {
        if (all[i]->getVersionNumber() != (int)(i + 1)) {
            passed = false;
            break;
        }
    }
    
    test_result("Linked List - Forward traversal integrity", passed);
}

void test_linked_list_backward_forward() {
    VersionHistory history;
    history.addVersion("V1", 1, "First", "test.txt");
    history.addVersion("V2", 2, "Second", "test.txt");
    history.addVersion("V3", 3, "Third", "test.txt");
    
    // Go backward twice
    history.getPreviousVersion();
    history.getPreviousVersion();
    
    // Go forward twice
    auto next1 = history.getNextVersion();
    auto next2 = history.getNextVersion();
    
    bool passed = (next1->getContent() == "V2" &&
                   next2->getContent() == "V3");
    
    test_result("Linked List - Backward-forward consistency", passed);
}

// ============================================================================
// TEST SUITE 5: Edge Cases
// ============================================================================

void test_edge_empty_history_operations() {
    VersionHistory history;
    
    auto current = history.getCurrentVersion();
    auto prev = history.getPreviousVersion();
    auto next = history.getNextVersion();
    
    bool passed = (current == nullptr &&
                   prev == nullptr &&
                   next == nullptr &&
                   history.getCount() == 0);
    
    test_result("Edge Case - Empty history operations", passed);
}

void test_edge_single_version_navigation() {
    VersionHistory history;
    history.addVersion("Only", 1, "Single", "test.txt");
    
    auto prev = history.getPreviousVersion();
    auto next = history.getNextVersion();
    
    bool passed = (prev == nullptr && next == nullptr);
    
    test_result("Edge Case - Single version navigation", passed);
}

void test_edge_large_content() {
    VersionHistory history;
    string large_content(10000, 'A');
    
    history.addVersion(large_content, 1, "Large", "test.txt");
    
    bool passed = (history.getCurrentVersion()->getContent() == large_content);
    
    test_result("Edge Case - Large content storage", passed);
}

void test_edge_special_characters() {
    VersionHistory history;
    string special = "Line1\nLine2\tTab\r\n\"Quotes\" and |pipes|";
    
    history.addVersion(special, 1, "Special", "test.txt");
    
    bool passed = (history.getCurrentVersion()->getContent() == special);
    
    test_result("Edge Case - Special characters", passed);
}

// ============================================================================
// TEST SUITE 6: Version Descriptions and Metadata
// ============================================================================

void test_metadata_preservation() {
    VersionHistory history;
    history.addVersion("Content", 42, "My Description", "myfile.cpp");
    
    auto ver = history.getCurrentVersion();
    
    bool passed = (ver->getVersionNumber() == 42 &&
                   ver->getDescription() == "My Description" &&
                   ver->getFilename() == "myfile.cpp");
    
    test_result("Metadata - Version metadata preservation", passed);
}

void test_timestamp_generation() {
    Version v1("Content", 1, "Desc", "file.txt");
    
    // Wait a tiny bit
    for (volatile int i = 0; i < 100000; i++);
    
    Version v2("Content", 2, "Desc", "file.txt");
    
    bool passed = (!v1.getTimestamp().empty() &&
                   !v2.getTimestamp().empty());
    
    test_result("Metadata - Timestamp generation", passed);
}

// ============================================================================
// TEST SUITE 7: Memory Management
// ============================================================================

void test_memory_cleanup() {
    // Create and destroy many versions
    for (int i = 0; i < 100; i++) {
        VersionHistory history;
        for (int j = 0; j < 10; j++) {
            history.addVersion("Content " + to_string(j), j, "Desc", "file.txt");
        }
        // history goes out of scope, destructor should clean up
    }
    
    // If no crash/leak, test passes
    bool passed = true;
    test_result("Memory - Cleanup on destruction", passed);
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main() {
    cout << "=======================================" << endl;
    cout << "   VERSION HISTORY UNIT TESTS" << endl;
    cout << "=======================================" << endl << endl;
    
    cout << "SUITE 1: Version Creation and Storage" << endl;
    cout << "-------------------------------------" << endl;
    test_version_creation();
    test_version_serialization();
    cout << endl;
    
    cout << "SUITE 2: VersionHistory Basic Operations" << endl;
    cout << "-------------------------------------" << endl;
    test_history_add_version();
    test_history_multiple_versions();
    test_history_navigation_backward();
    test_history_navigation_forward();
    test_history_navigation_boundaries();
    test_history_get_all_versions();
    test_history_clear();
    cout << endl;
    
    cout << "SUITE 3: File Persistence" << endl;
    cout << "-------------------------------------" << endl;
    test_history_save_and_load();
    test_history_persistence_navigation();
    cout << endl;
    
    cout << "SUITE 4: Doubly Linked List Integrity" << endl;
    cout << "-------------------------------------" << endl;
    test_linked_list_forward_traversal();
    test_linked_list_backward_forward();
    cout << endl;
    
    cout << "SUITE 5: Edge Cases" << endl;
    cout << "-------------------------------------" << endl;
    test_edge_empty_history_operations();
    test_edge_single_version_navigation();
    test_edge_large_content();
    test_edge_special_characters();
    cout << endl;
    
    cout << "SUITE 6: Version Descriptions and Metadata" << endl;
    cout << "-------------------------------------" << endl;
    test_metadata_preservation();
    test_timestamp_generation();
    cout << endl;
    
    cout << "SUITE 7: Memory Management" << endl;
    cout << "-------------------------------------" << endl;
    test_memory_cleanup();
    cout << endl;
    
    cout << "=======================================" << endl;
    cout << "RESULTS: " << tests_passed << " passed, " 
         << tests_failed << " failed" << endl;
    cout << "=======================================" << endl;
    
    return (tests_failed == 0) ? 0 : 1;
}