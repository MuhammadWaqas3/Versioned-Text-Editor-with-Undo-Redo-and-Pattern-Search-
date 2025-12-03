#include "textEditor.h"
#include <iostream>
#include <iomanip>
#include <limits>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMenu() {
    std::cout << "\n====== Versioned Text Editor ======\n";
    std::cout << "1.  Insert text\n";
    std::cout << "2.  Delete text\n";
    std::cout << "3.  Replace text\n";
    std::cout << "4.  View document\n";
    std::cout << "5.  Search pattern\n";
    std::cout << "6.  Search and replace\n";
    std::cout << "7.  Undo\n";
    std::cout << "8.  Redo\n";
    std::cout << "9.  Create snapshot\n";
    std::cout << "10. View versions\n";
    std::cout << "11. Restore version\n";
    std::cout << "12. Load from file\n";
    std::cout << "13. Save to file\n";
    std::cout << "14. Clear document\n";
    std::cout << "0.  Exit\n";
    std::cout << "===================================\n";
    std::cout << "Choice: ";
}

void displayDocument(const TextEditor& editor, int maxLines = 20) {
    std::string text = editor.getText();
    
    std::cout << "\n--- Document Content (Length: " << text.length() << " chars) ---\n";
    
    if (text.empty()) {
        std::cout << "[Empty document]\n";
        return;
    }
    
    // Display with line numbers
    int lineNum = 1;
    int charCount = 0;
    std::cout << std::setw(4) << lineNum << " | ";
    
    for (char c : text) {
        std::cout << c;
        charCount++;
        
        if (c == '\n') {
            lineNum++;
            if (lineNum <= maxLines) {
                std::cout << std::setw(4) << lineNum << " | ";
            } else {
                std::cout << "... [" << (text.length() - charCount) << " more characters]\n";
                break;
            }
        }
    }
    
    if (text.back() != '\n') std::cout << '\n';
    std::cout << "--- End of Document ---\n";
}

void handleInsert(TextEditor& editor) {
    size_t pos;
    std::cout << "Enter position to insert (0 to " << editor.getLength() << "): ";
    std::cin >> pos;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter text to insert (end with empty line):\n";
    std::string text, line;
    while (std::getline(std::cin, line) && !line.empty()) {
        if (!text.empty()) text += '\n';
        text += line;
    }
    
    editor.insert(pos, text);
    std::cout << "✓ Inserted " << text.length() << " characters.\n";
}

void handleDelete(TextEditor& editor) {
    size_t pos, length;
    std::cout << "Enter start position: ";
    std::cin >> pos;
    std::cout << "Enter length to delete: ";
    std::cin >> length;
    
    editor.deleteText(pos, length);
    std::cout << "✓ Deleted text.\n";
}

void handleReplace(TextEditor& editor) {
    size_t pos, length;
    std::cout << "Enter start position: ";
    std::cin >> pos;
    std::cout << "Enter length to replace: ";
    std::cin >> length;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "Enter replacement text: ";
    std::string text;
    std::getline(std::cin, text);
    
    editor.replace(pos, length, text);
    std::cout << "✓ Replaced text.\n";
}

void handleSearch(const TextEditor& editor) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter search pattern: ";
    std::string pattern;
    std::getline(std::cin, pattern);
    
    auto positions = editor.search(pattern);
    
    if (positions.empty()) {
        std::cout << "Pattern not found.\n";
    } else {
        std::cout << "Found " << positions.size() << " occurrences at positions: ";
        for (size_t i = 0; i < positions.size() && i < 10; i++) {
            std::cout << positions[i] << " ";
        }
        if (positions.size() > 10) {
            std::cout << "... (+" << (positions.size() - 10) << " more)";
        }
        std::cout << '\n';
    }
}

void handleSearchReplace(TextEditor& editor) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter search pattern: ";
    std::string pattern;
    std::getline(std::cin, pattern);
    
    std::cout << "Enter replacement text: ";
    std::string replacement;
    std::getline(std::cin, replacement);
    
    int count = editor.searchAndReplace(pattern, replacement);
    std::cout << "✓ Replaced " << count << " occurrences.\n";
}

void handleSnapshot(TextEditor& editor) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter snapshot description: ";
    std::string desc;
    std::getline(std::cin, desc);
    
    editor.createSnapshot(desc);
    std::cout << "✓ Snapshot created.\n";
}

void handleViewVersions(const TextEditor& editor) {
    auto versions = editor.getVersions();
    
    if (versions.empty()) {
        std::cout << "No versions available.\n";
        return;
    }
    
    std::cout << "\n--- Version History ---\n";
    for (const auto& version : versions) {
        std::cout << "Version " << version->getVersionNumber() << ": "
                  << version->getDescription() << '\n';
        std::cout << "  Timestamp: " << version->getTimestamp() << '\n';
        std::cout << "  Content length: " << version->getContent().length() << " chars\n\n";
    }
}

void handleRestoreVersion(TextEditor& editor) {
    auto versions = editor.getVersions();
    
    if (versions.empty()) {
        std::cout << "No versions available.\n";
        return;
    }
    
    handleViewVersions(editor);
    
    int versionNum;
    std::cout << "Enter version number to restore: ";
    std::cin >> versionNum;
    
    editor.restoreVersion(versionNum);
    std::cout << "✓ Restored to version " << versionNum << ".\n";
}

void handleLoadFile(TextEditor& editor) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter filename to load: ";
    std::string filename;
    std::getline(std::cin, filename);
    
    if (editor.loadFromFile(filename)) {
        std::cout << "✓ File loaded successfully.\n";
    } else {
        std::cout << "✗ Failed to load file.\n";
    }
}

void handleSaveFile(TextEditor& editor) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter filename to save (or press Enter for current file): ";
    std::string filename;
    std::getline(std::cin, filename);
    
    if (editor.saveToFile(filename)) {
        std::cout << "✓ File saved successfully.\n";
    } else {
        std::cout << "✗ Failed to save file.\n";
    }
}

int main() {
    TextEditor editor;
    int choice;
    
    std::cout << "Welcome to Versioned Text Editor!\n";
    std::cout << "A console-based text editor with undo/redo and pattern search.\n";
    
    while (true) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
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
                    std::cout << "✓ Undo successful.\n";
                } else {
                    std::cout << "Nothing to undo.\n";
                }
                break;
            case 8:
                if (editor.redo()) {
                    std::cout << "✓ Redo successful.\n";
                } else {
                    std::cout << "Nothing to redo.\n";
                }
                break;
            case 9:
                handleSnapshot(editor);
                break;
            case 10:
                handleViewVersions(editor);
                break;
            case 11:
                handleRestoreVersion(editor);
                break;
            case 12:
                handleLoadFile(editor);
                break;
            case 13:
                handleSaveFile(editor);
                break;
            case 14:
                editor.clear();
                std::cout << "✓ Document cleared.\n";
                break;
            case 0:
                if (editor.isModified()) {
                    char save;
                    std::cout << "Document has unsaved changes. Save before exit? (y/n): ";
                    std::cin >> save;
                    if (save == 'y' || save == 'Y') {
                        handleSaveFile(editor);
                    }
                }
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    
    return 0;
}