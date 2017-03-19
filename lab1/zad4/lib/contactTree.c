#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "contact.h"
#include "contactTree.h"

BST *BST_newBST(KeyType type){
    BST *tree = (BST*) malloc(sizeof(BST));
    tree->root = NULL;
    tree->elementCounter = 0;
    tree->keyType = type;
    switch(type){
        case SURNAME:
            tree->comparator = Comparator_surname;
            break;
        case BIRTHDATE:
            tree->comparator = Comparator_birthDate;
            break;
        case EMAIL:
            tree->comparator = Comparator_email;
            break;
        case PHONE:
            tree->comparator = Comparator_email;
            break;
    }
    return tree;
}

void BST_delete(BST *tree){
    if(tree != NULL){
        _BST_delete(tree->root);
        free(tree);
    }
}

void BST_addContact(BST *tree, Contact *contact){
    if(tree->root == NULL)
        tree->root = BSTNode_newNode(&contact);
    else
        _BST_addNode(tree, BSTNode_newNode(contact));
    tree->elementCounter++;
}

BSTNode *BSTNode_newNode(Contact *contact){
    BSTNode *node = malloc(sizeof(BSTNode));
    node->parent = node->left = node->right = NULL;
    node->value = contact;
    return node;
}

void BST_print(BST *tree){
    printf("Printing tree\n");
    _BST_printInOrder(tree->root);
}

BSTNode *BST_findContact(BST *tree, Contact *contact){
    return _BSTNode_findContact(tree->root, contact, tree->comparator);
}

bool BST_removeContact(BST *tree, Contact *contact){
    BSTNode *node = BST_findContact(tree, contact);
    if(tree == NULL || node == NULL)
        return false;
    else{
        _BST_removeNode(tree, node);
        return true;
    }
}

void BST_forEach(BST *tree, BSTNodeOperation operation){
    if(tree != NULL){
        _BST_forEach(tree->root, operation);
    }
}


BST *BST_sort(BST *tree, KeyType type){
    if(tree != NULL){
        if(tree->keyType == type)
            return tree;
        else{
            BST *newTree = BST_newBST(type);
            _BST_copyNodes(newTree, tree->root);
            BST_delete(tree);
            return newTree;
        }
    }
    else
        return NULL;
}



void _BST_delete(BSTNode *node){
    if(node != NULL){
        _BST_delete(node->left);
        _BST_delete(node->right);
        Contact_delete(node->value);
        free(node);
    }
}

void _BST_addNode(BST *tree, BSTNode *node){
    BSTNode *x = tree->root;
    BSTNode *y = x;
    Comparator cmp = tree->comparator;
    while(x != NULL){
        y = x;
        if(cmp(node->value, x->value) >= 0)
            x = x->right;
        else
            x = x->left;
    }
    node->parent = y;
    if(y == NULL)
        tree->root = node;
    else{
        if(cmp(node->value, y->value) >= 0)
            y->right = node;
        else
            y->left = node;
    }
}


void _BST_printInOrder(BSTNode *node){
    if(node != NULL){
        _BST_printInOrder(node->left);
        printf("%s\n", node->value->name);
        _BST_printInOrder(node->right);
    }
}

BSTNode *_BSTNode_findContact(BSTNode *root, Contact *contact, Comparator cmp){
    while(root != NULL && !Contact_equals(root->value, contact))
        if(cmp(contact, root->value) >= 0)
            root = root->right;
        else
            root = root->left;
    return root;
}

void _BST_removeNode(BST *tree, BSTNode *node){
    if(node->left == NULL && node->right == NULL){
        if(node->parent == NULL)
            tree->root = NULL;
        else if(node == node->parent->left)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
        Contact_delete(node->value);
        free(node);
    }
    else if(node->left == NULL || node->right == NULL){
        if(node->left == NULL){
            if(node->parent == NULL){
                tree->root = node->right;
                free(node);
                tree->root->parent = NULL;
            }
            else{
                if(node == node->parent->left){
                    node->parent->left = node->right;
                    node->right->parent = node->parent;
                }
                else{
                    node->parent->right = node->right;
                    node->right->parent = node->parent;
                }
                Contact_delete(node->value);
                free(node);
            }
        }
        else{
            if(node->parent == NULL){
                tree->root = node->left;
                Contact_delete(node->value);
                free(node);
                tree->root->parent = NULL;
            }
            else{
                if(node == node->parent->left){
                    node->parent->left = node->left;
                    node->left->parent = node->parent;
                }
                else{
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
                }
                Contact_delete(node->value);
                free(node);
            }
        }
    }
    else{
        BSTNode *tmp = _BSTNode_findMin(node->right);
        node->value = tmp->value;
        tmp->value = NULL;
        _BST_removeNode(tree, tmp);
    }
}

BSTNode *_BSTNode_findMin(BSTNode *node){
    while(node->left != NULL)
        node = node->left;
    return node; 
}

void _BST_forEach(BSTNode *root, BSTNodeOperation operation){
    if(root != NULL){
        _BST_forEach(root->left, operation);
        operation(root);
        _BST_forEach(root->right, operation);
    }
}

void _BST_copyNodes(BST *target, BSTNode *sourceRoot){
    if(sourceRoot != NULL){
        _BST_copyNodes(target, sourceRoot->left);
        BST_addContact(target, Contact_copy(sourceRoot->value));
        _BST_copyNodes(target, sourceRoot->right);
    }
}