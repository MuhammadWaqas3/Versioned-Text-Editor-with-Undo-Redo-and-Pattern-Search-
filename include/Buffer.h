#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

// Custom Stack Implementation
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    
    Node* topNode;
    int size;
    
public:
    Stack() : topNode(nullptr), size(0) {}
    
    ~Stack() {
        while (!empty()) {
            pop();
        }
    }
    
    void push(const T& data) {
        Node* newNode = new Node(data);
        newNode->next = topNode;
        topNode = newNode;
        size++;
    }
    
    void pop() {
        if (empty()) return;
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        size--;
    }
    
    T& top() {
        return topNode->data;
    }
    
    const T& top() const {
        return topNode->data;
    }
    
    bool empty() const {
        return topNode == nullptr;
    }
    
    int getSize() const {
        return size;
    }
};

// Command pattern for undo/redo
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual string getDescription() const = 0;
};

// Text buffer using chunked storage
class TextBuffer {
private:
    static const size_t CHUNK_SIZE = 256;
    vector<string> chunks;
    size_t totalLength;
    
    void rebalance();
    size_t getChunkIndex(size_t pos, size_t& localPos) const;
    
public:
    TextBuffer();
    explicit TextBuffer(const string& text);
    
    void insert(size_t pos, const string& text);
    void deleteText(size_t pos, size_t length);
    void replace(size_t pos, size_t length, const string& text);
    
    string getText() const;
    string getSubstring(size_t pos, size_t length) const;
    size_t length() const { return totalLength; }
    
    void clear();
    void setText(const string& text);
};

// Rolling hash implementation for pattern search (Rabin-Karp)
class SearchEngine {
private:
    static const long long PRIME = 1000000007;
    static const int BASE = 256;
    
    long long calculateHash(const string& str, int length) const;
    long long recalculateHash(long long oldHash, char oldChar, char newChar, 
                             long long pow, int length) const;
    long long power(long long base, int exp) const;
    
public:
    vector<size_t> search(const string& text, const string& pattern) const;
    vector<size_t> searchAndReplace(string& text, const string& pattern, 
                                    const string& replacement) const;
};

// Concrete Commands
class InsertCommand : public Command {
private:
    TextBuffer* buffer;
    size_t position;
    string text;
    
public:
    InsertCommand(TextBuffer* buf, size_t pos, const string& txt);
    void execute() override;
    void undo() override;
    string getDescription() const override;
};

class DeleteCommand : public Command {
private:
    TextBuffer* buffer;
    size_t position;
    size_t length;
    string deletedText;
    
public:
    DeleteCommand(TextBuffer* buf, size_t pos, size_t len);
    void execute() override;
    void undo() override;
    string getDescription() const override;
};

class ReplaceCommand : public Command {
private:
    TextBuffer* buffer;
    size_t position;
    size_t length;
    string newText;
    string oldText;
    
public:
    ReplaceCommand(TextBuffer* buf, size_t pos, size_t len, const string& txt);
    void execute() override;
    void undo() override;
    string getDescription() const override;
};

// Helper function to add data/ prefix to filenames
inline string addDataFolder(const string& filename) {
    // If filename already starts with "data/" or is absolute path, return as is
    if (filename.substr(0, 5) == "data/" || filename.substr(0, 5) == "data\\") {
        return filename;
    }
    // Check if it's an absolute path (Windows: C:\ or Linux: /)
    if ((filename.length() > 2 && filename[1] == ':') || filename[0] == '/') {
        return filename;
    }
    // Add data/ prefix
    return "data/" + filename;
}

// Main Editor class
class TextEditor {
private:
    TextBuffer buffer;
    Stack<shared_ptr<Command>> undoStack;
    Stack<shared_ptr<Command>> redoStack;
    SearchEngine searchEngine;
    string filename;
    bool modified;
    
    void clearRedoStack();
    
public:
    TextEditor();
    
    // Basic operations
    void insert(size_t pos, const string& text);
    void deleteText(size_t pos, size_t length);
    void replace(size_t pos, size_t length, const string& text);
    
    // Undo/Redo
    bool undo();
    bool redo();
    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }
    
    // Search operations
    vector<size_t> search(const string& pattern) const;
    int searchAndReplace(const string& pattern, const string& replacement);
    
    // File operations - now automatically use data/ folder
    bool loadFromFile(const string& filepath);
    bool saveToFile(const string& filepath = "");
    
    // Getters
    string getText() const { return buffer.getText(); }
    size_t getLength() const { return buffer.length(); }
    bool isModified() const { return modified; }
    string getFilename() const { return filename; }
    
    void clear();
};

#endif // BUFFER_H