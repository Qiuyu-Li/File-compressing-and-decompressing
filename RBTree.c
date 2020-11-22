#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RBTree.h"

void left_rotation(TNode* n){
    TNode* tmp_node = n->left;
    DNode* tmp_dnode = n->node;
    n->left = n->right;
    n->node = n->right->node;
    n->right = n->left->right;
    n->left->right = n->left->left;
    n->left->left = tmp_node;
    n->left->node = tmp_dnode;

    if(n->left->left)
        n->left->left->parent = n->left;
    if(n->right)
        n->right->parent = n;
}

void right_rotation(TNode* n){
    TNode* tmp_node = n->right;
    DNode* tmp_key = n->node;
    n->right = n->left;
    n->left = n->right->left;
    n->node = n->right->node;
    n->right->left = n->right->right;
    n->right->right = tmp_node;
    n->right->node = tmp_key;

    if(n->left)
        n->left->parent = n;
    if(n->right->right)
        n->right->right->parent = n;  
}

TNode* find_brother(TNode* n){
    TNode* brother;
    if(n->parent->left == n)
        brother = n->parent->right;
    else
        brother = n->parent->left;
    return brother;
}

void adjust_case3(TNode* n){
    TNode* parent = n->parent;
    if(!parent)
        n->color = 'B';
    else if(parent->color == 'B'){
        return;
    }
    else{
        TNode* uncle = find_brother(parent);
        parent->color = 'B';
        uncle->color = 'B';
        parent->parent->color = 'R';
        adjust_case3(parent->parent);
    }
}

void adjust(TNode* n){
    TNode* parent = n->parent;
    if (!parent){
        n->color = 'B';
        return;
    }
    else if(parent->color == 'B')
        return;
    else{
        TNode *uncle = find_brother(parent);
        if(uncle){
            if(uncle->color == 'R')
                adjust_case3(n);
                return;
        }
        if(n == parent->right & parent == parent->parent->left){
            left_rotation(parent);
        }
        else if(n == parent->left & parent == parent->parent->right){
            right_rotation(parent);
        }
        if(n == parent->left & parent == parent->parent->left){
            right_rotation(parent->parent);
            parent = n->parent;
            parent->color = 'B';
            if(parent->right)
                parent->right->color = 'R';
        }
        else{
            left_rotation(parent->parent);
            parent = n->parent;
            parent->color = 'B';
            if(parent->left)
                parent->left->color = 'R';
        }
    }

}

// Assume no equal keys.
TNode* addnode(TNode* root, char* word){
    TNode *t = malloc(sizeof(TNode));
    t->node = malloc(sizeof(DNode));
    t->left = t->right = NULL;
    strcpy(t->node->word,word);
    t->node->count = 1;
    t->color = 'R';
    TNode* crawl = root;
    if(!root){
        t->parent = NULL;
        t->color = 'B';
        return t;
    }
    while(1){
        if(strcmp(crawl->node->word,word) > 0){
            if(!crawl->left){
                crawl->left = t;
                t->parent = crawl;
                adjust(t);
                break;
            }
            else
                crawl = crawl->left;
        }
        else{
            if(!crawl->right){
                crawl->right = t;
                t->parent = crawl;
                adjust(t);
                break;
            }
            else
                crawl = crawl->right;
        }    
    }
    return root;
}

TNode* tree_lookup(TNode* root, char* word){
    if(!root)
        return NULL;
    int compar_rst = strcmp(root->node->word, word);
    if(compar_rst == 0)
        return root;
    else if (compar_rst > 0)
        return tree_lookup(root->left,word);
    else
        return tree_lookup(root->right,word);
}

void assign_key(TNode* root,int* key){
    if(root){
        assign_key(root->left,key);
        root->node->key = *key;
        (*key)++;
        assign_key(root->right,key);
    }
}

// O[Nlog(N) + N + Nlog(N)]: (1) Traverse the words array to create the dictionary according to the alphabet order. (2)Generate sorted keys. (3) Traverse the array again and add long words to the compressed array.
TNode* tree_compress(char** words, char** compressed, int size, int labbr){

    // (1) Create the tree
    int lword;
    TNode *root = NULL, *new, *p;
    for(int i = 0; i < size; i++){
        lword = strlen(words[i]);
        if(lword >= 4){
            p = tree_lookup(root,words[i]);
            if(p){
                p->node->count++;
            }
            else{
                root = addnode(root,words[i]);
            }
        }
    }
    
    // (2) Generate sorted keys
    int key = 1;
    assign_key(root,&key);

    // (3) Generate the compressed array
    for(int i = 0; i < size; i++){
        lword = strlen(words[i]);
        if(lword >= 4){
            compressed[i] = malloc(labbr*sizeof(char));
            p = tree_lookup(root,words[i]);
            sprintf(compressed[i],"%d",p->node->key);
        }
        else{
            compressed[i] = malloc((lword+1)*sizeof(char));
            strcpy(compressed[i],words[i]);
        }
    }
    return root;
}

void print_tree(TNode* root){
    if(root){
        print_tree(root->left);
        printf("word: %15s  | occurance: %d  |  abbr: %d\n",root->node->word, root->node->count, root->node->key);
        print_tree(root->right);
    }
}

char* tree_search_key(TNode* root, int key){
    if(root == NULL)
        return NULL;
    int curr_key = root->node->key;
    if(curr_key == key)
        return root->node->word;
    else if (curr_key > key)
        return tree_search_key(root->left,key);
    else
        return tree_search_key(root->right,key);
}

void tree_decompress(char* filename, char** compressed, int size, TNode* root){
    FILE* file = fopen(filename,"w");
    char *dword, *word;
    int key;
    for(int i = 0; i < size; i++){
        dword = compressed[i];
        if(key = atoi(dword)){
            dword = tree_search_key(root,key);
        }
        if(i + 1 < size && ((strcmp(compressed[i+1],",\0")==0) || (strcmp(compressed[i+1],".\0")==0)))
            fprintf(file,"%s",dword);
        else if(compressed[i][0] == '\r'|| compressed[i][0] == '\n')
            fprintf(file,"\n\n");
        else
            fprintf(file,"%s ",dword);
    }
    fclose(file);
}

void tree_destroy(TNode* root){
    if(root->left)
        tree_destroy(root->left);
    if(root->right)
        tree_destroy(root->right);
    free(root->node);
    free(root);
}