#include "History.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <sys/stat.h>
#include <direct.h>

using namespace std;

// Helper function to ensure data folder exists
void ensureDataFolder() {
    struct stat info;
    if (stat("data", &info) != 0) {
        #ifdef _WIN32
            _mkdir("data");
        #else
            mkdir("data", 0755);
        #endif
    }
}

// ==================== Version Implementation ====================

Version::Version(const string& content, int versionNum, const string& desc, const string& file)
    : content(content), versionNumber(versionNum), description(desc), filename(file) {
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    timestamp = buffer;
}

string Version::serialize() const {
    stringstream ss;
    ss << versionNumber << "|"
       << timestamp << "|"
       << filename << "|"
       << description << "|"
       << content.length() << "|"
       << content;
    return ss.str();
}

shared_ptr<Version> Version::deserialize(const string& data) {
    stringstream ss(data);
    int versionNum;
    string timestamp, filename, description, contentLengthStr;
    size_t contentLength;
    
    getline(ss, contentLengthStr, '|');
    versionNum = stoi(contentLengthStr);
    
    getline(ss, timestamp, '|');
    getline(ss, filename, '|');
    getline(ss, description, '|');
    getline(ss, contentLengthStr, '|');
    contentLength = stoi(contentLengthStr);
    
    string content;
    content.resize(contentLength);
    ss.read(&content[0], contentLength);
    
    return make_shared<Version>(content, versionNum, description, filename);
}

// ==================== VersionHistory Implementation ====================

VersionHistory::VersionHistory() 
    : head(nullptr), tail(nullptr), current(nullptr), versionCount(0) {
    ensureDataFolder();  // Create data folder if needed
    loadFromFile();  // Auto-load on startup
}

VersionHistory::~VersionHistory() {
    saveToFile();  // Auto-save on exit FIRST
    
    // Then clean up memory
    VersionNode* node = head;
    while (node) {
        VersionNode* next = node->next;
        delete node;
        node = next;
    }
}

void VersionHistory::autoSave() {
    saveToFile();
}

void VersionHistory::addVersion(shared_ptr<Version> version) {
    VersionNode* node = new VersionNode(version);
    
    if (!head) {
        head = tail = current = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
        current = node;
    }
    
    versionCount++;
    autoSave();  // Save after adding
}

void VersionHistory::addVersion(const string& content, int versionNum, const string& desc, const string& filename) {
    auto version = make_shared<Version>(content, versionNum, desc, filename);
    addVersion(version);
}

shared_ptr<Version> VersionHistory::getCurrentVersion() const {
    return current ? current->version : nullptr;
}

shared_ptr<Version> VersionHistory::getPreviousVersion() {
    if (current && current->prev) {
        current = current->prev;
        return current->version;
    }
    return nullptr;
}

shared_ptr<Version> VersionHistory::getNextVersion() {
    if (current && current->next) {
        current = current->next;
        return current->version;
    }
    return nullptr;
}

vector<shared_ptr<Version>> VersionHistory::getAllVersions() const {
    vector<shared_ptr<Version>> versions;
    VersionNode* node = head;
    while (node) {
        versions.push_back(node->version);
        node = node->next;
    }
    return versions;
}

void VersionHistory::clear() {
    VersionNode* node = head;
    while (node) {
        VersionNode* next = node->next;
        delete node;
        node = next;
    }
    head = tail = current = nullptr;
    versionCount = 0;
    
    // Save empty state to file
    ensureDataFolder();
    ofstream file(VERSION_FILE, ios::binary);
    if (file.is_open()) {
        file << "EDITOR_VERSIONS_V1" << endl;
        file << 0 << endl;  // count = 0
        file << -1 << endl; // currentNum = -1
        file.close();
    }
}

bool VersionHistory::saveToFile() {
    ensureDataFolder();  // Ensure data folder exists
    
    cout << "[DEBUG] Saving " << versionCount << " versions to " << VERSION_FILE << "..." << endl;
    
    ofstream file(VERSION_FILE, ios::binary);
    if (!file.is_open()) {
        cout << "[ERROR] Could not open file for writing!" << endl;
        return false;
    }
    
    // Write header
    file << "EDITOR_VERSIONS_V1" << endl;
    file << versionCount << endl;
    
    // Write current version number
    int currentNum = current ? current->version->getVersionNumber() : -1;
    file << currentNum << endl;
    
    // Write all versions
    VersionNode* node = head;
    int savedCount = 0;
    while (node) {
        string serialized = node->version->serialize();
        file << serialized.length() << endl;
        file << serialized << endl;
        savedCount++;
        node = node->next;
    }
    
    file.close();
    cout << "[DEBUG] Saved " << savedCount << " versions successfully to data folder!" << endl;
    return true;
}

bool VersionHistory::loadFromFile() {
    ifstream file(VERSION_FILE, ios::binary);
    if (!file.is_open()) {
        // File doesn't exist yet - first run
        cout << "[DEBUG] Version file not found in data folder - first run" << endl;
        return false;
    }
    
    string header;
    getline(file, header);
    
    if (header != "EDITOR_VERSIONS_V1") {
        cout << "[DEBUG] Invalid header: " << header << endl;
        file.close();
        return false;
    }
    
    // Read metadata
    int count;
    file >> count;
    cout << "[DEBUG] Loading " << count << " versions from data folder..." << endl;
    
    int currentNum;
    file >> currentNum;
    file.ignore();  // Skip newline
    
    // Clear existing data
    VersionNode* node = head;
    while (node) {
        VersionNode* next = node->next;
        delete node;
        node = next;
    }
    head = tail = current = nullptr;
    versionCount = 0;
    
    // Load all versions
    for (int i = 0; i < count; i++) {
        string lengthStr;
        getline(file, lengthStr);
        
        if (lengthStr.empty()) {
            cout << "[DEBUG] Empty length string at version " << i << endl;
            break;  // No more data
        }
        
        size_t length = stoi(lengthStr);
        cout << "[DEBUG] Reading version " << (i+1) << ", size: " << length << endl;
        
        string serialized;
        serialized.resize(length);
        file.read(&serialized[0], length);
        file.ignore();  // Skip newline
        
        auto version = Version::deserialize(serialized);
        
        VersionNode* newNode = new VersionNode(version);
        
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        
        versionCount++;
        
        if (version->getVersionNumber() == currentNum) {
            current = newNode;
        }
    }
    
    // Set current to tail if not found
    if (!current && tail) {
        current = tail;
    }
    
    file.close();
    
    if (count > 0) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 10);
        cout << "[SUCCESS] Loaded " << versionCount << " version(s) from data folder." << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
    
    return true;
}