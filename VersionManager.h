#ifndef VERSION_MANAGER_H
#define VERSION_MANAGER_H

#include <string>
#include <ctime>
#include "Buffer.h"  // Your existing Buffer.h

using namespace std;

/**
 * VersionNode represents a single snapshot in the version history.
 * Uses a doubly linked list structure for bidirectional traversal.
 */
struct VersionNode {
    int versionId;
    string snapshotData;
    time_t timestamp;
    string description;
    
    VersionNode* prev;
    VersionNode* next;
    
    VersionNode(int id, const string& data, const string& desc = "");
};

/**
 * VersionManager handles snapshot creation and restoration.
 * Works independently of TextEditor's undo/redo system.
 * 
 * Key Features:
 * - Doubly linked list for efficient navigation
 * - Complete buffer snapshots
 * - Timeline navigation (previous/next)
 * - Version listing and deletion
 */
class VersionManager {
private:
    VersionNode* head;
    VersionNode* tail;
    VersionNode* currentVersion;
    int nextVersionId;
    int versionCount;
    
    // Helper to find version by ID
    VersionNode* findVersion(int versionId) const;
    
public:
    VersionManager();
    ~VersionManager();
    
    // Core operations
    
    /**
     * Create a snapshot of current editor state
     * @param editor Reference to TextEditor
     * @param description Optional version description
     * @return Version ID of created snapshot
     */
    int createSnapshot(const TextEditor& editor, const string& description = "");
    
    /**
     * Restore editor to a specific version
     * @param editor Reference to TextEditor to restore
     * @param versionId Version to restore
     * @return true if successful
     */
    bool restoreVersion(TextEditor& editor, int versionId);
    
    /**
     * Navigate to previous version
     */
    bool restorePreviousVersion(TextEditor& editor);
    
    /**
     * Navigate to next version
     */
    bool restoreNextVersion(TextEditor& editor);
    
    // Query operations
    
    /**
     * Display all versions with metadata
     */
    void listVersions() const;
    
    /**
     * Get current version ID
     */
    int getCurrentVersionId() const;
    
    /**
     * Get total version count
     */
    int getVersionCount() const;
    
    /**
     * Check if version exists
     */
    bool versionExists(int versionId) const;
    
    /**
     * Delete a specific version
     */
    bool deleteVersion(int versionId);
    
    /**
     * Clear all versions
     */
    void clearAllVersions();
    
    /**
     * Export version to file
     */
    bool exportVersion(int versionId, const string& filepath) const;
    
private:
    // Prevent copying
    VersionManager(const VersionManager&);
    VersionManager& operator=(const VersionManager&);
};

#endif // VERSION_MANAGER_H