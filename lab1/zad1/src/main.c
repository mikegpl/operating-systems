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

int main(int argc, char*argv[]){
    List* list = createList();
    int contactNumber = atoi(argv[1]);
    int tmp;
    for(int i = 0; i < contactNumber; i++){
        scanf("%d", &tmp);
        addToList(list, newContact(tmp));
    }

    printList(list);

    ListNode* test = findContactInList(list, newContact(5));
    if(test != NULL){
        printf("%d\n", test->value.xD);
    }

    free(list);
    return 0;
}