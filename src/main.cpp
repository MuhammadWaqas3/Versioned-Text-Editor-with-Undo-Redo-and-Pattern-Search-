
#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <iomanip>
#include "Buffer.h"
#include "History.h"
#include "AdvancedSearch.h"
#include "VersionManager.h"

using namespace std;

// Global objects
TextEditor editor;
VersionHistory versionHistory;
AdvancedSearchEngine searchEngine;
SearchHistory searchHistory;
VersionManager versionManager;

// Color codes for Windows console
enum Color {
    BLACK = 0,
    DARK_BLUE = 1,
    DARK_GREEN = 2,
    DARK_CYAN = 3,
    DARK_RED = 4,
    DARK_MAGENTA = 5,
    DARK_YELLOW = 6,
    GRAY = 7,
    DARK_GRAY = 8,
    BLUE = 9,
    GREEN = 10,
    CYAN = 11,
    RED = 12,
    MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

// Function to set text color
void setColor(int textColor, int bgColor = BLACK) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

// Function to print colored text
void printColored(const string& text, int color, bool newline = true) {
    setColor(color);
    cout << text;
    setColor(GRAY);
    if (newline) cout << endl;
}

// Function to print a separator line
void printSeparator(char ch = '=', int length = 70) {
    setColor(CYAN);
    cout << string(length, ch) << endl;
    setColor(GRAY);
}

// Function to print a header
void printHeader(const string& title) {
    system("cls");
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(YELLOW);
    int padding = (70 - title.length()) / 2;
    cout << string(padding, ' ') << title << endl;
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(GRAY);
}

// Function to print success message
void printSuccess(const string& msg) {
    setColor(GREEN);
    cout << "\n[SUCCESS] " << msg << endl;
    setColor(GRAY);
}

// Function to print error message
void printError(const string& msg) {
    setColor(RED);
    cout << "\n[ERROR] " << msg << endl;
    setColor(GRAY);
}

// Function to print info message
void printInfo(const string& msg) {
    setColor(CYAN);
    cout << "\n[INFO] " << msg << endl;
    setColor(GRAY);
}

// Function declarations
void displayMenu();
void handleInsert();
void handleDelete();
void handleReplace();
void handleUndo();
void handleRedo();
void handleSearch();
void handleAdvancedSearch();
void handleSearchAndReplace();
void handleLoadFile();
void handleSaveFile();
void handleVersionManagement();

// ============================================================================
// VERSION MANAGEMENT
// ============================================================================

void handleVersionManagement() {
    printHeader("VERSION MANAGEMENT");
    
    setColor(YELLOW);
    cout << "\n  [1] Create Version" << endl;
    cout << "  [2] List All Versions" << endl;
    cout << "  [3] Restore Version by ID" << endl;
    cout << "  [4] Restore Previous Version" << endl;
    cout << "  [5] Restore Next Version" << endl;
    cout << "  [6] Delete Version" << endl;
    cout << "  [7] Export Version to File" << endl;
    cout << "  [8] Clear All Versions" << endl;
    setColor(RED);
    cout << "  [0] Back to Main Menu" << endl;
    setColor(GRAY);
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Enter your choice: ";
    setColor(GRAY);
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch(choice) {
        case 1: {
            printHeader("CREATE VERSION");
            setColor(WHITE);
            cout << "Enter description (or press Enter to skip): ";
            setColor(GRAY);
            string desc;
            getline(cin, desc);
            
            int versionId = versionManager.createSnapshot(editor, desc);
            printSuccess("Version " + to_string(versionId) + " created successfully!");
            break;
        }
        
        case 2: {
            printHeader("ALL VERSIONS");
            versionManager.listVersions();
            break;
        }
        
        case 3: {
            printHeader("RESTORE VERSION");
            versionManager.listVersions();
            
            setColor(WHITE);
            cout << "\nEnter Version ID to restore (0 to cancel): ";
            setColor(GRAY);
            int versionId;
            cin >> versionId;
            cin.ignore();
            
            if (versionId != 0) {
                if (versionManager.restoreVersion(editor, versionId)) {
                    printSuccess("Successfully restored to Version " + to_string(versionId));
                }
            } else {
                printInfo("Restore cancelled.");
            }
            break;
        }
        
        case 4: {
            printHeader("RESTORE PREVIOUS VERSION");
            if (versionManager.restorePreviousVersion(editor)) {
                printSuccess("Moved to previous version!");
            } else {
                printError("No previous version available!");
            }
            break;
        }
        
        case 5: {
            printHeader("RESTORE NEXT VERSION");
            if (versionManager.restoreNextVersion(editor)) {
                printSuccess("Moved to next version!");
            } else {
                printError("No next version available!");
            }
            break;
        }
        
        case 6: {
            printHeader("DELETE VERSION");
            versionManager.listVersions();
            
            setColor(WHITE);
            cout << "\nEnter Version ID to delete (0 to cancel): ";
            setColor(GRAY);
            int versionId;
            cin >> versionId;
            cin.ignore();
            
            if (versionId != 0) {
                versionManager.deleteVersion(versionId);
            } else {
                printInfo("Delete cancelled.");
            }
            break;
        }
        
        case 7: {
            printHeader("EXPORT VERSION");
            versionManager.listVersions();
            
            setColor(WHITE);
            cout << "\nEnter Version ID to export: ";
            setColor(GRAY);
            int versionId;
            cin >> versionId;
            cin.ignore();
            
            setColor(WHITE);
            cout << "Enter filename to export: ";
            setColor(GRAY);
            string filename;
            getline(cin, filename);
            
            versionManager.exportVersion(versionId, filename);
            break;
        }
        
        case 8: {
            printHeader("CLEAR ALL VERSIONS");
            setColor(RED);
            cout << "\nWARNING: This will delete ALL versions permanently!" << endl;
            setColor(WHITE);
            cout << "Are you sure? (y/n): ";
            setColor(GRAY);
            char confirm;
            cin >> confirm;
            cin.ignore();
            
            if (confirm == 'y' || confirm == 'Y') {
                versionManager.clearAllVersions();
                printSuccess("All versions cleared!");
            } else {
                printInfo("Operation cancelled.");
            }
            break;
        }
        
        case 0:
            return;
            
        default:
            printError("Invalid choice!");
    }
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

// ============================================================================
// BASIC OPERATIONS
// ============================================================================

void handleInsert() {
    printHeader("INSERT TEXT");
    
    setColor(CYAN);
    cout << "Current text length: ";
    setColor(YELLOW);
    cout << editor.getLength() << " characters" << endl;
    setColor(GRAY);
    
    setColor(WHITE);
    cout << "\nEnter position to insert at: ";
    setColor(GRAY);
    size_t pos;
    cin >> pos;
    cin.ignore();
    
    setColor(WHITE);
    cout << "Enter text to insert: ";
    setColor(GRAY);
    string text;
    getline(cin, text);
    
    editor.insert(pos, text);
    printSuccess("Text inserted successfully!");
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

void handleDelete() {
    printHeader("DELETE TEXT");
    
    setColor(CYAN);
    cout << "Current text:\n";
    setColor(YELLOW);
    cout << editor.getText() << endl;
    setColor(GRAY);
    
    setColor(WHITE);
    cout << "\nEnter position to delete from: ";
    setColor(GRAY);
    size_t pos;
    cin >> pos;
    cin.ignore();
    
    setColor(WHITE);
    cout << "Enter length to delete: ";
    setColor(GRAY);
    size_t length;
    cin >> length;
    cin.ignore();
    
    editor.deleteText(pos, length);
    printSuccess("Text deleted successfully!");
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

void handleReplace() {
    printHeader("REPLACE TEXT");
    
    setColor(CYAN);
    cout << "Current text:\n";
    setColor(YELLOW);
    cout << editor.getText() << endl;
    setColor(GRAY);
    
    setColor(WHITE);
    cout << "\nEnter position to replace from: ";
    setColor(GRAY);
    size_t pos;
    cin >> pos;
    cin.ignore();
    
    setColor(WHITE);
    cout << "Enter length to replace: ";
    setColor(GRAY);
    size_t length;
    cin >> length;
    cin.ignore();
    
    setColor(WHITE);
    cout << "Enter new text: ";
    setColor(GRAY);
    string text;
    getline(cin, text);
    
    editor.replace(pos, length, text);
    printSuccess("Text replaced successfully!");
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

void handleUndo() {
    printHeader("UNDO OPERATION");
    
    if (editor.undo()) {
        printSuccess("Undo successful!");
        setColor(CYAN);
        cout << "\nCurrent text:\n";
        setColor(YELLOW);
        cout << editor.getText() << endl;
        setColor(GRAY);
    } else {
        printError("Nothing to undo!");
    }
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

void handleRedo() {
    printHeader("REDO OPERATION");
    
    if (editor.redo()) {
        printSuccess("Redo successful!");
        setColor(CYAN);
        cout << "\nCurrent text:\n";
        setColor(YELLOW);
        cout << editor.getText() << endl;
        setColor(GRAY);
    } else {
        printError("Nothing to redo!");
    }
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

// ============================================================================
// SEARCH OPERATIONS
// ============================================================================

void handleSearch() {
    printHeader("SEARCH TEXT (Rolling Hash)");
    
    setColor(CYAN);
    cout << "Current text:\n";
    setColor(YELLOW);
    cout << editor.getText() << endl;
    setColor(GRAY);
    
    setColor(WHITE);
    cout << "\nEnter pattern to search: ";
    setColor(GRAY);
    string pattern;
    getline(cin, pattern);
    
    vector<size_t> results = editor.search(pattern);
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(CYAN);
    cout << "SEARCH RESULTS" << endl;
    printSeparator('-', 70);
    
    if (results.empty()) {
        printError("No matches found!");
    } else {
        printSuccess("Found " + to_string(results.size()) + " occurrence(s)");
        
        setColor(CYAN);
        cout << "\nPositions: ";
        setColor(YELLOW);
        for (size_t pos : results) {
            cout << pos << " ";
        }
        cout << endl;
        setColor(GRAY);
    }
    
    searchHistory.addSearch(pattern, results.size());
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

void handleAdvancedSearch() {
    printHeader("ADVANCED SEARCH (KMP)");
    
    setColor(CYAN);
    cout << "Current text:\n";
    setColor(YELLOW);
    cout << editor.getText() << endl;
    setColor(GRAY);
    
    setColor(WHITE);
    cout << "\nEnter pattern to search: ";
    setColor(GRAY);
    string pattern;
    getline(cin, pattern);
    
    SearchOptions options;
    
    setColor(WHITE);
    cout << "\nCase sensitive? (y/n): ";
    setColor(GRAY);
    char choice;
    cin >> choice;
    options.caseSensitive = (choice == 'y' || choice == 'Y');
    
    setColor(WHITE);
    cout << "Whole word only? (y/n): ";
    setColor(GRAY);
    cin >> choice;
    options.wholeWord = (choice == 'y' || choice == 'Y');
    
    setColor(WHITE);
    cout << "Context lines (0-5): ";
    setColor(GRAY);
    cin >> options.contextLines;
    cin.ignore();
    
    vector<SearchResult> results = searchEngine.search(editor.getText(), pattern, options);
    
    cout << "\n";
    printSeparator('=', 70);
    setColor(CYAN);
    cout << "SEARCH RESULTS" << endl;
    printSeparator('=', 70);
    
    if (results.empty()) {
        printError("No matches found!");
    } else {
        printSuccess("Found " + to_string(results.size()) + " occurrence(s)");
        
        for (const auto& result : results) {
            cout << "\n";
            printSeparator('-', 70);
            setColor(YELLOW);
            cout << "Match at position " << result.position 
                 << " (Line " << result.line << ", Col " << result.column << ")" << endl;
            setColor(CYAN);
            cout << "Context:" << endl;
            setColor(GRAY);
            cout << result.context << endl;
        }
    }
    
    searchHistory.addSearch(pattern, results.size());
    
    cout << "\n";
    printSeparator('=', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

void handleSearchAndReplace() {
    printHeader("SEARCH AND REPLACE");
    
    setColor(CYAN);
    cout << "Current text:\n";
    setColor(YELLOW);
    cout << editor.getText() << endl;
    setColor(GRAY);
    
    setColor(WHITE);
    cout << "\nEnter pattern to search: ";
    setColor(GRAY);
    string pattern;
    getline(cin, pattern);
    
    setColor(WHITE);
    cout << "Enter replacement text: ";
    setColor(GRAY);
    string replacement;
    getline(cin, replacement);
    
    int count = editor.searchAndReplace(pattern, replacement);
    
    printSuccess("Replaced " + to_string(count) + " occurrence(s)");
    
    setColor(CYAN);
    cout << "\nNew text:\n";
    setColor(YELLOW);
    cout << editor.getText() << endl;
    setColor(GRAY);
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

// ============================================================================
// FILE OPERATIONS
// ============================================================================

void handleLoadFile() {
    printHeader("LOAD FROM FILE");
    
    setColor(CYAN);
    cout << "Files are loaded from the 'data' folder" << endl;
    printSeparator('-', 70);
    setColor(GRAY);
    
    setColor(WHITE);
    cout << "\nEnter filename (e.g., sample_data.txt): ";
    setColor(GRAY);
    string filename;
    getline(cin, filename);
    
    if (editor.loadFromFile(filename)) {
        printSuccess("File loaded successfully!");
        setColor(CYAN);
        cout << "File: ";
        setColor(YELLOW);
        cout << filename << endl;
        setColor(CYAN);
        cout << "Text length: ";
        setColor(YELLOW);
        cout << editor.getLength() << " characters" << endl;
        
        setColor(CYAN);
        cout << "\nPreview (first 200 chars):" << endl;
        printSeparator('-', 70);
        setColor(GRAY);
        string text = editor.getText();
        cout << text.substr(0, min((size_t)200, text.length())) << endl;
        if (text.length() > 200) {
            setColor(DARK_GRAY);
            cout << "... (" << (text.length() - 200) << " more characters)" << endl;
        }
        printSeparator('-', 70);
    } else {
        printError("Failed to load file!");
        setColor(DARK_GRAY);
        cout << "Make sure the file exists in the 'data' folder." << endl;
        setColor(GRAY);
    }
    
    cout << "\n";
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

void handleSaveFile() {
    printHeader("SAVE TO FILE");
    
    setColor(CYAN);
    cout << "Files are saved to the 'data' folder" << endl;
    printSeparator('-', 70);
    setColor(GRAY);
    
    if (!editor.getFilename().empty()) {
        setColor(CYAN);
        cout << "Current filename: ";
        setColor(YELLOW);
        
        string currentFile = editor.getFilename();
        size_t lastSlash = currentFile.find_last_of("/\\");
        if (lastSlash != string::npos) {
            currentFile = currentFile.substr(lastSlash + 1);
        }
        
        cout << currentFile << endl;
        setColor(WHITE);
        cout << "\nSave to same file? (y/n): ";
        setColor(GRAY);
        char choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 'y' || choice == 'Y') {
            if (editor.saveToFile()) {
                printSuccess("File saved successfully!");
                setColor(CYAN);
                cout << "Saved to: ";
                setColor(YELLOW);
                cout << currentFile << endl;
                setColor(GRAY);
            } else {
                printError("Failed to save file!");
            }
            cout << "\n";
            setColor(WHITE);
            cout << "Press any key to continue...";
            setColor(GRAY);
            _getch();
            return;
        }
    }
    
    setColor(WHITE);
    cout << "\nEnter filename to save (e.g., myfile.txt): ";
    setColor(GRAY);
    string filename;
    getline(cin, filename);
    
    if (editor.saveToFile(filename)) {
        printSuccess("File saved successfully!");
        setColor(CYAN);
        cout << "Saved to: ";
        setColor(YELLOW);
        cout << filename << endl;
        setColor(CYAN);
        cout << "Location: data/" << filename << endl;
        setColor(GRAY);
    } else {
        printError("Failed to save file!");
        setColor(DARK_GRAY);
        cout << "Check if the 'data' folder exists and you have write permissions." << endl;
        setColor(GRAY);
    }
    
    cout << "\n";
    setColor(WHITE);
    cout << "Press any key to continue...";
    setColor(GRAY);
    _getch();
}

// ============================================================================
// MAIN MENU
// ============================================================================

void displayMenu() {
    system("cls");
    
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(YELLOW);
    int padding = 10;
    cout << string(padding, ' ') << "ADVANCED TEXT EDITOR" << endl;
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(GRAY);
    
    cout << "\n";
    setColor(GREEN);
    cout << "  BASIC OPERATIONS:" << endl;
    setColor(GRAY);
    cout << "    [1]  Insert Text" << endl;
    cout << "    [2]  Delete Text" << endl;
    cout << "    [3]  Replace Text" << endl;
    cout << "    [4]  Undo" << endl;
    cout << "    [5]  Redo" << endl;
    
    cout << "\n";
    setColor(MAGENTA);
    cout << "  SEARCH OPERATIONS:" << endl;
    setColor(GRAY);
    cout << "    [6]  Basic Search (Rolling Hash)" << endl;
    cout << "    [7]  Advanced Search (KMP/Boyer-Moore)" << endl;
    cout << "    [8]  Search and Replace" << endl;
    
    cout << "\n";
    setColor(BLUE);
    cout << "  FILE OPERATIONS:" << endl;
    setColor(GRAY);
    cout << "    [9]  Load from File" << endl;
    cout << "    [10] Save to File" << endl;
    
    cout << "\n";
    setColor(YELLOW);
    cout << "  VERSION MANAGEMENT:" << endl;
    setColor(GREEN);
    cout << "    [11] Version Management System" << endl;
    setColor(GRAY);
    
    cout << "\n";
    setColor(CYAN);
    cout << "  OTHER:" << endl;
    setColor(GRAY);
    cout << "    [12] Display Current Text" << endl;
    setColor(RED);
    cout << "    [0]  Exit" << endl;
    setColor(GRAY);
    
    cout << "\n";
    printSeparator('-', 70);
    setColor(CYAN);
    cout << "  STATUS:" << endl;
    setColor(GRAY);
    cout << "    Text Length: ";
    setColor(YELLOW);
    cout << editor.getLength() << " chars" << endl;
    setColor(GRAY);
    cout << "    Can Undo: ";
    setColor(editor.canUndo() ? GREEN : RED);
    cout << (editor.canUndo() ? "Yes" : "No") << endl;
    setColor(GRAY);
    cout << "    Can Redo: ";
    setColor(editor.canRedo() ? GREEN : RED);
    cout << (editor.canRedo() ? "Yes" : "No") << endl;
    setColor(GRAY);
    cout << "    Saved Versions: ";
    setColor(YELLOW);
    cout << versionManager.getVersionCount() << endl;
    setColor(GRAY);
    printSeparator('-', 70);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    system("cls");
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(YELLOW);
    cout << "\n" << string(22, ' ') << "WELCOME TO" << endl;
    cout << string(15, ' ') << "ADVANCED TEXT EDITOR" << endl << endl;
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(GREEN);
    cout << "\n  Features:" << endl;
    setColor(GRAY);
    cout << "    - Undo/Redo with Command Pattern" << endl;
    cout << "    - Advanced Search (Rolling Hash, KMP, Boyer-Moore)" << endl;
    cout << "    - Version Management System" << endl;
    cout << "    - File Operations (data folder)" << endl;
    cout << "    - Search History" << endl;
    setColor(CYAN);
    cout << "\n";
    printSeparator('=', 70);
    setColor(WHITE);
    cout << "\nPress any key to start...";
    setColor(GRAY);
    _getch();
    
    int choice;
    
    do {
        displayMenu();
        setColor(WHITE);
        cout << "\nEnter your choice: ";
        setColor(GRAY);
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                handleInsert();
                break;
            case 2:
                handleDelete();
                break;
            case 3:
                handleReplace();
                break;
            case 4:
                handleUndo();
                break;
            case 5:
                handleRedo();
                break;
            case 6:
                handleSearch();
                break;
            case 7:
                handleAdvancedSearch();
                break;
            case 8:
                handleSearchAndReplace();
                break;
            case 9:
                handleLoadFile();
                break;
            case 10:
                handleSaveFile();
                break;
            case 11:
                handleVersionManagement();
                break;
            case 12: {
                printHeader("CURRENT TEXT");
                setColor(CYAN);
                cout << "Length: ";
                setColor(YELLOW);
                cout << editor.getLength() << " characters" << endl;
                setColor(GRAY);
                cout << "\n";
                printSeparator('-', 70);
                setColor(YELLOW);
                cout << editor.getText() << endl;
                setColor(GRAY);
                printSeparator('-', 70);
                cout << "\n";
                setColor(WHITE);
                cout << "Press any key to continue...";
                setColor(GRAY);
                _getch();
                break;
            }
            case 0:
                system("cls");
                setColor(CYAN);
                printSeparator('=', 70);
                setColor(YELLOW);
                cout << "\n" << string(20, ' ') << "THANK YOU FOR USING" << endl;
                cout << string(15, ' ') << "ADVANCED TEXT EDITOR" << endl << endl;
                setColor(CYAN);
                printSeparator('=', 70);
                setColor(GREEN);
                cout << "\n  Goodbye! See you again soon." << endl;
                setColor(GRAY);
                cout << "\n";
                break;
            default:
                printError("Invalid choice! Please try again.");
                cout << "\n";
                setColor(WHITE);
                cout << "Press any key to continue...";
                setColor(GRAY);
                _getch();
        }
        
    } while(choice != 0);
    
    return 0;
}