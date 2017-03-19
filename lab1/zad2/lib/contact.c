#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "contact.h"

static const size_t KEY_LEN = 35;

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
