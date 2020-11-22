#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naive.h"

int main(){

    // Read in the file
    int size;   // total number of words in the original file.
    char* words[NWORDS];    // stores the words of the original file.
    read(datafile,words,&size); // read in the word, and calculate the total number of words.
    
    // Compress & decompress
    StartTimer();
    char* cmp[NWORDS];  // the array of compressed words, i.e. abbreviations of words of 4 or more characters; and original form of shorter words.
    LNode* dict = naive_compress(words,cmp,size,ABBR_SIZE); // compressing using the word array (as defined above), stores compressed words in the cmp array, and return the dictionary, which is a singlely linked list. 
    compressed_print(compressed_file, cmp, size);   //print the compressed file.
    const double time_compress = GetTimer() / 1000.0;

    dict_print(dict_file,dict);

    StartTimer();
    naive_decompress(output_file,cmp,size,dict);    //print the decompressed file using the compressed words array and the dictionary.
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
    dict_destroy(dict);
}
