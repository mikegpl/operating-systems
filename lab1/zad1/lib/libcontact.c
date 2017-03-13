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

void List_sort(List* list, KeyType type){
    switch(type){
        case SURNAME:
            _List_sort(list, Comparator_surname);
            break;
        case BIRTHDATE:
            _List_sort(list, Comparator_birthDate);
            break;
        case EMAIL:
            _List_sort(list, Comparator_email);
            break;
        case PHONE:
            _List_sort(list, Comparator_phone);
            break;
        case XD:
            _List_sort(list, Comparator_xD);
            break;
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

void _List_sort(List* list, Comparator cmp){
    list->head->next->prev = NULL;
    list->tail->prev->next = NULL;
    list->head->next = _ListNode_quickerSort(list->head->next, list->tail->prev, cmp);
    ListNode* tmp = _ListNode_getLast(list->head);
    tmp->next = list->tail;
    list->tail->prev = tmp;
}

void _ListNode_print(ListNode* node){
    while(node != NULL){
        printf("L %d\n", node->value.xD);
        node = node->next;
    }
}

ListNode* _ListNode_getLast(ListNode* list){
    if(list == NULL)
        return NULL;
    while(list->next != NULL)
        list = list->next;
    return list;
}

ListNode* _ListNode_quickerSort(ListNode* head, ListNode* tail, Comparator cmp){
    if(head != tail){
        ListNode* smaller = NULL;
        ListNode* larger = NULL;
        ListNode* pivot = head;
        ListNode* tmp = head->next;
        pivot->next = pivot->prev = tmp->prev = NULL;

        while(tmp != NULL){
            if(cmp(tmp->value, pivot->value) >= 0)
                larger = _ListNode_append(larger, tmp);
            else
                smaller = _ListNode_append(smaller, tmp);
            ListNode* endHolder = tmp->next;
            tmp->next = NULL;
            tmp = endHolder;
        }

        smaller = _ListNode_quickerSort(smaller, _ListNode_getLast(smaller), cmp);
        larger = _ListNode_quickerSort(larger, _ListNode_getLast(larger), cmp);
        if(smaller == NULL){
            if(larger != NULL){
                pivot->next = larger;
                pivot->prev = NULL;
                larger->prev = pivot;
            }
            return pivot;
        }
        else if (larger == NULL){
            ListNode* mid = _ListNode_getLast(smaller);   
            mid->next = pivot;
            pivot->prev = mid;
            pivot->next = NULL;
            smaller->prev = NULL;
            return smaller;
        }
        else{
            ListNode* mid = _ListNode_getLast(smaller);  
            mid->next = pivot;
            pivot->prev = mid;
            pivot->next = larger;
            larger->prev = pivot;
            smaller->prev = NULL;
            return smaller;  
        }
    }
    else
        return head;
}

ListNode* _ListNode_append(ListNode* list, ListNode* node){
    if(list == NULL){
        node->prev = NULL;
        return node;
    }
    else{
        ListNode* tmp = list;
        while(tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = node;
        node->prev = tmp;
        return list;
    }
}


// ---------------------------------------------------------------------
// ---------------------------------------------------------------------



// ---------------------------------------------------------------------
// ----------------------------     BST     ----------------------------
// ---------------------------------------------------------------------

BST *BST_newBST(KeyType type){
    BST* tree = (BST*) malloc(sizeof(BST));
    tree->root = NULL;
    tree->elementCounter = 0;
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
        case XD:
            tree->comparator = Comparator_xD;
            break;
    }
    return tree;
}

void BST_delete(BST* tree){
    if(tree != NULL){
        _BST_delete(tree->root);
        free(tree);
    }
}

void BST_addContact(BST* tree, Contact contact){
    if(tree->root == NULL)
        tree->root = BSTNode_newNode(contact);
    else
        _BST_addNode(tree, BSTNode_newNode(contact));
    tree->elementCounter++;
}

BSTNode *BSTNode_newNode(Contact contact){
    BSTNode* node = malloc(sizeof(BSTNode));
    node->parent = node->left = node->right = NULL;
    node->value = contact;
    return node;
}

void BST_print(BST* tree){
    printf("Printing tree\n");
    _BST_printPreOrder(tree->root);
}

BSTNode* BST_findContact(BST* tree, Contact contact){
    return _BSTNode_findContact(tree->root, contact, tree->comparator);
}

void _BST_delete(BSTNode* node){
    if(node != NULL){
        _BST_delete(node->left);
        _BST_delete(node->right);
        free(node);
    }
}

void _BST_addNode(BST* tree, BSTNode* node){
    BSTNode* x = tree->root;
    BSTNode* y = x;
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


void _BST_printPreOrder(BSTNode* node){
    if(node != NULL){
        printf("%d\n", node->value.xD);
        _BST_printPreOrder(node->left);
        _BST_printPreOrder(node->right);
    }
}

BSTNode* _BSTNode_findContact(BSTNode* root, Contact contact, Comparator cmp){
    while(root != NULL && !Contact_equals(root->value, contact))
        if(cmp(contact, root->value) >= 0)
            root = root->right;
        else
            root = root->left;
    return root;
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

// comparators
// >0 if first is bigger, 0 if equals, <0 if first is smaller

int Comparator_surname(Contact c1, Contact c2){
    return strcmp(c1.surname, c2.surname);
}

int Comparator_birthDate(Contact c1, Contact c2){
    return strcmp(c1.birthDate, c2.birthDate);
}

int Comparator_email(Contact c1, Contact c2){
    return strcmp(c1.email, c2.email);
}

int Comparator_phone(Contact c1, Contact c2){
    return strcmp(c1.phoneNumber, c2.phoneNumber);
}

int Comparator_xD(Contact c1, Contact c2){
    return (c1.xD - c2.xD);
}