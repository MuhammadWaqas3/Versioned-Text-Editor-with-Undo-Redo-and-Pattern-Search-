#include "VersionManager.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <sys/stat.h>
#include <direct.h>

using namespace std;

// Helper function to ensure data folder exists
void ensureDataFolderForVersions() {
    struct stat info;
    if (stat("data", &info) != 0) {
        #ifdef _WIN32
            _mkdir("data");
        #else
            mkdir("data", 0755);
        #endif
    }
}

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
    ensureDataFolderForVersions();  // Create data folder if needed
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
    cout << "                       SAVED SNAPSHOTS (from data folder)" << endl;
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
    
    // Export to data folder
    ensureDataFolderForVersions();
    string exportPath = addDataFolder(filename);
    
    ofstream file(exportPath);
    if (!file.is_open()) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\n[ERROR] Could not create file: " << exportPath << endl;
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }
    
    file << it->second->snapshot->getContent();
    file.close();
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "\n[SUCCESS] Version " << versionId << " exported to " << exportPath << endl;
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
    ensureDataFolderForVersions();  // Ensure data folder exists
    
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
    cout << "\n[INFO] Loaded " << count << " snapshot(s) from data folder." << endl;
    SetConsoleTextAttribute(hConsole, 7);
    
    return true;
}