### **Description**

1. **Tree Initialization:**

   - The B+ Tree is initialized with an order of 50 (maximum children per node).

2. **Data Loading:**

   - Words and their meanings are read from a file and inserted into the tree.

3. **Tree Operations:**

   - **Insert:** Keys and values are inserted, ensuring the tree remains balanced.
   - **Search:** User-entered words are searched in the tree.
   - **Display:** The structure of the tree can be displayed (for debugging or verification).

4. **User Interaction:**
   - The program continuously accepts user input for word searches.

---

### **Class `BPlusTreeNode`**

This class defines a node in the B+ Tree.

- **Attributes:**

  - `isLeaf`: A boolean indicating whether the node is a leaf.
  - `keys`: A vector of strings that holds the keys (words).
  - `values`: A vector of vectors where each inner vector contains meanings for a word (only relevant for leaf nodes).
  - `children`: A vector of pointers to child nodes (used in non-leaf nodes).
  - `next`: A pointer to the next leaf node (used for efficient traversal of leaf nodes in order).

- **Constructor:**
  - Initializes the node as either a leaf or a non-leaf and sets `next` to `nullptr`.

---

### **Class `BPlusTree`**

This class implements the B+ Tree.

---

#### **1. Constructor:**

```cpp
BPlusTree(int order) : order(order), root(new BPlusTreeNode(true)) {}
```

- Initializes the B+ Tree with a given order (maximum number of children per node).
- Sets the root node as an empty leaf node.

---

#### **2. `findLeafNode`**

```cpp
BPlusTreeNode* findLeafNode(const string& key)
```

- **Purpose:** Finds the appropriate leaf node where a given key should be located.
- **Logic:**
  - Start at the root.
  - While the current node is not a leaf:
    - Traverse the children of the node based on the comparison between the key and the keys in the current node.
  - Return the leaf node where the key should reside.

---

#### **3. `splitChild`**

```cpp
void splitChild(BPlusTreeNode* parent, int index)
```

- **Purpose:** Splits a child node that is full and promotes a key to the parent.
- **Logic:**
  - Identify the child node to split.
  - Determine the middle index (`midIndex`) of the node.
  - Create a new node (`newNode`) to hold the second half of the keys and children.
  - Adjust the original node (`nodeToSplit`) to keep only the first half of the keys and children.
  - Promote a key to the parent node:
    - For leaf nodes, promote the first key of the new node.
    - For non-leaf nodes, promote the middle key of the original node.
  - Update `next` pointers if the nodes are leaves.

---

#### **4. `insertInternal`**

```cpp
void insertInternal(const string& key, const string& value)
```

- **Purpose:** Handles the insertion of a key-value pair into the tree.
- **Logic:**
  - Find the appropriate leaf node using `findLeafNode`.
  - Insert the key in sorted order within the leaf node:
    - If the key already exists, add the new meaning to the list of values (if not a duplicate).
    - Otherwise, insert the key and the new value.
  - If the leaf node is full after insertion:
    - Split the node using `splitChild`.
    - If the root is full, create a new root.

---

#### **5. `findParent`**

```cpp
BPlusTreeNode* findParent(BPlusTreeNode* current, BPlusTreeNode* child)
```

- **Purpose:** Finds the parent node of a given child node.
- **Logic:**
  - Traverse the tree recursively to locate the parent of the child node.
  - If `current`'s children include `child`, return `current`.
  - Otherwise, recursively check each child of `current`.

---

#### **6. `findParentChildIndex`**

```cpp
int findParentChildIndex(BPlusTreeNode* parent, BPlusTreeNode* child)
```

- **Purpose:** Finds the index of a child node within its parent's children vector.
- **Logic:**
  - Iterate through the parent's children to find the index of the child.

---

#### **7. `insert`**

```cpp
void insert(const string& key, const string& value)
```

- **Purpose:** Public method to insert a key-value pair into the tree.
- **Logic:**
  - Calls `insertInternal` to perform the insertion.

---

#### **8. `search`**

```cpp
string search(const string& key)
```

- **Purpose:** Searches for a key in the tree and returns its meanings.
- **Logic:**
  - Locate the appropriate leaf node using `findLeafNode`.
  - Search for the key within the node's keys.
  - If found, concatenate and return its meanings.
  - If not found, return `"Word not found"`.

---

#### **9. `display`**

```cpp
void display()
```

- **Purpose:** Displays the contents of the tree.
- **Logic:**
  - Calls `displayNode` to recursively print the keys and values of each node.

---

#### **10. `displayNode`**

```cpp
void displayNode(BPlusTreeNode* node)
```

- **Purpose:** Helper function for `display`.
- **Logic:**
  - Print the keys and values (meanings) for the current node.
  - Recursively print the children (if any) for non-leaf nodes.

---

### **Utility Function: `loadDictionary`**

```cpp
void loadDictionary(const string& filePath, BPlusTree& tree)
```

- **Purpose:** Loads key-value pairs (words and meanings) from a file into the B+ Tree.
- **Logic:**
  - Open the file.
  - Read each line, split it into a word and its meaning using a delimiter (`,`).
  - Insert the word and meaning into the tree using `insert`.

---

### **`main` Function**

```cpp
int main()
```

- **Purpose:** Entry point of the program.
- **Logic:**
  1. Create a B+ Tree with an order of 50.
  2. Load a dictionary file (`EnglishDictionary.csv`) into the tree.
  3. Allow the user to search for words in a loop.
  4. Print the meanings of the entered words (if found) or an error message.

---
