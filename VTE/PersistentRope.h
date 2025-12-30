// #ifndef HISTORY_H
// #define HISTORY_H

// #include "PersistentRope.h"
// #include <string>
// #include <memory>
// #include <vector>
// #include <ctime>

// using namespace std;

// // Version node in the version tree
// struct VersionNode {
//     int versionId;
//     PersistentRope rope;
//     string description;
//     string timestamp;
//     shared_ptr<VersionNode> parent;
//     vector<shared_ptr<VersionNode>> children;
//     size_t memoryFootprint;  // Approximate memory usage
    
//     VersionNode(int id, const PersistentRope& r, const string& desc, shared_ptr<VersionNode> p = nullptr);
    
//     string getTimestamp() const;
// };

// // Version history manager with tree structure
// class VersionHistory {
// private:
//     shared_ptr<VersionNode> root;
//     shared_ptr<VersionNode> current;
//     int nextVersionId;
    
//     // Helper functions
//     void calculateMemoryFootprint(shared_ptr<VersionNode> node);
//     void printTree(shared_ptr<VersionNode> node, int depth, vector<bool>& hasNext) const;
//     vector<shared_ptr<VersionNode>> getPathToRoot(shared_ptr<VersionNode> node) const;
    
// public:
//     VersionHistory(const string& initialText = "");
    
//     // Version operations
//     void insert(size_t pos, const string& text, const string& description = "Insert");
//     void deleteText(size_t pos, size_t length, const string& description = "Delete");
//     void replace(size_t pos, size_t length, const string& text, const string& description = "Replace");
    
//     // Navigation
//     bool goBack();
//     bool goForward();  // Goes to first child if available
//     bool goToVersion(int versionId);
//     bool goToChild(int childIndex);
    
//     // Query operations
//     shared_ptr<VersionNode> getCurrentVersion() const { return current; }
//     shared_ptr<VersionNode> getRootVersion() const { return root; }
//     int getCurrentVersionId() const { return current ? current->versionId : -1; }
//     int getTotalVersions() const;
    
//     // Display operations
//     void printVersionTree() const;
//     void printVersionPath() const;
//     void listAllVersions() const;
    
//     // Analysis
//     void analyzeMemoryUsage() const;
//     void compareTwoVersions(int id1, int id2) const;
//     string getDiff(int id1, int id2) const;
// };

// // Simple linked list version history (alternative simpler approach)
// struct SimpleVersion {
//     string content;
//     int versionNumber;
//     string description;
//     string timestamp;
    
//     SimpleVersion(const string& c, int v, const string& d);
//     string getTimestamp() const;
//     string getDescription() const { return description; }
//     int getVersionNumber() const { return versionNumber; }
// };

// class SimpleVersionHistory {
// private:
//     struct Node {
//         shared_ptr<SimpleVersion> version;
//         Node* prev;
//         Node* next;
        
//         Node(shared_ptr<SimpleVersion> v) : version(v), prev(nullptr), next(nullptr) {}
//     };
    
//     Node* head;
//     Node* tail;
//     Node* current;
//     int count;
    
// public:
//     SimpleVersionHistory();
//     ~SimpleVersionHistory();
    
//     void addVersion(shared_ptr<SimpleVersion> version);
//     shared_ptr<SimpleVersion> getCurrentVersion() const;
//     shared_ptr<SimpleVersion> getPreviousVersion();
//     shared_ptr<SimpleVersion> getNextVersion();
    
//     vector<shared_ptr<SimpleVersion>> getAllVersions() const;
//     int getCount() const { return count; }
//     void clear();
// };

// #endif // HISTORY_H











#ifndef PERSISTENT_ROPE_H
#define PERSISTENT_ROPE_H

#include <string>
#include <memory>
#include <vector>

using namespace std;

// Node structure for rope
struct RopeNode {
    string data;
    size_t weight;
    shared_ptr<RopeNode> left;
    shared_ptr<RopeNode> right;
    
    RopeNode(const string& str) 
        : data(str), weight(str.length()), left(nullptr), right(nullptr) {}
    
    RopeNode(shared_ptr<RopeNode> l, shared_ptr<RopeNode> r)
        : data(""), left(l), right(r) {
        weight = l ? l->totalWeight() : 0;
    }
    
    bool isLeaf() const { return left == nullptr && right == nullptr; }
    
    size_t totalWeight() const {
        if (isLeaf()) return data.length();
        return weight + (right ? right->totalWeight() : 0);
    }
};

class PersistentRope {
private:
    shared_ptr<RopeNode> root;
    static const size_t MAX_LEAF_SIZE = 256;
    
    shared_ptr<RopeNode> concatenate(shared_ptr<RopeNode> left, shared_ptr<RopeNode> right) const;
    pair<shared_ptr<RopeNode>, shared_ptr<RopeNode>> split(shared_ptr<RopeNode> node, size_t pos) const;
    int getHeight(shared_ptr<RopeNode> node) const;
    shared_ptr<RopeNode> rebalance(shared_ptr<RopeNode> node) const;
    void collectText(shared_ptr<RopeNode> node, string& result) const;
    
public:
    PersistentRope(const string& text = "");
    PersistentRope(shared_ptr<RopeNode> r) : root(r) {}
    
    PersistentRope insert(size_t pos, const string& text) const;
    PersistentRope deleteText(size_t pos, size_t length) const;
    PersistentRope replace(size_t pos, size_t length, const string& text) const;
    
    string getText() const;
    string substring(size_t pos, size_t length) const;
    size_t length() const { return root ? root->totalWeight() : 0; }
    
    shared_ptr<RopeNode> getRoot() const { return root; }
};

#endif