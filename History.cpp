#include "History.h"
#include <ctime>
#include <iostream>

using namespace std;

// ==================== Version Implementation ====================

Version::Version(const string& content, int versionNum, const string& desc)
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

void VersionHistory::addVersion(shared_ptr<Version> version) {
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

shared_ptr<Version> VersionHistory::getCurrentVersion() const {
    return current ? current->version : nullptr;
}

shared_ptr<Version> VersionHistory::getPreviousVersion() {
    if (current && current->prev) {
        current = current->prev;
        return current->version;
    }
    return nullptr;
}

shared_ptr<Version> VersionHistory::getNextVersion() {
    if (current && current->next) {
        current = current->next;
        return current->version;
    }
    return nullptr;
}

vector<shared_ptr<Version>> VersionHistory::getAllVersions() const {
    vector<shared_ptr<Version>> versions;
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