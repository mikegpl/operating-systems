#include "libcontact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char end[] = "\0";

Contact newContact(int xD){
    Contact new;
    new.xD = xD;
    strcpy(new.name, end);
    strcpy(new.surname, end);
    strcpy(new.email, end);
    strcpy(new.phoneNumber, end);
    strcpy(new.birthDate, end);
    strcpy(new.address, end);
    return new;
}

void printNode(ListNode* node){
    node->value.xD *= 2;
    printf("%d\n", node->value.xD);
}

int main(int argc, char*argv[]){
    List* list = List_newList();
    int contactNumber = atoi(argv[1]);
    int tmp;
    for(int i = 0; i < contactNumber; i++){
        scanf("%d", &tmp);
        if(i == 2){
            Contact tContact = newContact(tmp);
            strcpy(tContact.name, "testo\0");
            List_addContact(list, tContact);
        }
        else{
            List_addContact(list, newContact(tmp));
        }

    }

    List_print(list);
    List_sort(list, XD);
    List_print(list);
    List_delete(list);
    return 0;
}