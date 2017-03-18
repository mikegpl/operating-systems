// contact.h

#ifndef CONTACT_H
#define CONTACT_H

typedef int bool;
#define true 1
#define false 0

typedef struct Contact Contact;
typedef struct ListNode ListNode;
typedef struct List List;
typedef struct BSTNode BSTNode;
typedef struct BST BST;
typedef int (*Comparator)(Contact, Contact);
typedef void (*ListNodeOperation)(ListNode*);
typedef void (*BSTNodeOperation)(BSTNode*);


typedef enum KeyType
{
    SURNAME, BIRTHDATE, EMAIL, PHONE
} KeyType;

struct Contact{
    char* name;
    char* surname;
    char* email;
    char* phoneNumber;
    // date format YYYY-MM-DD
    char* birthDate;
    char* address;
};

struct ListNode
{
    Contact* value;
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
    KeyType keyType;
    Comparator comparator;
};


// ---------------------------- Linked List ----------------------------
List* List_newList(); 
void List_delete(List*);
void List_addContact(List*, Contact*);
ListNode* ListNode_newNode(Contact*); 
void List_print(List*);
ListNode* List_findContact(List*, Contact*);
bool List_removeContact(List*, Contact*);
void List_forEach(List*, ListNodeOperation);
void List_sort(List*, KeyType);

void _List_delete(ListNode*);
void _List_addNode(List*, ListNode*);
void _List_sort(List*, Comparator);
void _ListNode_print(ListNode*);
ListNode* _ListNode_getLast(ListNode*);
ListNode* _ListNode_quickSort(ListNode*, ListNode*, Comparator);
ListNode* _ListNode_append(ListNode*, ListNode*);


// ----------------------------     BST     ----------------------------
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

// ----------------------------    Other    ----------------------------
Contact* Contact_new();
void Contact_delete(Contact*);
bool Contact_equals(Contact*, Contact*);
int Comparator_surname(Contact*, Contact*);
int Comparator_birthDate(Contact*, Contact*);
int Comparator_email(Contact*, Contact*);
int Comparator_phone(Contact*, Contact*);
#endif