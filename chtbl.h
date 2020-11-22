#include "list.h"

typedef struct CHTbl_{
    int nbins;
    int size;
    List* table;
    unsigned int (*hash)(const char* word, int nbins);
}CHTbl;

CHTbl* chtbl_compress(char** words, char** compressed, int size, int labbr, int num_buckets, unsigned int (*hash)(const char *word, int nbins));
void dict_print(char* filename, CHTbl* htbl);
char* chtbl_search_key(CHTbl* htbl, int key);
void chtbl_decompress(char* filename, char** compressed, int size, CHTbl* htbl);
float chtbl_cluster_coeff(CHTbl *htbl);
void chtbl_destroy(CHTbl* htbl, int nbins);