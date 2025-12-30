// #include "VersionManager.h"
// #include <iostream>
// #include <iomanip>
// #include <ctime>
// #include <fstream>
// #include <sstream>
// #include <algorithm>

// using namespace std;

// // ==================== VersionSnapshot Implementation ====================

// VersionSnapshot::VersionSnapshot(int id, const string& content, const string& desc)
//     : versionId(id), fullContent(content), description(desc), parent(nullptr) {
//     // Generate timestamp
//     time_t now = time(nullptr);
//     char buffer[80];
//     strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
//     timestamp = buffer;
// }

// void VersionSnapshot::displayInfo() const {
//     cout << "\n";
//     cout << "Version ID: " << versionId << endl;
//     cout << "Timestamp: " << timestamp << endl;
//     cout << "Description: " << (description.empty() ? "(No description)" : description) << endl;
//     cout << "Content Size: " << fullContent.length() << " characters" << endl;
    
//     if (parent) {
//         cout << "Parent Version: " << parent->getId() << endl;
//     } else {
//         cout << "Parent Version: None (Base version)" << endl;
//     }
// }

// // ==================== VersionManager Implementation ====================

// VersionManager::VersionManager() 
//     : head(nullptr), tail(nullptr), current(nullptr), nextVersionId(1) {}

// VersionManager::~VersionManager() {
//     clearAllVersions();
// }

// int VersionManager::createSnapshot(const TextEditor& editor, const string& description) {
//     string content = editor.getText();
//     auto snapshot = make_shared<VersionSnapshot>(nextVersionId, content, description);
    
//     // Set parent relationship
//     if (current && current->snapshot) {
//         snapshot->setParent(current->snapshot);
//     }
    
//     VersionNode* node = new VersionNode(snapshot);
    
//     if (!head) {
//         // First snapshot
//         head = tail = current = node;
//     } else {
//         // Add to end of list
//         tail->next = node;
//         node->prev = tail;
        
//         // Add as child of current
//         if (current) {
//             current->children.push_back(node);
//         }
        
//         tail = node;
//         current = node;
//     }
    
//     // Add to map for fast lookup
//     versionMap[nextVersionId] = node;
    
//     cout << "\n[SUCCESS] Snapshot created with Version ID: " << nextVersionId << endl;
    
//     return nextVersionId++;
// }

// bool VersionManager::restoreVersion(TextEditor& editor, int versionId) {
//     auto it = versionMap.find(versionId);
//     if (it == versionMap.end()) {
//         cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
//         return false;
//     }
    
//     VersionNode* node = it->second;
    
//     // Clear editor and load snapshot content
//     editor.clear();
//     string content = node->snapshot->getContent();
//     if (!content.empty()) {
//         editor.insert(0, content);
//     }
    
//     // Update current pointer
//     current = node;
    
//     return true;
// }

// bool VersionManager::restorePreviousVersion(TextEditor& editor) {
//     if (!current || !current->prev) {
//         cout << "\n[ERROR] No previous version available!" << endl;
//         return false;
//     }
    
//     current = current->prev;
    
//     editor.clear();
//     string content = current->snapshot->getContent();
//     if (!content.empty()) {
//         editor.insert(0, content);
//     }
    
//     cout << "\n[SUCCESS] Restored to Version " << current->snapshot->getId() << endl;
//     return true;
// }

// bool VersionManager::restoreNextVersion(TextEditor& editor) {
//     if (!current || !current->next) {
//         cout << "\n[ERROR] No next version available!" << endl;
//         return false;
//     }
    
//     current = current->next;
    
//     editor.clear();
//     string content = current->snapshot->getContent();
//     if (!content.empty()) {
//         editor.insert(0, content);
//     }
    
//     cout << "\n[SUCCESS] Restored to Version " << current->snapshot->getId() << endl;
//     return true;
// }

// shared_ptr<VersionSnapshot> VersionManager::getVersion(int versionId) const {
//     auto it = versionMap.find(versionId);
//     if (it != versionMap.end()) {
//         return it->second->snapshot;
//     }
//     return nullptr;
// }

// shared_ptr<VersionSnapshot> VersionManager::getCurrentVersion() const {
//     return current ? current->snapshot : nullptr;
// }

// int VersionManager::getCurrentVersionId() const {
//     return current ? current->snapshot->getId() : -1;
// }

// void VersionManager::listVersions() const {
//     if (!head) {
//         cout << "\n[INFO] No versions available." << endl;
//         return;
//     }
    
//     cout << "\n";
//     cout << "========================================" << endl;
//     cout << "          SNAPSHOT VERSIONS             " << endl;
//     cout << "========================================" << endl;
    
//     cout << setw(5) << "ID" << " | "
//          << setw(19) << left << "Timestamp" << " | "
//          << setw(10) << "Size" << " | "
//          << "Description" << endl;
//     cout << string(70, '-') << endl;
    
//     VersionNode* node = head;
//     while (node) {
//         auto snapshot = node->snapshot;
        
//         cout << setw(5) << snapshot->getId() << " | "
//              << setw(19) << left << snapshot->getTimestamp() << " | "
//              << setw(10) << snapshot->getContent().length() << " | "
//              << snapshot->getDescription();
        
//         if (node == current) {
//             cout << " <- CURRENT";
//         }
        
//         cout << endl;
        
//         node = node->next;
//     }
    
//     cout << string(70, '-') << endl;
//     cout << "Total Versions: " << versionMap.size() << endl;
// }

// vector<shared_ptr<VersionSnapshot>> VersionManager::getAllVersions() const {
//     vector<shared_ptr<VersionSnapshot>> versions;
//     VersionNode* node = head;
    
//     while (node) {
//         versions.push_back(node->snapshot);
//         node = node->next;
//     }
    
//     return versions;
// }

// bool VersionManager::deleteVersion(int versionId) {
//     auto it = versionMap.find(versionId);
//     if (it == versionMap.end()) {
//         cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
//         return false;
//     }
    
//     VersionNode* node = it->second;
    
//     // Don't allow deletion if it has children
//     if (!node->children.empty()) {
//         cout << "\n[ERROR] Cannot delete version with child versions!" << endl;
//         cout << "Please delete child versions first." << endl;
//         return false;
//     }
    
//     // Update linked list
//     if (node->prev) {
//         node->prev->next = node->next;
//     } else {
//         head = node->next;
//     }
    
//     if (node->next) {
//         node->next->prev = node->prev;
//     } else {
//         tail = node->prev;
//     }
    
//     // Update current if we're deleting current
//     if (current == node) {
//         current = node->prev ? node->prev : node->next;
//     }
    
//     // Remove from map
//     versionMap.erase(versionId);
    
//     // Delete node
//     delete node;
    
//     cout << "\n[SUCCESS] Version " << versionId << " deleted successfully!" << endl;
//     return true;
// }

// void VersionManager::clearAllVersions() {
//     VersionNode* node = head;
//     while (node) {
//         VersionNode* next = node->next;
//         delete node;
//         node = next;
//     }
    
//     head = tail = current = nullptr;
//     versionMap.clear();
//     nextVersionId = 1;
// }

// bool VersionManager::exportVersion(int versionId, const string& filename) const {
//     auto it = versionMap.find(versionId);
//     if (it == versionMap.end()) {
//         cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
//         return false;
//     }
    
//     ofstream file(filename);
//     if (!file.is_open()) {
//         cout << "\n[ERROR] Could not open file for writing: " << filename << endl;
//         return false;
//     }
    
//     auto snapshot = it->second->snapshot;
    
//     // Write metadata
//     file << "=== VERSION SNAPSHOT ===" << endl;
//     file << "Version ID: " << snapshot->getId() << endl;
//     file << "Timestamp: " << snapshot->getTimestamp() << endl;
//     file << "Description: " << snapshot->getDescription() << endl;
//     file << "Content Size: " << snapshot->getContent().length() << " characters" << endl;
//     file << "=== CONTENT START ===" << endl;
    
//     // Write content
//     file << snapshot->getContent();
    
//     file << endl << "=== CONTENT END ===" << endl;
    
//     file.close();
    
//     cout << "\n[SUCCESS] Version " << versionId << " exported to: " << filename << endl;
//     return true;
// }

// bool VersionManager::importVersion(const string& filename, const string& description) {
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cout << "\n[ERROR] Could not open file for reading: " << filename << endl;
//         return false;
//     }
    
//     stringstream buffer;
//     buffer << file.rdbuf();
//     string content = buffer.str();
    
//     file.close();
    
//     // Create new snapshot with imported content
//     auto snapshot = make_shared<VersionSnapshot>(nextVersionId, content, 
//                                                   description.empty() ? "Imported from " + filename : description);
    
//     VersionNode* node = new VersionNode(snapshot);
    
//     if (!head) {
//         head = tail = current = node;
//     } else {
//         tail->next = node;
//         node->prev = tail;
//         tail = node;
//         current = node;
//     }
    
//     versionMap[nextVersionId] = node;
    
//     cout << "\n[SUCCESS] Version imported with ID: " << nextVersionId << endl;
//     nextVersionId++;
    
//     return true;
// }

// void VersionManager::compareVersions(int versionId1, int versionId2) const {
//     auto snap1 = getVersion(versionId1);
//     auto snap2 = getVersion(versionId2);
    
//     if (!snap1 || !snap2) {
//         cout << "\n[ERROR] One or both versions not found!" << endl;
//         return;
//     }
    
//     string content1 = snap1->getContent();
//     string content2 = snap2->getContent();
    
//     cout << "\n========================================" << endl;
//     cout << "       VERSION COMPARISON               " << endl;
//     cout << "========================================" << endl;
    
//     cout << "\nVersion " << versionId1 << ":" << endl;
//     cout << "  Timestamp: " << snap1->getTimestamp() << endl;
//     cout << "  Size: " << content1.length() << " characters" << endl;
//     cout << "  Description: " << snap1->getDescription() << endl;
    
//     cout << "\nVersion " << versionId2 << ":" << endl;
//     cout << "  Timestamp: " << snap2->getTimestamp() << endl;
//     cout << "  Size: " << content2.length() << " characters" << endl;
//     cout << "  Description: " << snap2->getDescription() << endl;
    
//     // Calculate difference
//     int sizeDiff = (int)content2.length() - (int)content1.length();
//     cout << "\nSize Difference: " << (sizeDiff >= 0 ? "+" : "") << sizeDiff << " characters" << endl;
    
//     // Calculate similarity (simple character-by-character comparison)
//     size_t matches = 0;
//     size_t minLen = min(content1.length(), content2.length());
    
//     for (size_t i = 0; i < minLen; i++) {
//         if (content1[i] == content2[i]) {
//             matches++;
//         }
//     }
    
//     double similarity = minLen > 0 ? (100.0 * matches / minLen) : 100.0;
//     cout << "Similarity: " << fixed << setprecision(2) << similarity << "%" << endl;
    
//     cout << "========================================" << endl;
// }

// string VersionManager::getDiff(int versionId1, int versionId2) const {
//     auto snap1 = getVersion(versionId1);
//     auto snap2 = getVersion(versionId2);
    
//     if (!snap1 || !snap2) {
//         return "[ERROR] One or both versions not found!";
//     }
    
//     string content1 = snap1->getContent();
//     string content2 = snap2->getContent();
    
//     // Simple diff: show first difference
//     size_t minLen = min(content1.length(), content2.length());
    
//     for (size_t i = 0; i < minLen; i++) {
//         if (content1[i] != content2[i]) {
//             stringstream ss;
//             ss << "First difference at position " << i << ":\n";
//             ss << "Version " << versionId1 << ": '" << content1[i] << "'\n";
//             ss << "Version " << versionId2 << ": '" << content2[i] << "'";
//             return ss.str();
//         }
//     }
    
//     if (content1.length() != content2.length()) {
//         stringstream ss;
//         ss << "Content identical up to position " << minLen << "\n";
//         ss << "Length difference: " << ((int)content2.length() - (int)content1.length()) << " characters";
//         return ss.str();
//     }
    
//     return "Versions are identical!";
// }













#include "VersionManager.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <windows.h>

using namespace std;

// ==================== VersionSnapshot Implementation ====================

VersionSnapshot::VersionSnapshot(int versionId, const string& text, const string& desc, const string& file)
    : id(versionId), content(text), description(desc), filename(file) {
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    timestamp = buffer;
}

string VersionSnapshot::serialize() const {
    stringstream ss;
    ss << id << "|"
       << timestamp << "|"
       << filename << "|"
       << description << "|"
       << content.length() << "|"
       << content;
    return ss.str();
}

shared_ptr<VersionSnapshot> VersionSnapshot::deserialize(const string& data) {
    stringstream ss(data);
    int id;
    string timestamp, filename, description, contentLengthStr;
    size_t contentLength;
    
    getline(ss, contentLengthStr, '|');
    id = stoi(contentLengthStr);
    
    getline(ss, timestamp, '|');
    getline(ss, filename, '|');
    getline(ss, description, '|');
    getline(ss, contentLengthStr, '|');
    contentLength = stoi(contentLengthStr);
    
    string content;
    content.resize(contentLength);
    ss.read(&content[0], contentLength);
    
    auto snapshot = make_shared<VersionSnapshot>(id, content, description, filename);
    return snapshot;
}

// ==================== VersionManager Implementation ====================

VersionManager::VersionManager() 
    : head(nullptr), tail(nullptr), current(nullptr), nextId(1) {
    loadFromFile();  // Auto-load on startup
}

VersionManager::~VersionManager() {
    saveToFile();  // Auto-save on exit
    
    SnapshotNode* node = head;
    while (node) {
        SnapshotNode* next = node->next;
        delete node;
        node = next;
    }
}

void VersionManager::autoSave() {
    saveToFile();
}

int VersionManager::createSnapshot(TextEditor& editor, const string& description) {
    string content = editor.getText();
    string filename = editor.getFilename();
    if (filename.empty()) {
        filename = "(Untitled)";
    }
    
    auto snapshot = make_shared<VersionSnapshot>(nextId++, content, description, filename);
    
    SnapshotNode* node = new SnapshotNode(snapshot);
    
    if (!head) {
        head = tail = current = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
        current = node;
    }
    
    idToNode[snapshot->getId()] = node;
    
    autoSave();  // Save after creating
    return snapshot->getId();
}

bool VersionManager::restoreVersion(TextEditor& editor, int versionId) {
    auto it = idToNode.find(versionId);
    if (it == idToNode.end()) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
        SetConsoleTextAttribute(hConsole, 7);
        return false;
    }
    
    current = it->second;
    editor.clear();
    editor.insert(0, current->snapshot->getContent());
    
    return true;
}

bool VersionManager::restorePreviousVersion(TextEditor& editor) {
    if (!current || !current->prev) {
        return false;
    }
    
    current = current->prev;
    editor.clear();
    editor.insert(0, current->snapshot->getContent());
    return true;
}

bool VersionManager::restoreNextVersion(TextEditor& editor) {
    if (!current || !current->next) {
        return false;
    }
    
    current = current->next;
    editor.clear();
    editor.insert(0, current->snapshot->getContent());
    return true;
}

void VersionManager::listVersions() const {
    if (!head) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 11);
        cout << "\n[INFO] No versions saved yet." << endl;
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n" << string(90, '=') << endl;
    cout << "                       SAVED SNAPSHOTS" << endl;
    cout << string(90, '=') << endl;
    SetConsoleTextAttribute(hConsole, 7);
    
    cout << setw(5) << "ID" << " | "
         << setw(19) << "Timestamp" << " | "
         << setw(20) << left << "File" << " | "
         << setw(10) << "Size" << " | "
         << "Description" << endl;
    cout << string(90, '-') << endl;
    
    SnapshotNode* node = head;
    while (node) {
        auto snapshot = node->snapshot;
        
        if (node == current) {
            SetConsoleTextAttribute(hConsole, 10);
            cout << "► ";
        } else {
            cout << "  ";
        }
        
        string displayFilename = snapshot->getFilename();
        if (displayFilename.length() > 20) {
            displayFilename = displayFilename.substr(0, 17) + "...";
        }
        
        cout << setw(3) << snapshot->getId() << " | "
             << setw(19) << snapshot->getTimestamp() << " | "
             << setw(20) << left << displayFilename << " | "
             << setw(8) << snapshot->getContent().length() << " | "
             << snapshot->getDescription() << endl;
        
        SetConsoleTextAttribute(hConsole, 7);
        node = node->next;
    }
    
    cout << string(90, '-') << endl;
    SetConsoleTextAttribute(hConsole, 14);
    cout << "Total snapshots: " << idToNode.size() << endl;
    SetConsoleTextAttribute(hConsole, 7);
}

void VersionManager::deleteVersion(int versionId) {
    auto it = idToNode.find(versionId);
    if (it == idToNode.end()) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }
    
    SnapshotNode* node = it->second;
    
    // Update linked list
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }
    
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }
    
    // Update current
    if (current == node) {
        current = node->prev ? node->prev : node->next;
    }
    
    idToNode.erase(versionId);
    delete node;
    
    autoSave();  // Save after deletion
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "\n[SUCCESS] Version " << versionId << " deleted!" << endl;
    SetConsoleTextAttribute(hConsole, 7);
}

void VersionManager::exportVersion(int versionId, const string& filename) {
    auto it = idToNode.find(versionId);
    if (it == idToNode.end()) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\n[ERROR] Could not create file: " << filename << endl;
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }
    
    file << it->second->snapshot->getContent();
    file.close();
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "\n[SUCCESS] Version " << versionId << " exported to " << filename << endl;
    SetConsoleTextAttribute(hConsole, 7);
}

void VersionManager::clearAllVersions() {
    SnapshotNode* node = head;
    while (node) {
        SnapshotNode* next = node->next;
        delete node;
        node = next;
    }
    
    head = tail = current = nullptr;
    idToNode.clear();
    nextId = 1;
    
    autoSave();  // Save empty state
}

bool VersionManager::saveToFile() {
    ofstream file(SNAPSHOT_FILE, ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // Write header
    file << "EDITOR_SNAPSHOTS_V1" << endl;
    file << nextId << endl;
    file << idToNode.size() << endl;
    
    // Write current version ID
    int currentId = current ? current->snapshot->getId() : -1;
    file << currentId << endl;
    
    // Write all snapshots
    SnapshotNode* node = head;
    while (node) {
        string serialized = node->snapshot->serialize();
        file << serialized.length() << endl;
        file << serialized << endl;
        node = node->next;
    }
    
    file.close();
    return true;
}

bool VersionManager::loadFromFile() {
    ifstream file(SNAPSHOT_FILE, ios::binary);
    if (!file.is_open()) {
        return false;  // File doesn't exist yet, that's okay
    }
    
    string header;
    getline(file, header);
    
    if (header != "EDITOR_SNAPSHOTS_V1") {
        file.close();
        return false;
    }
    
    // Read metadata
    file >> nextId;
    
    int count;
    file >> count;
    
    int currentId;
    file >> currentId;
    file.ignore();  // Skip newline
    
    // Clear existing data
    SnapshotNode* node = head;
    while (node) {
        SnapshotNode* next = node->next;
        delete node;
        node = next;
    }
    head = tail = current = nullptr;
    idToNode.clear();
    
    // Load all snapshots
    for (int i = 0; i < count; i++) {
        string lengthStr;
        getline(file, lengthStr);
        size_t length = stoi(lengthStr);
        
        string serialized;
        serialized.resize(length);
        file.read(&serialized[0], length);
        file.ignore();  // Skip newline
        
        auto snapshot = VersionSnapshot::deserialize(serialized);
        
        SnapshotNode* newNode = new SnapshotNode(snapshot);
        
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        
        idToNode[snapshot->getId()] = newNode;
        
        if (snapshot->getId() == currentId) {
            current = newNode;
        }
    }
    
    file.close();
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "\n[INFO] Loaded " << count << " snapshot(s) from file." << endl;
    SetConsoleTextAttribute(hConsole, 7);
    
    return true;
}