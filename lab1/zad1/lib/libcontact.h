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
typedef void (*ListNodeOperation)(ListNode*);

typedef enum KeyType
{
    SURNAME, BIRTHDATE, EMAIL, PHONE, XD
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


// ---------------------------- Linked List ----------------------------
List* List_newList(); 
void List_delete(List*);
void List_addContact(List*, Contact);
ListNode* ListNode_newNode(Contact); 
void List_print(List*);
ListNode* List_findContact(List*, Contact);
bool List_removeContact(List*, Contact);
void List_forEach(List*, ListNodeOperation);
void List_sort(List*, KeyType);

void _List_delete(ListNode*);
void _List_addNode(List*, ListNode*);
void _List_sort(List*, Comparator);
ListNode* _ListNode_getLast(ListNode*);
ListNode* _ListNode_quickerSort(ListNode*, ListNode*, Comparator);
ListNode* _ListNode_append(ListNode*, ListNode*);


// ----------------------------     BST     ----------------------------
BST* BST_newBST(Comparator);
void BST_delete(BST*);
void BST_addContact(BST*, Contact);
BSTNode*BSTNode_newNode(Contact);
void BST_print(BST*);
BSTNode * BST_findContact(BST*, Contact);
bool BST_removeContact(BST*, Contact);
void BST_forEach(BST*);
BST* BST_sort(BST*, KeyType);


void _BST_delete(BSTNode*);
void _BST_addNode(BST*, BSTNode*);
BST* _BST_sort(BST*, Comparator);


// ----------------------------    Other    ----------------------------
bool Contact_equals(Contact, Contact);
int Comparator_surname(Contact, Contact);
int Comparator_birthDate(Contact, Contact);
int Comparator_email(Contact, Contact);
int Comparator_phone(Contact, Contact);
int Comparator_xD(Contact, Contact);
#endif