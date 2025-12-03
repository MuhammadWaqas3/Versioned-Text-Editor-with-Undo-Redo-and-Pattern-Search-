#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <string>
#include <vector>
#include <stack>
#include <memory>
#include <fstream>

// Forward declarations
class Version;
class SearchEngine;

// Command pattern for undo/redo
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getDescription() const = 0;
};

// Text buffer using chunked storage
class TextBuffer {
private:
    static const size_t CHUNK_SIZE = 256;
    std::vector<std::string> chunks;
    size_t totalLength;
    
    void rebalance();
    size_t getChunkIndex(size_t pos, size_t& localPos) const;
    
public:
    TextBuffer();
    explicit TextBuffer(const std::string& text);
    
    void insert(size_t pos, const std::string& text);
    void deleteText(size_t pos, size_t length);
    void replace(size_t pos, size_t length, const std::string& text);
    
    std::string getText() const;
    std::string getSubstring(size_t pos, size_t length) const;
    size_t length() const { return totalLength; }
    
    void clear();
    void setText(const std::string& text);
};

// Version/Snapshot management
class Version {
private:
    std::string content;
    std::string timestamp;
    std::string description;
    int versionNumber;
    
public:
    Version(const std::string& content, int versionNum, const std::string& desc = "");
    
    std::string getContent() const { return content; }
    std::string getTimestamp() const { return timestamp; }
    std::string getDescription() const { return description; }
    int getVersionNumber() const { return versionNumber; }
};

// Doubly linked list for version history
class VersionHistory {
private:
    struct VersionNode {
        std::shared_ptr<Version> version;
        VersionNode* next;
        VersionNode* prev;
        
        VersionNode(std::shared_ptr<Version> v) : version(v), next(nullptr), prev(nullptr) {}
    };
    
    VersionNode* head;
    VersionNode* tail;
    VersionNode* current;
    int versionCount;
    
public:
    VersionHistory();
    ~VersionHistory();
    
    void addVersion(std::shared_ptr<Version> version);
    std::shared_ptr<Version> getCurrentVersion() const;
    std::shared_ptr<Version> getPreviousVersion();
    std::shared_ptr<Version> getNextVersion();
    
    std::vector<std::shared_ptr<Version>> getAllVersions() const;
    void clear();
    int getCount() const { return versionCount; }
};

// Rolling hash implementation for pattern search (Rabin-Karp)
class SearchEngine {
private:
    static const long long PRIME = 1000000007;
    static const int BASE = 256;
    
    long long calculateHash(const std::string& str, int length) const;
    long long recalculateHash(long long oldHash, char oldChar, char newChar, 
                             long long pow, int length) const;
    long long power(long long base, int exp) const;
    
public:
    std::vector<size_t> search(const std::string& text, const std::string& pattern) const;
    std::vector<size_t> searchAndReplace(std::string& text, const std::string& pattern, 
                                         const std::string& replacement) const;
};

// Concrete Commands
class InsertCommand : public Command {
private:
    TextBuffer* buffer;
    size_t position;
    std::string text;
    
public:
    InsertCommand(TextBuffer* buf, size_t pos, const std::string& txt);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

class DeleteCommand : public Command {
private:
    TextBuffer* buffer;
    size_t position;
    size_t length;
    std::string deletedText;
    
public:
    DeleteCommand(TextBuffer* buf, size_t pos, size_t len);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

class ReplaceCommand : public Command {
private:
    TextBuffer* buffer;
    size_t position;
    size_t length;
    std::string newText;
    std::string oldText;
    
public:
    ReplaceCommand(TextBuffer* buf, size_t pos, size_t len, const std::string& txt);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

// Main Editor class
class TextEditor {
private:
    TextBuffer buffer;
    std::stack<std::shared_ptr<Command>> undoStack;
    std::stack<std::shared_ptr<Command>> redoStack;
    VersionHistory versionHistory;
    SearchEngine searchEngine;
    std::string filename;
    bool modified;
    
    void clearRedoStack();
    
public:
    TextEditor();
    
    // Basic operations
    void insert(size_t pos, const std::string& text);
    void deleteText(size_t pos, size_t length);
    void replace(size_t pos, size_t length, const std::string& text);
    
    // Undo/Redo
    bool undo();
    bool redo();
    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }
    
    // Version management
    void createSnapshot(const std::string& description = "");
    void restoreVersion(int versionNumber);
    std::vector<std::shared_ptr<Version>> getVersions() const;
    
    // Search operations
    std::vector<size_t> search(const std::string& pattern) const;
    int searchAndReplace(const std::string& pattern, const std::string& replacement);
    
    // File operations
    bool loadFromFile(const std::string& filepath);
    bool saveToFile(const std::string& filepath = "");
    
    // Getters
    std::string getText() const { return buffer.getText(); }
    size_t getLength() const { return buffer.length(); }
    bool isModified() const { return modified; }
    std::string getFilename() const { return filename; }
    
    void clear();
};

#endif // TEXT_EDITOR_H