#ifndef VERSION_MANAGER_H
#define VERSION_MANAGER_H

#include "Buffer.h"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <fstream>

using namespace std;

// Snapshot with file persistence support
class VersionSnapshot {
private:
    int id;
    string description;
    string timestamp;
    string content;
    string filename;  // Track which file this snapshot is from
    
public:
    VersionSnapshot(int versionId, const string& text, const string& desc = "", const string& file = "");
    
    int getId() const { return id; }
    string getDescription() const { return description; }
    string getTimestamp() const { return timestamp; }
    string getContent() const { return content; }
    string getFilename() const { return filename; }
    
    // Serialization
    string serialize() const;
    static shared_ptr<VersionSnapshot> deserialize(const string& data);
};

// Version Manager with auto-save/load
class VersionManager {
private:
    struct SnapshotNode {
        shared_ptr<VersionSnapshot> snapshot;
        SnapshotNode* next;
        SnapshotNode* prev;
        
        SnapshotNode(shared_ptr<VersionSnapshot> snap)
            : snapshot(snap), next(nullptr), prev(nullptr) {}
    };
    
    SnapshotNode* head;
    SnapshotNode* tail;
    SnapshotNode* current;
    int nextId;
    
    map<int, SnapshotNode*> idToNode;
    
    const string SNAPSHOT_FILE = "data/editor_snapshots.txt";
    const string VERSION_FILE = "data/editor_version.txt";
    
    // Auto-save helper
    void autoSave();
    
public:
    VersionManager();
    ~VersionManager();
    
    // Snapshot operations
    int createSnapshot(TextEditor& editor, const string& description = "");
    bool restoreVersion(TextEditor& editor, int versionId);
    bool restorePreviousVersion(TextEditor& editor);
    bool restoreNextVersion(TextEditor& editor);
    
    // Management
    void listVersions() const;
    void deleteVersion(int versionId);
    void exportVersion(int versionId, const string& filename);
    void clearAllVersions();
    
    // Getters
    int getVersionCount() const { return idToNode.size(); }
    int getCurrentVersionId() const { return current ? current->snapshot->getId() : -1; }
    
    // Persistence
    bool saveToFile();
    bool loadFromFile();
};

#endif