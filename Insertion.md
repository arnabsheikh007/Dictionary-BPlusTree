### **Insert Overview**
The insertion process in a B+ Tree involves the following key steps:
1. Find the appropriate **leaf node** to insert the key.
2. Insert the key in sorted order in the leaf node.
3. If the leaf node becomes overfull (contains `order` or more keys), **split the node**:
   - Create a new node for the second half of the keys.
   - Promote a key to the parent.
4. Repeat this process up the tree if necessary.
5. If the root node is split, create a new root.

---

### **Insert Process Details**
Here’s how the insertion process works in this implementation:

---

#### **1. Start the Insertion**
```cpp
void insert(const string& key, const string& value) {
    insertInternal(key, value);
}
```
- `insert()` is the public function called to insert a key-value pair.
- It delegates the actual work to `insertInternal()`.

---

#### **2. Find the Appropriate Leaf Node**
```cpp
BPlusTreeNode* leaf = findLeafNode(key);
```
- `findLeafNode()` starts at the root and traverses down the tree until it reaches the leaf node where the key should reside.
- At each level, it compares the key with the keys in the current node to determine the appropriate child to follow.

---

#### **3. Insert the Key in the Leaf Node**
```cpp
auto it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
int pos = it - leaf->keys.begin();

if (pos < leaf->keys.size() && leaf->keys[pos] == key) {
    if (find(leaf->values[pos].begin(), leaf->values[pos].end(), value) == leaf->values[pos].end()) {
        leaf->values[pos].push_back(value);  // Add the new meaning
    }
} else {
    leaf->keys.insert(it, key);
    leaf->values.insert(leaf->values.begin() + (it - leaf->keys.begin()), {value});
}
```
- Find the correct position for the new key in the `keys` vector using `lower_bound()` to maintain sorted order.
- **If the key already exists:**
  - Check if the `value` (meaning) already exists for the key.
  - If not, add the new value to the `values` vector.
- **If the key does not exist:**
  - Insert the new key into the `keys` vector.
  - Insert the corresponding value into the `values` vector.

---

#### **4. Check for Node Overflow**
```cpp
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
```
- If the leaf node has `order` or more keys after the insertion, it overflows and needs to be split.
- **If the leaf node is the root:**
  - Create a new root node and split the current root.
  - Update the root pointer to point to the new root.
- **If the leaf node is not the root:**
  - Find its parent node using `findParent()`.
  - Split the leaf node and promote a key to the parent.

---

#### **5. Split the Node**
```cpp
void splitChild(BPlusTreeNode* parent, int index) {
    BPlusTreeNode* nodeToSplit = parent->children[index];
    int midIndex = order / 2;

    BPlusTreeNode* newNode = new BPlusTreeNode(nodeToSplit->isLeaf);
    newNode->keys.assign(nodeToSplit->keys.begin() + midIndex, nodeToSplit->keys.end());
    if (nodeToSplit->isLeaf) {
        newNode->values.assign(nodeToSplit->values.begin() + midIndex, nodeToSplit->values.end());
        newNode->next = nodeToSplit->next;
        nodeToSplit->next = newNode;
    } else {
        newNode->children.assign(nodeToSplit->children.begin() + midIndex + 1, nodeToSplit->children.end());
    }

    nodeToSplit->keys.resize(midIndex);
    if (nodeToSplit->isLeaf) {
        nodeToSplit->values.resize(midIndex);
    } else {
        nodeToSplit->children.resize(midIndex + 1);
    }

    string promoteKey = nodeToSplit->isLeaf ? newNode->keys[0] : nodeToSplit->keys[midIndex];
    parent->keys.insert(parent->keys.begin() + index, promoteKey);
    parent->children.insert(parent->children.begin() + index + 1, newNode);
}
```
- The split divides the overflowing node into two nodes:
  - The original node keeps the first half of its keys and values/children.
  - A new node is created to hold the second half.
- For **leaf nodes**:
  - Copy the second half of `keys` and `values` to the new node.
  - Update the `next` pointer to maintain the linked list of leaf nodes.
- For **non-leaf nodes**:
  - Copy the second half of `keys` and `children` to the new node.
- Promote a key to the parent:
  - For leaf nodes, promote the first key of the new node.
  - For non-leaf nodes, promote the middle key of the original node.

---

#### **6. Repeat Splits Up the Tree (if Needed)**
- If splitting a child causes the parent node to overflow, the parent is also split.
- This process repeats recursively up the tree until:
  - No node overflows, or
  - A new root is created.

---

This process ensures the B+ Tree maintains its **balance** and **sorted order** while satisfying the properties of the tree.