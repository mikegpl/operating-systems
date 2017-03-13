#include <libcontact.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

List *createList(){
    List* list = (List*) malloc(sizeof(List));
    list->head = list->tail = NULL;
    list->elementCounter = 0;
    return list;
}

BST *createBST(Comparator comparator){
    BST* tree = (BST*) malloc(sizeof(BST));
    tree->root = NULL;
    tree->elementCounter = 0;
    tree->comparator = comparator;
    return tree;
}

void deleteBST(BST* tree){
    if(tree != NULL){
        _deleteBST(tree->root);
        free(tree);
    }
}

void deleteList(List* list){
    if(list != NULL){
        _deleteList(list->)
        free(list);
    }
}

// <private procedures>
void _deleteBST(BSTNode* node){
    if(node != NULL){
        _deleteBST(node->left);
        _deleteBST(node->right);
        free(node);
    }
}

void _deleteList(ListNode* node){
    if(node != NULL){
        _deleteList(node->next);
        free(node);
    }
}
// </private procedures>

