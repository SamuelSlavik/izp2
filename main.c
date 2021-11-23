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
//checks if the word is correct (to True/False or numbers)
// rewrite it if necessary, only prototype
bool check_word(char *token){
    for(size_t i = 0; i < strlen(token);i++){
        if (! ((token[i] >='a' && token[i] <='z') || (token[i] >='A' && token[i] <='Z') || token[i] == '\n' || token[i] == '\0') ){
            return false;
        }
    }
    return true;
}

bool parse_words(char *line){
    int counter = 0;
    unsigned long len = strlen(line);
    char *help_string = malloc(sizeof (char) * len);
    strcpy(help_string,line);
    const char delim[2] = " ";
    char *token;

    token = strtok(help_string, delim);

    while( token != NULL ) {
        counter++;
        printf( "word is %s\n", token );

        token = strtok(NULL, delim);
    }
    strcpy(help_string,line);
    char *array[counter];

    int i = 0;
    array[i] = (token = strtok(help_string, delim));
    printf("end me %s \n", array[i]);
    while( token != NULL ) {
        i++;
        token = strtok(NULL, delim);
        array[i] = token;
        printf("end me %s \n", array[i]);
    }

    free(help_string);
    return true;
}
/*
bool check_duplicates(char *line, int size){
    parse_words(line) ;
}
*/
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
    list universum =  create_list();
    Dict dictionary = {NULL,create_list(), create_list(), create_list()};
    list data =  create_list();
    bool is_universum = false;
    fp = fopen(argv[1], "r");
    if (fp == NULL) return 2;
    while ((getline(&line, &len, fp) != -1)){
        add_item(&data, line);
        switch (line[0]) {
            case 'U':
                if (!is_universum) {
                    parse_words(line);
                    if(!check_line(line)){
                        fprintf(stderr,"Wrong format");
                        return 2;
                    }
                    is_universum = true;
                    dictionary.U = line;
                    printf("dic is working %s\n",dictionary.U);
                }
                else{
                    fprintf(stderr,"More than 1 universum");
                    return 2;
                }
                break;
            case 'S':
                if(!check_line(line)){
                    fprintf(stderr,"Wrong format");
                    return 2;
                }
                add_item(&dictionary.S,line);
                printf("set is %s \n", dictionary.S.top->value);
                break;
            case 'R':
                if(!check_line(line)){
                    fprintf(stderr,"Wrong format");
                    return 2;
                }
                add_item(&dictionary.R,line);
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
    destroy_list(&universum);
    destroy_list(&dictionary.S);
    destroy_list(&dictionary.R);
    destroy_list(&dictionary.C);
    destroy_list(&data);
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
