#ifdef DYNAMIC_LOADING

#include <dlfcn.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "contactBook.h"
#include "measureTime.h"


const int contacts_number = 10000;
const int read_buffer_size = 256;
const char *read_delimiters = ",\n";

static Contact* (*pContact_new)();
static Contact* (*pContact_copy)(Contact*);
static void (*pContact_delete)(Contact*);

static BST* (*pBST_newBST)(KeyType);
static void (*pBST_addContact)(BST*, Contact*);
static BST* (*pBST_sort)(BST*, KeyType);
static bool (*pBST_removeContact)(BST*, Contact*);
static BSTNode* (*p_BSTNode_findMin)(BSTNode*);
static BSTNode* (*pBST_findContact)(BST*, Contact*);
static void (*pBST_delete)(BST*);

static List* (*pList_newList)();
static void (*pList_addContact)(List*, Contact*);
static void (*pList_sort)(List*, KeyType);
static bool (*pList_removeContact)(List*, Contact*);
static ListNode* (*pList_findContact)(List*, Contact*);
static void (*pList_delete)(List*);

/**********************Dynamic loading************************/
static void *loadSymbols() {
#ifdef DYNAMIC_LOADING
    void *symbols = dlopen("libcontactBook.so", RTLD_LAZY);

    if (symbols == NULL) {
        fprintf(stderr, "Error opening library contactBook: %s", dlerror());
        exit(1);
    }

    pContact_new = dlsym(symbols, "Contact_new");
    pContact_copy = dlsym(symbols, "Contact_copy");
    pContact_delete = dlsym(symbols, "Contact_delete");

    pBST_newBST = dlsym(symbols, "BST_newBST");
    pBST_addContact = dlsym(symbols, "BST_addContact");
    pBST_sort = dlsym(symbols, "BST_sort");
    pBST_removeContact = dlsym(symbols, "BST_removeContact");
    p_BSTNode_findMin = dlsym(symbols, "_BSTNode_findMin");
    pBST_findContact = dlsym(symbols, "BST_findContact");
    pBST_delete = dlsym(symbols, "BST_delete");

    pList_newList = dlsym(symbols, "List_newList");
    pList_addContact = dlsym(symbols, "List_addContact");
    pList_sort = dlsym(symbols, "List_sort");
    pList_removeContact = dlsym(symbols, "List_removeContact");
    pList_findContact = dlsym(symbols, "List_findContact");
    pList_delete = dlsym(symbols, "List_delete");

    return symbols;
#else

    pContact_new = Contact_new;
    pContact_copy = Contact_copy;
    pContact_delete = Contact_delete;

    pBST_newBST = BST_newBST;
    pBST_addContact = BST_addContact;
    pBST_sort = BST_sort;
    pBST_removeContact = BST_removeContact;
    p_BSTNode_findMin = _BSTNode_findMin;
    pBST_findContact = BST_findContact;
    pBST_delete = BST_delete;
    
    pList_newList = List_newList;
    pList_addContact = List_addContact;
    pList_sort = List_sort;
    pList_removeContact = List_removeContact;
    pList_findContact = List_findContact;
    pList_delete = List_delete;
    return NULL;
#endif
}

/*************************************************************/

static void parseContact(char *raw, Contact *parsed) {
    strcpy(parsed->name, strtok(raw, read_delimiters));
    strcpy(parsed->surname, strtok(NULL, read_delimiters));
    strcpy(parsed->birthDate, strtok(NULL, read_delimiters));
    strcpy(parsed->email, strtok(NULL, read_delimiters));
    strcpy(parsed->phoneNumber, strtok(NULL, read_delimiters));
    strcpy(parsed->address, strtok(NULL, read_delimiters));
}

static void loadContacts(Contact **contacts) {
    char *buffer = malloc(read_buffer_size);
    FILE *data = fopen("data.csv", "r");
    for (int i = 0; i < contacts_number; ++i) {
        fgets(buffer, read_buffer_size, data);
        contacts[i] = (*pContact_new)();
        parseContact(buffer, contacts[i]);
    }

    fclose(data);
    free(buffer);
}

static void deleteContacts(Contact **contacts) {
    for (int i = 0; i < contacts_number; ++i) {
        free(contacts[i]);
    }
    free(contacts);
}

int main(void) {
#ifdef DYNAMIC_LOADING
    void *symbols = loadSymbols();
#else
    loadSymbols();
#endif

    Contact **contacts;
    BST *treeBook;
    List *listBook;

    /* ------------------------Test procedures---------------------- */
    void _loadContacts(){
        contacts = calloc(contacts_number, sizeof(Contact *));
        loadContacts(contacts);
    }

    void _createTreeBook(){
        treeBook = (*pBST_newBST)(BIRTHDATE);
    }

    void _createListBook(){
        listBook = (*pList_newList)();
    }

    void _addFirstTree(){
        (*pBST_addContact)(treeBook, (*pContact_copy)(contacts[0]));
    }

    void _addFirstList(){
        (*pList_addContact)(listBook, (*pContact_copy)(contacts[0]));
    }

    void _addContactsTree(){
        for(int i = 1; i < contacts_number; i++)
            (*pBST_addContact)(treeBook, (*pContact_copy)(contacts[i]));
    }

    void _addContactsList(){
        for(int i = 1; i < contacts_number; i++)
            (*pList_addContact)(listBook, (*pContact_copy)(contacts[i]));
    }

    void _sortTreeBook(){
        treeBook = (*pBST_sort)(treeBook, EMAIL);
    }

    void _sortListBook(){
        (*pList_sort)(listBook, EMAIL);
    }

    void _deleteContactOptTree(){
        (*pBST_removeContact)(treeBook, treeBook->root->value);
    }

    void _deleteContactOptList(){
        (*pList_removeContact)(listBook, listBook->head->next->value);
    }

    void _deleteContactPesTree(){
        BSTNode* minContact = (*p_BSTNode_findMin)(treeBook->root);
        (*pBST_removeContact)(treeBook, minContact->value);
    }

    void _deleteContactPesList(){
        (*pList_removeContact)(listBook, listBook->tail->prev->value);
    }

    void _findContactOptTree(){
        (*pBST_findContact)(treeBook, treeBook->root->value);
    }

    void _findContactOptList(){
        (*pList_findContact)(listBook, listBook->head->next->value);
    }

    void _findContactPesTree(){
        BSTNode* minContact = (*p_BSTNode_findMin)(treeBook->root);
        (*pBST_findContact)(treeBook, minContact->value);
    }

    void _findContactPesList(){
        (*pList_findContact)(listBook, listBook->tail->prev->value);
    }

    void _deleteTreeBook(){
        (*pBST_delete)(treeBook);
    }

    void _deleteListBook(){
        (*pList_delete)(listBook);
    }

    /* ------------------------------------------------------------- */

    measureTime(_loadContacts, "Alloc space for and load contacts from .csv");
    measureTime(_createTreeBook, "Create BST book");
    measureTime(_createListBook, "Create list book");
    measureTime(_addFirstTree, "Add first element to BST book");
    measureTime(_addFirstList, "Add first element to list book");

    ExecTime avgTree = measureTime(_addContactsTree, "Add all remaining contacts to BST book");
    ExecTime avgList = measureTime(_addContactsList, "Add all remaining contats to List book");
    
    avgTree.user /= contacts_number - 1;
    avgTree.sys /= contacts_number - 1;
    avgTree.real /= contacts_number - 1;
    printTime(avgTree, "Average time of adding contact to BST book");
    
    avgList.user /= contacts_number - 1;
    avgList.sys /= contacts_number - 1;
    avgList.real /= contacts_number - 1;
    printTime(avgList, "Average time of adding contact to list book");

    measureTime(_sortTreeBook, "Sort contacts in BST book");
    measureTime(_sortListBook, "Sort contacts in list book");
    measureTime(_deleteContactOptTree, "Delete contact in BST book (optimistic case)");
    measureTime(_deleteContactOptList, "Delete contact in list book (optimistic case)");
    measureTime(_deleteContactPesTree, "Delete contact in BST book (pessimistic case)");
    measureTime(_deleteContactPesList, "Delete contact in list book (pessimistic case)");
    measureTime(_findContactOptTree, "Find contact in BST book (optimistic case)");
    measureTime(_findContactOptList, "Find contact in list book (optimistic case)");
    measureTime(_findContactPesTree, "Find contact in BST book (pessimistic case)");
    measureTime(_findContactPesList, "Find contact in list book (pessimistic case)");
    measureTime(_deleteTreeBook, "Delete BST book");
    measureTime(_deleteListBook, "Delete list book");

    deleteContacts(contacts);
    return 0;
}