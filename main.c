#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"

typedef struct list_item {
    struct list_item *next;
    char *value;

} list_item;

typedef struct list {
    list_item *top;
    int size;
} list;

typedef struct {
    char *U;
    list R;
    list S;
    list C;
}Dict;

list create_list(){
    list list = {NULL, 0};
    return list;
}

bool list_find(list *list, char *needle){
    list_item  *item = list->top;
    while (item != NULL) {
        if (item->value == needle) {
            return true;
        }
        item = item->next;
    }
    return false;
}

void add_item(list *list, char *value){
    list_item *item = malloc(sizeof(list_item) + 1);
    item->value = value;
    //printf("line is %s\n", item->value);
    list_item *current = list->top;
    if (current == NULL){
        list->top = item;
        list->size++;
        item->next = NULL;
        return;
    }
    item->next = current;
    list->top = item;
    list->size++;
}

void destroy_list(list *list){
    list_item *current = list->top;
    while(current != NULL) {
        list_item  *next = current->next;
        free(current);
        current = next;
    }
}
bool check_duplicates(char *substring, char *line){
    int count = 0;
    const char *tmp = line;
    tmp = strstr(tmp, substring);
    while(tmp)
    {
        count++;
        tmp++;
        tmp = strstr(tmp, substring);
    }
    if(count != 1){
        return false;
    }
    return true;
}

bool parse_words(char *line){
    int counter = 0;
    unsigned long len = strlen(line);
    char *help_string = malloc(sizeof (char) * len);
    strcpy(help_string,line);
    const char delim[4] = " \n()";
    char *token;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        counter++;
        printf( "word is %s\n", token );
        token = strtok(NULL, delim);
    }
    char *array[counter];
    strcpy(help_string,line);
    int i = 0;
    array[i] = (token = strtok(help_string, delim));
    //printf("end me %s \n", array[i]);
    while( token != NULL ) {
        i++;
        token = strtok(NULL, delim);
        array[i] = token;
        //printf("word is %s\n", array[i]);
    }
    for(int j = 1; j < counter-1;j++) {
        if( !check_duplicates(array[j], line)){
            fprintf(stderr,"duplicit words");
            return false;
        }
    }

    free(help_string);
    return true;
}

bool check_set_with_uni(Dict dictionary){
    char *help_string = malloc(sizeof (char) * strlen(dictionary.S.top->value));
    strcpy(help_string,dictionary.S.top->value);
    const char delim[4] = " \n()";
    char *token;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        if((strstr(dictionary.U, token) == NULL)){
            fprintf(stderr,"Word is not in universum");
            free(help_string);
            return false;
        }
    }
    printf("sdasd %s \n",dictionary.S.top->value);
    free(help_string);
    return true;
}

bool check_rel_with_uni(Dict dictionary){
    char *help_string = malloc(sizeof (char) * strlen(dictionary.R.top->value));
    strcpy(help_string,dictionary.R.top->value);
    const char delim[4] = " \n()";
    char *token;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        if((strstr(dictionary.U, token) == NULL)){
            fprintf(stderr,"Word is not in universum");
            free(help_string);
            return false;
        }
    }
    printf("sdasd %s \n",dictionary.R.top->value);
    free(help_string);
    return true;
}

// edge case what if peachtrue ???
bool check_line(char *line){
    for (size_t i = 0; i < sizeof (line); i++) {
        if(line[i] >= '0' && line[i]<='9'){
            return false;
        }
    }
    if (strstr(line, "true")|| strstr(line, "false")) return false;
    return true;
}

int main(int argc, char **argv) {
    printf("num of args is %d \n", argc);
    if (argv[1] == NULL){
        return 5;
    }
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    Dict dictionary = {NULL,create_list(), create_list(), create_list()};
    bool is_universum = false;
    bool error = false;
    fp = fopen(argv[1], "r");
    char *help_string;
    if (fp == NULL) return 2;
    while ((getline(&line, &len, fp) != -1) && error==false){
        switch (line[0]) {
            case 'U':
                if (!is_universum) {
                    if(!parse_words(line)){error = true;break;}
                    if(!check_line(line)){
                        fprintf(stderr,"Wrong format");
                        error = true;
                        break;
                    }
                    is_universum = true;
                    help_string = malloc(sizeof (char) * strlen(line));
                    strcpy(help_string,line);
                    dictionary.U = help_string;
                    printf("dic is working %s\n",dictionary.U);
                }
                else{
                    fprintf(stderr,"More than 1 universum");
                    error = true;
                }
                break;
            case 'S':
                if(!parse_words(line)){error = true;break;}
                if(!check_line(line)){
                    fprintf(stderr,"Wrong format");
                    error = true;
                    break;
                }
                add_item(&dictionary.S,line);
                printf("set is %s \n", dictionary.S.top->value);
                if(!check_set_with_uni(dictionary)){
                    error = true;
                    break;
                }
                break;
            case 'R':
                if(!check_line(line)){
                    fprintf(stderr,"Wrong format");
                    error = true;
                    break;
                }
                add_item(&dictionary.R,line);
                if(!check_rel_with_uni(dictionary)){ error = true;break;}
                printf("relation is %s \n", dictionary.R.top->value);
                break;
            case 'C':
                add_item(&dictionary.C,line);
                printf("operation is %s \n", dictionary.C.top->value);
                break;
            default:
                fprintf(stderr,"wrong identifier");
                return 2;


        }
    }

    fclose(fp);
    if(line) free(line);
    if (help_string) free(help_string);
    destroy_list(&dictionary.S);
    destroy_list(&dictionary.R);
    destroy_list(&dictionary.C);
    if(error)return 2;
    return 0;
}

/*
char *readline (FILE *fp){
    size_t capacity  = 0u;
    size_t length = 0u;
    size_t chunk_size = 64u;
    size_t offset = 0u;
    char *memory = NULL;
    char *tmp;
    do {
        capacity+=chunnk_size;
        if((tmp = realpath(memory, capacity))) == NULL){
            free(memory);
            return NULL;
        }
        memory = tmp;
        tmp = memory + length - offset;
        if(fgets(tmp, chunk_size+offset, file) == NULL) break;
        offset = 1;
        length += chunk_size;
    }
    while (strchr(tmp, '\n') == NULL );
    if (length == 0u || ferror(fp)){
        free(memory);
        return NULL;
    }
    return memory;
}
*/
