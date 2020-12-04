# File compressing and decompressing

## Content

1. Introduction

2. Data structures and implementations

3. How to compile

4. Accuracy test

5. Sample output

6. Discussions

## Introduction

This project is created for compressing and decompressing the given text file using three approaches, or three types of dictionaries: naive list, BST(Red-Black Tree) and chained hash table.

### Description of the question

Imagine we have a large document with many repeated long strings. A simple way to compress the file is to replace frequently occurring words with a simple abbreviation. For example in the text:

>But good morning! Good morning to ye and thou! I’d say to all my patients, because I was the worst of the hypocrites, of all the hypocrites, the cruel and phony hypocrites, I was the very worst.

If we create a simple rule, e.g., that all words with at least four characters and one or more occurrence are encoded, then we might encode according the following abbreviation table:

| word        | occurrences | byte code |
|-------------|-------------|-----------|
| hyprocrites |      3      |     1     |
| morning     |      2      |     2     |
| good        |      2      |     3     |
| worst       |      2      |     4     |

and thus the encoded document becomes:

>But 3 2! 3 2 to ye and thou! I’d say to all my patients, because I was the 4 of the 1, of all the 1, the cruel and phony 1, I was the very 4.

Of course, the compressed file would also have to encode the translation table so that it could be uncompressed.

### Introduction of the project

I create three `test_*.c` (test_naive.c, test_tree.c, test_chtbl.c) files for each of the approaches, and related data structures to achive the purpose. If successfully run, each of the test program will produce below output:

1. dict.txt: a dictionary text file that prints the dictionary table(the occurance times and abbreviation of each word). The BST dictionary is printed in the terminal instead of a text file. The hash dictionary prints words contained in each bin (I recommend not to open it).

2. compressed.txt: the compressed file. Words are written in exactly the same order in the original file, except that words with 4 characters or more are written as abbreviates. (Whitespaces and newlines may also be different)

3. decompressed.txt: decompressed file from the compressed array (the array that contains each word in the compressed file). It should be the same with the original file.

4. the time costs for compressing and decompressing, printed in the terminal.

5. the result of the Accuracy test (see the Accuracy test part).

6. the cluster coefficient for the hash table, printed in the terminal.

## Data structures and implementations

1. **DictNode:** the DictNode structure is shared by all three implementations, mainly used for building the dictionaries. Each of the DictNode stores a word, its occurences times, and its abbreviation(key). The abbreviation is stored as an integer.

2. **Three data structures** are built for each of the approach: (1) singlely linked list (naive.c, naive.h) for test_naive.c; (2) Red-Black tree (RBTree.c, RBTree.h) for test_tree.c; (3) Chained hash table (chtbl.c, chtbl.h) for test_chtbl.c (see details in Disscussion)

3. **Implement procedures:** Each of the test_*.c is implemented following the same procedure:

    (1) read: The original text file is readed into an array of words. Total number of words are calculated.

    (2) compress: For each of the word, calculate its length. If it contains 4 or more characters, add it into the dictionary, and writes it into the compressed array in the abbreviation form. Otherwise, it will not be added into the dictionary, and is stored in the compressed array in its original form. The function needs the total words numbers, and the maximum length of the abbreviations. It returns the head/root of dictionary list/tree, or the hash table.

    (3) print the `compressed.txt` (see Introduction for details).

    (4) print `dict.txt` (see Introduction for details).

    (5) decompress: take the compressed array, words total number, and the dictionary as parameters, and print the `decompressed.txt` (see Introduction for details).

    (6) print the time costs for compressing (2-3) and decompressing (5)

    (7) accuracy test

## How to compile

### STEP 1: Set parameters (not necessary if the test file is "test_document.txt")

All the parameters are listed in `DictNode.h`， where the 1st to 7th are shared by all three implementations while the 8th to 10th are exclusively for the hash implementation:

1. NWORDS: the maximum number of words to read in.Note: must be equal to or larger than the total number of words in the original file.

2. WORD_SIZE: the maximum length of a word in the original file. It is used to create the dictionary.

3. ABBR_SIZE: the maximum length of the abbreviation.

4. datafile: the file that contains the original text. The default file name is "test_document.txt".

5. dictfile: the file that is used to write the dictionary table. The default file name is "dict.txt". The program will automatically create one if it does not exist.

6. compressed_file: the compressed file, as described above. The default file name is "compressed.txt". The program will automatically create one if it does not exist.

7. output_file: the file that is used to write the decompressed file, which should be almost or exactly the same as the original file.

8. HASH_FUN: the hash function you want to use for creating the hash table. It can be naive_hash , bernstein_hash or FNV_hash.

9. NBINS: number of buckets in the hash table. As the assignment description required, it is set to be the total number of the words (i.e. NWORDS). Note: since it is allowed to set NWORDS larger than the actual size of the file, NBINS may not necessarily equal to the actual size of the file. This will also affect the calculation of hash table cluster coefficint.

10. MULTIPLIER: to make searches convenient, I set abbreviations to be `umber_of_bucket * MULTIPLIER + number_of_element_in_the_list`. Note: you may want to set it according to the total number of words and the total number of buckets.

### STEP2: How to compile

```
$make
$./test_naive
$./test_tree
$./test_chtbl
```

## Accuracy test

The accuracy test check the equivalence of the decompressed file and the original file character by character. Only one kind of error is tolerated:
Character 'a' from file 1 and character 'b' from file 2 at the position i are different, but both of them are one of " ", "\n", or "\r". In that case, start comprasion from the first character next to them that is not one of the three kind.

## Sample output

### 1. Naive approach

```
Time for compressing the file: 0.723049; Time for decompressing the file: 0.352835
Total Errors : 0
```

### 2. Tree approach

```
......(lines of the printed dictionary table)
word:         vivamus  | occurance: 466  |  abbr: 294
word:         viverra  | occurance: 3043  |  abbr: 295
word:        volutpat  | occurance: 2265  |  abbr: 296
word:       vulputate  | occurance: 1637  |  abbr: 297
Time for compressing the file: 0.349510; Time for decompressing the file: 0.153236
Total Errors : 0
```

### 3. Hash approach

Naive hash:

```
Hash table cluster coefficient: 1757.777100
Time for compressing the file: 0.222521; Time for decompressing the file: 0.110612
Total Errors : 0
```

Bernstein hash:

```
Hash table cluster coefficient: 1257.975464
Time for compressing the file: 0.228439; Time for decompressing the file: 0.107315
Total Errors : 0
```

FNV hash:

```
Hash table cluster coefficient: 1257.975464
Time for compressing the file: 0.205343; Time for decompressing the file: 0.102251
Total Errors : 0
```

## Discussions

Performance table:

| Data structre  | Compressing time(s) | Decompressing time(s) | Relative comp. time | Relative decomp. time |
| ------------- | ------------- | ------------- | ------------- | ------------- |
| linked list  | 0.723049  | 0.352835  | 1 | 1 |
| Red-Black tree  | 0.349510  | 0.153236  | 0.48 | 0.43 |
| Chained hash table (naive hash)  | 0.222521  | 0.110612  | 0.31 | 0.31 |
| Chained hash table (Bernstein hash)  | 0.228439  | 0.107315 | 0.32 | 0.30 |
| Chained hash table (FNV hash)  | 0.205343  | 0.102251  | 0.28 | 0.29 |

1. Naive approach: both compressing and decompressing are O(N^2) process. As expected, it costs the most time for both compressing and decompressing.

2. Red-Black tree: there are actually two kind of searching: searching the dictionary node given the word (when compressing); searching the word given the abbreviation (when decompressing). Therefore, it is most convenient to have both the word and the abbreviations sorted. To achieve this, I first generate the tree with the words array(since words are sorted, it's O(Nlog(N))); then, I traverse the tree in order and assign the abbreviations (O(N)); finally, I generate the compressed array with the original words array and the completed tree O(Nlog(N)). Therefore, both compressing and decompressing are O(Nlog(N)). The results of the performance test are consistent with the theory.

3. Hash table: The compressing is nearly O(N), since each word can be located using hash function. However, in order to make the decompressing also a O(N) process, the words have to be located fast given the abbreviation value. Therefore, I set the abbreviations to be `number_of_bucket * MULTIPLIER + number_of_element_in_the_bucket`(default value of the MULTIPLIER is 10). As is expected, the hash tables takes the least time in both compressing and decompressing.

4. Comparing three hash functions: The Bernstein hash and the FNV hash have exactly the same cluster coefficient, which is smaller than that of the naive hash. Correspondingly, it takes less time for the former two to decompress. On the other hand, the compressing time costs for FNV hash is the least. Therefore, FNV hash turns out to be the most efficient among the tree.
