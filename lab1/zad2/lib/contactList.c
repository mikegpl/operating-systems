#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "contactList.h"

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