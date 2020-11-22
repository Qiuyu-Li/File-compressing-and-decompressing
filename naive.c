#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "naive.h"

LNode* dict_lookup(LNode* head, char* word){
    if(head == NULL)
        return NULL;
    for(LNode* curr = head; curr != NULL; curr = curr->next){
        if(strcmp(curr->node->word,word) == 0)
            return curr;
    }
    return NULL;
}

LNode* naive_compress(char** words, char** compressed, int size, int labbr){
    int abbr = 1;   // position in the dict array that stores the current word.
    int lword;
    LNode *head = NULL, *curr, *new, *p;    // *p store the key if found in dict. 
    for(int i = 0; i < size; i++){
        if(!words[i])
            break;
        lword = strlen(words[i]);
        if (lword >= 4){
            compressed[i] = malloc(labbr*sizeof(char));
            p = dict_lookup(head,words[i]);
            if(p){
                p->node->count++;
                sprintf(compressed[i],"%d",p->node->key);
            }
            else{
                new = malloc(sizeof(LNode));
                new->node = malloc(sizeof(DNode));
                strcpy(new->node->word,words[i]);
                new->node->count = 1;
                new->node->key = abbr;
                new->next = NULL;
                if(!head){
                    head = new;
                    curr = head;
                }
                else{
                    curr->next = new;
                    curr = curr->next;
                }
                sprintf(compressed[i],"%d",curr->node->key);
                abbr++;
            }
        }
        else{
            compressed[i] = malloc((lword+1)*sizeof(char));
            strcpy(compressed[i],words[i]);
        }
    }
    return head;
}

void dict_print(char* filename, LNode* head){
    FILE* file = fopen(filename, "w");
    fprintf(file,"           word  |occur| abbr\n--------------------------------------\n");
    for(LNode* curr = head; curr != NULL; curr = curr->next){
        fprintf(file,"%15s  |  %d  | %d\n",curr->node->word,curr->node->count,curr->node->key);
    }
    fclose(file);
}

char* dict_search_key(LNode* head, int key){
    for(LNode* curr = head; curr != NULL; curr = curr->next)
        if(curr->node->key == key)
            return curr->node->word;
    return NULL;
}

void naive_decompress(char* filename, char** compressed, int size, LNode* dict){
    FILE* file = fopen(filename,"w");
    char *dword, *word;
    int key;
    for(int i = 0; i < size; i++){
        if(!compressed[i])
            break;
        dword = compressed[i];
        if(key = atoi(dword))
            dword = dict_search_key(dict,key);
        if(i + 1 < size && ((strcmp(compressed[i+1],",\0")==0) || (strcmp(compressed[i+1],".\0")==0)))
            fprintf(file,"%s",dword);
        else if(compressed[i][0] == '\r'|| compressed[i][0] == '\n')
            fprintf(file,"\r\r");
        else
            fprintf(file,"%s ",dword);
    }
    fclose(file);
}

void dict_destroy(LNode* head){
    LNode *curr, *next;
    for(curr = head; curr != NULL; curr = next){
        next = curr->next;
        free(curr->node);
        free(curr);
    }
}