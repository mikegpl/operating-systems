#include <contact.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static const size_t KEY_LEN = 35;

// ---------------------------------------------------------------------
// ---------------------------- Linked List ----------------------------
// ---------------------------------------------------------------------

List *List_newList(){
    List *list = (List*) malloc(sizeof(List));
    list->head = (ListNode*) malloc(sizeof(ListNode));
    list->tail = (ListNode*) malloc(sizeof(ListNode));

    list->head->prev = NULL;
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->tail->next = NULL;
    list->elementCounter = 0;

    return list;
}

void List_delete(List *list){
    if(list != NULL){
        list->tail->prev->next = NULL;
        _List_delete(list->head->next);
        free(list->head);
        free(list->tail);
        free(list);
    }
}

void List_addContact(List *list, Contact *contact){
    if(list != NULL){
        ListNode *node = ListNode_newNode(contact);
        _List_addNode(list, node);
    }
}

ListNode *ListNode_newNode(Contact *contact){
    ListNode *node = malloc(sizeof(ListNode));
    node->next = node->prev = NULL;
    node->value = contact;
    return node;
}

void List_print(List *list){
    if(list != NULL){
        printf("List size: %d\n", list->elementCounter);
        int i = 0;
        ListNode *tmp = list->head->next;
        while(tmp != NULL && tmp != list->tail){
            printf("Element nr: %d \t value: %s \n", i, tmp->value->name);
            tmp = tmp->next;
            i++;
        }
    } 
    else{
        printf("This list is empty\n");
    }
}

ListNode *List_findContact(List *list, Contact *contact){
    ListNode *tmp = list->head;
    while(tmp->next != NULL && !Contact_equals(tmp->next->value, contact))
        tmp = tmp->next;
    return tmp->next;
}

bool List_removeContact(List *list, Contact *contact){
    ListNode *node = List_findContact(list, contact);
    if(node != NULL){
        node->prev->next = node->next;
        node->next->prev = node->prev;
        Contact_delete(node->value);
        free(node);
        list->elementCounter--;
        return true;
    }
    else
        return false;
}

void List_forEach(List *list, ListNodeOperation operation){
    ListNode *tmp = list->head->next;
    while(tmp != NULL && tmp != list->tail){
        operation(tmp);
        tmp = tmp->next;
    }
}

void List_sort(List *list, KeyType type){
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
    }
}

void _List_delete(ListNode *node){
    if(node != NULL){
        _List_delete(node->next);
        Contact_delete(node->value);
        free(node);
    }
}

void _List_addNode(List *list, ListNode *node){
    ListNode *tmp = list->tail->prev;
    tmp->next = node;
    node->prev = tmp;
    node->next = list->tail;
    list->tail->prev = node;
    list->elementCounter++;
}

void _List_sort(List *list, Comparator cmp){
    list->head->next->prev = NULL;
    list->tail->prev->next = NULL;
    list->head->next = _ListNode_quickSort(list->head->next, list->tail->prev, cmp);
    list->head->next->prev = list->head;
    ListNode *tmp = _ListNode_getLast(list->head);
    tmp->next = list->tail;
    list->tail->prev = tmp;
}

void _ListNode_print(ListNode *node){
    while(node != NULL){
        printf("L %s\n", node->value->name);
        node = node->next;
    }
}

ListNode *_ListNode_getLast(ListNode *list){
    if(list == NULL)
        return NULL;
    while(list->next != NULL)
        list = list->next;
    return list;
}

ListNode *_ListNode_quickSort(ListNode *head, ListNode *tail, Comparator cmp){
    if(head != tail){
        ListNode *smaller = NULL;
        ListNode *larger = NULL;
        ListNode *pivot = head;
        ListNode *tmp = head->next;
        pivot->next = pivot->prev = tmp->prev = NULL;

        while(tmp != NULL){
            if(cmp(tmp->value, pivot->value) >= 0)
                larger = _ListNode_append(larger, tmp);
            else
                smaller = _ListNode_append(smaller, tmp);
            ListNode *endHolder = tmp->next;
            tmp->next = NULL;
            tmp = endHolder;
        }

        smaller = _ListNode_quickSort(smaller, _ListNode_getLast(smaller), cmp);
        larger = _ListNode_quickSort(larger, _ListNode_getLast(larger), cmp);
        if(smaller == NULL){
            if(larger != NULL){
                pivot->next = larger;
                pivot->prev = NULL;
                larger->prev = pivot;
            }
            return pivot;
        }
        else if (larger == NULL){
            ListNode *mid = _ListNode_getLast(smaller);   
            mid->next = pivot;
            pivot->prev = mid;
            pivot->next = NULL;
            smaller->prev = NULL;
            return smaller;
        }
        else{
            ListNode *mid = _ListNode_getLast(smaller);  
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

ListNode *_ListNode_append(ListNode *list, ListNode *node){
    if(list == NULL){
        node->prev = NULL;
        return node;
    }
    else{
        ListNode *tmp = list;
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
        tree->root = BSTNode_newNode(contact);
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


// ---------------------------------------------------------------------
// ---------------------------------------------------------------------


// ---------------------------------------------------------------------
// ----------------------------    Other    ----------------------------
// ---------------------------------------------------------------------


bool Contact_equals(Contact *c1, Contact *c2){
    int comp = strcmp(c1->name, c2->name) + strcmp(c1->surname, c2->surname) +
               strcmp(c1->email, c2->email) + strcmp(c1->phoneNumber, c2->phoneNumber) +
               strcmp(c1->birthDate, c2->birthDate) + strcmp(c1->address, c2->address);
    return (comp == 0);
}

Contact *Contact_new(){
    Contact *newContact = malloc(sizeof(Contact));
    newContact->name = malloc(KEY_LEN);
    newContact->surname = malloc(KEY_LEN);
    newContact->email = malloc(KEY_LEN);
    newContact->birthDate = malloc(KEY_LEN);
    newContact->phoneNumber = malloc(KEY_LEN);
    newContact->address = malloc(KEY_LEN);

    return newContact;
}

Contact *Contact_copy(Contact* contact){
    Contact *newContact = Contact_new();
    strcpy(newContact->name, contact->name);
    strcpy(newContact->surname, contact->surname);
    strcpy(newContact->email, contact->email);
    strcpy(newContact->birthDate, contact->birthDate);
    strcpy(newContact->phoneNumber, contact->phoneNumber);
    strcpy(newContact->address, contact->address);
    return newContact;
}  

void Contact_delete(Contact *contact){
    if(contact != NULL){
        free(contact->name);
        free(contact->surname);
        free(contact->birthDate);
        free(contact->email);
        free(contact->phoneNumber);
        free(contact->address);
        free(contact);
    }
}

// comparators
// >0 if first is bigger, 0 if equals, <0 if first is smaller

int Comparator_surname(Contact *c1, Contact *c2){
    return strcmp(c1->surname, c2->surname);
}

int Comparator_birthDate(Contact *c1, Contact *c2){
    return strcmp(c1->birthDate, c2->birthDate);
}

int Comparator_email(Contact *c1, Contact *c2){
    return strcmp(c1->email, c2->email);
}

int Comparator_phone(Contact *c1, Contact *c2){
    return strcmp(c1->phoneNumber, c2->phoneNumber);
}
