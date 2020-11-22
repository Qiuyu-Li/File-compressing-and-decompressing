#include "DictNode.h"

typedef struct tnode_{
    DNode* node;
    char color;
    struct tnode_ *left, *right, *parent;
}TNode;

TNode* addnode(TNode* root, char* word);
TNode* tree_lookup(TNode* root, char* word);
TNode* tree_compress(char** words, char** compressed, int size, int labbr);
void print_tree(TNode* root);
char* tree_search_key(TNode* root, int key);
void tree_decompress(char* filename, char** compressed, int size, TNode* root);
void tree_destroy(TNode* root);