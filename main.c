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

//listkey, linekey
// listkey = pointer na list, ma value char expl 'U'
//line key = value = char , pointer na *line value = key aka 'u' etc
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
/*
bool read_words(list *list,char *line){
    const char s[2] = " ";
    char *token;
    token = strtok(line, s);
    while (token != NULL){
        if(!check_word(token)){

            fprintf(stderr,"wrong word");
            // not sure if the program should end
            return false;
        }
    }
    return true;
}
 */

int main(int argc, char **argv) {
    printf("num of args is %d \n", argc);
    if (argv[1] == NULL){
        return 2;
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
                add_item(&dictionary.S,line);
                printf("set is %s \n", dictionary.S.top->value);
                break;
            case 'R':
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
    destroy_list(&dictionary.C);
    destroy_list(&dictionary.R);
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
