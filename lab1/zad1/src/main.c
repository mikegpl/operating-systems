#include "libcontact.h"
#include <stdlib.h>

int main(){
    List* list = createList();
    free(list);
    return 0;
}