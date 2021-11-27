#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"

typedef struct {
    char *universum;
}Universum;

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
        token = strtok(NULL, delim);
    }
    char *array[counter];
    strcpy(help_string,line);
    int i = 0;
    array[i] = (token = strtok(help_string, delim));
    while( token != NULL ) {
        i++;
        token = strtok(NULL, delim);
        array[i] = token;
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

bool check_set_with_uni(char *line,Universum universum){
    char *help_string = malloc(sizeof (char) * strlen(line));
    strcpy(help_string,line);
    const char delim[4] = " \n";
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
    free(help_string);
    return true;
}

bool check_rel_with_uni(char *line,Universum universum){
    char *help_string = malloc(sizeof (char) * strlen(line));
    strcpy(help_string,line);
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
    printf("correct\n");
    free(help_string);
    return true;
}

bool is_number(char * str){
    int len = (int) strlen(str);
    for(int i =0 ; i< len;i++){
        if(!(str[i]>='0' && str[i] <= '9')){
            fprintf(stderr, "wrong command");
            return false;
        }
    }
    return true;
}

int check_command_arg(char *line){
    char *help_string = malloc(sizeof (char) * strlen(line));
    strcpy(help_string,line);
    const char delim[4] = " \n";
    char *token;
    token = strtok(help_string, delim);
    int counter = 1;
    int number;
    char *ptr;
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        counter++;
        if (counter>3){
            fprintf(stderr,"wrong command");
            return 0;
        }
        if(counter==3){
            if(!is_number(token)) return false;
            else number=(int) strtol(token, &ptr,10);
        }
    }
    free(help_string);
    return number;
}

bool card(char *line,char **data){
    int index = check_command_arg(line);
    if(index == 0){
        return false;
    }
    char * string = malloc(sizeof (char)* strlen(data[index]));
            string = strcpy(string,data[index]);
    const char delim[4] = " \n";
    char *token;
    token = strtok(string, delim);
    int counter = 0;
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        counter++;
    }
    printf("\nnumber of words is %d\n", counter);
    free(string);
}
bool empty(char *line, char **data){
    int index = check_command_arg(line);
    if(index == 0){
        return false;
    }
    const char delim[4] = " \n";
    char * string = malloc(sizeof (char)* strlen(data[index]));
    string = strcpy(string,data[index]);
    char *token;
    token = strtok(string, delim);
    token = strtok(NULL, delim);
    if (token == NULL)printf("false\n");
    else printf("true\n");
    free(string);
    return true;
}
bool complement(char *line, char **data) {
    int index = check_command_arg(line);
    if(index == 0){
        return false;
    }
    const char delim[4] = " \n";
    char *universum = malloc(sizeof (char) * strlen(data[1]));
    strcpy(universum,data[1]);
    char * string = malloc(sizeof (char)* strlen(data[index]));
    string = strcpy(string,data[index]);
    char * token;
    char *ptr;
    token = strtok(universum, delim);
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        ptr = strstr(string, token);
        if (ptr == NULL){
            printf("word is %s \n", token);
        }

    }
    free(universum);
    free(string);
    return  true;
}
void union_f(char *line){ printf("works");}
void intersect(char *line){ printf("works");}
void minus(char *line){ printf("works");}
void subseteq(char *line){ printf("works");}
void subset(char *line){ printf("works");}
void equals(char *line){ printf("works");}
void reflexive(char *line){ printf("works");}
void symmetric(char *line){ printf("works");}
void antisymmetric(char *line){ printf("works");}
void transitive(char *line){ printf("works");}
void function(char *line){ printf("works");}
void domain(char *line){ printf("works");}
void codomain(char *line){ printf("works");}
void injective(char *line){ printf("works");}
void surjetive(char *line){ printf("works");}
void bijective(char *line){ printf("works");}
//bonus
void closure_ref(char *line){ printf("works");}
void closure_sym(char *line){ printf("works");}
void closure_trans(char *line){ printf("works");}
void select_random(char *line){ printf("works");}

bool check_commands(char *line,char **data, Universum universum){
    char *help_string = malloc(sizeof (char) * strlen(line));
    strcpy(help_string,line);
    const char delim[4] = " ";
    char *token;
    token = strtok(help_string, delim);
    token = strtok(NULL, delim);
    if (token == NULL) {
        free(help_string);
        return false;
    }
    if (!strcmp(token,"empty")){
        empty(line,data);
    }
    else if (!strcmp(token,"card")){
        card(line,data);
    }
    else if (!strcmp(token,"complement")){
        complement(line,data);
    }
    else if (!strcmp(token,"union")) {
        union_f( line);// union is already defined function in C
    }
    else if (!strcmp(token,"intersect")) {
        intersect(line);
    }
    else if (!strcmp(token,"minus")) {
        minus(line);
    }
    else if (!strcmp(token,"subseteq")) {
        subseteq(line);
    }
    else if (!strcmp(token,"subset")) {
        subset(line);
    }
    else if (!strcmp(token,"equals")) {
        equals(line);
    }
    else if (!strcmp(token,"reflexive")) {
        reflexive(line);
    }
    else if (!strcmp(token,"symmetric")) {
        symmetric(line);
    }
    else if (!strcmp(token,"antisymmetric")) {
        antisymmetric(line);
    }
    else if (!strcmp(token,"transitive")) {
        transitive(line);
    }
    else if (!strcmp(token,"function")) {
        function(line);
    }
    else if (!strcmp(token,"domain")) {
        domain(line);
    }
    else if (!strcmp(token,"codomain")) {
        codomain(line);
    }
    else if (!strcmp(token,"injective")) {
        injective(line);
    }
    else if (!strcmp(token,"surjective")) {
        surjetive(line);
    }
    else if (!strcmp(token,"bijective")) {
        bijective(line);
    }
        //bonus functions
    else if (!strcmp(token,"closure_sym")) {
        closure_sym(line);
    }
    else if (!strcmp(token,"closure_ref")) {
        closure_ref(line);
    }
    else if (!strcmp(token,"closure_trans")) {
        closure_trans(line);
    }
    else if (!strcmp(token,"select")) {
        select_random(line);//can not use select, already defined C function
    }
    else{
        fprintf(stderr, "wrong command");
        free(help_string);
        return false;
    }
    free(help_string);
    return true;
}

bool check_document(FILE *fp,char  **argv, Universum universum){
    char *line = NULL;
    size_t len = 0;
    bool is_universum = false;
    bool error = false;
    int counter = 0;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr,"File is empty");
        return 2;}
    char **data = NULL;
    int index = 1;
    while ((getline(&line, &len, fp) != -1) && error == false){
        data = (char **) realloc(data, (index + 1) * sizeof(*data));
        data[index] = (char *)malloc(sizeof(char)*  strlen(line));
        strcpy(data[index], line);
        index++;
        counter++;
        if(counter> 1000){
            error = false;
        }
        switch (line[0]) {
            case 'U':
                if (!is_universum) {
                    if(!check_words(line)){ error = true;break;}
                    universum.universum = malloc(sizeof (char)* strlen(line));
                    strcpy(universum.universum,line);
                    is_universum = true;
                }
                else{
                    fprintf(stderr,"More than 1 universum");
                    error = true;
                }
                break;
            case 'S':
                if(!check_words(line)){ error = true;break;}
                if(!check_set_with_uni(line,universum)){
                    error = true;
                    break;
                }
                break;
            case 'R':
                if(!check_rel_with_uni(line,universum)){ error = true;break;}
                break;
            case 'C':
                if(!check_commands(line,data,universum)){
                    break;
                }
                break;
            default:
                fprintf(stderr,"wrong identifier");
                return 2;
        }
    }
    free(universum.universum);
    fclose(fp);
    free(line);
    for(int j = 0; j <counter+1;j++){
        free(data[j]);
    }
    free(data);
    if(error) return false;
    return true;
}

int main(int argc, char **argv) {
    printf("%d aby werror nejebal ze argc not used\n\n",argc);
    if (argv[1] == NULL){
        fprintf(stderr,"wrong argument");
        return 2;
    }
    FILE *fp;
    Universum universum;
    check_document(fp,argv,universum);
    return 0;
}
