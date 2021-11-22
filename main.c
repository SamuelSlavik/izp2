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
void read_words(list *list,char *line){
    const char s[2] = " ";
    char *token;
    token = strtok(line, s);
    while (token != NULL){
        add_item(list, token);
        printf("%s\n", token);
        token = strtok(NULL, s);
    }
}

int main() {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    list universum =  create_list();

    fp = fopen("/home/grimm/Programming/C/setcal/file.txt","r");
    if (fp == NULL)return 2;
    while ((read = getline(&line, &len, fp) != -1)){
        printf("%s\n", line);
        read_words(&universum,line);
    }
    fclose(fp);
    if(line) free(line);
    /*
    add_item(&universum, "Apple");
    add_item(&universum, "Pear");
    add_item(&universum, "Chery");
     */
    destroy_list(&universum);
    return 0;
}
