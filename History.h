#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

// Version/Snapshot management
class Version {
private:
    string content;
    string timestamp;
    string description;
    int versionNumber;
    
public:
    Version(const string& content, int versionNum, const string& desc = "");
    
    string getContent() const { return content; }
    string getTimestamp() const { return timestamp; }
    string getDescription() const { return description; }
    int getVersionNumber() const { return versionNumber; }
};

// Doubly linked list for version history
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
    
public:
    VersionHistory();
    ~VersionHistory();
    
    void addVersion(shared_ptr<Version> version);
    shared_ptr<Version> getCurrentVersion() const;
    shared_ptr<Version> getPreviousVersion();
    shared_ptr<Version> getNextVersion();
    
    vector<shared_ptr<Version>> getAllVersions() const;
    void clear();
    int getCount() const { return versionCount; }
};

#endif // HISTORY_H