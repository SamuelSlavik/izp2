#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"

typedef struct{
    char * universum;
}uni;

uni init(uni u,char *line){
    u.universum = malloc(sizeof (char) * strlen(line));
    strcpy(u.universum,line);
    return u;
}

typedef struct list_item {
    struct list_item *next;
    char *value;
    int index;

} list_item;

typedef struct list {
    list_item *top;
    int size;
} list;

typedef struct {
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
    item->value = malloc(sizeof (char)* strlen(value));
    strcpy(item->value, value);
    if(list->top==NULL){
        item->index=0;
    }
    else item->index++;
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
        free(current->value);
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
bool check_words(char *line){
    if(!check_line(line)){
        fprintf(stderr,"Wrong format");
        return false;
    }
    int counter = 0;
    unsigned long len = strlen(line);
    char *help_string = malloc(sizeof (char) * len);
    strcpy(help_string,line);
    const char delim[4] = " \n()";
    char *token;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        counter++;
        //printf( "word is %s\n", token );
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

bool check_set_with_uni(Dict dictionary,uni universum){
    char *help_string = malloc(sizeof (char) * strlen(dictionary.S.top->value));
    strcpy(help_string,dictionary.S.top->value);
    const char delim[4] = " \n()";
    char *token;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        if((strstr(universum.universum, token) == NULL)){
            fprintf(stderr,"Word is not in universum");
            free(help_string);
            return false;
        }
    }
    //printf("sdasd %s \n",dictionary.S.top->value);
    free(help_string);
    return true;
}

bool check_rel_with_uni(Dict dictionary, char *line,uni universum){
    if(!check_line(line)){
        fprintf(stderr,"Wrong format");
        return false;
    }
    char *help_string = malloc(sizeof (char) * strlen(dictionary.R.top->value));
    strcpy(help_string,dictionary.R.top->value);
    const char delim[4] = " \n()";
    char *token;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        if((strstr(universum.universum, token) == NULL)){
            fprintf(stderr,"Word is not in universum");
            free(help_string);
            return false;
        }
    }
    //printf("sdasd %s \n",dictionary.R.top->value);
    free(help_string);
    return true;
}
// mby functions will be bool, return false if parsing goes wrong i.e. empty d or empty 4(out of index)
void empty(Dict dictionary,char *line){ printf("works");}
void card(Dict dictionary,char *line){ printf("works");}
void complement(Dict dictionary,char *line){ printf("works");}
void union_f(Dict dictionary,char *line){ printf("works");}
void intersect(Dict dictionary,char *line){ printf("works");}
void minus(Dict dictionary,char *line){ printf("works");}
void subseteq(Dict dictionary,char *line){ printf("works");}
void subset(Dict dictionary,char *line){ printf("works");}
void equals(Dict dictionary,char *line){ printf("works");}
void reflexive(Dict dictionary,char *line){ printf("works");}
void symmetric(Dict dictionary,char *line){ printf("works");}
void antisymmetric(Dict dictionary,char *line){ printf("works");}
void transitive(Dict dictionary,char *line){ printf("works");}
void function(Dict dictionary,char *line){ printf("works");}
void domain(Dict dictionary,char *line){ printf("works");}
void codomain(Dict dictionary,char *line){ printf("works");}
void injective(Dict dictionary,char *line){ printf("works");}
void surjetive(Dict dictionary,char *line){ printf("works");}
void bijective(Dict dictionary,char *line){ printf("works");}
//bonus
void closure_ref(Dict dictionary,char *line){ printf("works");}
void closure_sym(Dict dictionary,char *line){ printf("works");}
void closure_trans(Dict dictionary,char *line){ printf("works");}
void select_random(Dict dictionary,char *line){ printf("works");}

//NOTE: ONLY check the FIRST WORD (apart from C)
bool check_commands(Dict dictionary, char *line){
    char *help_string = malloc(sizeof (char) * strlen(dictionary.C.top->value));
    strcpy(help_string,dictionary.C.top->value);
    const char delim[4] = " ";
    char *token;
    token = strtok(help_string, delim);
    token = strtok(NULL, delim);
    if (token == NULL) {
        free(help_string);
        return false;
    }
    if (!strcmp(token,"empty")){
        empty(dictionary,line);
    }
    else if (!strcmp(token,"card")){
        card(dictionary,line);
    }
    else if (!strcmp(token,"complement")){
        complement(dictionary,line);
    }
    else if (!strcmp(token,"union")) {
        union_f(dictionary, line);// union is already defined function in C
    }
    else if (!strcmp(token,"intersect")) {
        intersect(dictionary, line);
    }
    else if (!strcmp(token,"minus")) {
        minus(dictionary, line);
    }
    else if (!strcmp(token,"subseteq")) {
        subseteq(dictionary, line);
    }
    else if (!strcmp(token,"subset")) {
        subset(dictionary, line);
    }
    else if (!strcmp(token,"equals")) {
        equals(dictionary, line);
    }
    else if (!strcmp(token,"reflexive")) {
        reflexive(dictionary, line);
    }
    else if (!strcmp(token,"symmetric")) {
        symmetric(dictionary, line);
    }
    else if (!strcmp(token,"antisymmetric")) {
        antisymmetric(dictionary, line);
    }
    else if (!strcmp(token,"transitive")) {
        transitive(dictionary, line);
    }
    else if (!strcmp(token,"function")) {
        function(dictionary, line);
    }
    else if (!strcmp(token,"domain")) {
        domain(dictionary, line);
    }
    else if (!strcmp(token,"codomain")) {
        codomain(dictionary, line);
    }
    else if (!strcmp(token,"injective")) {
        injective(dictionary, line);
    }
    else if (!strcmp(token,"surjective")) {
        surjetive(dictionary, line);
    }
    else if (!strcmp(token,"bijective")) {
        bijective(dictionary, line);
    }

        //bonus functions
    else if (!strcmp(token,"closure_sym")) {
        closure_sym(dictionary, line);
    }
    else if (!strcmp(token,"closure_ref")) {
        closure_ref(dictionary, line);
    }
    else if (!strcmp(token,"closure_trans")) {
        closure_trans(dictionary, line);
    }
    else if (!strcmp(token,"select")) {
        select_random(dictionary, line);//can not use select, already defined C function
    }
    else{
        fprintf(stderr, "wrong command");
        free(help_string);
        return false;
    }
    free(help_string);
    return true;
}

int main(int argc, char **argv) {
    printf("%d aby werror nejebal ze argc not used\n\n",argc);
    if (argv[1] == NULL){
        fprintf(stderr,"wrong argument");
        return 2;
    }
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    Dict dictionary = {create_list(), create_list(), create_list()};
    bool is_universum = false;
    bool error = false;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr,"File is empty");
        return 2;}
    uni univ;
    while ((getline(&line, &len, fp) != -1) && error == false){
        switch (line[0]) {
            case 'U':
                if (!is_universum) {
                    if(!check_words(line)){ error = true;break;}
                    is_universum = true;
                    univ  = init(univ,line);
                    printf("%s", univ.universum);
                }
                else{
                    fprintf(stderr,"More than 1 universum");
                    error = true;
                }
                break;
            case 'S':
                if(!check_words(line)){ error = true;break;}
                add_item(&dictionary.S, line);
                if(!check_set_with_uni(dictionary,univ)){
                    error = true;
                    break;
                }
                printf("%s \n", dictionary.S.top->value);
                break;
            case 'R':
                add_item(&dictionary.R, line);
                if(!check_rel_with_uni(dictionary, line,univ)){ error = true;break;}
                printf("%s \n", dictionary.R.top->value);
                break;
            case 'C':
                add_item(&dictionary.C, line);
                if(!check_commands(dictionary, line)){
                    break;
                }
                break;
            default:
                fprintf(stderr,"wrong identifier");
                return 2;


        }
    }
    //check how to access items in listf
    fclose(fp);
    free(line);
    free(univ.universum);
    destroy_list(&dictionary.S);
    destroy_list(&dictionary.R);
    destroy_list(&dictionary.C);
    if(error)return 2;
    return 0;
}
