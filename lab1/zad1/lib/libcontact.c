#include <libcontact.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// List
List *createList(){
    List* list = (List*) malloc(sizeof(List));
    list->head = (ListNode*) malloc(sizeof(ListNode));
    list->tail = (ListNode*) malloc(sizeof(ListNode));

    list->head->prev = NULL;
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->tail->next = NULL;
    list->elementCounter = 0;

    return list;
}

void deleteList(List* list){
    if(list != NULL){
        _deleteList(list->head);
        free(list);
    }
}

void addToList(List* list, Contact contact){
    if(list != NULL){
        ListNode* node = createListNode(contact);
        _addToList(list, node);
    }
}

ListNode *createListNode(Contact contact){
    ListNode* node = malloc(sizeof(ListNode));
    node->next = node->prev = NULL;
    node->value = contact;
    return node;
}

void printList(List* list){
    if(list != NULL){
        printf("List size: %d\n", list->elementCounter);
        int i = 0;
        ListNode* tmp = list->head->next;
        while(tmp != NULL && tmp!= list->tail){
            printf("Element nr: %d \t value: %d \n", i, tmp->value.xD);
            tmp = tmp->next;
            i++;
        }
    }
    else{
        printf("This list is empty\n");
    }
}

ListNode* findContactInList(List* list, Contact contact){
    ListNode* tmp = list->head;
    while(tmp->next != NULL && !contactsEqual(tmp->next->value, contact))
        tmp = tmp->next;
    return tmp->next;
}

bool contactsEqual(Contact c1, Contact c2){
    int comp =  strcmp(c1.name, c2.name) + strcmp(c1.surname, c2.surname) +
               strcmp(c1.email, c2.email) + strcmp(c1.phoneNumber, c2.phoneNumber) +
               strcmp(c1.birthDate, c2.birthDate) + strcmp(c1.address, c2.address);
    return (comp == 0);
}

// BST
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

BSTNode *createBSTNode(Contact contact){
    BSTNode* node = malloc(sizeof(BSTNode));
    node->parent = node->left = node->right = NULL;
    node->value = contact;
    return node;
}


// <private procedures>
// Lists
void _deleteList(ListNode* node){
    if(node != NULL){
        _deleteList(node->next);
        free(node);
    }
}

void _addToList(List* list, ListNode* node){
    ListNode* tmp = list->tail->prev;
    tmp->next = node;
    node->prev = tmp;
    node->next = list->tail;
    list->tail->prev = node;
    list->elementCounter++;
}

// BST
void _deleteBST(BSTNode* node){
    if(node != NULL){
        _deleteBST(node->left);
        _deleteBST(node->right);
        free(node);
    }
}



// </private procedures>

