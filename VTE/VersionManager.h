// #ifndef VERSION_MANAGER_H
// #define VERSION_MANAGER_H

// #include <string>
// #include <vector>
// #include <memory>
// #include <map>
// #include "Buffer.h"

// using namespace std;

// // Snapshot version with delta-based storage
// class VersionSnapshot {
// private:
//     int versionId;
//     string description;
//     string timestamp;
//     string fullContent;  // Full content stored
//     shared_ptr<VersionSnapshot> parent;
    
// public:
//     VersionSnapshot(int id, const string& content, const string& desc = "");
    
//     int getId() const { return versionId; }
//     string getContent() const { return fullContent; }
//     string getDescription() const { return description; }
//     string getTimestamp() const { return timestamp; }
//     shared_ptr<VersionSnapshot> getParent() const { return parent; }
    
//     void setParent(shared_ptr<VersionSnapshot> p) { parent = p; }
//     void displayInfo() const;
// };

// // Manages snapshots with doubly linked list
// class VersionManager {
// private:
//     struct VersionNode {
//         shared_ptr<VersionSnapshot> snapshot;
//         VersionNode* prev;
//         VersionNode* next;
//         vector<VersionNode*> children;
        
//         VersionNode(shared_ptr<VersionSnapshot> snap)
//             : snapshot(snap), prev(nullptr), next(nullptr) {}
        
//         ~VersionNode() {
//             // Don't delete children here - they're managed by the list
//         }
//     };
    
//     VersionNode* head;
//     VersionNode* tail;
//     VersionNode* current;
//     int nextVersionId;
    
//     map<int, VersionNode*> versionMap;  // Fast lookup by version ID
    
// public:
//     VersionManager();
//     ~VersionManager();
    
//     // Create snapshot from current editor state
//     int createSnapshot(const TextEditor& editor, const string& description = "");
    
//     // Restore specific version
//     bool restoreVersion(TextEditor& editor, int versionId);
    
//     // Navigate versions
//     bool restorePreviousVersion(TextEditor& editor);
//     bool restoreNextVersion(TextEditor& editor);
    
//     // Query operations
//     shared_ptr<VersionSnapshot> getVersion(int versionId) const;
//     shared_ptr<VersionSnapshot> getCurrentVersion() const;
//     int getCurrentVersionId() const;
    
//     // List operations
//     void listVersions() const;
//     vector<shared_ptr<VersionSnapshot>> getAllVersions() const;
    
//     // Delete operations
//     bool deleteVersion(int versionId);
//     void clearAllVersions();
    
//     // Export/Import
//     bool exportVersion(int versionId, const string& filename) const;
//     bool importVersion(const string& filename, const string& description = "");
    
//     // Statistics
//     int getVersionCount() const { return versionMap.size(); }
//     bool hasVersions() const { return head != nullptr; }
    
//     // Comparison
//     void compareVersions(int versionId1, int versionId2) const;
//     string getDiff(int versionId1, int versionId2) const;
// };

// #endif // VERSION_MANAGER_H












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
    
    const string SNAPSHOT_FILE = "editor_snapshots.txt";
    const string VERSION_FILE = "editor_versions.txt";
    
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

#endif // VERSION_MANAGER_H