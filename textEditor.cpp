#include "textEditor.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>

// ==================== TextBuffer Implementation ====================

TextBuffer::TextBuffer() : totalLength(0) {
    chunks.push_back("");
}

TextBuffer::TextBuffer(const std::string& text) : totalLength(0) {
    setText(text);
}

void TextBuffer::setText(const std::string& text) {
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

void TextBuffer::insert(size_t pos, const std::string& text) {
    if (text.empty()) return;
    
    size_t localPos;
    size_t chunkIdx = getChunkIndex(pos, localPos);
    
    chunks[chunkIdx].insert(localPos, text);
    totalLength += text.length();
    
    rebalance();
}

void TextBuffer::deleteText(size_t pos, size_t length) {
    if (length == 0 || pos >= totalLength) return;
    
    length = std::min(length, totalLength - pos);
    
    size_t remaining = length;
    size_t localPos;
    size_t chunkIdx = getChunkIndex(pos, localPos);
    
    while (remaining > 0 && chunkIdx < chunks.size()) {
        size_t availableInChunk = chunks[chunkIdx].length() - localPos;
        size_t toDelete = std::min(remaining, availableInChunk);
        
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

void TextBuffer::replace(size_t pos, size_t length, const std::string& text) {
    deleteText(pos, length);
    insert(pos, text);
}

void TextBuffer::rebalance() {
    std::vector<std::string> newChunks;
    std::string current;
    
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
    
    chunks = std::move(newChunks);
}

std::string TextBuffer::getText() const {
    std::string result;
    result.reserve(totalLength);
    for (const auto& chunk : chunks) {
        result += chunk;
    }
    return result;
}

std::string TextBuffer::getSubstring(size_t pos, size_t length) const {
    if (pos >= totalLength) return "";
    length = std::min(length, totalLength - pos);
    
    return getText().substr(pos, length);
}

void TextBuffer::clear() {
    chunks.clear();
    chunks.push_back("");
    totalLength = 0;
}

// ==================== Version Implementation ====================

Version::Version(const std::string& content, int versionNum, const std::string& desc)
    : content(content), versionNumber(versionNum), description(desc) {
    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    timestamp = buffer;
}

// ==================== VersionHistory Implementation ====================

VersionHistory::VersionHistory() : head(nullptr), tail(nullptr), current(nullptr), versionCount(0) {}

VersionHistory::~VersionHistory() {
    clear();
}

void VersionHistory::addVersion(std::shared_ptr<Version> version) {
    VersionNode* node = new VersionNode(version);
    
    if (!head) {
        head = tail = current = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
        current = node;
    }
    
    versionCount++;
}

std::shared_ptr<Version> VersionHistory::getCurrentVersion() const {
    return current ? current->version : nullptr;
}

std::shared_ptr<Version> VersionHistory::getPreviousVersion() {
    if (current && current->prev) {
        current = current->prev;
        return current->version;
    }
    return nullptr;
}

std::shared_ptr<Version> VersionHistory::getNextVersion() {
    if (current && current->next) {
        current = current->next;
        return current->version;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Version>> VersionHistory::getAllVersions() const {
    std::vector<std::shared_ptr<Version>> versions;
    VersionNode* node = head;
    while (node) {
        versions.push_back(node->version);
        node = node->next;
    }
    return versions;
}

void VersionHistory::clear() {
    VersionNode* node = head;
    while (node) {
        VersionNode* next = node->next;
        delete node;
        node = next;
    }
    head = tail = current = nullptr;
    versionCount = 0;
}

// ==================== SearchEngine Implementation ====================

long long SearchEngine::power(long long base, int exp) const {
    long long result = 1;
    for (int i = 0; i < exp; i++) {
        result = (result * base) % PRIME;
    }
    return result;
}

long long SearchEngine::calculateHash(const std::string& str, int length) const {
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

std::vector<size_t> SearchEngine::search(const std::string& text, const std::string& pattern) const {
    std::vector<size_t> positions;
    
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

std::vector<size_t> SearchEngine::searchAndReplace(std::string& text, const std::string& pattern, 
                                                   const std::string& replacement) const {
    std::vector<size_t> positions = search(text, pattern);
    
    // Replace from end to start to maintain position validity
    for (auto it = positions.rbegin(); it != positions.rend(); ++it) {
        text.replace(*it, pattern.length(), replacement);
    }
    
    return positions;
}

// ==================== Command Implementations ====================

InsertCommand::InsertCommand(TextBuffer* buf, size_t pos, const std::string& txt)
    : buffer(buf), position(pos), text(txt) {}

void InsertCommand::execute() {
    buffer->insert(position, text);
}

void InsertCommand::undo() {
    buffer->deleteText(position, text.length());
}

std::string InsertCommand::getDescription() const {
    return "Insert " + std::to_string(text.length()) + " chars at " + std::to_string(position);
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

std::string DeleteCommand::getDescription() const {
    return "Delete " + std::to_string(length) + " chars at " + std::to_string(position);
}

ReplaceCommand::ReplaceCommand(TextBuffer* buf, size_t pos, size_t len, const std::string& txt)
    : buffer(buf), position(pos), length(len), newText(txt) {}

void ReplaceCommand::execute() {
    oldText = buffer->getSubstring(position, length);
    buffer->replace(position, length, newText);
}

void ReplaceCommand::undo() {
    buffer->replace(position, newText.length(), oldText);
}

std::string ReplaceCommand::getDescription() const {
    return "Replace " + std::to_string(length) + " chars at " + std::to_string(position);
}

// ==================== TextEditor Implementation ====================

TextEditor::TextEditor() : modified(false) {
    createSnapshot("Initial version");
}

void TextEditor::clearRedoStack() {
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void TextEditor::insert(size_t pos, const std::string& text) {
    auto cmd = std::make_shared<InsertCommand>(&buffer, pos, text);
    cmd->execute();
    undoStack.push(cmd);
    clearRedoStack();
    modified = true;
}

void TextEditor::deleteText(size_t pos, size_t length) {
    auto cmd = std::make_shared<DeleteCommand>(&buffer, pos, length);
    cmd->execute();
    undoStack.push(cmd);
    clearRedoStack();
    modified = true;
}

void TextEditor::replace(size_t pos, size_t length, const std::string& text) {
    auto cmd = std::make_shared<ReplaceCommand>(&buffer, pos, length, text);
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

void TextEditor::createSnapshot(const std::string& description) {
    int versionNum = versionHistory.getCount() + 1;
    auto version = std::make_shared<Version>(buffer.getText(), versionNum, description);
    versionHistory.addVersion(version);
}

void TextEditor::restoreVersion(int versionNumber) {
    auto versions = versionHistory.getAllVersions();
    for (const auto& version : versions) {
        if (version->getVersionNumber() == versionNumber) {
            buffer.setText(version->getContent());
            modified = true;
            return;
        }
    }
}

std::vector<std::shared_ptr<Version>> TextEditor::getVersions() const {
    return versionHistory.getAllVersions();
}

std::vector<size_t> TextEditor::search(const std::string& pattern) const {
    return searchEngine.search(buffer.getText(), pattern);
}

int TextEditor::searchAndReplace(const std::string& pattern, const std::string& replacement) {
    std::string text = buffer.getText();
    auto positions = searchEngine.searchAndReplace(text, pattern, replacement);
    
    if (!positions.empty()) {
        buffer.setText(text);
        modified = true;
    }
    
    return positions.size();
}

bool TextEditor::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;
    
    std::stringstream ss;
    ss << file.rdbuf();
    buffer.setText(ss.str());
    filename = filepath;
    modified = false;
    
    createSnapshot("Loaded from " + filepath);
    return true;
}

bool TextEditor::saveToFile(const std::string& filepath) {
    std::string path = filepath.empty() ? filename : filepath;
    if (path.empty()) return false;
    
    std::ofstream file(path);
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