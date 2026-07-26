#include "BST.h"
#include <iostream>
#include <vector>

BSTNode* inventoryRoot = nullptr;

static int nextItemId = 1; // simple auto-increment ID generator for new items

int nextInventoryItemId() { return nextItemId++; }

BSTNode* bstInsert(BSTNode* root, const InventoryItem& item) {
    if (root == nullptr) {
        BSTNode* node = new BSTNode();
        node->data = item;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    if (item.itemId < root->data.itemId) {
        root->left = bstInsert(root->left, item);
    } else if (item.itemId > root->data.itemId) {
        root->right = bstInsert(root->right, item);
    } else {
        root->data.quantity = item.quantity;
        root->data.itemName = item.itemName;
    }
    return root;
}

BSTNode* bstSearch(BSTNode* root, int itemId) {
    if (root == nullptr || root->data.itemId == itemId) return root;
    if (itemId < root->data.itemId) return bstSearch(root->left, itemId);
    return bstSearch(root->right, itemId);
}

static BSTNode* findMin(BSTNode* root) {
    while (root->left != nullptr) root = root->left;
    return root;
}

BSTNode* bstDelete(BSTNode* root, int itemId) {
    if (root == nullptr) return nullptr;

    if (itemId < root->data.itemId) {
        root->left = bstDelete(root->left, itemId);
    } else if (itemId > root->data.itemId) {
        root->right = bstDelete(root->right, itemId);
    } else {
        // This is the node to delete.
        if (root->left == nullptr) {
            BSTNode* rightChild = root->right;
            delete root;
            return rightChild;
        }
        if (root->right == nullptr) {
            BSTNode* leftChild = root->left;
            delete root;
            return leftChild;
        }

        BSTNode* successor = findMin(root->right);
        root->data = successor->data;
        root->right = bstDelete(root->right, successor->data.itemId);
    }
    return root;
}

void bstInOrderCollect(BSTNode* root, std::vector<InventoryItem>& out) {
    if (root == nullptr) return;
    bstInOrderCollect(root->left, out);
    out.push_back(root->data);
    bstInOrderCollect(root->right, out);
}

void bstFreeAll(BSTNode* root) {
    if (root == nullptr) return;
    bstFreeAll(root->left);
    bstFreeAll(root->right);
    delete root;
}

