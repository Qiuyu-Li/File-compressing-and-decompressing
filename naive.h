#include "DictNode.h"

typedef struct list_node{
    DNode* node;
    struct list_node *next;
}LNode;

LNode* dict_lookup(LNode* head, char* word);
LNode* naive_compress(char** words, char** compressed, int size, int labbr);
void dict_print(char* filename,LNode* head);
char* dict_search_key(LNode* head, int key);
void naive_decompress(char* filename, char** compressed, int size, LNode* dict);
void dict_destroy(LNode* head);
