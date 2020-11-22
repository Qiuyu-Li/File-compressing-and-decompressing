#include "DictNode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

int read(char* filename, char* words[NWORDS], int* size){
    FILE* file = fopen(filename, "r");
    if(!file){
        printf("File doesn't exist.\n");
        return -1;
    }

    char* line = NULL;      // Store each line as a string
    size_t words_num = 0;   // params for getline
    int j = 0, k = 0, linenum = 1;       // track current character in a word, and current word in the words array
    while(getline(&line, &words_num, file) > 0){
        char c[15];
        for (int i = 0; i < strlen(line); i++){
            if (k > NWORDS){
                printf("Warning: file too long. Now get to word %s in line %d\n",words[k-1],linenum);
                return -1;
            }
            if (line[i] == ' '){
                c[j] = '\0';
                words[k] = malloc((strlen(c)+1)*sizeof(char));
                strcpy(words[k],c);
                k++;
                j = 0;
            }
            else if (line[0] == '\n' || line[0] == '\r'){
                c[j] = '\0';
                words[k] = malloc((strlen(c)+1)*sizeof(char));
                strcpy(words[k],c);
                words[k+1] = malloc(2*sizeof(char));
                words[k+1][0] = line[0];
                words[k+1][1] = '\0';
                k += 2;
                i++;
                j = 0;
            }
            else if (line[i] == ',' || line[i] == '.'){
                c[j] = '\0';
                words[k] = malloc((strlen(c)+1)*sizeof(char));
                strcpy(words[k],c);
                words[k+1] = malloc(2*sizeof(char));
                words[k+1][0] = line[i];
                words[k+1][1] = '\0';
                k += 2;
                i++;
                j = 0;
            }
            else{
                c[j] = line[i];
                j++;
            }
        }
        linenum++;
    }
    (*size) = k;
    free(line);
    fclose(file);
    return 0;
}

void compressed_print(char* filename, char** compressed, int size){
    FILE* file = fopen(filename,"w");
    for(int i = 0; i < size; i++){
        if(strlen(compressed[i]) >= 0)
            fprintf(file,"%s ",compressed[i]);
        else{
            printf("HERE\n");
            break;
        }
    }
    fclose(file);
}

void compare_text(char* file1, char* file2){

    FILE* fp1 = fopen(file1,"r");
    FILE* fp2 = fopen(file2,"r");

    char ch1 = getc(fp1); 
    char ch2 = getc(fp2); 

    int error = 0, pos = 0, line = 1; 

    while (ch1 != EOF && ch2 != EOF){ 
        pos++; 
  
        if ((ch1 == '\n' || ch1 == '\r') && (ch2 == '\n' || ch2 == '\r')){ 
            printf("Newline.\n");
            line++; 
            pos = 0; 
        } 
  
        if (ch1 != ch2){
            if((ch1 == '\n' || ch1 == '\r' || ch1 == ' ') && (ch2 == '\n' || ch2 == '\r' || ch2 == ' ')){
                while(ch1 == ' ' || ch1 == '\n' || ch1 == '\r')
                ch1 = getc(fp1);
                while(ch2 == ' ' || ch2 == '\n' || ch2 == '\r')
                ch2 = getc(fp2);
            }
            else{
                error++; 
                printf("Line Number : %d; Error Position : %d, where the character in file 1 is '%c', in file 2 is '%c'\n", line, pos, ch1, ch2);
            }
        } 
 
        ch1 = getc(fp1); 
        ch2 = getc(fp2); 
    } 
    printf("Total Errors : %d\n", error); 
    fclose(fp1);
    fclose(fp2);
}

struct timeval timerStart;

void StartTimer(){
  gettimeofday(&timerStart, NULL);
}

double GetTimer(){
  struct timeval timerStop, timerElapsed;
  gettimeofday(&timerStop, NULL);
  timersub(&timerStop, &timerStart, &timerElapsed);

  return timerElapsed.tv_sec*1000.0+timerElapsed.tv_usec/1000.0;
}