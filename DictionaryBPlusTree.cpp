#include<bits/stdc++.h>
using namespace std;

class BPlusTreeNode {
public:
    bool isLeaf;
    vector<string> keys;
    vector<string> values;
    vector<BPlusTreeNode*> children;
    BPlusTreeNode* next;

    BPlusTreeNode(bool isLeaf) : isLeaf(isLeaf), next(nullptr) {}
};

class BPlusTree {
private:
    int order;
    BPlusTreeNode* root;

    BPlusTreeNode* findLeafNode(const string& key) {
        BPlusTreeNode* current = root;
        while (!current->isLeaf) {
            size_t i = 0;
            while (i < current->keys.size() && key > current->keys[i]) ++i;
            current = current->children[i];
        }
        return current;
    }

    void splitChild(BPlusTreeNode* parent, int index) {
        BPlusTreeNode* nodeToSplit = parent->children[index];
        int midIndex = order / 2;

        // Create a new node to hold the second half of the keys and children
        BPlusTreeNode* newNode = new BPlusTreeNode(nodeToSplit->isLeaf);
        newNode->keys.assign(nodeToSplit->keys.begin() + midIndex, nodeToSplit->keys.end());
        if (nodeToSplit->isLeaf) {
            newNode->values.assign(nodeToSplit->values.begin() + midIndex, nodeToSplit->values.end());
            newNode->next = nodeToSplit->next;
            nodeToSplit->next = newNode;
        } else {
            newNode->children.assign(nodeToSplit->children.begin() + midIndex + 1, nodeToSplit->children.end());
        }

        // Reduce the size of the original node
        nodeToSplit->keys.resize(midIndex);
        if (nodeToSplit->isLeaf) {
            nodeToSplit->values.resize(midIndex);
        } else {
            nodeToSplit->children.resize(midIndex + 1);
        }

        // Promote a key to the parent node
        string promoteKey = nodeToSplit->isLeaf ? newNode->keys[0] : nodeToSplit->keys[midIndex];
        parent->keys.insert(parent->keys.begin() + index, promoteKey);
        parent->children.insert(parent->children.begin() + index + 1, newNode);
    }

    void insertInternal(const string& key, const string& value) {
        BPlusTreeNode* leaf = findLeafNode(key);

        // Insert key-value pair into the leaf node
        auto it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        int pos = it - leaf->keys.begin();
        leaf->keys.insert(it, key);
        leaf->values.insert(leaf->values.begin() + pos, value);

        // Check for overflow and handle splits
        if (leaf->keys.size() >= order) {
            if (leaf == root) {
                BPlusTreeNode* newRoot = new BPlusTreeNode(false);
                newRoot->children.push_back(root);
                splitChild(newRoot, 0);
                root = newRoot;
            } else {
                BPlusTreeNode* parent = findParent(root, leaf);
                int index = findParentChildIndex(parent, leaf);
                splitChild(parent, index);
            }
        }
    }

    BPlusTreeNode* findParent(BPlusTreeNode* current, BPlusTreeNode* child) {
        if (current->isLeaf || current->children.empty()) return nullptr;

        for (size_t i = 0; i < current->children.size(); ++i) {
            if (current->children[i] == child) return current;
            BPlusTreeNode* parent = findParent(current->children[i], child);
            if (parent) return parent;
        }
        return nullptr;
    }

    int findParentChildIndex(BPlusTreeNode* parent, BPlusTreeNode* child) {
        for (size_t i = 0; i < parent->children.size(); ++i) {
            if (parent->children[i] == child) return i;
        }
        return -1;
    }

public:
    BPlusTree(int order) : order(order), root(new BPlusTreeNode(true)) {}

    void insert(const string& key, const string& value) {
        insertInternal(key, value);
    }

    string search(const string& key) {
        BPlusTreeNode* leaf = findLeafNode(key);

        for (size_t i = 0; i < leaf->keys.size(); ++i) {
            if (leaf->keys[i] == key) {
                return leaf->values[i];
            }
        }
        return "Word not found";
    }

    void display() {
        displayNode(root);
    }

private:
    void displayNode(BPlusTreeNode* node) {
        if (node == nullptr) return;

        for (const string& key : node->keys) {
            cout << key << " ";
        }
        cout << endl;

        if (!node->isLeaf) {
            for (BPlusTreeNode* child : node->children) {
                displayNode(child);
            }
        }
    }
};

void loadDictionary(const string& filePath, BPlusTree& tree) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t delimiterPos = line.find(',');
        if (delimiterPos == string::npos) continue;

        string word = line.substr(0, delimiterPos);
        string meaning = line.substr(delimiterPos + 1);
        tree.insert(word, meaning);
    }
    file.close();
}

int main() {
    BPlusTree tree(50);
    loadDictionary("EnglishDictionary.csv", tree);

    cout << "Dictionary loaded. Testing search:" << endl;

    while(true){
        string word;
        cout << "Enter a word to search: ";
        cin >> word;

        string meaning = tree.search(word);
        cout << "Meaning: " << meaning << endl;
    }

    return 0;
}

