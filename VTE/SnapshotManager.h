// #ifndef SNAPSHOT_MANAGER_H
// #define SNAPSHOT_MANAGER_H

// #include <string>
// #include <vector>
// #include <memory>
// #include <map>

// using namespace std;

// // Represents a single snapshot with delta storage
// class Snapshot {
// private:
//     int id;
//     string name;
//     string timestamp;
//     string description;
    
//     // Delta storage - stores only changes from parent
//     struct Delta {
//         enum Type { INSERT, DELETE, REPLACE };
//         Type type;
//         size_t position;
//         string content;
//         size_t length;  // For DELETE/REPLACE operations
        
//         Delta(Type t, size_t pos, const string& cont, size_t len = 0)
//             : type(t), position(pos), content(cont), length(len) {}
//     };
    
//     vector<Delta> deltas;
//     shared_ptr<Snapshot> parent;  // Pointer to parent snapshot
//     string fullContent;  // Only stored if it's a base snapshot
//     bool isBase;
    
// public:
//     Snapshot(int snapshotId, const string& snapName, const string& content, 
//              const string& desc = "", shared_ptr<Snapshot> parentSnap = nullptr);
    
//     // Getters
//     int getId() const { return id; }
//     string getName() const { return name; }
//     string getTimestamp() const { return timestamp; }
//     string getDescription() const { return description; }
//     bool isBaseSnapshot() const { return isBase; }
//     shared_ptr<Snapshot> getParent() const { return parent; }
    
//     // Reconstruct full content by applying deltas from parent chain
//     string reconstructContent() const;
    
//     // Store delta from parent
//     void addDelta(Delta::Type type, size_t pos, const string& content, size_t length = 0);
    
//     // Get delta count
//     size_t getDeltaCount() const { return deltas.size(); }
    
//     // Display snapshot info
//     void displayInfo() const;
// };

// // Manages all snapshots with linked list structure
// class SnapshotManager {
// private:
//     struct SnapshotNode {
//         shared_ptr<Snapshot> snapshot;
//         SnapshotNode* next;
//         SnapshotNode* prev;
//         vector<SnapshotNode*> children;  // Support branching
        
//         SnapshotNode(shared_ptr<Snapshot> snap) 
//             : snapshot(snap), next(nullptr), prev(nullptr) {}
//     };
    
//     SnapshotNode* head;
//     SnapshotNode* tail;
//     SnapshotNode* current;
//     int nextId;
    
//     map<int, SnapshotNode*> idToNode;  // Fast lookup by ID
//     map<string, SnapshotNode*> nameToNode;  // Fast lookup by name
    
//     // Helper to compute deltas between two texts
//     void computeDeltas(const string& oldText, const string& newText, 
//                       shared_ptr<Snapshot> snapshot);
    
// public:
//     SnapshotManager();
//     ~SnapshotManager();
    
//     // Create new snapshot
//     shared_ptr<Snapshot> createSnapshot(const string& name, const string& content, 
//                                        const string& description = "");
    
//     // Create snapshot as child of current (branching)
//     shared_ptr<Snapshot> createBranchSnapshot(const string& name, const string& content,
//                                              const string& description = "");
    
//     // Retrieve snapshot
//     shared_ptr<Snapshot> getSnapshot(int id) const;
//     shared_ptr<Snapshot> getSnapshot(const string& name) const;
//     shared_ptr<Snapshot> getCurrentSnapshot() const;
    
//     // Navigation
//     bool moveToSnapshot(int id);
//     bool moveToSnapshot(const string& name);
//     bool moveToNext();
//     bool moveToPrevious();
    
//     // List operations
//     vector<shared_ptr<Snapshot>> getAllSnapshots() const;
//     void displaySnapshotTree() const;
//     void displaySnapshotList() const;
    
//     // Comparison
//     void compareSnapshots(int id1, int id2) const;
//     void compareSnapshots(const string& name1, const string& name2) const;
    
//     // Delete snapshot
//     bool deleteSnapshot(int id);
//     bool deleteSnapshot(const string& name);
    
//     // Utilities
//     int getSnapshotCount() const { return idToNode.size(); }
//     bool hasSnapshots() const { return head != nullptr; }
    
//     // Clear all
//     void clear();
// };

// #endif // SNAPSHOT_MANAGER_H













#ifndef SNAPSHOT_MANAGER_H
#define SNAPSHOT_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <map>

using namespace std;

// Represents a single snapshot with delta storage
class Snapshot {
public:
    // Delta storage - stores only changes from parent
    struct Delta {
        enum Type { INSERT, DELETE, REPLACE };
        Type type;
        size_t position;
        string content;
        size_t length;  // For DELETE/REPLACE operations
        
        Delta(Type t, size_t pos, const string& cont, size_t len = 0)
            : type(t), position(pos), content(cont), length(len) {}
    };

private:
    int id;
    string name;
    string timestamp;
    string description;
    
    vector<Delta> deltas;
    shared_ptr<Snapshot> parent;  // Pointer to parent snapshot
    string fullContent;  // Only stored if it's a base snapshot
    bool isBase;
    
public:
    Snapshot(int snapshotId, const string& snapName, const string& content, 
             const string& desc = "", shared_ptr<Snapshot> parentSnap = nullptr);
    
    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getTimestamp() const { return timestamp; }
    string getDescription() const { return description; }
    bool isBaseSnapshot() const { return isBase; }
    shared_ptr<Snapshot> getParent() const { return parent; }
    
    // Reconstruct full content by applying deltas from parent chain
    string reconstructContent() const;
    
    // Store delta from parent
    void addDelta(Delta::Type type, size_t pos, const string& content, size_t length = 0);
    
    // Get delta count
    size_t getDeltaCount() const { return deltas.size(); }
    
    // Display snapshot info
    void displayInfo() const;
};

// Manages all snapshots with linked list structure
class SnapshotManager {
private:
    struct SnapshotNode {
        shared_ptr<Snapshot> snapshot;
        SnapshotNode* next;
        SnapshotNode* prev;
        vector<SnapshotNode*> children;  // Support branching
        
        SnapshotNode(shared_ptr<Snapshot> snap) 
            : snapshot(snap), next(nullptr), prev(nullptr) {}
    };
    
    SnapshotNode* head;
    SnapshotNode* tail;
    SnapshotNode* current;
    int nextId;
    
    map<int, SnapshotNode*> idToNode;  // Fast lookup by ID
    map<string, SnapshotNode*> nameToNode;  // Fast lookup by name
    
    // Helper to compute deltas between two texts
    void computeDeltas(const string& oldText, const string& newText, 
                      shared_ptr<Snapshot> snapshot);
    
public:
    SnapshotManager();
    ~SnapshotManager();
    
    // Create new snapshot
    shared_ptr<Snapshot> createSnapshot(const string& name, const string& content, 
                                       const string& description = "");
    
    // Create snapshot as child of current (branching)
    shared_ptr<Snapshot> createBranchSnapshot(const string& name, const string& content,
                                             const string& description = "");
    
    // Retrieve snapshot
    shared_ptr<Snapshot> getSnapshot(int id) const;
    shared_ptr<Snapshot> getSnapshot(const string& name) const;
    shared_ptr<Snapshot> getCurrentSnapshot() const;
    
    // Navigation
    bool moveToSnapshot(int id);
    bool moveToSnapshot(const string& name);
    bool moveToNext();
    bool moveToPrevious();
    
    // List operations
    vector<shared_ptr<Snapshot>> getAllSnapshots() const;
    void displaySnapshotTree() const;
    void displaySnapshotList() const;
    
    // Comparison
    void compareSnapshots(int id1, int id2) const;
    void compareSnapshots(const string& name1, const string& name2) const;
    
    // Delete snapshot
    bool deleteSnapshot(int id);
    bool deleteSnapshot(const string& name);
    
    // Utilities
    int getSnapshotCount() const { return idToNode.size(); }
    bool hasSnapshots() const { return head != nullptr; }
    
    // Clear all
    void clear();
};

#endif // SNAPSHOT_MANAGER_H