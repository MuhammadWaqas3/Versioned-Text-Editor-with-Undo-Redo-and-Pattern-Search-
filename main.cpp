// Waqas's Work
#include "Buffer.h"
#include "History.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMenu() {
    cout << "\n====== Text Editor with Version History ======\n";
    cout << "1.  Insert text\n";
    cout << "2.  Delete text\n";
    cout << "3.  Replace text\n";
    cout << "4.  View document\n";
    cout << "5.  Search pattern\n";
    cout << "6.  Search and replace\n";
    cout << "7.  Undo\n";
    cout << "8.  Redo\n";
    cout << "9.  Load from file\n";
    cout << "10. Save to file\n";
    cout << "11. Clear document\n";
    cout << "0.  Exit\n";
    cout << "==============================================\n";
    cout << "Choice: ";
}

void displayDocument(const TextEditor& editor, int maxLines = 20) {
    string text = editor.getText();
    
    cout << "\n--- Document Content (Length: " << text.length() << " chars) ---\n";
    
    if (text.empty()) {
        cout << "[Empty document]\n";
        return;
    }
    
    // Display with line numbers
    int lineNum = 1;
    int charCount = 0;
    cout << setw(4) << lineNum << " | ";
    
    for (char c : text) {
        cout << c;
        charCount++;
        
        if (c == '\n') {
            lineNum++;
            if (lineNum <= maxLines) {
                cout << setw(4) << lineNum << " | ";
            } else {
                cout << "... [" << (text.length() - charCount) << " more characters]\n";
                break;
            }
        }
    }
    
    if (text.back() != '\n') cout << '\n';
    cout << "--- End of Document ---\n";
}

void handleInsert(TextEditor& editor) {
    size_t pos;
    cout << "Enter position to insert (0 to " << editor.getLength() << "): ";
    cin >> pos;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Enter text to insert (end with empty line):\n";
    string text, line;
    while (getline(cin, line) && !line.empty()) {
        if (!text.empty()) text += '\n';
        text += line;
    }
    
    editor.insert(pos, text);
    cout << "✓ Inserted " << text.length() << " characters.\n";
}

void handleDelete(TextEditor& editor) {
    size_t pos, length;
    cout << "Enter start position: ";
    cin >> pos;
    cout << "Enter length to delete: ";
    cin >> length;
    
    editor.deleteText(pos, length);
    cout << "✓ Deleted text.\n";
}

void handleReplace(TextEditor& editor) {
    size_t pos, length;
    cout << "Enter start position: ";
    cin >> pos;
    cout << "Enter length to replace: ";
    cin >> length;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Enter replacement text: ";
    string text;
    getline(cin, text);
    
    editor.replace(pos, length, text);
    cout << "✓ Replaced text.\n";
}

void handleSearch(const TextEditor& editor) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter search pattern: ";
    string pattern;
    getline(cin, pattern);
    
    auto positions = editor.search(pattern);
    
    if (positions.empty()) {
        cout << "Pattern not found.\n";
    } else {
        cout << "Found " << positions.size() << " occurrences at positions: ";
        for (size_t i = 0; i < positions.size() && i < 10; i++) {
            cout << positions[i] << " ";
        }
        if (positions.size() > 10) {
            cout << "... (+" << (positions.size() - 10) << " more)";
        }
        cout << '\n';
    }
}

void handleSearchReplace(TextEditor& editor) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter search pattern: ";
    string pattern;
    getline(cin, pattern);
    
    cout << "Enter replacement text: ";
    string replacement;
    getline(cin, replacement);
    
    int count = editor.searchAndReplace(pattern, replacement);
    cout << "✓ Replaced " << count << " occurrences.\n";
}

void handleLoadFile(TextEditor& editor) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter filename to load: ";
    string filename;
    getline(cin, filename);
    
    if (editor.loadFromFile(filename)) {
        cout << "✓ File loaded successfully.\n";
        cout << "  Loaded " << editor.getLength() << " characters.\n";
    } else {
        cout << "✗ Failed to load file.\n";
    }
}

void handleSaveFile(TextEditor& editor) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter filename to save (or press Enter for current file): ";
    string filename;
    getline(cin, filename);
    
    if (editor.saveToFile(filename)) {
        cout << "✓ File saved successfully.\n";
    } else {
        cout << "✗ Failed to save file.\n";
    }
}

int main() {
    TextEditor editor;
    int choice;
    
    cout << "Welcome to Text Editor!\n";
    cout << "A console-based text editor with undo/redo and pattern search.\n";
    
    while (true) {
        displayMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                handleInsert(editor);
                break;
            case 2:
                handleDelete(editor);
                break;
            case 3:
                handleReplace(editor);
                break;
            case 4:
                displayDocument(editor);
                break;
            case 5:
                handleSearch(editor);
                break;
            case 6:
                handleSearchReplace(editor);
                break;
            case 7:
                if (editor.undo()) {
                    cout << "✓ Undo successful.\n";
                } else {
                    cout << "Nothing to undo.\n";
                }
                break;
            case 8:
                if (editor.redo()) {
                    cout << "✓ Redo successful.\n";
                } else {
                    cout << "Nothing to redo.\n";
                }
                break;
            case 9:
                handleLoadFile(editor);
                break;
            case 10:
                handleSaveFile(editor);
                break;
            case 11:
                editor.clear();
                cout << "✓ Document cleared.\n";
                break;
            case 0:
                if (editor.isModified()) {
                    char save;
                    cout << "Document has unsaved changes. Save before exit? (y/n): ";
                    cin >> save;
                    if (save == 'y' || save == 'Y') {
                        handleSaveFile(editor);
                    }
                }
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    
    return 0;
}

//Ahsan's Advanced Search - Errors Yet to resolve 
// #include <iostream>
// #include "AdvancedSearch.h"

// using namespace std;

// int main()
// {
//     AdvancedSearchEngine engine;
//     SearchHistory history;

//     string text =
//         "Data Structures and Algorithms are important.\n"
//         "Data structures help organize data.\n"
//         "Algorithms process data efficiently.\n"
//         "Learning Data Structures is fun.";

//     cout << "===== ORIGINAL TEXT =====\n";
//     cout << text << endl;

//     string pattern = "Data";

//     // ------------------ BASIC SEARCH ------------------
//     cout << "\n===== SEARCH RESULTS =====\n";

//     SearchOptions options;
//     options.caseSensitive = true;
//     options.wholeWord = true;
//     options.contextLines = 1;

//     vector<SearchResult> results = engine.search(text, pattern, options);

//     for (const auto &r : results)
//     {
//         cout << "Found at position: " << r.position
//              << ", Line: " << r.line
//              << ", Column: " << r.column << endl;
//         cout << "Context:\n"
//              << r.context << endl;
//         cout << "--------------------------\n";
//     }

//     history.addSearch(pattern, results.size());

//     // ------------------ HIGHLIGHT ------------------
//     cout << "\n===== HIGHLIGHTED TEXT =====\n";
//     string highlighted = engine.findAndHighlight(text, pattern, "[[", "]]");
//     cout << highlighted << endl;

//     // ------------------ REPLACE FIRST ------------------
//     cout << "\n===== REPLACE FIRST =====\n";
//     string replacedFirst = engine.replaceFirst(text, "Data", "DATA");
//     cout << replacedFirst << endl;

//     // ------------------ REPLACE ALL ------------------
//     cout << "\n===== REPLACE ALL =====\n";
//     string replacedAll = engine.replaceAll(text, "Data", "DATA");
//     cout << replacedAll << endl;

//     // ------------------ COUNT OCCURRENCES ------------------
//     cout << "\n===== COUNT OCCURRENCES =====\n";
//     int count = engine.countOccurrences(text, "Data");
//     cout << "'Data' found " << count << " times.\n";

//     // ------------------ FUZZY SEARCH ------------------
//     cout << "\n===== FUZZY SEARCH =====\n";
//     vector<size_t> fuzzyPositions = engine.fuzzySearch(text, "Dat", 1);

//     for (size_t pos : fuzzyPositions)
//     {
//         cout << "Approximate match at position: " << pos << endl;
//     }

//     // ------------------ SEARCH HISTORY ------------------
//     cout << "\n===== SEARCH HISTORY =====\n";
//     history.displayHistory();

//     return 0;
// }
