// Change parameters here
#define NWORDS 371107
#define WORD_SIZE 15
#define ABBR_SIZE 8
#define datafile "test_document.txt"
#define dict_file "dict.txt"
#define compressed_file "compressed.txt"
#define output_file "decompressed.txt"

// Hash params:
#define HASH_FUN (FNV_hash)    // can be naive_hash or bernstein_hash or FNV_hash
#define NBINS NWORDS
#define MULTIPLIER 10    // I want to store the abbr as number_of_bucket * MULTIPLIER + number_of_element_in_the_bucket. 

typedef struct dict_node{
    int key;
    int count;
    char word[WORD_SIZE];
}DNode;

int read(char* filename, char* words[NWORDS], int* size);
void compressed_print(char* filename, char** compressed, int size);
void compare_text(char* file1, char* file2);
void StartTimer();
double GetTimer();
