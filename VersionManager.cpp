#include "VersionManager.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// ============================================================================
// VersionNode Implementation
// ============================================================================

VersionNode::VersionNode(int id, const string& data, const string& desc)
    : versionId(id),
      snapshotData(data),
      timestamp(time(nullptr)),
      description(desc),
      prev(nullptr),
      next(nullptr) {
}

// ============================================================================
// VersionManager Implementation
// ============================================================================

VersionManager::VersionManager()
    : head(nullptr),
      tail(nullptr),
      currentVersion(nullptr),
      nextVersionId(1),
      versionCount(0) {
}

VersionManager::~VersionManager() {
    clearAllVersions();
}

int VersionManager::createSnapshot(const TextEditor& editor, const string& description) {
    // Get complete buffer content from editor
    string bufferContent = editor.getText();
    
    // Create new version node
    VersionNode* newVersion = new VersionNode(nextVersionId, bufferContent, description);
    
    // Insert at end of linked list (chronological order)
    if (tail == nullptr) {
        // First version
        head = tail = newVersion;
    } else {
        // Append to tail
        tail->next = newVersion;
        newVersion->prev = tail;
        tail = newVersion;
    }
    
    // Update tracking variables
    currentVersion = newVersion;
    versionCount++;
    int createdId = nextVersionId;
    nextVersionId++;
    
    cout << "\n╔════════════════════════════════════════════════════╗" << endl;
    cout << "║           SNAPSHOT CREATED                         ║" << endl;
    cout << "╚════════════════════════════════════════════════════╝" << endl;
    cout << "  Version ID: " << createdId << endl;
    if (!description.empty()) {
        cout << "  Description: " << description << endl;
    }
    cout << "  Size: " << bufferContent.length() << " characters" << endl;
    
    char timeStr[26];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&newVersion->timestamp));
    cout << "  Timestamp: " << timeStr << endl;
    
    return createdId;
}

bool VersionManager::restoreVersion(TextEditor& editor, int versionId) {
    VersionNode* version = findVersion(versionId);
    
    if (version == nullptr) {
        cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
        return false;
    }
    
    // Clear editor and load snapshot
    editor.clear();
    
    // Insert the saved content
    if (!version->snapshotData.empty()) {
        editor.insert(0, version->snapshotData);
    }
    
    currentVersion = version;
    
    cout << "\n╔════════════════════════════════════════════════════╗" << endl;
    cout << "║           VERSION RESTORED                         ║" << endl;
    cout << "╚════════════════════════════════════════════════════╝" << endl;
    cout << "  Version ID: " << versionId << endl;
    if (!version->description.empty()) {
        cout << "  Description: " << version->description << endl;
    }
    
    char timeStr[26];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&version->timestamp));
    cout << "  Original Timestamp: " << timeStr << endl;
    
    return true;
}

bool VersionManager::restorePreviousVersion(TextEditor& editor) {
    if (currentVersion == nullptr || currentVersion->prev == nullptr) {
        cout << "\n[INFO] No previous version available." << endl;
        return false;
    }
    
    return restoreVersion(editor, currentVersion->prev->versionId);
}

bool VersionManager::restoreNextVersion(TextEditor& editor) {
    if (currentVersion == nullptr || currentVersion->next == nullptr) {
        cout << "\n[INFO] No next version available." << endl;
        return false;
    }
    
    return restoreVersion(editor, currentVersion->next->versionId);
}

void VersionManager::listVersions() const {
    if (head == nullptr) {
        cout << "\n╔════════════════════════════════════════════════════╗" << endl;
        cout << "║      No snapshots saved yet                        ║" << endl;
        cout << "╚════════════════════════════════════════════════════╝" << endl;
        return;
    }
    
    cout << "\n╔═══════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                          VERSION HISTORY                                  ║" << endl;
    cout << "╠═══════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║ Total Snapshots: " << setw(57) << left << versionCount << "║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════════════════════╝" << endl;
    
    VersionNode* current = head;
    int index = 1;
    
    while (current != nullptr) {
        cout << "\n┌───────────────────────────────────────────────────────────────────────────┐" << endl;
        
        // Version header
        cout << "│ Version " << setw(2) << current->versionId;
        
        if (current == currentVersion) {
            cout << " ★ CURRENT ★";
            cout << setw(53) << " " << "│" << endl;
        } else {
            cout << setw(67) << " " << "│" << endl;
        }
        
        cout << "├───────────────────────────────────────────────────────────────────────────┤" << endl;
        
        // Description
        if (!current->description.empty()) {
            string desc = current->description;
            if (desc.length() > 70) {
                desc = desc.substr(0, 67) + "...";
            }
            cout << "│ Description: " << setw(62) << left << desc << "│" << endl;
        } else {
            cout << "│ Description: (no description)                                             │" << endl;
        }
        
        // Timestamp
        char timeStr[26];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&current->timestamp));
        cout << "│ Created: " << setw(66) << left << timeStr << "│" << endl;
        
        // Size
        cout << "│ Size: " << current->snapshotData.length() << " characters";
        int padding = 66 - to_string(current->snapshotData.length()).length() - 12;
        for (int i = 0; i < padding; i++) cout << " ";
        cout << "│" << endl;
        
        // Preview (first 50 chars)
        string preview = current->snapshotData;
        if (preview.length() > 50) {
            preview = preview.substr(0, 47) + "...";
        }
        // Replace newlines with spaces for preview
        for (char& c : preview) {
            if (c == '\n' || c == '\r') c = ' ';
        }
        cout << "│ Preview: " << setw(66) << left << preview << "│" << endl;
        
        cout << "└───────────────────────────────────────────────────────────────────────────┘" << endl;
        
        current = current->next;
        index++;
    }
    
    cout << endl;
}

int VersionManager::getCurrentVersionId() const {
    return (currentVersion != nullptr) ? currentVersion->versionId : -1;
}

int VersionManager::getVersionCount() const {
    return versionCount;
}

bool VersionManager::versionExists(int versionId) const {
    return findVersion(versionId) != nullptr;
}

bool VersionManager::deleteVersion(int versionId) {
    VersionNode* version = findVersion(versionId);
    
    if (version == nullptr) {
        cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
        return false;
    }
    
    // Cannot delete if it's the only version
    if (versionCount == 1) {
        cout << "\n[ERROR] Cannot delete the only remaining version!" << endl;
        return false;
    }
    
    // Update links
    if (version->prev != nullptr) {
        version->prev->next = version->next;
    } else {
        // Deleting head
        head = version->next;
    }
    
    if (version->next != nullptr) {
        version->next->prev = version->prev;
    } else {
        // Deleting tail
        tail = version->prev;
    }
    
    // If deleting current version, move to previous or next
    if (currentVersion == version) {
        currentVersion = (version->prev != nullptr) ? version->prev : version->next;
    }
    
    delete version;
    versionCount--;
    
    cout << "\n[SUCCESS] Version " << versionId << " deleted." << endl;
    return true;
}

void VersionManager::clearAllVersions() {
    VersionNode* current = head;
    while (current != nullptr) {
        VersionNode* next = current->next;
        delete current;
        current = next;
    }
    
    head = tail = currentVersion = nullptr;
    versionCount = 0;
    nextVersionId = 1;
}

bool VersionManager::exportVersion(int versionId, const string& filepath) const {
    VersionNode* version = findVersion(versionId);
    
    if (version == nullptr) {
        cout << "\n[ERROR] Version " << versionId << " not found!" << endl;
        return false;
    }
    
    ofstream file(filepath);
    if (!file.is_open()) {
        cout << "\n[ERROR] Could not open file: " << filepath << endl;
        return false;
    }
    
    file << version->snapshotData;
    file.close();
    
    cout << "\n[SUCCESS] Version " << versionId << " exported to: " << filepath << endl;
    return true;
}

VersionNode* VersionManager::findVersion(int versionId) const {
    VersionNode* current = head;
    while (current != nullptr) {
        if (current->versionId == versionId) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Prevent copying
VersionManager::VersionManager(const VersionManager&) {}
VersionManager& VersionManager::operator=(const VersionManager&) { return *this; }