// libcontact.h

#ifndef CONTACT_H
#define CONTACT_H

typedef int bool;
#define true 1
#define false 0

typedef struct ContactBook ContactBook;
typedef struct Contact Contact;
typedef struct ListNode ListNode;
typedef struct List List;
typedef struct BSTNode BSTNode;
typedef struct BST BST;
typedef int (*Comparator)(Contact, Contact);

typedef enum KeyType
{
    SURNAME, BIRTHDATE, EMAIL, PHONE
} KeyType;

struct Contact{
    int xD;
    char name[20];
    char surname[20];
    char email[30];
    char phoneNumber[10];
    // date format YYYY-MM-DD
    char birthDate[10];
    char address[30];
};

struct ListNode
{
    Contact value;
    ListNode* next;
    ListNode* prev;
};

struct List
{
    ListNode* head;
    ListNode* tail;
    int elementCounter;
};

struct BSTNode
{
    Contact value;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
};

struct BST
{
    BSTNode* root;
    int elementCounter;
    Comparator comparator;
};

List *createList(); //
void deleteList(List*);// 
void addToList(List*, Contact);//
ListNode *createListNode(Contact); //
void printList(List*);//
ListNode* findContactInList(List*, Contact);
void removeFromList(List*, ListNode*);
void forEachInList(List*);
List* sortList(List*, KeyType);

BST *createBST(Comparator);//
void deleteBST(BST*);//
void addToBST(BST*, Contact);
BSTNode *createBSTNode(Contact);//
void printBST(BST*);
BSTNode* findContactInBST(BST*, Contact);
void removeFromBST(BST*, BSTNode*);
void forEachInBST(BST*);
BST* sortBST(BST*, KeyType);

List* _sortList(List*, Comparator);
BST* _sortBST(BST*, Comparator);

void _deleteBST(BSTNode*);//
void _deleteList(ListNode*);//

void _addToList(List* list, ListNode* node);//
void _addToBST(BST* tree, BSTNode* node);

bool contactsEqual(Contact, Contact);

#endif