#include "chtbl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void chtbl_init(CHTbl *htbl, int nbins, unsigned int (*hash)(const char* word, int nbins)){
    htbl->table = malloc(nbins * sizeof(List));
    htbl->nbins  = nbins;
    for(int i = 0; i < htbl->nbins; i++){
        htbl->table[i].head = NULL;
        htbl->table[i].tail = NULL;
        htbl->table[i].size = 0;
    }
    htbl->hash = hash;
    htbl->size = 0;
}

ListElmt* chtbl_lookup(const CHTbl *htbl, const char* word, int* key){
    ListElmt *element;
    int bucket = htbl->hash(word,htbl->nbins);
    *key = bucket * MULTIPLIER;
    for(element = htbl->table[bucket].head; element != NULL; element = element->next){
        (*key)++;
        if(strcmp(element->node->word,word) == 0)
            return element;
    }
    return NULL;
}

int chtbl_insert(CHTbl* htbl, const char* word){
    int bucket = htbl->hash(word,htbl->nbins);
    list_append(&htbl->table[bucket],word);
    htbl->size++;
    return (bucket * MULTIPLIER + htbl->table[bucket].size);
}

CHTbl* chtbl_compress(char** words, char** compressed, int size, int labbr, int num_buckets, unsigned int (*hash)(const char* word, int nbins)){
    CHTbl *htbl = malloc(sizeof(CHTbl));
    chtbl_init(htbl, num_buckets, hash);
    int key = 0;
    int lword;
    ListElmt *p;
    for(int i = 0; i < size; i++){
        lword = strlen(words[i]);
        if(lword >= 4){
            compressed[i] = malloc(labbr*sizeof(char));
            p = chtbl_lookup(htbl,words[i],&key);
            if(p){
                p->node->count++;
                sprintf(compressed[i],"%d",key);
            }
            else{
                key = chtbl_insert(htbl, words[i]);
                sprintf(compressed[i],"%d",key);
            }
        }
        else{
            compressed[i] = malloc((lword+1)*sizeof(char));
            strcpy(compressed[i],words[i]);
        }
    }
    return htbl;
}

void dict_print(char* filename, CHTbl* htbl){
    FILE* file = fopen(filename,"w");
    ListElmt *curr;
    int key,j;
    for(int i = 0; i < htbl->nbins; i++){
        curr = htbl->table[i].head;
        fprintf(file,"Bin %d:\n",i);
        j = 1;
        while(curr){
            key = i * MULTIPLIER + j;
            fprintf(file,"word: %15s  | occur: %d  | abbr: %d\n",curr->node->word, curr->node->count, key);
            curr = curr->next;
            j++;
        }
        fprintf(file,"---------------------\n");
    }
    fclose(file);
}

char* chtbl_search_key(CHTbl* htbl, int key){
    int bucket = key / MULTIPLIER;
    int n = key % MULTIPLIER;
    ListElmt *curr = htbl->table[bucket].head;
    int i = 1;
    while(i < n){
        curr = curr->next;
        i++;
    }
    return curr->node->word;
}

void chtbl_decompress(char* filename, char** compressed, int size, CHTbl* htbl){
    FILE* file = fopen(filename,"w");
    char* dword;
    int key;
    for(int i = 0; i < size; i++){
        dword = compressed[i];
        if(key = atoi(dword))
            dword = chtbl_search_key(htbl,key);
        if(i + 1 < size && ((strcmp(compressed[i+1],",\0")==0) || (strcmp(compressed[i+1],".\0")==0)))
            fprintf(file,"%s",dword);
        else if(compressed[i][0] == '\r'|| compressed[i][0] == '\n')
            fprintf(file,"\r\r");
        else
            fprintf(file,"%s ",dword);
    }
    fclose(file);
}

float chtbl_cluster_coeff(CHTbl *htbl){
    int m = htbl->nbins;
    int n = htbl->size;
    int term = 0;
    for(int i = 0; i < m; i++){
        List *bucket_list = &(htbl->table[i]);
        int sz = bucket_list->size;
        term += sz*sz;
    }
    float c = ((float)m/(float)(n-1)) * ((float)term/(float)(n-1));
    return c;
}

void chtbl_destroy(CHTbl* htbl, int nbins){
    for(int i = 0; i < nbins; i++){
        list_destroy(&htbl->table[i]);
    }
    free(htbl->table);
    free(htbl);
}