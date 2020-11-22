#include "list.h"
#include <stdlib.h>
#include <string.h>

void list_append(List* list, const char* word){
    ListElmt *curr = malloc(sizeof(ListElmt));
    curr->node = malloc(sizeof(DNode));
    strcpy(curr->node->word,word);
    curr->node->count = 1;
    curr->next = NULL;
    if(list->head == NULL){
        list->head = curr;
        curr->prev = NULL;
    }
    else{
        list->tail->next = curr;
        curr->prev = list->tail;
    }
    list->tail = curr;
    list->size++;
}

void list_destroy(List* list){
    ListElmt *curr, *next;
    for(curr = list->head; curr != NULL; curr = next){
        next = curr->next;
        free(curr->node);
        free(curr);
    }
}
