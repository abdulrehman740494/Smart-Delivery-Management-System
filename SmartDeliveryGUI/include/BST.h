#pragma once
#include "Global.h"
#include <vector>



struct BSTNode {
    InventoryItem data;
    BSTNode* left;
    BSTNode* right;
};

extern BSTNode* inventoryRoot; // defined in BST.cpp

// ---- Core BST operations (all take/return BSTNode* explicitly, the
// standard recursive-pointer style taught in DSA courses) ------------------
BSTNode* bstInsert(BSTNode* root, const InventoryItem& item);
BSTNode* bstDelete(BSTNode* root, int itemId);
BSTNode* bstSearch(BSTNode* root, int itemId);
void bstInOrderCollect(BSTNode* root, std::vector<InventoryItem>& out); // sorted by itemId
void bstFreeAll(BSTNode* root);                         // frees every node (used on program exit / reset)
int nextInventoryItemId();                                // auto-increment ID generator
