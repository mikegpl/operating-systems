#include <libcontact.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ---------------------------------------------------------------------
// ---------------------------- Linked List ----------------------------
// ---------------------------------------------------------------------

List *List_newList(){
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

void List_delete(List* list){
    if(list != NULL){
        _List_delete(list->head);
        free(list);
    }
}

void List_addContact(List* list, Contact contact){
    if(list != NULL){
        ListNode* node = ListNode_newNode(contact);
        _List_addNode(list, node);
    }
}

ListNode *ListNode_newNode(Contact contact){
    ListNode* node = malloc(sizeof(ListNode));
    node->next = node->prev = NULL;
    node->value = contact;
    return node;
}

void List_print(List* list){
    if(list != NULL){
        printf("List size: %d\n", list->elementCounter);
        int i = 0;
        ListNode* tmp = list->head->next;
        while(tmp != NULL && tmp != list->tail){
            printf("Element nr: %d \t value: %d \n", i, tmp->value.xD);
            tmp = tmp->next;
            i++;
        }
    } 
    else{
        printf("This list is empty\n");
    }
}

ListNode* List_findContact(List* list, Contact contact){
    ListNode* tmp = list->head;
    while(tmp->next != NULL && !Contact_equals(tmp->next->value, contact))
        tmp = tmp->next;
    return tmp->next;
}

bool List_removeContact(List* list, Contact contact){
    ListNode* node = List_findContact(list, contact);
    if(node != NULL){
        node->prev->next = node->next;
        node->next->prev = node->prev;
        free(node);
        list->elementCounter--;
        return true;
    }
    else
        return false;
}

void List_forEach(List* list, ListNodeOperation operation){
    ListNode* tmp = list->head->next;
    while(tmp != NULL && tmp != list->tail){
        operation(tmp);
        tmp = tmp->next;
    }
}

void _List_delete(ListNode* node){
    if(node != NULL){
        _List_delete(node->next);
        free(node);
    }
}

void _List_addNode(List* list, ListNode* node){
    ListNode* tmp = list->tail->prev;
    tmp->next = node;
    node->prev = tmp;
    node->next = list->tail;
    list->tail->prev = node;
    list->elementCounter++;
}

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------



// ---------------------------------------------------------------------
// ----------------------------     BST     ----------------------------
// ---------------------------------------------------------------------

BST *BST_newBST(Comparator comparator){
    BST* tree = (BST*) malloc(sizeof(BST));
    tree->root = NULL;
    tree->elementCounter = 0;
    tree->comparator = comparator;
    return tree;
}

void BST_delete(BST* tree){
    if(tree != NULL){
        _BST_delete(tree->root);
        free(tree);
    }
}

BSTNode *BSTNode_newNode(Contact contact){
    BSTNode* node = malloc(sizeof(BSTNode));
    node->parent = node->left = node->right = NULL;
    node->value = contact;
    return node;
}

void _BST_delete(BSTNode* node){
    if(node != NULL){
        _BST_delete(node->left);
        _BST_delete(node->right);
        free(node);
    }
}

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------


// ---------------------------------------------------------------------
// ----------------------------    Other    ----------------------------
// ---------------------------------------------------------------------


bool Contact_equals(Contact c1, Contact c2){
    int comp = strcmp(c1.name, c2.name) + strcmp(c1.surname, c2.surname) +
               strcmp(c1.email, c2.email) + strcmp(c1.phoneNumber, c2.phoneNumber) +
               strcmp(c1.birthDate, c2.birthDate) + strcmp(c1.address, c2.address);
    return (comp == 0);
}


