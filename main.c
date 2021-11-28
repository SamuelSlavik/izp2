#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"

typedef struct {
    char *universum;
}Universum;

typedef struct {
    int count;
}Word_count;

typedef struct {
    int first;
    int second;
}Args;

//parses words into 2d struct, use with struct that stores the number of words, having 2d array is pain in struct...
char **parse_words(char *string,Word_count *w){
    char *help_string = malloc(sizeof (char) * strlen(string));
    strcpy(help_string,string);
    char **data = NULL;
    const char delim[4] = " \n";
    char *token;
    int counter = 0;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        data = (char **) realloc(data, (counter + 1) * sizeof(data));
        data[counter] = (char *)malloc(sizeof(char)*  strlen(token));
        strcpy(data[counter], token);
        counter++;

    }
    w->count = counter;
    free(help_string);
    if (data == NULL) return 0;
    return data;
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

bool find_args(char *string,Args *arguments){
    char * help_str = malloc(sizeof (char) * strlen(string));
    if(help_str == NULL) return false;
    strcpy(help_str,string);
    char * token;
    const char delim[2] = " \n";
    int counter = 0;
    char *ptr;
    token = strtok(help_str, delim);
    bool error = false;
    while (token != NULL && error==false) {
        if (counter == 2){
            if(!is_number(token)) error = true;
            arguments->first = (int)strtol(token,&ptr,10);
        }
        if (counter == 3){
            if(!is_number(token)) error = true;
            arguments->second = (int)strtol(token,&ptr,10);
        }
        if (counter > 3) error = true;
        token = strtok(NULL, delim);
        counter++;
    }
    free(help_str);
    if (error)return  false;
    return true;
}

void free_words(char **data,Word_count w){
    for(int i = 0; i <w.count;i++){
        free(data[i]);
    }
    free(data);
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

bool card(char **data,Args argumnts){
    char * string = malloc(sizeof (char)* strlen(data[argumnts.first]));
    string = strcpy(string,data[argumnts.first]);
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
    return true;

}
bool empty(char **data, Args arguments){
    const char delim[4] = " \n";
    char * string = malloc(sizeof (char)* strlen(data[arguments.first]));
    string = strcpy(string,data[arguments.first]);
    char *token;
    token = strtok(string, delim);
    token = strtok(NULL, delim);
    if (token == NULL)printf("false\n");
    else printf("true\n");
    free(string);
    return true;
}
bool complement(char **data, Args arguments) {
    const char delim[4] = " \n";
    char *universum = malloc(sizeof (char) * strlen(data[1]));
    strcpy(universum,data[1]);
    char * string = malloc(sizeof (char)* strlen(data[arguments.first]));
    string = strcpy(string,data[arguments.first]);
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

bool union_f(char *line,char **data, Args arguments){
    //add fction like check_command_arg but cheks 2 not 1
    const char delim[2] = " \n";
    char *token;
    //copies the line with command to string for strtok since it changes the string and need the line intact
    char * string = malloc(sizeof (char)* strlen(line));
    string = strcpy(string,line);
    Word_count first_w; //to store how many words sets have
    char *firt_set;
    char *sec_set;
    //**************************************************************
    firt_set = malloc(sizeof (char) * strlen(data[arguments.first]));
    if (firt_set == NULL){
        fprintf(stderr,"error in allocating commands");
        return false;
    }
    sec_set = malloc(sizeof (char) * strlen(data[arguments.second]));
    if (sec_set == NULL){
        fprintf(stderr,"error in allocating commands");
        return false;
    }
    //****************************************************************
    strcpy(firt_set,data[arguments.first]);
    strcpy(sec_set,data[arguments.second]);
    char **first_words= NULL;
    first_words = parse_words(firt_set,&first_w);
    Word_count sec_w;
    char **second_words = NULL;
    //parse 1st set and compare it to second
    //if not in 2nd, print, of ye, nothing
    //then, print whole second
    char *h_s = malloc(sizeof (char ) * strlen(firt_set));
    strcpy(h_s,firt_set);
    token = strtok(h_s,delim);
    printf("\nunion is: \n\n");
    while (token != NULL){
        if(strstr(sec_set, token) == NULL) printf("%s ", token);
        token = strtok(NULL, delim);
    }
    second_words = parse_words(sec_set,&sec_w);
    for (int i = 0; i < sec_w.count; i++) {
        printf("%s ", second_words[i]);
    }
    free_words(first_words,first_w);
    free_words(second_words,sec_w);
    //need to arse this cuz S ... ffs we need parse fction; printf("%s",sec_set);
    free(h_s);
    free(firt_set);
    free(sec_set);
    free(string);
    return true;
}
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
    //using help string because strtok changes the string it is using
    //and need line intact for further use
    char *help_string = malloc(sizeof (char) * strlen(line));
    strcpy(help_string,line);
    const char delim[4] = " ";
    Args arguments;
    if (!find_args(help_string,&arguments)){
        free(help_string);
        return false;
    }
    char *token;
    token = strtok(help_string, delim);
    token = strtok(NULL, delim);
    if (token == NULL) {
        free(help_string);
        return false;
    }
    if (!strcmp(token,"empty")){
        empty(data, arguments);
    }
    else if (!strcmp(token,"card")){
        card(data,arguments);
    }
    else if (!strcmp(token,"complement")){
        complement(data, arguments);
    }
    else if (!strcmp(token,"union")) {
        (union_f(line,data, arguments));
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

void print_results(char ** data, int counter){
    //print only U,S and R
    //then call a function that has results of operations
    for (int i = 0; i <counter;i++){
        //if (data[i][0] == 'C') {
        //printf("line %s \n", data[i]);
        //}
    }
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
        //allocs memory for data and load the current line
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
                    //probably unnecessary, w/e
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
                if(!check_set_with_uni(line,universum)){//checks if elements are in universum
                    error = true;
                    break;
                }
                break;
            case 'R':
                if(!check_rel_with_uni(line,universum)){ error = true;break;}//checks if elements are in universum
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
    if(!error){
        print_results(data,counter);
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
