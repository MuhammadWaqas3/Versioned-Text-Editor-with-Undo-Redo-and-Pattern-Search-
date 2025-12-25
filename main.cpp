// #include "Buffer.h"
// #include "History.h"
// #include <iostream>
// #include <iomanip>
// #include <limits>

// using namespace std;

// void clearScreen() {
//     #ifdef _WIN32
//         system("cls");
//     #else
//         system("clear");
//     #endif
// }

// void displayMenu() {
//     cout << "\n====== Text Editor with Version History ======\n";
//     cout << "1.  Insert text\n";
//     cout << "2.  Delete text\n";
//     cout << "3.  Replace text\n";
//     cout << "4.  View document\n";
//     cout << "5.  Search pattern\n";
//     cout << "6.  Search and replace\n";
//     cout << "7.  Undo\n";
//     cout << "8.  Redo\n";
//     cout << "9.  Load from file\n";
//     cout << "10. Save to file\n";
//     cout << "11. Clear document\n";
//     cout << "0.  Exit\n";
//     cout << "==============================================\n";
//     cout << "Choice: ";
// }

// void displayDocument(const TextEditor& editor, int maxLines = 20) {
//     string text = editor.getText();
    
//     cout << "\n--- Document Content (Length: " << text.length() << " chars) ---\n";
    
//     if (text.empty()) {
//         cout << "[Empty document]\n";
//         return;
//     }
    
//     // Display with line numbers
//     int lineNum = 1;
//     int charCount = 0;
//     cout << setw(4) << lineNum << " | ";
    
//     for (char c : text) {
//         cout << c;
//         charCount++;
        
//         if (c == '\n') {
//             lineNum++;
//             if (lineNum <= maxLines) {
//                 cout << setw(4) << lineNum << " | ";
//             } else {
//                 cout << "... [" << (text.length() - charCount) << " more characters]\n";
//                 break;
//             }
//         }
//     }
    
//     if (text.back() != '\n') cout << '\n';
//     cout << "--- End of Document ---\n";
// }

// void handleInsert(TextEditor& editor) {
//     size_t pos;
//     cout << "Enter position to insert (0 to " << editor.getLength() << "): ";
//     cin >> pos;
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
//     cout << "Enter text to insert (end with empty line):\n";
//     string text, line;
//     while (getline(cin, line) && !line.empty()) {
//         if (!text.empty()) text += '\n';
//         text += line;
//     }
    
//     editor.insert(pos, text);
//     cout << "✓ Inserted " << text.length() << " characters.\n";
// }

// void handleDelete(TextEditor& editor) {
//     size_t pos, length;
//     cout << "Enter start position: ";
//     cin >> pos;
//     cout << "Enter length to delete: ";
//     cin >> length;
    
//     editor.deleteText(pos, length);
//     cout << "✓ Deleted text.\n";
// }

// void handleReplace(TextEditor& editor) {
//     size_t pos, length;
//     cout << "Enter start position: ";
//     cin >> pos;
//     cout << "Enter length to replace: ";
//     cin >> length;
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
//     cout << "Enter replacement text: ";
//     string text;
//     getline(cin, text);
    
//     editor.replace(pos, length, text);
//     cout << "✓ Replaced text.\n";
// }

// void handleSearch(const TextEditor& editor) {
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     cout << "Enter search pattern: ";
//     string pattern;
//     getline(cin, pattern);
    
//     auto positions = editor.search(pattern);
    
//     if (positions.empty()) {
//         cout << "Pattern not found.\n";
//     } else {
//         cout << "Found " << positions.size() << " occurrences at positions: ";
//         for (size_t i = 0; i < positions.size() && i < 10; i++) {
//             cout << positions[i] << " ";
//         }
//         if (positions.size() > 10) {
//             cout << "... (+" << (positions.size() - 10) << " more)";
//         }
//         cout << '\n';
//     }
// }

// void handleSearchReplace(TextEditor& editor) {
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     cout << "Enter search pattern: ";
//     string pattern;
//     getline(cin, pattern);
    
//     cout << "Enter replacement text: ";
//     string replacement;
//     getline(cin, replacement);
    
//     int count = editor.searchAndReplace(pattern, replacement);
//     cout << "✓ Replaced " << count << " occurrences.\n";
// }

// void handleLoadFile(TextEditor& editor) {
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     cout << "Enter filename to load: ";
//     string filename;
//     getline(cin, filename);
    
//     if (editor.loadFromFile(filename)) {
//         cout << "✓ File loaded successfully.\n";
//         cout << "  Loaded " << editor.getLength() << " characters.\n";
//     } else {
//         cout << "✗ Failed to load file.\n";
//     }
// }

// void handleSaveFile(TextEditor& editor) {
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     cout << "Enter filename to save (or press Enter for current file): ";
//     string filename;
//     getline(cin, filename);
    
//     if (editor.saveToFile(filename)) {
//         cout << "✓ File saved successfully.\n";
//     } else {
//         cout << "✗ Failed to save file.\n";
//     }
// }

// int main() {
//     TextEditor editor;
//     int choice;
    
//     cout << "Welcome to Text Editor!\n";
//     cout << "A console-based text editor with undo/redo and pattern search.\n";
    
//     while (true) {
//         displayMenu();
        
//         if (!(cin >> choice)) {
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             cout << "Invalid input. Please enter a number.\n";
//             continue;
//         }
        
//         switch (choice) {
//             case 1:
//                 handleInsert(editor);
//                 break;
//             case 2:
//                 handleDelete(editor);
//                 break;
//             case 3:
//                 handleReplace(editor);
//                 break;
//             case 4:
//                 displayDocument(editor);
//                 break;
//             case 5:
//                 handleSearch(editor);
//                 break;
//             case 6:
//                 handleSearchReplace(editor);
//                 break;
//             case 7:
//                 if (editor.undo()) {
//                     cout << "✓ Undo successful.\n";
//                 } else {
//                     cout << "Nothing to undo.\n";
//                 }
//                 break;
//             case 8:
//                 if (editor.redo()) {
//                     cout << "✓ Redo successful.\n";
//                 } else {
//                     cout << "Nothing to redo.\n";
//                 }
//                 break;
//             case 9:
//                 handleLoadFile(editor);
//                 break;
//             case 10:
//                 handleSaveFile(editor);
//                 break;
//             case 11:
//                 editor.clear();
//                 cout << "✓ Document cleared.\n";
//                 break;
//             case 0:
//                 if (editor.isModified()) {
//                     char save;
//                     cout << "Document has unsaved changes. Save before exit? (y/n): ";
//                     cin >> save;
//                     if (save == 'y' || save == 'Y') {
//                         handleSaveFile(editor);
//                     }
//                 }
//                 cout << "Goodbye!\n";
//                 return 0;
//             default:
//                 cout << "Invalid choice. Please try again.\n";
//         }
        
//         cout << "\nPress Enter to continue...";
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         cin.get();
//     }
    
//     return 0;
// }



// // #include "Buffer.h"
// // #include "History.h"
// // #include "SnapshotManager.h"
// // #include "AdvancedSearch.h"
// // #include <iostream>
// // #include <iomanip>
// // #include <limits>

// // using namespace std;

// // class CompleteTextEditor {
// // private:
// //     TextEditor editor;
// //     VersionHistory versionHistory;
// //     SnapshotManager snapshotManager;
// //     AdvancedSearchEngine advancedSearch;
// //     SearchHistory searchHistory;
    
// // public:
// //     CompleteTextEditor() {}
    
// //     TextEditor& getEditor() { return editor; }
// //     VersionHistory& getVersionHistory() { return versionHistory; }
// //     SnapshotManager& getSnapshotManager() { return snapshotManager; }
// //     AdvancedSearchEngine& getSearchEngine() { return advancedSearch; }
// //     SearchHistory& getSearchHistory() { return searchHistory; }
// // };

// // void clearScreen() {
// //     #ifdef _WIN32
// //         system("cls");
// //     #else
// //         system("clear");
// //     #endif
// // }

// // void displayMainMenu() {
// //     cout << "\n╔════════════════════════════════════════════════╗\n";
// //     cout << "║     VERSIONED TEXT EDITOR - Main Menu         ║\n";
// //     cout << "╚════════════════════════════════════════════════╝\n";
    
// //     cout << "\n┌─ TEXT OPERATIONS ─────────────────────────────┐\n";
// //     cout << "│ 1.  Insert text                               │\n";
// //     cout << "│ 2.  Delete text                               │\n";
// //     cout << "│ 3.  Replace text                              │\n";
// //     cout << "│ 4.  View document                             │\n";
// //     cout << "└───────────────────────────────────────────────┘\n";
    
// //     cout << "\n┌─ HISTORY (Undo/Redo) ─────────────────────────┐\n";
// //     cout << "│ 5.  Undo (Ctrl+Z)                             │\n";
// //     cout << "│ 6.  Redo (Ctrl+Y)                             │\n";
// //     cout << "└───────────────────────────────────────────────┘\n";
    
// //     cout << "\n┌─ PERSISTENT SNAPSHOTS ────────────────────────┐\n";
// //     cout << "│ 7.  Create snapshot                           │\n";
// //     cout << "│ 8.  List all snapshots                        │\n";
// //     cout << "│ 9.  Restore snapshot                          │\n";
// //     cout << "│ 10. Compare snapshots                         │\n";
// //     cout << "│ 11. Delete snapshot                           │\n";
// //     cout << "│ 12. Display snapshot tree                     │\n";
// //     cout << "└───────────────────────────────────────────────┘\n";
    
// //     cout << "\n┌─ PATTERN SEARCH (Ctrl+F) ─────────────────────┐\n";
// //     cout << "│ 13. Basic search                              │\n";
// //     cout << "│ 14. Advanced search (with options)            │\n";
// //     cout << "│ 15. Search and replace                        │\n";
// //     cout << "│ 16. Multi-pattern search                      │\n";
// //     cout << "│ 17. Fuzzy search (approximate)                │\n";
// //     cout << "│ 18. Find with context                         │\n";
// //     cout << "│ 19. Search history                            │\n";
// //     cout << "│ 20. Search in snapshots                       │\n";
// //     cout << "└───────────────────────────────────────────────┘\n";
    
// //     cout << "\n┌─ FILE OPERATIONS ─────────────────────────────┐\n";
// //     cout << "│ 21. Load from file                            │\n";
// //     cout << "│ 22. Save to file                              │\n";
// //     cout << "│ 23. Clear document                            │\n";
// //     cout << "└───────────────────────────────────────────────┘\n";
    
// //     cout << "\n 0.  Exit\n";
// //     cout << "════════════════════════════════════════════════\n";
// //     cout << "Choice: ";
// // }

// // void displayDocument(const TextEditor& editor, int maxLines = 25) {
// //     string text = editor.getText();
    
// //     cout << "\n┌─ Document Content ─────────────────────────────┐\n";
// //     cout << "│ Length: " << text.length() << " characters" << string(28, ' ') << "│\n";
// //     cout << "└────────────────────────────────────────────────┘\n";
    
// //     if (text.empty()) {
// //         cout << "[Empty document]\n";
// //         return;
// //     }
    
// //     int lineNum = 1;
// //     int charCount = 0;
// //     cout << setw(4) << lineNum << " │ ";
    
// //     for (char c : text) {
// //         cout << c;
// //         charCount++;
        
// //         if (c == '\n') {
// //             lineNum++;
// //             if (lineNum <= maxLines) {
// //                 cout << setw(4) << lineNum << " │ ";
// //             } else {
// //                 cout << "... [" << (text.length() - charCount) << " more characters]\n";
// //                 break;
// //             }
// //         }
// //     }
    
// //     if (!text.empty() && text.back() != '\n') cout << '\n';
// //     cout << "─────────────────────────────────────────────────\n";
// // }

// // // Text Operations
// // void handleInsert(TextEditor& editor) {
// //     size_t pos;
// //     cout << "\nEnter position to insert (0 to " << editor.getLength() << "): ";
// //     cin >> pos;
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "Enter text to insert (end with empty line):\n";
// //     string text, line;
// //     while (getline(cin, line) && !line.empty()) {
// //         if (!text.empty()) text += '\n';
// //         text += line;
// //     }
    
// //     if (!text.empty()) {
// //         editor.insert(pos, text);
// //         cout << "✓ Inserted " << text.length() << " characters.\n";
// //     }
// // }

// // void handleDelete(TextEditor& editor) {
// //     size_t pos, length;
// //     cout << "\nEnter start position: ";
// //     cin >> pos;
// //     cout << "Enter length to delete: ";
// //     cin >> length;
    
// //     editor.deleteText(pos, length);
// //     cout << "✓ Deleted text.\n";
// // }

// // void handleReplace(TextEditor& editor) {
// //     size_t pos, length;
// //     cout << "\nEnter start position: ";
// //     cin >> pos;
// //     cout << "Enter length to replace: ";
// //     cin >> length;
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "Enter replacement text: ";
// //     string text;
// //     getline(cin, text);
    
// //     editor.replace(pos, length, text);
// //     cout << "✓ Replaced text.\n";
// // }

// // // Snapshot Operations
// // void handleCreateSnapshot(CompleteTextEditor& edi) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "\nEnter snapshot name: ";
// //     string name;
// //     getline(cin, name);
    
// //     cout << "Enter description (optional): ";
// //     string desc;
// //     getline(cin, desc);
    
// //     string content = edi.getEditor().getText();
// //     auto snapshot = edi.getSnapshotManager().createSnapshot(name, content, desc);
    
// //     if (snapshot) {
// //         cout << "✓ Snapshot created: [" << snapshot->getId() << "] " 
// //              << snapshot->getName() << endl;
// //     }
// // }

// // void handleRestoreSnapshot(CompleteTextEditor& edi) {
// //     cout << "\nRestore by (1) ID or (2) Name? ";
// //     int choice;
// //     cin >> choice;
    
// //     shared_ptr<Snapshot> snapshot = nullptr;
    
// //     if (choice == 1) {
// //         int id;
// //         cout << "Enter snapshot ID: ";
// //         cin >> id;
// //         snapshot = edi.getSnapshotManager().getSnapshot(id);
// //     } else {
// //         cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //         cout << "Enter snapshot name: ";
// //         string name;
// //         getline(cin, name);
// //         snapshot = edi.getSnapshotManager().getSnapshot(name);
// //     }
    
// //     if (snapshot) {
// //         string content = snapshot->reconstructContent();
// //         edi.getEditor().clear();
// //         edi.getEditor().insert(0, content);
// //         cout << "✓ Restored snapshot: " << snapshot->getName() << endl;
// //     } else {
// //         cout << "✗ Snapshot not found.\n";
// //     }
// // }

// // void handleCompareSnapshots(CompleteTextEditor& edi) {
// //     int id1, id2;
// //     cout << "\nEnter first snapshot ID: ";
// //     cin >> id1;
// //     cout << "Enter second snapshot ID: ";
// //     cin >> id2;
    
// //     edi.getSnapshotManager().compareSnapshots(id1, id2);
// // }

// // void handleDeleteSnapshot(CompleteTextEditor& edi) {
// //     int id;
// //     cout << "\nEnter snapshot ID to delete: ";
// //     cin >> id;
    
// //     if (edi.getSnapshotManager().deleteSnapshot(id)) {
// //         cout << "✓ Snapshot deleted.\n";
// //     } else {
// //         cout << "✗ Failed to delete snapshot.\n";
// //     }
// // }

// // // Search Operations
// // void handleBasicSearch(CompleteTextEditor& edi) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //     cout << "\nEnter search pattern: ";
// //     string pattern;
// //     getline(cin, pattern);
    
// //     auto positions = edi.getEditor().search(pattern);
// //     edi.getSearchHistory().addSearch(pattern, positions.size());
    
// //     if (positions.empty()) {
// //         cout << "Pattern not found.\n";
// //     } else {
// //         cout << "Found " << positions.size() << " occurrences at positions:\n";
// //         for (size_t i = 0; i < positions.size() && i < 20; i++) {
// //             cout << positions[i];
// //             if (i < positions.size() - 1 && i < 19) cout << ", ";
// //         }
// //         if (positions.size() > 20) {
// //             cout << "\n... (+" << (positions.size() - 20) << " more)";
// //         }
// //         cout << '\n';
// //     }
// // }

// // void handleAdvancedSearch(CompleteTextEditor& edi) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "\nEnter search pattern: ";
// //     string pattern;
// //     getline(cin, pattern);
    
// //     SearchOptions options;
    
// //     cout << "Case sensitive? (y/n): ";
// //     char resp;
// //     cin >> resp;
// //     options.caseSensitive = (resp == 'y' || resp == 'Y');
    
// //     cout << "Whole word only? (y/n): ";
// //     cin >> resp;
// //     options.wholeWord = (resp == 'y' || resp == 'Y');
    
// //     cout << "Context lines (default 1): ";
// //     int ctx;
// //     cin >> ctx;
// //     options.contextLines = ctx;
    
// //     string text = edi.getEditor().getText();
// //     auto results = edi.getSearchEngine().search(text, pattern, options);
// //     edi.getSearchHistory().addSearch(pattern, results.size());
    
// //     if (results.empty()) {
// //         cout << "\nPattern not found.\n";
// //     } else {
// //         cout << "\nFound " << results.size() << " matches:\n\n";
// //         for (size_t i = 0; i < results.size() && i < 10; i++) {
// //             cout << "Match " << (i + 1) << " - Line " << results[i].line 
// //                  << ", Col " << results[i].column << ":\n";
// //             cout << results[i].context << "\n";
// //             cout << string(40, '-') << endl;
// //         }
// //         if (results.size() > 10) {
// //             cout << "... (+" << (results.size() - 10) << " more matches)\n";
// //         }
// //     }
// // }

// // void handleSearchAndReplace(CompleteTextEditor& edi) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "\nEnter search pattern: ";
// //     string pattern;
// //     getline(cin, pattern);
    
// //     cout << "Enter replacement text: ";
// //     string replacement;
// //     getline(cin, replacement);
    
// //     int count = edi.getEditor().searchAndReplace(pattern, replacement);
// //     cout << "✓ Replaced " << count << " occurrences.\n";
// // }

// // void handleMultiPatternSearch(CompleteTextEditor& edi) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "\nEnter number of patterns: ";
// //     int n;
// //     cin >> n;
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     vector<string> patterns;
// //     for (int i = 0; i < n; i++) {
// //         cout << "Pattern " << (i + 1) << ": ";
// //         string pattern;
// //         getline(cin, pattern);
// //         patterns.push_back(pattern);
// //     }
    
// //     string text = edi.getEditor().getText();
// //     auto results = edi.getSearchEngine().multiPatternSearch(text, patterns);
    
// //     cout << "\n=== Multi-Pattern Search Results ===\n";
// //     for (const auto& result : results) {
// //         cout << "Pattern '" << result.first << "': " 
// //              << result.second.size() << " matches\n";
// //     }
// // }

// // void handleFuzzySearch(CompleteTextEditor& edi) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "\nEnter search pattern: ";
// //     string pattern;
// //     getline(cin, pattern);
    
// //     cout << "Max edit distance (default 2): ";
// //     int maxDist;
// //     cin >> maxDist;
    
// //     string text = edi.getEditor().getText();
// //     auto positions = edi.getSearchEngine().fuzzySearch(text, pattern, maxDist);
    
// //     cout << "\nFound " << positions.size() << " approximate matches.\n";
// // }

// // void handleFindWithContext(CompleteTextEditor& edi) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "\nEnter search pattern: ";
// //     string pattern;
// //     getline(cin, pattern);
    
// //     cout << "Context lines (default 2): ";
// //     int lines;
// //     cin >> lines;
    
// //     string text = edi.getEditor().getText();
// //     auto results = edi.getSearchEngine().findWithContext(text, pattern, lines);
    
// //     if (results.empty()) {
// //         cout << "\nPattern not found.\n";
// //     } else {
// //         cout << "\nFound " << results.size() << " matches with context:\n\n";
// //         for (size_t i = 0; i < results.size() && i < 5; i++) {
// //             cout << "Match " << (i + 1) << " at Line " << results[i].line << ":\n";
// //             cout << results[i].context << "\n";
// //             cout << string(50, '=') << endl;
// //         }
// //     }
// // }

// // void handleSearchInSnapshots(CompleteTextEditor& edi) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "\nEnter search pattern: ";
// //     string pattern;
// //     getline(cin, pattern);
    
// //     auto snapshots = edi.getSnapshotManager().getAllSnapshots();
    
// //     if (snapshots.empty()) {
// //         cout << "No snapshots available.\n";
// //         return;
// //     }
    
// //     cout << "\n=== Searching in all snapshots for: '" << pattern << "' ===\n";
    
// //     bool found = false;
// //     for (const auto& snap : snapshots) {
// //         string content = snap->reconstructContent();
// //         auto positions = edi.getSearchEngine().rollingHashSearch(content, pattern);
        
// //         if (!positions.empty()) {
// //             cout << "Snapshot [" << snap->getId() << "] '" << snap->getName() 
// //                  << "': Found " << positions.size() << " occurrences\n";
// //             found = true;
// //         }
// //     }
    
// //     if (!found) {
// //         cout << "Pattern not found in any snapshot.\n";
// //     }
// // }

// // // File Operations
// // void handleLoadFile(TextEditor& editor) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //     cout << "\nEnter filename to load: ";
// //     string filename;
// //     getline(cin, filename);
    
// //     if (editor.loadFromFile(filename)) {
// //         cout << "✓ File loaded successfully.\n";
// //         cout << "  Loaded " << editor.getLength() << " characters.\n";
// //     } else {
// //         cout << "✗ Failed to load file.\n";
// //     }
// // }

// // void handleSaveFile(TextEditor& editor) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //     cout << "\nEnter filename to save (or press Enter for current file): ";
// //     string filename;
// //     getline(cin, filename);
    
// //     if (editor.saveToFile(filename)) {
// //         cout << "✓ File saved successfully.\n";
// //     } else {
// //         cout << "✗ Failed to save file.\n";
// //     }
// // }

// // int main() {
// //     CompleteTextEditor editor;
// //     int choice;
    
// //     cout << "╔════════════════════════════════════════════════╗\n";
// //     cout << "║   VERSIONED TEXT EDITOR                        ║\n";
// //     cout << "║   Data Structures & Algorithms Project         ║\n";
// //     cout << "║   Team: Ahsan & Waqas                          ║\n";
// //     cout << "║   Fall 2025                                    ║\n";
// //     cout << "╚════════════════════════════════════════════════╝\n";
// //     cout << "\nFeatures:\n";
// //     cout << "✓ Buffer Management (Chunked Array)\n";
// //     cout << "✓ Undo/Redo (Custom Stack)\n";
// //     cout << "✓ Persistent Snapshots (Linked List with Deltas)\n";
// //     cout << "✓ Pattern Search (Rolling Hash + KMP + Boyer-Moore)\n";
    
// //     while (true) {
// //         displayMainMenu();
        
// //         if (!(cin >> choice)) {
// //             cin.clear();
// //             cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //             cout << "Invalid input. Please enter a number.\n";
// //             continue;
// //         }
        
// //         switch (choice) {
// //             case 1: handleInsert(editor.getEditor()); break;
// //             case 2: handleDelete(editor.getEditor()); break;
// //             case 3: handleReplace(editor.getEditor()); break;
// //             case 4: displayDocument(editor.getEditor()); break;
            
// //             case 5:
// //                 if (editor.getEditor().undo()) {
// //                     cout << "✓ Undo successful.\n";
// //                 } else {
// //                     cout << "Nothing to undo.\n";
// //                 }
// //                 break;
            
// //             case 6:
// //                 if (editor.getEditor().redo()) {
// //                     cout << "✓ Redo successful.\n";
// //                 } else {
// //                     cout << "Nothing to redo.\n";
// //                 }
// //                 break;
            
// //             case 7: handleCreateSnapshot(editor); break;
// //             case 8: editor.getSnapshotManager().displaySnapshotList(); break;
// //             case 9: handleRestoreSnapshot(editor); break;
// //             case 10: handleCompareSnapshots(editor); break;
// //             case 11: handleDeleteSnapshot(editor); break;
// //             case 12: editor.getSnapshotManager().displaySnapshotTree(); break;
            
// //             case 13: handleBasicSearch(editor); break;
// //             case 14: handleAdvancedSearch(editor); break;
// //             case 15: handleSearchAndReplace(editor); break;
// //             case 16: handleMultiPatternSearch(editor); break;
// //             case 17: handleFuzzySearch(editor); break;
// //             case 18: handleFindWithContext(editor); break;
// //             case 19: editor.getSearchHistory().displayHistory(); break;
// //             case 20: handleSearchInSnapshots(editor); break;
            
// //             case 21: handleLoadFile(editor.getEditor()); break;
// //             case 22: handleSaveFile(editor.getEditor()); break;
// //             case 23:
// //                 editor.getEditor().clear();
// //                 cout << "✓ Document cleared.\n";
// //                 break;
            
// //             case 0:
// //                 if (editor.getEditor().isModified()) {
// //                     char save;
// //                     cout << "Document has unsaved changes. Save before exit? (y/n): ";
// //                     cin >> save;
// //                     if (save == 'y' || save == 'Y') {
// //                         handleSaveFile(editor.getEditor());
// //                     }
// //                 }
// //                 cout << "\n╔════════════════════════════════════════════════╗\n";
// //                 cout << "║   Thank you for using Versioned Text Editor!  ║\n";
// //                 cout << "║   Project by: Ahsan & Waqas                    ║\n";
// //                 cout << "╚════════════════════════════════════════════════╝\n";
// //                 return 0;
            
// //             default:
// //                 cout << "Invalid choice. Please try again.\n";
// //         }
        
// //         cout << "\n⏎ Press Enter to continue...";
// //         cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //         cin.get();
// //     }
    
// //     return 0;
// // }











/////////////////////////////////////////////////////////////////////////////////////////////



// // #include "Buffer.h"
// // #include "History.h"
// // #include "AdvancedSearch.h"
// // #include <iostream>
// // #include <iomanip>
// // #include <limits>

// // using namespace std;

// // void clearScreen() {
// //     #ifdef _WIN32
// //         system("cls");
// //     #else
// //         system("clear");
// //     #endif
// // }

// // void displayMenu() {
// //     cout << "\n====== Text Editor with Version History ======\n";
// //     cout << "1.  Insert text\n";
// //     cout << "2.  Delete text\n";
// //     cout << "3.  Replace text\n";
// //     cout << "4.  View document\n";
// //     cout << "5.  Search pattern\n";
// //     cout << "6.  Search and replace\n";
// //     cout << "7.  Undo\n";
// //     cout << "8.  Redo\n";
// //     cout << "9.  Load from file\n";
// //     cout << "10. Save to file\n";
// //     cout << "11. Clear document\n";
// //     cout << "0.  Exit\n";
// //     cout << "==============================================\n";
// //     cout << "Choice: ";
// // }

// // void displayDocument(const TextEditor& editor, int maxLines = 20) {
// //     string text = editor.getText();
    
// //     cout << "\n--- Document Content (Length: " << text.length() << " chars) ---\n";
    
// //     if (text.empty()) {
// //         cout << "[Empty document]\n";
// //         return;
// //     }
    
// //     // Display with line numbers
// //     int lineNum = 1;
// //     int charCount = 0;
// //     cout << setw(4) << lineNum << " | ";
    
// //     for (char c : text) {
// //         cout << c;
// //         charCount++;
        
// //         if (c == '\n') {
// //             lineNum++;
// //             if (lineNum <= maxLines) {
// //                 cout << setw(4) << lineNum << " | ";
// //             } else {
// //                 cout << "... [" << (text.length() - charCount) << " more characters]\n";
// //                 break;
// //             }
// //         }
// //     }
    
// //     if (text.back() != '\n') cout << '\n';
// //     cout << "--- End of Document ---\n";
// // }

// // void handleInsert(TextEditor& editor) {
// //     size_t pos;
// //     cout << "Enter position to insert (0 to " << editor.getLength() << "): ";
// //     cin >> pos;
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "Enter text to insert (end with empty line):\n";
// //     string text, line;
// //     while (getline(cin, line) && !line.empty()) {
// //         if (!text.empty()) text += '\n';
// //         text += line;
// //     }
    
// //     editor.insert(pos, text);
// //     cout << "✓ Inserted " << text.length() << " characters.\n";
// // }

// // void handleDelete(TextEditor& editor) {
// //     size_t pos, length;
// //     cout << "Enter start position: ";
// //     cin >> pos;
// //     cout << "Enter length to delete: ";
// //     cin >> length;
    
// //     editor.deleteText(pos, length);
// //     cout << "✓ Deleted text.\n";
// // }

// // void handleReplace(TextEditor& editor) {
// //     size_t pos, length;
// //     cout << "Enter start position: ";
// //     cin >> pos;
// //     cout << "Enter length to replace: ";
// //     cin >> length;
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
// //     cout << "Enter replacement text: ";
// //     string text;
// //     getline(cin, text);
    
// //     editor.replace(pos, length, text);
// //     cout << "✓ Replaced text.\n";
// // }

// // void handleSearch(const TextEditor& editor) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //     cout << "Enter search pattern: ";
// //     string pattern;
// //     getline(cin, pattern);
    
// //     auto positions = editor.search(pattern);
    
// //     if (positions.empty()) {
// //         cout << "Pattern not found.\n";
// //     } else {
// //         cout << "Found " << positions.size() << " occurrences at positions: ";
// //         for (size_t i = 0; i < positions.size() && i < 10; i++) {
// //             cout << positions[i] << " ";
// //         }
// //         if (positions.size() > 10) {
// //             cout << "... (+" << (positions.size() - 10) << " more)";
// //         }
// //         cout << '\n';
// //     }
// // }

// // void handleSearchReplace(TextEditor& editor) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //     cout << "Enter search pattern: ";
// //     string pattern;
// //     getline(cin, pattern);
    
// //     cout << "Enter replacement text: ";
// //     string replacement;
// //     getline(cin, replacement);
    
// //     int count = editor.searchAndReplace(pattern, replacement);
// //     cout << "✓ Replaced " << count << " occurrences.\n";
// // }

// // void handleLoadFile(TextEditor& editor) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //     cout << "Enter filename to load: ";
// //     string filename;
// //     getline(cin, filename);
    
// //     if (editor.loadFromFile(filename)) {
// //         cout << "✓ File loaded successfully.\n";
// //         cout << "  Loaded " << editor.getLength() << " characters.\n";
// //     } else {
// //         cout << "✗ Failed to load file.\n";
// //     }
// // }

// // void handleSaveFile(TextEditor& editor) {
// //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //     cout << "Enter filename to save (or press Enter for current file): ";
// //     string filename;
// //     getline(cin, filename);
    
// //     if (editor.saveToFile(filename)) {
// //         cout << "✓ File saved successfully.\n";
// //     } else {
// //         cout << "✗ Failed to save file.\n";
// //     }
// // }

// // int main() {
// //     TextEditor editor;
// //     int choice;
    
// //     cout << "Welcome to Text Editor!\n";
// //     cout << "A console-based text editor with undo/redo and pattern search.\n";
    
// //     while (true) {
// //         displayMenu();
        
// //         if (!(cin >> choice)) {
// //             cin.clear();
// //             cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //             cout << "Invalid input. Please enter a number.\n";
// //             continue;
// //         }
        
// //         switch (choice) {
// //             case 1:
// //                 handleInsert(editor);
// //                 break;
// //             case 2:
// //                 handleDelete(editor);
// //                 break;
// //             case 3:
// //                 handleReplace(editor);
// //                 break;
// //             case 4:
// //                 displayDocument(editor);
// //                 break;
// //             case 5:
// //                 handleSearch(editor);
// //                 break;
// //             case 6:
// //                 handleSearchReplace(editor);
// //                 break;
// //             case 7:
// //                 if (editor.undo()) {
// //                     cout << "✓ Undo successful.\n";
// //                 } else {
// //                     cout << "Nothing to undo.\n";
// //                 }
// //                 break;
// //             case 8:
// //                 if (editor.redo()) {
// //                     cout << "✓ Redo successful.\n";
// //                 } else {
// //                     cout << "Nothing to redo.\n";
// //                 }
// //                 break;
// //             case 9:
// //                 handleLoadFile(editor);
// //                 break;
// //             case 10:
// //                 handleSaveFile(editor);
// //                 break;
// //             case 11:
// //                 editor.clear();
// //                 cout << "✓ Document cleared.\n";
// //                 break;
// //             case 0:
// //                 if (editor.isModified()) {
// //                     char save;
// //                     cout << "Document has unsaved changes. Save before exit? (y/n): ";
// //                     cin >> save;
// //                     if (save == 'y' || save == 'Y') {
// //                         handleSaveFile(editor);
// //                     }
// //                 }
// //                 cout << "Goodbye!\n";
// //                 return 0;
// //             default:
// //                 cout << "Invalid choice. Please try again.\n";
// //         }
        
// //         cout << "\nPress Enter to continue...";
// //         cin.ignore(numeric_limits<streamsize>::max(), '\n');
// //         cin.get();
// //     }
    
// //     return 0;
// // }











// #include "Buffer.h"
// #include "History.h"
// #include "AdvancedSearch.h"
// #include <iostream>
// #include <iomanip>
// #include <limits>

// using namespace std;

// void clearScreen() {
//     #ifdef _WIN32
//         system("cls");
//     #else
//         system("clear");
//     #endif
// }

// void displayMenu() {
//     cout << "\n====== Text Editor with Version History ======\n";
//     cout << "1.  Insert text\n";
//     cout << "2.  Delete text\n";
//     cout << "3.  Replace text\n";
//     cout << "4.  View document\n";
//     cout << "5.  Search pattern\n";
//     cout << "6.  Search and replace\n";
//     cout << "7.  Undo\n";
//     cout << "8.  Redo\n";
//     cout << "9.  Load from file\n";
//     cout << "10. Save to file\n";
//     cout << "11. Clear document\n";
//     cout << "12. Show document info\n";
//     cout << "0.  Exit\n";
//     cout << "==============================================\n";
//     cout << "Choice: ";
// }

// void displayDocument(const TextEditor& editor) {
//     string text = editor.getText();
    
//     cout << "\n==================== DOCUMENT VIEW ====================\n";
//     cout << "Length: " << text.length() << " characters\n";
//     cout << "=======================================================\n";
    
//     if (text.empty()) {
//         cout << "[Empty document]\n";
//         cout << "=======================================================\n";
//         return;
//     }
    
//     // Display with line numbers - SHOW ALL LINES
//     int lineNum = 1;
//     cout << setw(4) << lineNum << " | ";
    
//     for (size_t i = 0; i < text.length(); i++) {
//         char c = text[i];
//         cout << c;
        
//         if (c == '\n') {
//             lineNum++;
//             // Show all lines, no limit
//             cout << setw(4) << lineNum << " | ";
//         }
//     }
    
//     // Add newline if text doesn't end with one
//     if (!text.empty() && text.back() != '\n') {
//         cout << '\n';
//     }
    
//     cout << "=======================================================\n";
//     cout << "Total lines: " << lineNum << "\n";
//     cout << "=======================================================\n";
// }

// void showDocumentInfo(const TextEditor& editor) {
//     cout << "\n=============== DOCUMENT INFORMATION ===============\n";
//     cout << "Total characters: " << editor.getLength() << "\n";
//     cout << "Modified: " << (editor.isModified() ? "Yes" : "No") << "\n";
//     cout << "Can undo: " << (editor.canUndo() ? "Yes" : "No") << "\n";
//     cout << "Can redo: " << (editor.canRedo() ? "Yes" : "No") << "\n";
    
//     if (!editor.getFilename().empty()) {
//         cout << "Current file: " << editor.getFilename() << "\n";
//     } else {
//         cout << "Current file: [Not saved yet]\n";
//     }
    
//     cout << "====================================================\n";
// }

// void handleInsert(TextEditor& editor) {
//     size_t pos;
//     cout << "Enter position to insert (0 to " << editor.getLength() << "): ";
//     cin >> pos;
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
//     cout << "Enter text to insert (type END on a new line to finish):\n";
//     string text, line;
    
//     while (getline(cin, line)) {
//         if (line == "END") break;
//         if (!text.empty()) text += '\n';
//         text += line;
//     }
    
//     if (!text.empty()) {
//         editor.insert(pos, text);
//         cout << "✓ Inserted " << text.length() << " characters.\n";
//     } else {
//         cout << "No text inserted.\n";
//     }
// }

// void handleDelete(TextEditor& editor) {
//     size_t pos, length;
//     cout << "Enter start position (0 to " << editor.getLength() << "): ";
//     cin >> pos;
//     cout << "Enter length to delete: ";
//     cin >> length;
    
//     if (pos >= editor.getLength()) {
//         cout << "Invalid position!\n";
//         return;
//     }
    
//     editor.deleteText(pos, length);
//     cout << "✓ Deleted text.\n";
// }

// void handleReplace(TextEditor& editor) {
//     size_t pos, length;
//     cout << "Enter start position (0 to " << editor.getLength() << "): ";
//     cin >> pos;
//     cout << "Enter length to replace: ";
//     cin >> length;
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
//     if (pos >= editor.getLength()) {
//         cout << "Invalid position!\n";
//         return;
//     }
    
//     cout << "Enter replacement text: ";
//     string text;
//     getline(cin, text);
    
//     editor.replace(pos, length, text);
//     cout << "✓ Replaced text.\n";
// }

// void handleSearch(const TextEditor& editor) {
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     cout << "Enter search pattern: ";
//     string pattern;
//     getline(cin, pattern);
    
//     if (pattern.empty()) {
//         cout << "Pattern cannot be empty!\n";
//         return;
//     }
    
//     auto positions = editor.search(pattern);
    
//     if (positions.empty()) {
//         cout << "Pattern not found.\n";
//     } else {
//         cout << "Found " << positions.size() << " occurrence(s) at position(s): ";
//         for (size_t i = 0; i < positions.size() && i < 20; i++) {
//             cout << positions[i];
//             if (i < positions.size() - 1 && i < 19) cout << ", ";
//         }
//         if (positions.size() > 20) {
//             cout << " ... (+" << (positions.size() - 20) << " more)";
//         }
//         cout << '\n';
        
//         // Show first match context
//         if (!positions.empty()) {
//             string text = editor.getText();
//             size_t start = (positions[0] > 20) ? positions[0] - 20 : 0;
//             size_t end = min(positions[0] + pattern.length() + 20, text.length());
            
//             cout << "\nFirst match context:\n";
//             cout << "..." << text.substr(start, end - start) << "...\n";
//         }
//     }
// }

// void handleSearchReplace(TextEditor& editor) {
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     cout << "Enter search pattern: ";
//     string pattern;
//     getline(cin, pattern);
    
//     if (pattern.empty()) {
//         cout << "Pattern cannot be empty!\n";
//         return;
//     }
    
//     cout << "Enter replacement text: ";
//     string replacement;
//     getline(cin, replacement);
    
//     // First search to show what will be replaced
//     auto positions = editor.search(pattern);
//     if (positions.empty()) {
//         cout << "Pattern not found. Nothing to replace.\n";
//         return;
//     }
    
//     cout << "Found " << positions.size() << " occurrence(s). Replace all? (y/n): ";
//     char confirm;
//     cin >> confirm;
    
//     if (confirm == 'y' || confirm == 'Y') {
//         int count = editor.searchAndReplace(pattern, replacement);
//         cout << "✓ Replaced " << count << " occurrence(s).\n";
//     } else {
//         cout << "Replace cancelled.\n";
//     }
// }

// void handleLoadFile(TextEditor& editor) {
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
//     if (editor.isModified()) {
//         cout << "Warning: Current document has unsaved changes!\n";
//         cout << "Continue loading? (y/n): ";
//         char confirm;
//         cin >> confirm;
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
//         if (confirm != 'y' && confirm != 'Y') {
//             cout << "Load cancelled.\n";
//             return;
//         }
//     }
    
//     cout << "Enter filename to load: ";
//     string filename;
//     getline(cin, filename);
    
//     if (editor.loadFromFile(filename)) {
//         cout << "✓ File loaded successfully.\n";
//         cout << "  Loaded " << editor.getLength() << " characters.\n";
//     } else {
//         cout << "✗ Failed to load file. Check if file exists.\n";
//     }
// }

// void handleSaveFile(TextEditor& editor) {
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
//     string filename;
    
//     // If file already has a name, show it
//     if (!editor.getFilename().empty()) {
//         cout << "Current file: " << editor.getFilename() << "\n";
//         cout << "Press Enter to save to same file, or type new filename: ";
//     } else {
//         cout << "Enter filename to save: ";
//     }
    
//     getline(cin, filename);
    
//     // If empty and has current filename, use that
//     if (filename.empty() && !editor.getFilename().empty()) {
//         filename = editor.getFilename();
//     }
    
//     if (filename.empty()) {
//         cout << "✗ No filename specified.\n";
//         return;
//     }
    
//     if (editor.saveToFile(filename)) {
//         cout << "✓ File saved successfully to: " << filename << "\n";
//         cout << "  Saved " << editor.getLength() << " characters.\n";
//     } else {
//         cout << "✗ Failed to save file. Check permissions.\n";
//     }
// }

// int main() {
//     TextEditor editor;
//     int choice;
    
//     cout << "╔════════════════════════════════════════════════════╗\n";
//     cout << "║     Welcome to Advanced Text Editor!              ║\n";
//     cout << "║     Console-based editor with undo/redo           ║\n";
//     cout << "╚════════════════════════════════════════════════════╝\n";
    
//     while (true) {
//         displayMenu();
        
//         if (!(cin >> choice)) {
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             cout << "Invalid input. Please enter a number.\n";
//             continue;
//         }
        
//         switch (choice) {
//             case 1:
//                 handleInsert(editor);
//                 break;
//             case 2:
//                 handleDelete(editor);
//                 break;
//             case 3:
//                 handleReplace(editor);
//                 break;
//             case 4:
//                 displayDocument(editor);
//                 break;
//             case 5:
//                 handleSearch(editor);
//                 break;
//             case 6:
//                 handleSearchReplace(editor);
//                 break;
//             case 7:
//                 if (editor.undo()) {
//                     cout << "✓ Undo successful.\n";
//                 } else {
//                     cout << "Nothing to undo.\n";
//                 }
//                 break;
//             case 8:
//                 if (editor.redo()) {
//                     cout << "✓ Redo successful.\n";
//                 } else {
//                     cout << "Nothing to redo.\n";
//                 }
//                 break;
//             case 9:
//                 handleLoadFile(editor);
//                 break;
//             case 10:
//                 handleSaveFile(editor);
//                 break;
//             case 11:
//                 if (editor.isModified()) {
//                     cout << "Document has unsaved changes. Clear anyway? (y/n): ";
//                     char confirm;
//                     cin >> confirm;
//                     if (confirm != 'y' && confirm != 'Y') {
//                         cout << "Clear cancelled.\n";
//                         break;
//                     }
//                 }
//                 editor.clear();
//                 cout << "✓ Document cleared.\n";
//                 break;
//             case 12:
//                 showDocumentInfo(editor);
//                 break;
//             case 0:
//                 if (editor.isModified()) {
//                     char save;
//                     cout << "Document has unsaved changes. Save before exit? (y/n): ";
//                     cin >> save;
//                     if (save == 'y' || save == 'Y') {
//                         handleSaveFile(editor);
//                     }
//                 }
//                 cout << "\n╔════════════════════════════════════════════════════╗\n";
//                 cout << "║           Thank you for using Text Editor!        ║\n";
//                 cout << "╚════════════════════════════════════════════════════╝\n";
//                 return 0;
//             default:
//                 cout << "Invalid choice. Please try again.\n";
//         }
        
//         cout << "\nPress Enter to continue...";
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         cin.get();
//     }
    
//     return 0;
// }



// #include <iostream>
// #include "PersistentRope.h"
// #include "Buffer.h"
// #include "History.h"
// #include "AdvancedSearch.h"

// using namespace std;

// void testPersistentRope() {
//     cout << "\n===== Testing Persistent Rope =====\n";
    
//     PersistentRope rope1("Hello World");
//     cout << "Initial text: " << rope1.getText() << "\n";
//     cout << "Length: " << rope1.length() << "\n";
    
//     PersistentRope rope2 = rope1.insert(6, "Beautiful ");
//     cout << "After insert: " << rope2.getText() << "\n";
    
//     PersistentRope rope3 = rope2.deleteText(0, 6);
//     cout << "After delete: " << rope3.getText() << "\n";
    
//     cout << "Original still exists: " << rope1.getText() << "\n";
// }

// void testVersionHistory() {
//     cout << "\n===== Testing Version History =====\n";
    
//     VersionHistory history("Hello World");
//     cout << "Initial version created\n";
    
//     history.insert(6, "Beautiful ", "Added Beautiful");
//     history.insert(0, "Oh, ", "Added greeting");
//     history.deleteText(16, 6, "Removed Beautiful");
    
//     history.printVersionTree();
//     history.printVersionPath();
    
//     cout << "\nCurrent text: " << history.getCurrentVersion()->rope.getText() << "\n";
    
//     cout << "\nGoing back in history...\n";
//     history.goBack();
//     cout << "Text after goBack: " << history.getCurrentVersion()->rope.getText() << "\n";
    
//     history.goBack();
//     cout << "Text after another goBack: " << history.getCurrentVersion()->rope.getText() << "\n";
    
//     cout << "\nGoing forward...\n";
//     history.goForward();
//     cout << "Text after goForward: " << history.getCurrentVersion()->rope.getText() << "\n";
    
//     history.analyzeMemoryUsage();
//     history.compareTwoVersions(1, 3);
// }

// void testTextEditor() {
//     cout << "\n===== Testing Text Editor =====\n";
    
//     TextEditor editor;
//     editor.insert(0, "Hello World");
//     cout << "After insert: " << editor.getText() << "\n";
    
//     editor.insert(6, "Beautiful ");
//     cout << "After insert: " << editor.getText() << "\n";
    
//     editor.deleteText(0, 6);
//     cout << "After delete: " << editor.getText() << "\n";
    
//     cout << "\nUndo operations:\n";
//     editor.undo();
//     cout << "After undo: " << editor.getText() << "\n";
    
//     editor.undo();
//     cout << "After undo: " << editor.getText() << "\n";
    
//     cout << "\nRedo operations:\n";
//     editor.redo();
//     cout << "After redo: " << editor.getText() << "\n";
// }

// void testAdvancedSearch() {
//     cout << "\n===== Testing Advanced Search =====\n";
    
//     AdvancedSearchEngine searchEngine;
//     string text = "Hello World! hello world! HELLO WORLD!";
    
//     SearchOptions options;
//     options.caseSensitive = false;
    
//     vector<SearchResult> results = searchEngine.search(text, "hello", options);
    
//     cout << "Searching for 'hello' (case insensitive):\n";
//     cout << "Found " << results.size() << " matches:\n";
    
//     for (const auto& result : results) {
//         cout << "  Position " << result.position 
//              << " (Line " << result.lineNumber 
//              << ", Col " << result.columnNumber << "): "
//              << result.matchedText << "\n";
//     }
    
//     string testText = "cat dog cat bird cat";
//     cout << "\nOriginal: " << testText << "\n";
    
//     int replaced = searchEngine.searchAndReplace(testText, "cat", "tiger", options);
//     cout << "After replacing 'cat' with 'tiger': " << testText << "\n";
//     cout << "Replaced " << replaced << " occurrences\n";
// }

// void testOldHistory() {
//     cout << "\n===== Testing Old History System =====\n";
    
//     VersionHistory oldHistory;
    
//     auto v1 = make_shared<Version>("First version", 1, "Version 1");
//     oldHistory.addVersion(v1);
    
//     auto v2 = make_shared<Version>("Second version", 2, "Version 2");
//     oldHistory.addVersion(v2);
    
//     auto v3 = make_shared<Version>("Third version", 3, "Version 3");
//     oldHistory.addVersion(v3);
    
//     cout << "Total versions: " << oldHistory.getCount() << "\n";
//     cout << "Current version: " << oldHistory.getCurrentVersion()->getDescription() << "\n";
    
//     cout << "\nGoing back...\n";
//     auto prev = oldHistory.getPreviousVersion();
//     if (prev) {
//         cout << "Previous version: " << prev->getDescription() << "\n";
//     }
    
//     cout << "\nAll versions:\n";
//     auto allVersions = oldHistory.getAllVersions();
//     for (const auto& v : allVersions) {
//         cout << "  v" << v->getVersionNumber() << ": " << v->getDescription() 
//              << " (" << v->getTimestamp() << ")\n";
//     }
// }

// void displayMenu() {
//     cout << "\n========================================\n";
//     cout << "   Text Editor Testing Menu\n";
//     cout << "========================================\n";
//     cout << "1. Test Persistent Rope\n";
//     cout << "2. Test Version History\n";
//     cout << "3. Test Text Editor (Undo/Redo)\n";
//     cout << "4. Test Advanced Search\n";
//     cout << "5. Test Old History System\n";
//     cout << "6. Run All Tests\n";
//     cout << "0. Exit\n";
//     cout << "========================================\n";
//     cout << "Enter choice: ";
// }

// int main() {
//     int choice;
    
//     while (true) {
//         displayMenu();
//         cin >> choice;
//         cin.ignore();
        
//         switch (choice) {
//             case 1:
//                 testPersistentRope();
//                 break;
//             case 2:
//                 testVersionHistory();
//                 break;
//             case 3:
//                 testTextEditor();
//                 break;
//             case 4:
//                 testAdvancedSearch();
//                 break;
//             case 5:
//                 testOldHistory();
//                 break;
//             case 6:
//                 testPersistentRope();
//                 testVersionHistory();
//                 testTextEditor();
//                 testAdvancedSearch();
//                 testOldHistory();
//                 break;
//             case 0:
//                 cout << "\nExiting program. Goodbye!\n";
//                 return 0;
//             default:
//                 cout << "\nInvalid choice! Please try again.\n";
//         }
        
//         cout << "\n\nPress Enter to continue...";
//         cin.get();
//     }
    
//     return 0;
// }















#include <iostream>
#include "Buffer.h"
#include "PersistentRope.h"
#include "History.h"
#include "AdvancedSearch.h"

using namespace std;

void testPersistentRope() {
    cout << "\n===== Testing Persistent Rope =====\n";
    
    PersistentRope rope1("Hello World");
    cout << "Initial text: " << rope1.getText() << "\n";
    cout << "Length: " << rope1.length() << "\n";
    
    PersistentRope rope2 = rope1.insert(6, "Beautiful ");
    cout << "After insert: " << rope2.getText() << "\n";
    
    PersistentRope rope3 = rope2.deleteText(0, 6);
    cout << "After delete: " << rope3.getText() << "\n";
    
    cout << "Original still exists: " << rope1.getText() << "\n";
}

void testVersionHistory() {
    cout << "\n===== Testing Version History =====\n";
    
    VersionHistory history("Hello World");
    cout << "Initial version created\n";
    
    history.insert(6, "Beautiful ", "Added Beautiful");
    history.insert(0, "Oh, ", "Added greeting");
    history.deleteText(16, 6, "Removed Beautiful");
    
    history.printVersionTree();
    history.printVersionPath();
    
    cout << "\nCurrent text: " << history.getCurrentVersion()->rope->getText() << "\n";
    
    cout << "\nGoing back in history...\n";
    history.goBack();
    cout << "Text after goBack: " << history.getCurrentVersion()->rope->getText() << "\n";
    
    history.goBack();
    cout << "Text after another goBack: " << history.getCurrentVersion()->rope->getText() << "\n";
    
    cout << "\nGoing forward...\n";
    history.goForward();
    cout << "Text after goForward: " << history.getCurrentVersion()->rope->getText() << "\n";
    
    history.analyzeMemoryUsage();
    history.compareTwoVersions(1, 3);
}

void testTextEditor() {
    cout << "\n===== Testing Text Editor =====\n";
    
    TextEditor editor;
    editor.insert(0, "Hello World");
    cout << "After insert: " << editor.getText() << "\n";
    
    editor.insert(6, "Beautiful ");
    cout << "After insert: " << editor.getText() << "\n";
    
    editor.deleteText(0, 6);
    cout << "After delete: " << editor.getText() << "\n";
    
    cout << "\nUndo operations:\n";
    editor.undo();
    cout << "After undo: " << editor.getText() << "\n";
    
    editor.undo();
    cout << "After undo: " << editor.getText() << "\n";
    
    cout << "\nRedo operations:\n";
    editor.redo();
    cout << "After redo: " << editor.getText() << "\n";
}

void testAdvancedSearch() {
    cout << "\n===== Testing Advanced Search =====\n";
    
    AdvancedSearchEngine searchEngine;
    string text = "Hello World! hello world! HELLO WORLD!";
    
    SearchOptions options;
    options.caseSensitive = false;
    
    vector<SearchResult> results = searchEngine.search(text, "hello", options);
    
    cout << "Searching for 'hello' (case insensitive):\n";
    cout << "Found " << results.size() << " matches:\n";
    
    for (const auto& result : results) {
        cout << "  Position " << result.position 
             << " (Line " << result.lineNumber 
             << ", Col " << result.columnNumber << "): "
             << result.matchedText << "\n";
    }
    
    string testText = "cat dog cat bird cat";
    cout << "\nOriginal: " << testText << "\n";
    
    int replaced = searchEngine.searchAndReplace(testText, "cat", "tiger", options);
    cout << "After replacing 'cat' with 'tiger': " << testText << "\n";
    cout << "Replaced " << replaced << " occurrences\n";
}

void testOldHistory() {
    cout << "\n===== Testing Old History System =====\n";
    
    SimpleVersionHistory oldHistory;
    
    auto v1 = make_shared<SimpleVersion>("First version", 1, "Version 1");
    oldHistory.addVersion(v1);
    
    auto v2 = make_shared<SimpleVersion>("Second version", 2, "Version 2");
    oldHistory.addVersion(v2);
    
    auto v3 = make_shared<SimpleVersion>("Third version", 3, "Version 3");
    oldHistory.addVersion(v3);
    
    cout << "Total versions: " << oldHistory.getCount() << "\n";
    cout << "Current version: " << oldHistory.getCurrentVersion()->getDescription() << "\n";
    
    cout << "\nGoing back...\n";
    auto prev = oldHistory.getPreviousVersion();
    if (prev) {
        cout << "Previous version: " << prev->getDescription() << "\n";
    }
    
    cout << "\nAll versions:\n";
    auto allVersions = oldHistory.getAllVersions();
    for (const auto& v : allVersions) {
        cout << "  v" << v->getVersionNumber() << ": " << v->getDescription() 
             << " (" << v->getTimestamp() << ")\n";
    }
}

void displayMenu() {
    cout << "\n========================================\n";
    cout << "   Text Editor Testing Menu\n";
    cout << "========================================\n";
    cout << "1. Test Persistent Rope\n";
    cout << "2. Test Version History\n";
    cout << "3. Test Text Editor (Undo/Redo)\n";
    cout << "4. Test Advanced Search\n";
    cout << "5. Test Old History System\n";
    cout << "6. Run All Tests\n";
    cout << "0. Exit\n";
    cout << "========================================\n";
    cout << "Enter choice: ";
}

int main() {
    int choice;
    
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                testPersistentRope();
                break;
            case 2:
                testVersionHistory();
                break;
            case 3:
                testTextEditor();
                break;
            case 4:
                testAdvancedSearch();
                break;
            case 5:
                testOldHistory();
                break;
            case 6:
                testPersistentRope();
                testVersionHistory();
                testTextEditor();
                testAdvancedSearch();
                testOldHistory();
                break;
            case 0:
                cout << "\nExiting program. Goodbye!\n";
                return 0;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
        
        cout << "\n\nPress Enter to continue...";
        cin.get();
    }
    
    return 0;
}