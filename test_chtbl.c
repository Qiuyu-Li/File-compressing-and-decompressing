#include "chtbl.h"
#include <stdlib.h>
#include <stdio.h>

unsigned int naive_hash(const char *word, int nbins);
unsigned int bernstein_hash(const char *word, int nbins);
unsigned int FNV_hash(const char *word, int nbins);

int main(){

    // Read in the file
    int size;
    char* words[NWORDS];
    read(datafile,words, &size);

    // Compress & decompress
    StartTimer();
    char* cmp[NWORDS];
    CHTbl* htbl = chtbl_compress(words,cmp,size,ABBR_SIZE, NBINS,HASH_FUN);
    compressed_print(compressed_file, cmp, size);
    const double time_compress = GetTimer() / 1000.0;

    dict_print(dict_file,htbl);

    StartTimer();
    chtbl_decompress(output_file,cmp,size,htbl);
    printf("Hash table cluster coefficient: %f\n",chtbl_cluster_coeff(htbl));
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
    chtbl_destroy(htbl,NBINS);
}

unsigned int naive_hash(const char *word, int nbins)
{
    unsigned int h = 0;
    int c;
    while(c = *word++)
        h += c;
    return h % nbins;
}

unsigned int bernstein_hash(const char *word, int nbins)
{
    unsigned int h = 5381;
    int c;
    while(c = *word++)
        h = 33 * h + c;
    return h % nbins;
}

unsigned int FNV_hash(const char *word, int nbins)
{
    unsigned long h = 14695981039346656037lu;
    char c;
    while(c = *word++)
    {
        h = h * 1099511628211lu;
        h = h ^ c;
    }
    return h % nbins;
}