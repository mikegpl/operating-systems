#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <contact.h>


const int contacts_number = 1000;
const int read_buffer_size = 256;
const char *read_delimiters = ",\n";

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
        contacts[i] = malloc(sizeof(Contact));
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
    Contact **contacts;
    BST *treeBook;
    List *listBook;
    deleteContacts(contacts);
    return 0;
}