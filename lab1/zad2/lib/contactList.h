#ifndef CONTACTLIST_H
#define CONTACTLIST_H

typedef struct ListNode ListNode;
typedef struct List List;
typedef void (*ListNodeOperation)(ListNode*);

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

#endif