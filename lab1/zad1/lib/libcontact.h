// libcontact.h

#ifndef CONTACT_H
#define CONTACT_H

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
};

struct Contact{
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
    KeyType type;
};

List *createList();
void *removeList(List*);
void addToList(List*, Contact);
void removeFromList(List*, ListNode*);
ListNode* findContactInList(List*);
void forEachInList(List*);
List* sortList(List*, KeyType);

BST *createBST();
void *removeBST(BST*);
void addToBST(BST*, Contact);
void removeFromBST(BST*, BSTNode*);
BSTNode* findContactInBST(BST*);
void forEachInBST(BST*);
BST* sortBST(BST*, KeyType);

List* _sortList(List*, Comparator);
BST* _sortBST(BST*, Comparator);


#endif