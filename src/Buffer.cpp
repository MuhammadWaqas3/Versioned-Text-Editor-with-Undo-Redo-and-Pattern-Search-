#include "Buffer.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include <direct.h>  // For Windows _mkdir

using namespace std;

// Helper function to create data directory if it doesn't exist
void ensureDataFolderExists() {
    struct stat info;
    if (stat("data", &info) != 0) {
        // Directory doesn't exist, create it
        #ifdef _WIN32
            _mkdir("data");
        #else
            mkdir("data", 0755);
        #endif
    }
}

// ==================== TextBuffer Implementation ====================

TextBuffer::TextBuffer() : totalLength(0) {
    chunks.push_back("");
}

TextBuffer::TextBuffer(const string& text) : totalLength(0) {
    setText(text);
}

void TextBuffer::setText(const string& text) {
    chunks.clear();
    totalLength = 0;
    
    if (text.empty()) {
        chunks.push_back("");
        return;
    }
    
    for (size_t i = 0; i < text.length(); i += CHUNK_SIZE) {
        chunks.push_back(text.substr(i, CHUNK_SIZE));
    }
    totalLength = text.length();
}

size_t TextBuffer::getChunkIndex(size_t pos, size_t& localPos) const {
    if (pos >= totalLength) {
        localPos = chunks.back().length();
        return chunks.size() - 1;
    }
    
    size_t accumulated = 0;
    for (size_t i = 0; i < chunks.size(); i++) {
        if (accumulated + chunks[i].length() > pos) {
            localPos = pos - accumulated;
            return i;
        }
        accumulated += chunks[i].length();
    }
    
    localPos = 0;
    return 0;
}

void TextBuffer::insert(size_t pos, const string& text) {
    if (text.empty()) return;
    
    size_t localPos;
    size_t chunkIdx = getChunkIndex(pos, localPos);
    
    chunks[chunkIdx].insert(localPos, text);
    totalLength += text.length();
    
    rebalance();
}

void TextBuffer::deleteText(size_t pos, size_t length) {
    if (length == 0 || pos >= totalLength) return;
    
    length = min(length, totalLength - pos);
    
    size_t remaining = length;
    size_t localPos;
    size_t chunkIdx = getChunkIndex(pos, localPos);
    
    while (remaining > 0 && chunkIdx < chunks.size()) {
        size_t availableInChunk = chunks[chunkIdx].length() - localPos;
        size_t toDelete = min(remaining, availableInChunk);
        
        chunks[chunkIdx].erase(localPos, toDelete);
        remaining -= toDelete;
        
        if (remaining > 0) {
            chunkIdx++;
            localPos = 0;
        }
    }
    
    totalLength -= length;
    rebalance();
}

void TextBuffer::replace(size_t pos, size_t length, const string& text) {
    deleteText(pos, length);
    insert(pos, text);
}

void TextBuffer::rebalance() {
    vector<string> newChunks;
    string current;
    
    for (const auto& chunk : chunks) {
        current += chunk;
        while (current.length() >= CHUNK_SIZE) {
            newChunks.push_back(current.substr(0, CHUNK_SIZE));
            current = current.substr(CHUNK_SIZE);
        }
    }
    
    if (!current.empty() || newChunks.empty()) {
        newChunks.push_back(current);
    }
    
    chunks = move(newChunks);
}

string TextBuffer::getText() const {
    string result;
    result.reserve(totalLength);
    for (const auto& chunk : chunks) {
        result += chunk;
    }
    return result;
}

string TextBuffer::getSubstring(size_t pos, size_t length) const {
    if (pos >= totalLength) return "";
    length = min(length, totalLength - pos);
    
    return getText().substr(pos, length);
}

void TextBuffer::clear() {
    chunks.clear();
    chunks.push_back("");
    totalLength = 0;
}

// ==================== SearchEngine Implementation ====================

long long SearchEngine::power(long long base, int exp) const {
    long long result = 1;
    for (int i = 0; i < exp; i++) {
        result = (result * base) % PRIME;
    }
    return result;
}

long long SearchEngine::calculateHash(const string& str, int length) const {
    long long hash = 0;
    for (int i = 0; i < length && i < str.length(); i++) {
        hash = (hash * BASE + static_cast<unsigned char>(str[i])) % PRIME;
    }
    return hash;
}

long long SearchEngine::recalculateHash(long long oldHash, char oldChar, char newChar, 
                                       long long pow, int length) const {
    long long newHash = (oldHash - (static_cast<unsigned char>(oldChar) * pow) % PRIME + PRIME) % PRIME;
    newHash = (newHash * BASE + static_cast<unsigned char>(newChar)) % PRIME;
    return newHash;
}

vector<size_t> SearchEngine::search(const string& text, const string& pattern) const {
    vector<size_t> positions;
    
    if (pattern.empty() || pattern.length() > text.length()) {
        return positions;
    }
    
    int patternLen = pattern.length();
    long long patternHash = calculateHash(pattern, patternLen);
    long long textHash = calculateHash(text, patternLen);
    long long pow = power(BASE, patternLen - 1);
    
    for (size_t i = 0; i <= text.length() - patternLen; i++) {
        if (textHash == patternHash) {
            // Verify match
            if (text.substr(i, patternLen) == pattern) {
                positions.push_back(i);
            }
        }
        
        if (i < text.length() - patternLen) {
            textHash = recalculateHash(textHash, text[i], text[i + patternLen], pow, patternLen);
        }
    }
    
    return positions;
}

vector<size_t> SearchEngine::searchAndReplace(string& text, const string& pattern, 
                                              const string& replacement) const {
    vector<size_t> positions = search(text, pattern);
    
    // Replace from end to start to maintain position validity
    for (auto it = positions.rbegin(); it != positions.rend(); ++it) {
        text.replace(*it, pattern.length(), replacement);
    }
    
    return positions;
}

// ==================== Command Implementations ====================

InsertCommand::InsertCommand(TextBuffer* buf, size_t pos, const string& txt)
    : buffer(buf), position(pos), text(txt) {}

void InsertCommand::execute() {
    buffer->insert(position, text);
}

void InsertCommand::undo() {
    buffer->deleteText(position, text.length());
}

string InsertCommand::getDescription() const {
    return "Insert " + to_string(text.length()) + " chars at " + to_string(position);
}

DeleteCommand::DeleteCommand(TextBuffer* buf, size_t pos, size_t len)
    : buffer(buf), position(pos), length(len) {}

void DeleteCommand::execute() {
    deletedText = buffer->getSubstring(position, length);
    buffer->deleteText(position, length);
}

void DeleteCommand::undo() {
    buffer->insert(position, deletedText);
}

string DeleteCommand::getDescription() const {
    return "Delete " + to_string(length) + " chars at " + to_string(position);
}

ReplaceCommand::ReplaceCommand(TextBuffer* buf, size_t pos, size_t len, const string& txt)
    : buffer(buf), position(pos), length(len), newText(txt) {}

void ReplaceCommand::execute() {
    oldText = buffer->getSubstring(position, length);
    buffer->replace(position, length, newText);
}

void ReplaceCommand::undo() {
    buffer->replace(position, newText.length(), oldText);
}

string ReplaceCommand::getDescription() const {
    return "Replace " + to_string(length) + " chars at " + to_string(position);
}

// ==================== TextEditor Implementation ====================

TextEditor::TextEditor() : modified(false) {}

void TextEditor::clearRedoStack() {
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void TextEditor::insert(size_t pos, const string& text) {
    auto cmd = make_shared<InsertCommand>(&buffer, pos, text);
    cmd->execute();
    undoStack.push(cmd);
    clearRedoStack();
    modified = true;
}

void TextEditor::deleteText(size_t pos, size_t length) {
    auto cmd = make_shared<DeleteCommand>(&buffer, pos, length);
    cmd->execute();
    undoStack.push(cmd);
    clearRedoStack();
    modified = true;
}

void TextEditor::replace(size_t pos, size_t length, const string& text) {
    auto cmd = make_shared<ReplaceCommand>(&buffer, pos, length, text);
    cmd->execute();
    undoStack.push(cmd);
    clearRedoStack();
    modified = true;
}

bool TextEditor::undo() {
    if (undoStack.empty()) return false;
    
    auto cmd = undoStack.top();
    undoStack.pop();
    cmd->undo();
    redoStack.push(cmd);
    modified = true;
    return true;
}

bool TextEditor::redo() {
    if (redoStack.empty()) return false;
    
    auto cmd = redoStack.top();
    redoStack.pop();
    cmd->execute();
    undoStack.push(cmd);
    modified = true;
    return true;
}

vector<size_t> TextEditor::search(const string& pattern) const {
    return searchEngine.search(buffer.getText(), pattern);
}

int TextEditor::searchAndReplace(const string& pattern, const string& replacement) {
    string text = buffer.getText();
    auto positions = searchEngine.searchAndReplace(text, pattern, replacement);
    
    if (!positions.empty()) {
        buffer.setText(text);
        modified = true;
    }
    
    return positions.size();
}

bool TextEditor::loadFromFile(const string& filepath) {
    // Automatically add data/ prefix
    string fullPath = addDataFolder(filepath);
    
    ifstream file(fullPath);
    if (!file.is_open()) {
        return false;
    }
    
    stringstream ss;
    ss << file.rdbuf();
    buffer.setText(ss.str());
    filename = fullPath;  // Store full path with data/
    modified = false;
    
    return true;
}

bool TextEditor::saveToFile(const string& filepath) {
    // Ensure data folder exists
    ensureDataFolderExists();
    
    string path = filepath.empty() ? filename : addDataFolder(filepath);
    if (path.empty()) return false;
    
    ofstream file(path);
    if (!file.is_open()) return false;
    
    file << buffer.getText();
    filename = path;
    modified = false;
    return true;
}

void TextEditor::clear() {
    buffer.clear();
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
    modified = false;
}