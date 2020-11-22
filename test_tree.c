#include "RBTree.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

    // Read in the file
    int size;
    char* words[NWORDS];
    read(datafile,words, &size);
    
    // Compress & decompress
    StartTimer();
    char* cmp[NWORDS];
    TNode* dict = tree_compress(words,cmp,size,ABBR_SIZE);
    compressed_print(compressed_file, cmp, size);
    const double time_compress = GetTimer() / 1000.0;

    print_tree(dict);

    StartTimer();
    tree_decompress(output_file,cmp,size,dict);
    const double time_decompress = GetTimer() / 1000.0;

    printf("Time for compressing the file: %f; Time for decompressing the file: %f\n",time_compress,time_decompress);
    
    // Accuracy test
    compare_text(datafile,output_file);

    // Clear the memory
    for(int i = 0; i < size; i++){
        free(words[i]);
    }
    for(int i = 0; i < size; i++){
        free(cmp[i]);
    }
    tree_destroy(dict);
}