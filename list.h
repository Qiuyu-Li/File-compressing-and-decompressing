#include "DictNode.h"

typedef struct ListElmt_{
    DNode *node;
    struct ListElmt_ *next;
    struct ListElmt_ *prev;
}ListElmt;

typedef struct List_{
    int size;
    ListElmt *head, *tail;
}List;

void list_append(List* list, const char* word);
void list_destroy(List* head);