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


typedef char **array;
//////////////////////////////////////////////////////////////////////////
typedef struct {
    int num_of_words;
    array array;
}Parsed_line_t;

Parsed_line_t parse_words(char *line){
    Parsed_line_t parsed_line = {0, NULL};
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
    parsed_line.num_of_words = counter;
    strcpy(help_string,line);
    char** array = malloc(sizeof (int) * counter);
    int i = 0;
    //array[0] = (token = strtok(help_string, delim));
    //printf("end me %s \n", array[i]);
    token = strtok(help_string, delim);
    while( i < counter ) {
        array[i] = token;
        printf("end me %s \n", array[i]);
        i++;
        token = strtok(NULL, delim);
    }
    parsed_line.array = array;
    free(help_string);
    free(array);
    return parsed_line;
}
void destroy_array(char **array){
    free(*array);
    free(array);
}

bool check_duplicates(char *line){
    Parsed_line_t parsed_line = parse_words(line) ;
    for(int i = 0; i < parsed_line.num_of_words;i++) {
        printf("parsed line is %s \n", parsed_line.array[i]);
    }
    free(parsed_line.array);
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
                    check_duplicates(line);
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