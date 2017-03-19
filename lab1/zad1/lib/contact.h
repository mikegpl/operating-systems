// contact.h

#ifndef CONTACT_H
#define CONTACT_H

typedef int bool;
#define true 1
#define false 0

typedef struct Contact Contact;
typedef int (*Comparator)(Contact*, Contact*);

typedef enum KeyType
{
    SURNAME, BIRTHDATE, EMAIL, PHONE
} KeyType;

struct Contact{
    char* name;
    char* surname;
    char* email;
    char* phoneNumber;
    char* birthDate;
    char* address;
};

Contact* Contact_new();
Contact* Contact_copy(Contact*);
void Contact_delete(Contact*);
bool Contact_equals(Contact*, Contact*);
int Comparator_surname(Contact*, Contact*);
int Comparator_birthDate(Contact*, Contact*);
int Comparator_email(Contact*, Contact*);
int Comparator_phone(Contact*, Contact*);
#endif