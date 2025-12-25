#include "PersistentRope.h"
#include <algorithm>
#include <cmath>

using namespace std;

// ==================== PersistentRope Implementation ====================

PersistentRope::PersistentRope(const string& text) {
    if (text.empty()) {
        root = make_shared<RopeNode>("");
        return;
    }
    
    // Split text into leaf nodes
    vector<shared_ptr<RopeNode>> leaves;
    for (size_t i = 0; i < text.length(); i += MAX_LEAF_SIZE) {
        string chunk = text.substr(i, min(MAX_LEAF_SIZE, text.length() - i));
        leaves.push_back(make_shared<RopeNode>(chunk));
    }
    
    // Build balanced tree from leaves
    while (leaves.size() > 1) {
        vector<shared_ptr<RopeNode>> newLevel;
        for (size_t i = 0; i < leaves.size(); i += 2) {
            if (i + 1 < leaves.size()) {
                newLevel.push_back(make_shared<RopeNode>(leaves[i], leaves[i + 1]));
            } else {
                newLevel.push_back(leaves[i]);
            }
        }
        leaves = newLevel;
    }
    
    root = leaves.empty() ? make_shared<RopeNode>("") : leaves[0];
}

shared_ptr<RopeNode> PersistentRope::concatenate(shared_ptr<RopeNode> left, shared_ptr<RopeNode> right) {
    if (!left) return right;
    if (!right) return left;
    
    auto newNode = make_shared<RopeNode>(left, right);
    return rebalance(newNode);
}

pair<shared_ptr<RopeNode>, shared_ptr<RopeNode>> PersistentRope::split(shared_ptr<RopeNode> node, size_t pos) {
    if (!node || pos == 0) {
        return {nullptr, node};
    }
    
    if (pos >= node->totalWeight()) {
        return {node, nullptr};
    }
    
    if (node->isLeaf()) {
        if (pos >= node->data.length()) {
            return {node, nullptr};
        }
        auto leftNode = make_shared<RopeNode>(node->data.substr(0, pos));
        auto rightNode = make_shared<RopeNode>(node->data.substr(pos));
        return {leftNode, rightNode};
    }
    
    if (pos < node->weight) {
        auto [leftLeft, leftRight] = split(node->left, pos);
        auto newRight = concatenate(leftRight, node->right);
        return {leftLeft, newRight};
    } else {
        auto [rightLeft, rightRight] = split(node->right, pos - node->weight);
        auto newLeft = concatenate(node->left, rightLeft);
        return {newLeft, rightRight};
    }
}

int PersistentRope::getHeight(shared_ptr<RopeNode> node) const {
    if (!node) return 0;
    if (node->isLeaf()) return 1;
    return 1 + max(getHeight(node->left), getHeight(node->right));
}

shared_ptr<RopeNode> PersistentRope::rebalance(shared_ptr<RopeNode> node) {
    if (!node || node->isLeaf()) return node;
    
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    
    // If balanced enough, return as is
    if (abs(leftHeight - rightHeight) <= 1) {
        return node;
    }
    
    // Simple rebalancing - rebuild subtree
    string text;
    collectText(node, text);
    return PersistentRope(text).root;
}

void PersistentRope::collectText(shared_ptr<RopeNode> node, string& result) const {
    if (!node) return;
    
    if (node->isLeaf()) {
        result += node->data;
        return;
    }
    
    collectText(node->left, result);
    collectText(node->right, result);
}

PersistentRope PersistentRope::insert(size_t pos, const string& text) const {
    if (text.empty()) return *this;
    
    auto [left, right] = split(root, pos);
    auto middle = PersistentRope(text).root;
    
    auto newRoot = concatenate(concatenate(left, middle), right);
    return PersistentRope(newRoot);
}

PersistentRope PersistentRope::deleteText(size_t pos, size_t length) const {
    if (length == 0 || pos >= this->length()) return *this;
    
    length = min(length, this->length() - pos);
    
    auto [left, temp] = split(root, pos);
    auto [middle, right] = split(temp, length);
    
    auto newRoot = concatenate(left, right);
    return PersistentRope(newRoot);
}

PersistentRope PersistentRope::replace(size_t pos, size_t length, const string& text) const {
    return deleteText(pos, length).insert(pos, text);
}

string PersistentRope::getText() const {
    string result;
    collectText(root, result);
    return result;
}

string PersistentRope::substring(size_t pos, size_t length) const {
    if (pos >= this->length()) return "";
    length = min(length, this->length() - pos);
    
    auto [left, temp] = split(root, pos);
    auto [middle, right] = split(temp, length);
    
    string result;
    collectText(middle, result);
    return result;
}