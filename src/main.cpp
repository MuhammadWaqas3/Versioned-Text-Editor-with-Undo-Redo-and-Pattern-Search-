
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
SearchHistory searchHistory;  // Make sure this is declared
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

//

// void ensureDataFolderExists() {
//     CreateDirectory("../data", NULL); // Creates data folder in parent directory
// }

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
void handleVersionControl();
void handleSnapshots();


// ============================================================================
// SEARCH HISTORY FUNCTION (NEW FIX)
// ============================================================================



// ============================================================================
// SNAPSHOT MANAGEMENT
// ============================================================================

void handleSnapshots() {
    printHeader("SNAPSHOT MANAGEMENT");
    
    setColor(YELLOW);
    cout << "\n  [1] Create Snapshot" << endl;
    cout << "  [2] List All Snapshots" << endl;
    cout << "  [3] Restore Snapshot by ID" << endl;
    cout << "  [4] Restore Previous Snapshot" << endl;
    cout << "  [5] Restore Next Snapshot" << endl;
    cout << "  [6] Delete Snapshot" << endl;
    cout << "  [7] Export Snapshot to File" << endl;
    cout << "  [8] Clear All Snapshots" << endl;
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
            printHeader("CREATE SNAPSHOT");
            setColor(WHITE);
            cout << "Enter description (or press Enter to skip): ";
            setColor(GRAY);
            string desc;
            getline(cin, desc);
            
            int versionId = versionManager.createSnapshot(editor, desc);
            printSuccess("Snapshot " + to_string(versionId) + " created successfully!");
            break;
        }
        
        case 2: {
            printHeader("ALL SNAPSHOTS");
            versionManager.listVersions();
            break;
        }
        
        case 3: {
            printHeader("RESTORE SNAPSHOT");
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
            printHeader("RESTORE PREVIOUS SNAPSHOT");
            if (versionManager.restorePreviousVersion(editor)) {
                printSuccess("Moved to previous version!");
            } else {
                printError("No previous version available!");
            }
            break;
        }
        
        case 5: {
            printHeader("RESTORE NEXT SNAPSHOT");
            if (versionManager.restoreNextVersion(editor)) {
                printSuccess("Moved to next version!");
            } else {
                printError("No next version available!");
            }
            break;
        }
        
        case 6: {
            printHeader("DELETE SNAPSHOT");
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
            printHeader("EXPORT SNAPSHOT");
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
            printHeader("CLEAR ALL SNAPSHOTS");
            setColor(RED);
            cout << "\nWARNING: This will delete ALL snapshots permanently!" << endl;
            setColor(WHITE);
            cout << "Are you sure? (y/n): ";
            setColor(GRAY);
            char confirm;
            cin >> confirm;
            cin.ignore();
            
            if (confirm == 'y' || confirm == 'Y') {
                versionManager.clearAllVersions();
                printSuccess("All snapshots cleared!");
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
    printHeader("ADVANCED SEARCH (KMP/Boyer-Moore)");
    
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

// ============================================================================
// FILE OPERATIONS (UPDATED)
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
    
    // Add ../data/ prefix if not already present
    string filepath = filename;
    if (filepath.find("../data/") == string::npos && 
        filepath.find("data/") == string::npos &&
        filepath.find(":\\") == string::npos) {  // Not an absolute path
        filepath = "../data/" + filename;
    }
    
    if (editor.loadFromFile(filepath)) {
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
    
    // Check if file already has a name
    if (!editor.getFilename().empty()) {
        setColor(CYAN);
        cout << "Current filename: ";
        setColor(YELLOW);
        
        // Extract just the filename from the full path
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
    
    // Add ../data/ prefix if not already present
    string filepath = filename;
    if (filepath.find("../data/") == string::npos && 
        filepath.find("data/") == string::npos &&
        filepath.find(":\\") == string::npos) {  // Not an absolute path
        filepath = "../data/" + filename;
    }
    
    if (editor.saveToFile(filepath)) {
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
// VERSION CONTROL
// ============================================================================

void handleVersionControl() {
    printHeader("VERSION CONTROL (History System)");
    
    setColor(YELLOW);
    cout << "\n  [1] Create Version" << endl;
    cout << "  [2] List All Versions" << endl;
    cout << "  [3] Go to Previous Version" << endl;
    cout << "  [4] Go to Next Version" << endl;
    cout << "  [5] Clear Version History" << endl;
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
            cout << "Enter version description: ";
            setColor(GRAY);
            string desc;
            getline(cin, desc);
            
            string filename = editor.getFilename();
            if (filename.empty()) {
                filename = "(Untitled)";
            }
            
            versionHistory.addVersion(editor.getText(), 
                                     versionHistory.getCount() + 1, 
                                     desc,
                                     filename);
            printSuccess("Version created successfully!");
            break;
        }
        
        case 2: {
            printHeader("ALL VERSIONS");
            auto versions = versionHistory.getAllVersions();
            if (versions.empty()) {
                printInfo("No versions saved.");
            } else {
                setColor(CYAN);
                cout << "\n" << string(90, '=') << endl;
                cout << "                       VERSION HISTORY" << endl;
                cout << string(90, '=') << endl;
                setColor(GRAY);
                
                cout << setw(5) << "Ver" << " | "
                     << setw(19) << "Timestamp" << " | "
                     << setw(20) << left << "File" << " | "
                     << setw(8) << "Size" << " | "
                     << "Description" << endl;
                cout << string(90, '-') << endl;
                
                for (const auto& ver : versions) {
                    string displayFilename = ver->getFilename();
                    if (displayFilename.length() > 20) {
                        displayFilename = displayFilename.substr(0, 17) + "...";
                    }
                    
                    setColor(YELLOW);
                    cout << setw(5) << ver->getVersionNumber() << " | ";
                    setColor(CYAN);
                    cout << setw(19) << ver->getTimestamp() << " | ";
                    setColor(MAGENTA);
                    cout << setw(20) << left << displayFilename << " | ";
                    setColor(YELLOW);
                    cout << setw(6) << ver->getContent().length() << " | ";
                    setColor(GRAY);
                    cout << ver->getDescription() << endl;
                }
                setColor(CYAN);
                cout << string(90, '-') << endl;
                setColor(GRAY);
            }
            break;
        }
        
        case 3: {
            printHeader("GO TO PREVIOUS VERSION");
            auto prev = versionHistory.getPreviousVersion();
            if (prev) {
                editor.clear();
                editor.insert(0, prev->getContent());
                printSuccess("Moved to previous version!");
                setColor(CYAN);
                cout << "Version: " << prev->getVersionNumber() 
                     << " | File: " << prev->getFilename() << endl;
                setColor(GRAY);
            } else {
                printError("No previous version available!");
            }
            break;
        }
        
        case 4: {
            printHeader("GO TO NEXT VERSION");
            auto next = versionHistory.getNextVersion();
            if (next) {
                editor.clear();
                editor.insert(0, next->getContent());
                printSuccess("Moved to next version!");
                setColor(CYAN);
                cout << "Version: " << next->getVersionNumber() 
                     << " | File: " << next->getFilename() << endl;
                setColor(GRAY);
            } else {
                printError("No next version available!");
            }
            break;
        }
        
        case 5: {
            printHeader("CLEAR VERSION HISTORY");
            setColor(RED);
            cout << "\nWARNING: This will delete ALL version history!" << endl;
            setColor(WHITE);
            cout << "Are you sure? (y/n): ";
            setColor(GRAY);
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                versionHistory.clear();
                printSuccess("Version history cleared!");
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
// MAIN MENU
// ============================================================================

void displayMenu() {
    system("cls");
    
    // Title
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(YELLOW);
    int padding = 10;
    cout << string(padding, ' ') << "ADVANCED TEXT EDITOR WITH SNAPSHOTS" << endl;
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(GRAY);
    
    // Menu options
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
   // cout << "    [14] Show Search History" << endl;
    
    cout << "\n";
    setColor(BLUE);
    cout << "  FILE OPERATIONS:" << endl;
    setColor(GRAY);
    cout << "    [9]  Load from File" << endl;
    cout << "    [10] Save to File" << endl;
    
    cout << "\n";
    setColor(YELLOW);
    cout << "  VERSION MANAGEMENT:" << endl;
    setColor(GRAY);
    cout << "    [11] Version Control (History System)" << endl;
    setColor(GREEN);
    cout << "    [12] SNAPSHOT MANAGEMENT (NEW)" << endl;
    setColor(GRAY);
    
    cout << "\n";
    setColor(CYAN);
    cout << "  OTHER:" << endl;
    setColor(GRAY);
    cout << "    [13] Display Current Text" << endl;
    setColor(RED);
    cout << "    [0]  Exit" << endl;
    setColor(GRAY);
    
    // Status bar
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
    cout << "    Snapshots: ";
    setColor(YELLOW);
    cout << versionManager.getVersionCount() << endl;
    setColor(GRAY);
    cout << "    Versions: ";
    setColor(YELLOW);
    cout << versionHistory.getCount() << endl;
    setColor(GRAY);
    printSeparator('-', 70);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {

 //ensureDataFolderExists();

    // Welcome screen
    system("cls");
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(YELLOW);
    cout << "\n" << string(22, ' ') << "WELCOME TO" << endl;
    cout << string(12, ' ') << "ADVANCED TEXT EDITOR WITH SNAPSHOTS" << endl << endl;
    setColor(CYAN);
    printSeparator('=', 70);
    setColor(GREEN);
    cout << "\n  Features:" << endl;
    setColor(GRAY);
    cout << "    - Undo/Redo with Command Pattern" << endl;
    cout << "    - Advanced Search (Rolling Hash, KMP, Boyer-Moore)" << endl;
    cout << "    - Snapshot Management (Doubly Linked List)" << endl;
    cout << "    - Version Control System" << endl;
    cout << "    - File Operations" << endl;
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
                handleVersionControl();
                break;
            case 12:
                handleSnapshots();
                break;
            case 13: {
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
                cout << string(12, ' ') << "ADVANCED TEXT EDITOR WITH SNAPSHOTS" << endl << endl;
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