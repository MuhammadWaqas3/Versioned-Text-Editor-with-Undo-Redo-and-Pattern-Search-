#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>

using namespace std;

// Version/Snapshot management with persistence
class Version {
private:
    string content;
    string timestamp;
    string description;
    string filename;  // Track which file
    int versionNumber;
    
public:
    Version(const string& content, int versionNum, const string& desc = "", const string& file = "");
    
    string getContent() const { return content; }
    string getTimestamp() const { return timestamp; }
    string getDescription() const { return description; }
    string getFilename() const { return filename; }
    int getVersionNumber() const { return versionNumber; }
    
    // Serialization
    string serialize() const;
    static shared_ptr<Version> deserialize(const string& data);
};

// Doubly linked list for version history with file persistence
class VersionHistory {
private:
    struct VersionNode {
        shared_ptr<Version> version;
        VersionNode* next;
        VersionNode* prev;
        
        VersionNode(shared_ptr<Version> v) : version(v), next(nullptr), prev(nullptr) {}
    };
    
    VersionNode* head;
    VersionNode* tail;
    VersionNode* current;
    int versionCount;
    
    const string VERSION_FILE = "data/editor_version_history.txt";
    
    void autoSave();
    
public:
    VersionHistory();
    ~VersionHistory();
    
    void addVersion(shared_ptr<Version> version);
    void addVersion(const string& content, int versionNum, const string& desc, const string& filename);
    shared_ptr<Version> getCurrentVersion() const;
    shared_ptr<Version> getPreviousVersion();
    shared_ptr<Version> getNextVersion();
    
    vector<shared_ptr<Version>> getAllVersions() const;
    void clear();
    int getCount() const { return versionCount; }
    
    // Persistence
    bool saveToFile();
    bool loadFromFile();
};

#endif // HISTORY_H