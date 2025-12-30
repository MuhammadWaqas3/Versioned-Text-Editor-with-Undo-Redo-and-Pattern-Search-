#include "SnapshotManager.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;

// ==================== Snapshot Implementation ====================

Snapshot::Snapshot(int snapshotId, const string& snapName, const string& content,
                   const string& desc, shared_ptr<Snapshot> parentSnap)
    : id(snapshotId), name(snapName), description(desc), 
      parent(parentSnap), isBase(parentSnap == nullptr) {
    
    // Generate timestamp
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    timestamp = buffer;
    
    if (isBase) {
        // Base snapshot stores full content
        fullContent = content;
    } else {
        // Child snapshot stores only deltas
        // We'll compute deltas in SnapshotManager
    }
}

string Snapshot::reconstructContent() const {
    if (isBase) {
        return fullContent;
    }
    
    // Reconstruct from parent chain
    if (!parent) {
        return "";
    }
    
    string content = parent->reconstructContent();
    
    // Apply deltas in order
    for (const auto& delta : deltas) {
        switch (delta.type) {
            case Delta::INSERT:
                if (delta.position <= content.length()) {
                    content.insert(delta.position, delta.content);
                }
                break;
                
            case Delta::DELETE:
                if (delta.position < content.length()) {
                    size_t len = min(delta.length, content.length() - delta.position);
                    content.erase(delta.position, len);
                }
                break;
                
            case Delta::REPLACE:
                if (delta.position < content.length()) {
                    size_t len = min(delta.length, content.length() - delta.position);
                    content.replace(delta.position, len, delta.content);
                }
                break;
        }
    }
    
    return content;
}

void Snapshot::addDelta(Delta::Type type, size_t pos, const string& content, size_t length) {
    deltas.push_back(Delta(type, pos, content, length));
}

void Snapshot::displayInfo() const {
    cout << "ID: " << id << " | Name: " << name 
         << " | Time: " << timestamp << endl;
    cout << "Description: " << (description.empty() ? "(none)" : description) << endl;
    cout << "Type: " << (isBase ? "Base" : "Delta") 
         << " | Deltas: " << deltas.size() << endl;
    if (parent) {
        cout << "Parent: " << parent->getName() << " (ID: " << parent->getId() << ")" << endl;
    }
}

// ==================== SnapshotManager Implementation ====================

SnapshotManager::SnapshotManager() 
    : head(nullptr), tail(nullptr), current(nullptr), nextId(1) {}

SnapshotManager::~SnapshotManager() {
    clear();
}

void SnapshotManager::computeDeltas(const string& oldText, const string& newText,
                                   shared_ptr<Snapshot> snapshot) {
    // Simple delta computation using LCS-like approach
    // For efficiency, we'll use a simplified approach
    
    if (oldText == newText) {
        return;  // No changes
    }
    
    // Find common prefix
    size_t commonPrefix = 0;
    size_t minLen = min(oldText.length(), newText.length());
    while (commonPrefix < minLen && oldText[commonPrefix] == newText[commonPrefix]) {
        commonPrefix++;
    }
    
    // Find common suffix
    size_t commonSuffix = 0;
    size_t oldIdx = oldText.length();
    size_t newIdx = newText.length();
    while (commonSuffix < minLen - commonPrefix && 
           oldIdx > commonPrefix && newIdx > commonPrefix &&
           oldText[oldIdx - 1] == newText[newIdx - 1]) {
        commonSuffix++;
        oldIdx--;
        newIdx--;
    }
    
    // Calculate the changed portion
    size_t oldChangeStart = commonPrefix;
    size_t oldChangeEnd = oldText.length() - commonSuffix;
    size_t newChangeStart = commonPrefix;
    size_t newChangeEnd = newText.length() - commonSuffix;
    
    if (oldChangeStart == oldChangeEnd && newChangeStart == newChangeEnd) {
        return;  // No changes
    }
    
    if (oldChangeStart == oldChangeEnd) {
        // Pure insertion
        string inserted = newText.substr(newChangeStart, newChangeEnd - newChangeStart);
        snapshot->addDelta(Snapshot::Delta::INSERT, oldChangeStart, inserted);
    } else if (newChangeStart == newChangeEnd) {
        // Pure deletion
        snapshot->addDelta(Snapshot::Delta::DELETE, oldChangeStart, "", 
                          oldChangeEnd - oldChangeStart);
    } else {
        // Replacement
        string replacement = newText.substr(newChangeStart, newChangeEnd - newChangeStart);
        snapshot->addDelta(Snapshot::Delta::REPLACE, oldChangeStart, replacement,
                          oldChangeEnd - oldChangeStart);
    }
}

shared_ptr<Snapshot> SnapshotManager::createSnapshot(const string& name, 
                                                     const string& content,
                                                     const string& description) {
    // Check if name already exists
    if (nameToNode.find(name) != nameToNode.end()) {
        cout << "Error: Snapshot with name '" << name << "' already exists." << endl;
        return nullptr;
    }
    
    shared_ptr<Snapshot> parentSnapshot = nullptr;
    if (current) {
        parentSnapshot = current->snapshot;
    }
    
    auto snapshot = make_shared<Snapshot>(nextId++, name, content, 
                                         description, parentSnapshot);
    
    // Compute deltas if this is not a base snapshot
    if (parentSnapshot) {
        string parentContent = parentSnapshot->reconstructContent();
        computeDeltas(parentContent, content, snapshot);
    }
    
    SnapshotNode* node = new SnapshotNode(snapshot);
    
    if (!head) {
        head = tail = current = node;
    } else {
        tail->next = node;
        node->prev = tail;
        
        if (current) {
            current->children.push_back(node);
        }
        
        tail = node;
        current = node;
    }
    
    idToNode[snapshot->getId()] = node;
    nameToNode[name] = node;
    
    return snapshot;
}

shared_ptr<Snapshot> SnapshotManager::createBranchSnapshot(const string& name,
                                                           const string& content,
                                                           const string& description) {
    if (!current) {
        return createSnapshot(name, content, description);
    }
    
    return createSnapshot(name, content, description);
}

shared_ptr<Snapshot> SnapshotManager::getSnapshot(int id) const {
    auto it = idToNode.find(id);
    if (it != idToNode.end()) {
        return it->second->snapshot;
    }
    return nullptr;
}

shared_ptr<Snapshot> SnapshotManager::getSnapshot(const string& name) const {
    auto it = nameToNode.find(name);
    if (it != nameToNode.end()) {
        return it->second->snapshot;
    }
    return nullptr;
}

shared_ptr<Snapshot> SnapshotManager::getCurrentSnapshot() const {
    return current ? current->snapshot : nullptr;
}

bool SnapshotManager::moveToSnapshot(int id) {
    auto it = idToNode.find(id);
    if (it != idToNode.end()) {
        current = it->second;
        return true;
    }
    return false;
}

bool SnapshotManager::moveToSnapshot(const string& name) {
    auto it = nameToNode.find(name);
    if (it != nameToNode.end()) {
        current = it->second;
        return true;
    }
    return false;
}

bool SnapshotManager::moveToNext() {
    if (current && current->next) {
        current = current->next;
        return true;
    }
    return false;
}

bool SnapshotManager::moveToPrevious() {
    if (current && current->prev) {
        current = current->prev;
        return true;
    }
    return false;
}

vector<shared_ptr<Snapshot>> SnapshotManager::getAllSnapshots() const {
    vector<shared_ptr<Snapshot>> snapshots;
    SnapshotNode* node = head;
    while (node) {
        snapshots.push_back(node->snapshot);
        node = node->next;
    }
    return snapshots;
}

void SnapshotManager::displaySnapshotTree() const {
    if (!head) {
        cout << "No snapshots available." << endl;
        return;
    }
    
    cout << "\n=== Snapshot Tree Structure ===" << endl;
    
    // Display as a tree
    SnapshotNode* node = head;
    int level = 0;
    
    while (node) {
        auto snapshot = node->snapshot;
        
        // Indentation based on parent chain
        string indent = "";
        auto parent = snapshot->getParent();
        while (parent) {
            indent += "  ";
            parent = parent->getParent();
        }
        
        cout << indent << "├─ [" << snapshot->getId() << "] " 
             << snapshot->getName();
        
        if (node == current) {
            cout << " ◄ CURRENT";
        }
        
        cout << endl;
        cout << indent << "│  " << snapshot->getTimestamp() 
             << " | " << snapshot->getDeltaCount() << " deltas" << endl;
        
        if (!snapshot->getDescription().empty()) {
            cout << indent << "│  " << snapshot->getDescription() << endl;
        }
        
        node = node->next;
    }
}

void SnapshotManager::displaySnapshotList() const {
    if (!head) {
        cout << "No snapshots available." << endl;
        return;
    }
    
    cout << "\n=== Snapshot List ===" << endl;
    cout << setw(5) << "ID" << " | " 
         << setw(20) << left << "Name" << " | "
         << setw(19) << "Timestamp" << " | "
         << setw(8) << "Deltas" << " | "
         << "Status" << endl;
    cout << string(80, '-') << endl;
    
    SnapshotNode* node = head;
    while (node) {
        auto snapshot = node->snapshot;
        cout << setw(5) << snapshot->getId() << " | "
             << setw(20) << left << snapshot->getName() << " | "
             << setw(19) << snapshot->getTimestamp() << " | "
             << setw(8) << snapshot->getDeltaCount() << " | ";
        
        if (node == current) {
            cout << "◄ CURRENT";
        }
        
        cout << endl;
        
        node = node->next;
    }
}

void SnapshotManager::compareSnapshots(int id1, int id2) const {
    auto snap1 = getSnapshot(id1);
    auto snap2 = getSnapshot(id2);
    
    if (!snap1 || !snap2) {
        cout << "One or both snapshots not found." << endl;
        return;
    }
    
    string content1 = snap1->reconstructContent();
    string content2 = snap2->reconstructContent();
    
    cout << "\n=== Snapshot Comparison ===" << endl;
    cout << "Snapshot 1: [" << id1 << "] " << snap1->getName() << endl;
    cout << "  Size: " << content1.length() << " characters" << endl;
    
    cout << "Snapshot 2: [" << id2 << "] " << snap2->getName() << endl;
    cout << "  Size: " << content2.length() << " characters" << endl;
    
    int diff = (int)content2.length() - (int)content1.length();
    cout << "\nDifference: " << (diff >= 0 ? "+" : "") << diff << " characters" << endl;
    
    // Calculate similarity
    size_t matches = 0;
    size_t minLen = min(content1.length(), content2.length());
    for (size_t i = 0; i < minLen; i++) {
        if (content1[i] == content2[i]) matches++;
    }
    
    double similarity = minLen > 0 ? (100.0 * matches / minLen) : 100.0;
    cout << "Similarity: " << fixed << setprecision(2) << similarity << "%" << endl;
}

void SnapshotManager::compareSnapshots(const string& name1, const string& name2) const {
    auto snap1 = getSnapshot(name1);
    auto snap2 = getSnapshot(name2);
    
    if (!snap1 || !snap2) {
        cout << "One or both snapshots not found." << endl;
        return;
    }
    
    compareSnapshots(snap1->getId(), snap2->getId());
}

bool SnapshotManager::deleteSnapshot(int id) {
    auto it = idToNode.find(id);
    if (it == idToNode.end()) {
        return false;
    }
    
    SnapshotNode* node = it->second;
    
    // Don't allow deletion of snapshots with children
    if (!node->children.empty()) {
        cout << "Cannot delete snapshot with children. Delete children first." << endl;
        return false;
    }
    
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
    
    // Update current if needed
    if (current == node) {
        current = node->prev ? node->prev : node->next;
    }
    
    // Remove from maps
    idToNode.erase(id);
    nameToNode.erase(node->snapshot->getName());
    
    delete node;
    return true;
}

bool SnapshotManager::deleteSnapshot(const string& name) {
    auto snap = getSnapshot(name);
    if (!snap) {
        return false;
    }
    return deleteSnapshot(snap->getId());
}

void SnapshotManager::clear() {
    SnapshotNode* node = head;
    while (node) {
        SnapshotNode* next = node->next;
        delete node;
        node = next;
    }
    
    head = tail = current = nullptr;
    idToNode.clear();
    nameToNode.clear();
    nextId = 1;
}