#ifndef CONTACTTREE_H
#define CONTACTTREE_H

typedef struct BSTNode BSTNode;
typedef struct BST BST;
typedef void (*BSTNodeOperation)(BSTNode*);

struct BSTNode
{
    Contact* value;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
};

struct BST
{
    BSTNode* root;
    int elementCounter;
    KeyType keyType;
    Comparator comparator;
};

BST* BST_newBST(KeyType);
void BST_delete(BST*);
void BST_addContact(BST*, Contact*);
BSTNode* BSTNode_newNode(Contact*);
void BST_print(BST*);
BSTNode* BST_findContact(BST*, Contact*);
bool BST_removeContact(BST*, Contact*);
void BST_forEach(BST*, BSTNodeOperation);
BST* BST_sort(BST*, KeyType);

BST* _BST_newBST(Comparator);
void _BST_delete(BSTNode*);
void _BST_addNode(BST*, BSTNode*);
void _BST_printInOrder(BSTNode*);
BSTNode* _BSTNode_findContact(BSTNode*, Contact*, Comparator);
void _BST_removeNode(BST*, BSTNode*);
BSTNode* _BSTNode_findMin(BSTNode*);
void _BST_forEach(BSTNode*, BSTNodeOperation);
BST* _BST_sort(BST*, Comparator);
void _BST_copyNodes(BST*, BSTNode*);

#endif