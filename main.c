/**
 * IZP 2. projekt
 * This project was created by: xspace39, xvalko13, xslavi37, xpetra30
 * last date of change: 12/5/2021 23:02
 */

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
    int third;
}Args;

//parses words into 2d array
char **parse_words(char *string,Word_count *w){
    char *help_string = malloc(strlen(string)+1);//allocates memory for help string
    strcpy(help_string,string); // copies the passed string into help string
    // help string is necessary, because strtok would change the string, and we need the string unchanged therefore we use the help string instead
    char **data = NULL;
    const char delim[2] = " \n";
    char *token;
    int counter = 0; // counts the number of words created by strtok
    // strtok parses the string, searches for delim, when it finds it, it store the string between either start, or previous delim in token
    token = strtok(help_string, delim); // starts at the beginning
    while( token != NULL ) {
        token = strtok(NULL, delim);//starts at the current delim , also skipsthe first word, which is either U or S
        if (token == NULL) break;
        data = (char **) realloc(data, (counter + 1) * sizeof(data)); // allocs the number of columns of 2d array
        data[counter] = (char *)malloc(sizeof(char)*  strlen(token)+1); // allocs the current row
        strcpy(data[counter], token); // copies the token to current row
        counter++;
    }
    w->count = counter;
    free(help_string);
    if (data == NULL) return 0;
    return data;
}
// similar to parse_words, but has different delim and delim has to be const( will not work otherwise )
char **parse_relation(char *string,Word_count *w){
    char *help_string = malloc(sizeof (char) * strlen(string)+1);
    strcpy(help_string,string);
    char **data = NULL;
    const char delim[4] = " ()\n"; //() are specific for relations
    char *token;
    int counter = 0;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        data = (char **) realloc(data, (counter + 1) * sizeof(data));
        data[counter] = (char *)malloc(sizeof(char)*  strlen(token)+1);
        strcpy(data[counter], token);
        counter++;

    }
    w->count = counter;
    free(help_string);
    if (data == NULL) return 0;
    return data;
}

bool check_relation(char *string){
    char *help_string = malloc(sizeof (char) * strlen(string)+1);
    strcpy(help_string,string);
    const char delim[4] = " \n";
    char *token;
    int counter = 0;
    token = strtok(help_string, delim);
    while( token != NULL ) {
        token = strtok(NULL, delim); // skips the first word R
        if (token == NULL) break;
        if (counter%2 == 0){
            if (token[0] != '('){ // if even words do not have ( that means brackets do not match
                free(help_string);
                return false;
            }
        }
        if (counter%2 == 1){
            if(token[strlen(token)-1] != ')'){ //if odd words do not have ), it means brackets do not match
                free(help_string);
                return false;
            }
        }
        counter++;
    }
    free(help_string);
    return true;

}


bool is_number(char * str){
    int len = (int) strlen(str);
    for(int i =0 ; i< len;i++){
        if(!(str[i]>='0' && str[i] <= '9') && (str[i] != '\n')){ // if char is not a number or \n, returns false
            return false;
        }
    }
    return true;
}

bool find_args(char *string,Args *arguments){
    char * help_str = malloc(sizeof (char) * strlen(string)+1);
    if(help_str == NULL) return false;
    strcpy(help_str,string);
    char * token;
    const char delim[2] = " \n";
    int counter = 0;
    char *ptr;
    token = strtok(help_str, delim);
    while (token != NULL) {
        if (counter == 2){  //the second word is the first argument
            arguments->first = (int)strtol(token,&ptr,10);
        }
        if (counter == 3){ // the third word is the second argument
            arguments->second = (int)strtol(token,&ptr,10);
        }
        if (counter == 4){// the fourth word is the second argument
            arguments->third  = (int)strtol(token,&ptr,10);
        }
        token = strtok(NULL, delim);
        counter++;
    }
    free(help_str);
    return true;
}

void free_words(char **data,Word_count w){
    for(int i = 0; i <w.count;i++){ // frees all the rows
        free(data[i]);
    }
    free(data);// frees the whole array
}

bool duplicit(char *string){
    Word_count w;
    char *help_string = malloc(strlen(string)+1);
    strcpy(help_string,string);
    char **data = NULL;
    const char delim[2] = " \n";
    char *token;
    int counter = 0;
    token = strtok(help_string, delim);
    if (strlen(token) != 1){ // checks if the first word is only one letter to prevent cases like Ur
        fprintf(stderr,"wrong identifier");
        free(help_string);
        return false;
    }
    while( token != NULL ) {
        token = strtok(NULL, delim);
        if (token == NULL) break;
        if(strlen(token) > 30){//checks if the words if longer than 30 character (limit)
            w.count = counter;
            if (data != NULL){
                free_words(data,w);
            }
            free(help_string);
            fprintf(stderr,"element is too long");
            return false;
        }
        data = (char **) realloc(data, (counter + 1) * sizeof(data));
        data[counter] = (char *)malloc(sizeof(char)*  strlen(token)+1);
        strcpy(data[counter], token);
        counter++;
    }
    w.count = counter;
    if (data == NULL){ // if no data were stored, return true because there are no duplicit in empty array
        free(help_string);
        free_words(data,w);
        return true;
    }
    for(int i = 0; i <counter;i++){     //checks all words with each other, if were are duplicates, returns false
        for(int j = i+1;j<counter;j++){
            if(!strcmp(data[i],data[j])){
                free(help_string);
                free_words(data,w);
                return false;
            }
        }
    }
    free(help_string);
    free_words(data,w);
    return true;
}

bool check_line(char *line){
    size_t len = strlen(line); // finds the length of passed line
    for (size_t i = 0; i < len; i++) {
        if(line[i] > 'z') return false; // if ascii value is greater than value of z returns false
        else if(line[i] < 10)return false;// if ascii value is smaller than 10 returns false
        else if(line[i] > 'Z' && line[i] < 'a')return false; // basically just checks it line consists only of lower&uppercase letters , numbers, spaces and \n
        else if( line[i] > 32 && line[i] < 'A')return false;
        else if(line[i] > 10 && line[i] < 32)return false;
        else if(line[i] >= '0' && line[i]<='9')return false;
    }
    if (strstr(line, "true")|| strstr(line, "false")) return false; // checks for the presence of words true and false
    return true;
}
bool check_words(char *line){
    if(!check_line(line)){ // calls a function that checks for invalid characters
        fprintf(stderr,"Wrong format");
        return false;
    }
    if(!duplicit(line))return false; // calls a function that checks for duplicate words
    return true;
}
// checks if all elements of line  are in universum
bool check_set_with_uni(char *line,Universum universum){
    Word_count w;
    char **set_words = parse_words(line,&w);
    for (int i = 0; i < w.count;i++){ // for cycle checks all parsed word of line
        if((strstr(universum.universum, set_words[i]) == NULL)){ // if element is not in universum, returns false
            fprintf(stderr,"Word is not in universum");
            free_words(set_words,w);
            return false;
        }
    }
    free_words(set_words,w);
    return true;
}
// same as check_set_with_uni, difference is in different delim made specially for relation
bool check_rel_with_uni(char *line,Universum universum){
    char *help_string = malloc(sizeof (char) * strlen(line)+1);
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
    //printf("correct\n");
    free(help_string);
    return true;
}
/***
 *
 * @param data array of strings with all stored lines
 * @param arguments for the operation
 * prints the number of words in relation
 */
void card(char **data,Args arguments){
    Word_count w;
    char **words = parse_words(data[arguments.first], &w);
    printf("%d", w.count);
    free_words(words,w);
}
// prints true if array is empty
bool empty(char **data, Args arguments){
    const char delim[4] = " \n";
    char * string = malloc(sizeof (char)* strlen(data[arguments.first])+1);
    string = strcpy(string,data[arguments.first]);
    char *token;
    // if token is NULL, that means there are only whitespaces or nothing
    token = strtok(string, delim);
    token = strtok(NULL, delim);
    if (token == NULL)printf("true");
    else printf("false");
    free(string);
    return true;
}

bool complement(char **data, Args arguments) {
    Word_count w;
    //parses elements of universum into an array of string
    char **universum = parse_words(data[1], &w);
    printf("S ");
    for(int i = 0; i <w.count;i++){
        //prints elements of universum that are not in set
        if(strstr(data[arguments.first],universum[i])== NULL)printf("%s ", universum[i]);
    }
    free_words(universum,w);
    return  true;
}

bool union_f(char **data, Args arguments){
    Word_count first_w; //to store how many words sets have
    //parses elements of first set into an array of string
    char **first_words= NULL;
    first_words = parse_words(data[arguments.first],&first_w);
    Word_count sec_w;
    char **second_words = NULL;
    printf("S ");
    for (int i = 0;i <first_w.count;i++){
        // prints if element of 1st set  is not in second
        if(strstr(data[arguments.second], first_words[i]) == NULL) printf("%s ", first_words[i]);
    }
    second_words = parse_words(data[arguments.second],&sec_w);
    for (int i = 0; i < sec_w.count; i++) {
        // prints the second set
        printf("%s ", second_words[i]);
    }
    free_words(first_words,first_w);
    free_words(second_words,sec_w);
    return true;
}
bool intersect(char ** data, Args arguments){
    Word_count first_w; //to store how many words sets have
    char **first_words= NULL;
    first_words = parse_words(data[arguments.first],&first_w);
    printf("S ");
    for (int i = 0;i < first_w.count;i++){
        //prints if elements is in both sets
        if(strstr(data[arguments.second], first_words[i]) != NULL) printf("%s ", first_words[i]);
    }
    free_words(first_words,first_w);
    return true;
}
bool minus(char **data, Args arguments){
    Word_count first_w;
    char **first_words= NULL;
    first_words = parse_words(data[arguments.first],&first_w);
    printf("S ");
    for (int i = 0; i < first_w.count;i++){
        //prints the element if it is not in the second set
        if(strstr(data[arguments.second], first_words[i]) == NULL) printf("%s ", first_words[i]);
    }
    free_words(first_words,first_w);
    return true;
}
bool subseteq(char **data, Args arguments){
    Word_count first_w;
    char **first_words= NULL;
    //prints true if sets are identical
    if (!strcmp(data[arguments.first],data[arguments.second])){
        printf("true");
        return true;
    }
    first_words = parse_words(data[arguments.first],&first_w);
    int counter = 0;
    for(int i = 0;i < first_w.count;i++){
        //increments counter  if element is present in second set
        if(strstr(data[arguments.second], first_words[i]) != NULL)counter++;
    }
    free_words(first_words,first_w);
    //prints true if all elements of 1st set are present in 2nd
    if(counter == first_w.count)printf("true");
    else printf("false");
    return true;
}
bool subset(char **data, Args arguments){
    Word_count first_w; //to store how many words sets have
    char **first_words= NULL;
    int counter = 0;
    first_words = parse_words(data[arguments.first],&first_w);
    bool elements = true;
    for (int i = 0; i < first_w.count;i++){
        //increments counter  if element is present in second set
        if (strstr(data[arguments.second], first_words[i]) != NULL)counter++;
        else elements = false;
    }
    char **sec_words= NULL;
    Word_count  sec_w;
    sec_words = parse_words(data[arguments.second],&sec_w);
    free_words(first_words,first_w);
    free_words(sec_words,sec_w);
    //false if some element of 1st set is not in 2nd
    if (!elements)printf("false");
        //prints true if either there are more elements of 2nd set than 1st or first set is empty and second is not
    else if((counter < sec_w.count && counter != 0) || (first_words == 0 && sec_words != 0))printf("true");
    else printf("false");
    return true;
}
bool equals(char **data, Args arguments){
    Word_count first_w;
    char **first_words= NULL;
    first_words = parse_words(data[arguments.first],&first_w);
    int counter = 0;
    for(int i = 0; i < first_w.count;i++){
        //increments counter  if element is present in second set
        if(strstr(data[arguments.second], first_words[i]) != NULL) {
            counter++;
        }
    }
    char **sec_words= NULL;
    Word_count  sec_w;
    sec_words = parse_words(data[arguments.second],&sec_w);
    free_words(first_words,first_w);
    free_words(sec_words,sec_w);
    //prints true if both sets are empty
    if (!first_w.count && !sec_w.count)printf("true");
        //prints false if only one set is empty
    else if(!first_w.count || !sec_w.count)printf("false");
        // prints true if all elements are in both sets
    else if(counter == sec_w.count)printf("true");
    else printf("false");
    return true;
}
// OPERATIONS ON RELATIONS
bool findInArray(char **words, int size, char **array, int counter){

    for (int index =0; index < size; index++)
    {
        if(strstr(array[index], words[counter]))
        {
            return false;
        }
    }
    return true;
}
void reflexive(char **data, Args arguments){
    Word_count count;
    Word_count u;
    char **universum= NULL;
    universum = parse_relation(data[1], &u);
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);
    if (count.count == 0 && u.count == 0){
        free_words(words,count);
        printf("true");
        return;
    }
    if (count.count == 0){
        free_words(words,count);
        free_words(universum,u);
        printf("false");
        return;
    }
    bool reflexive_element = false;
    for( int i = 0; i < u.count;i++){
        for(int j = 0; j < count.count;j+=2){
            if ( (strcmp(universum[i],words[j])==0) && (strcmp(universum[i],words[j+1])==0) ){
                reflexive_element = true;
                break;
            }

        }
        if(!reflexive_element){
            free_words(words,count);
            free_words(universum,u);
            printf("false");
            return;

        }
        reflexive_element =false;
    }

    free_words(universum,u);
    free_words(words,count);
    printf("true");
}
void symmetric(char **data, Args arguments){
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);
    char **array = NULL;
    int array_size = 0;
    char **array2 = NULL;
    char **array3 = NULL;
    char **array4 = NULL;
    int index = 0;
    int index2 = 0;
    int symmetric_relations = 0;
    int relations_counter = 0;

    for (int i = 0; i < count.count; i++) {
        array = (char **) realloc(array, (index + 1) * sizeof(*array));
        array2 = (char **) realloc(array2, (index + 1) * sizeof(*array2));

        if (i % 2 == 0) {
            array[index] = (char *) malloc(sizeof(char) * strlen(words[i])+1);
            strcpy(array[index], words[i]);
            array_size++;

            array2[index] = (char *) malloc(sizeof(char) * strlen(words[i + 1])+1);
            strcpy(array2[index], words[i + 1]);

            relations_counter++;
            index++;
        }
    }

    for (int i = 0; i < count.count; i++) {
        array3 = (char **) realloc(array3, (index2 + 1) * sizeof(*array3));
        array4 = (char **) realloc(array4, (index2 + 1) * sizeof(*array4));

        if (i % 2 == 0) {
            array3[index2] = (char *) malloc(sizeof(char) * strlen(words[i])+1);
            strcpy(array3[index2], words[i + 1]);

            array4[index2] = (char *) malloc(sizeof(char) * strlen(words[i])+1);
            strcpy(array4[index2], words[i]);

            for (int j = 0; j < array_size; j++) {
                if ((strstr(array[j], array3[index2])) && (strstr(array2[j], array4[index2]))) {
                    symmetric_relations++;
                }
            }
            index2++;
        }
    }


    if (symmetric_relations == relations_counter) {
        printf("true");
    } else {
        printf("false");
    }
    int i = 0;
    while(i != index){
        free(array[i]);
        free(array2[i]);
        i++;
    }

    i = 0;
    while(i != index2){
        free(array3[i]);
        free(array4[i]);
        i++;
    }

    free(array);
    free(array2);
    free(array3);
    free(array4);
    free_words(words, count);
}
void antisymmetric(char **data, Args arguments){
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);



    char **array = NULL;
    int array_size = 0;
    char **array2 = NULL;
    char **array3 = NULL;
    char **array4 = NULL;
    int index = 0;
    int index2 = 0;
    int index3 = 0;
    int relations_counter = 0;

    for (int i = 0; i < count.count; i++) {
        array = (char **) realloc(array, (index + 1) * sizeof(*array));
        array2 = (char **) realloc(array2, (index + 1) * sizeof(*array2));

        if (i % 2 == 0) {
            array[index] = (char *) malloc(sizeof(char) * strlen(words[i])+1);
            strcpy(array[index], words[i]);
            array_size++;

            array2[index] = (char *) malloc(sizeof(char) * strlen(words[i + 1])+1);
            strcpy(array2[index], words[i + 1]);

            relations_counter++;
            index++;
        }
    }

    for (int i = 0; i < count.count; i++) {
        array3 = (char **) realloc(array3, (index2 + 1) * sizeof(*array3));
        array4 = (char **) realloc(array4, (index2 + 1) * sizeof(*array4));

        if (i % 2 == 0) {
            array3[index2] = (char *) malloc(sizeof(char) * strlen(words[i])+1);
            strcpy(array3[index2], words[i + 1]);

            array4[index2] = (char *) malloc(sizeof(char) * strlen(words[i])+1);
            strcpy(array4[index2], words[i]);
            index3++;

            for (int j = 0; j < array_size; j++) {
                if ((strstr(array[j], array3[index2])) && (strstr(array2[j], array4[index2])) && !(strstr(array3[index2], array4[index2]))) {
                    printf("false");
                    int k = 0;
                    while(k != index){
                        free(array[k]);
                        free(array2[k]);
                        k++;
                    }

                    k = 0;
                    while(k != index3){
                        free(array3[k]);
                        free(array4[k]);
                        k++;
                    }
                    free(array);
                    free(array2);
                    free(array3);
                    free(array4);
                    free_words(words, count);
                    return;
                }
            }
            index2++;
        }
    }
    printf("true");
    int i = 0;
    while(i != index){
        free(array[i]);
        free(array2[i]);
        i++;
    }

    i = 0;
    while(i != index2){
        free(array3[i]);
        free(array4[i]);
        i++;
    }
    free(array);
    free(array2);
    free(array3);
    free(array4);
    free_words(words, count);
}
void transitive(char **data, Args arguments){
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);

    char **array = NULL;
    int array_size = 0;
    char **array2 = NULL;
    int index = 0;
    int relations_counter = 0;
    int is_transitive = 0;

    for (int i = 0; i < count.count; i++) {
        array = (char **) realloc(array, (index + 1) * sizeof(*array));
        array2 = (char **) realloc(array2, (index + 1) * sizeof(*array2));

        if (i % 2 == 0) {
            array[index] = (char *) malloc(sizeof(char) * strlen(words[i])+1);
            strcpy(array[index], words[i]);
            array_size++;

            array2[index] = (char *) malloc(sizeof(char) * strlen(words[i + 1])+1);
            strcpy(array2[index], words[i + 1]);

            relations_counter++;
            index++;
        }
    }

    for (int i = 0; i < array_size; i++) {
        for (int j = 0; j < array_size; j++) {
            if (!strcmp(array2[i], array[j])) {
                is_transitive = 0;
                for (int k = 0; k < array_size; k++) {
                    int x = strcmp(array[i], array[k]);
                    int y = strcmp(array2[j], array2[k]);

                    if ((!x) && (!y)) {
                        is_transitive = 1;
                    }
                    if ((k == array_size-1) && (is_transitive == 0)) {
                        printf("false");
                        int l = 0;
                        while(l != index){
                            free(array[l]);
                            free(array2[l]);
                            l++;
                        }
                        free(array);
                        free(array2);
                        free_words(words, count);
                        return;
                    }
                }
            }
        }
    }

    printf("true");

    int i = 0;
    while(i != index){
        free(array[i]);
        free(array2[i]);
        i++;
    }
    free(array);
    free(array2);
    free_words(words, count);
}


void function(char **data, Args arguments){
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);
    char **array = NULL;
    int index = 0;
    int help = 0;
    for (int i = 0; i < count.count; i++)
    {
        array = (char **) realloc(array, ( index + 1) * sizeof(*array));
        //array[index] = (char *)malloc(sizeof(char)*  strlen(words[counter]));

        if (i%2 == 0)
        {

            while(findInArray(words, index, array, i) != false){

                help++;
                array[index] = (char *)malloc(sizeof(char)*  strlen(words[i])+1);
                strcpy(array[index], words[i]);
                index++;
            }
        }
    }

    if(index != count.count/2){printf("false");}
    else{printf("true");}

    int i = 0;
    while(i != index){
        free(array[i]);
        i++;
    }

    free(array);
    free_words(words, count);

}


void domain(char **data, Args arguments){
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);

    char **array = NULL;
    int index = 0;

    printf("S ");
    for (int i = 0; i < count.count; i++)
    {
        array = (char **) realloc(array, ( index + 1) * sizeof(*array));
        //array[index] = (char *)malloc(sizeof(char)*  strlen(words[counter]));

        if (i%2 == 0)
        {
            if(findInArray(words, index, array, i) == true){

                array[index] = (char *)malloc(sizeof(char)*  strlen(words[i])+1);
                strcpy(array[index], words[i]);
                printf("%s ", array[index]);
                index++;
            }
        }
    }


    int i = 0;
    while(i != index){
        free(array[i]);
        i++;
    }
    free(array);
    free_words(words, count);
}


// fix printing, it prints R Apple instead of Apple
void codomain(char **data, Args arguments){
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);

    char **array = NULL;
    int index = 0;

    printf("S ");
    for (int i = 0; i < count.count; i++)
    {
        array = (char **) realloc(array, ( index +1) * sizeof(*array));
        //array[index] = (char *)malloc(sizeof(char)*  strlen(words[counter]));

        if (i%2 != 0)
        {
            if(findInArray(words, index, array, i) == true){
                array[index] = (char *)malloc(sizeof(char)*  strlen(words[i])+1);
                strcpy(array[index], words[i]);
                printf("%s ", array[index]);
                index++;
            }
        }
    }
    int i = 0;
    while(i != index){
        free(array[i]);
        i++;
    }
    free(array);
    free_words(words, count);
}

void injective(char **data, Args arguments){
    if(!duplicit(data[arguments.first])){
        printf("false");
        return;
    }
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);
    Word_count sec_w;
    char **sec_words= NULL;
    sec_words = parse_relation(data[arguments.second], &sec_w);

    Word_count third_w;
    char **third_words= NULL;
    third_words = parse_relation(data[arguments.third], &third_w);

    int x_counter = 0;
    int y_counter = 0;
    for(int index = 0; index <count.count;index++){
        if(index%2 == 0){
            for(int i = 0; i < sec_w.count;i++){
                if(strcmp(words[index],sec_words[i]) == 0){
                    x_counter++;
                    break;
                }
            }
        }
        else {
            for(int i = 0; i < third_w.count;i++){
                if(strcmp(words[index],third_words[i]) == 0){
                    y_counter++;
                    break;
                }
            }
        }
    }
    if (count.count == 0 && sec_w.count == 0 && third_w.count == 0){
        printf("true");
    }
    else if (count.count != 0 && sec_w.count == 0 && third_w.count == 0){
        printf("false");
    }
    else if ( count.count == 0){
        printf("false");
    }
    else if(x_counter == sec_w.count && x_counter == y_counter){
        printf("true");
    }
    else printf("false");
    free_words(words,count);
    free_words(sec_words,sec_w);
    free_words(third_words,third_w);
}
void surjetive(char **data, Args arguments){
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);
    Word_count sec_w;
    char **sec_words= NULL;
    sec_words = parse_relation(data[arguments.second], &sec_w);

    Word_count third_w;
    char **third_words= NULL;
    third_words = parse_relation(data[arguments.third], &third_w);

    int x_counter = 0;
    int y_counter = 0;
    for(int index = 0; index <count.count;index++){
        if(index%2 == 0){
            for(int i = 0; i < sec_w.count;i++){
                if(strcmp(words[index],sec_words[i]) == 0){
                    x_counter++;
                    break;
                }
            }
        }
        else {
            for(int i = 0; i < third_w.count;i++){
                if(strcmp(words[index],third_words[i]) == 0){
                    y_counter++;
                    break;
                }
            }
        }
    }
    for(int i = 0; i <count.count;i+=2){
        for(int j = i+1;j<count.count;j+=2){
            if(!strcmp(words[i],words[j])){
                printf("false");
                free_words(words,count);
                free_words(sec_words,sec_w);
                free_words(third_words,third_w);
                return;
            }
        }
    }
    bool found = false;
    for(int i = 0; i <third_w.count;i++){
        found = false;
        for(int j = 1;j<count.count;j+=2){
            if(strcmp(third_words[i],words[j]) == 0){
                found = true;
                break;
            }
        }
        if (!found){
            printf("false");
            free_words(words,count);
            free_words(sec_words,sec_w);
            free_words(third_words,third_w);
            return;
        }
    }

    if (count.count == 0 && sec_w.count == 0 && third_w.count == 0){
        printf("true");
    }
    else if (count.count != 0 && sec_w.count == 0 && third_w.count == 0){
        printf("false");
    }
    else if ( count.count == 0){
        printf("false");
    }
    else if(x_counter == sec_w.count && x_counter == y_counter){
        printf("true");
    }
    else printf("false");
    free_words(words,count);
    free_words(sec_words,sec_w);
    free_words(third_words,third_w);
}
void bijective(char **data, Args arguments){
    if(!duplicit(data[arguments.first])){
        printf("false");
        return;
    }
    Word_count count;
    char **words= NULL;
    words = parse_relation(data[arguments.first], &count);
    Word_count sec_w;
    char **sec_words= NULL;
    sec_words = parse_relation(data[arguments.second], &sec_w);

    Word_count third_w;
    char **third_words= NULL;
    third_words = parse_relation(data[arguments.third], &third_w);

    int x_counter = 0;
    int y_counter = 0;
    for(int index = 0; index <count.count;index++){
        if(index%2 == 0){
            for(int i = 0; i < sec_w.count;i++){
                if(strcmp(words[index],sec_words[i]) == 0){
                    x_counter++;
                    break;
                }
            }
        }
        else {
            for(int i = 0; i < third_w.count;i++){
                if(strcmp(words[index],third_words[i]) == 0){
                    y_counter++;
                    break;
                }
            }
        }
    }
    if (count.count == 0 && sec_w.count == 0 && third_w.count == 0){
        printf("true");
    }
    else if (count.count != 0 && sec_w.count == 0 && third_w.count == 0){
        printf("false");
    }
    else if ( count.count == 0){
        printf("false");
    }
    else if(x_counter == sec_w.count && (y_counter == third_w.count) && x_counter == y_counter){
        printf("true");
    }
    else printf("false");
    free_words(words,count);
    free_words(sec_words,sec_w);
    free_words(third_words,third_w);
}

// checks operations with 1 argument
bool check_op_one(char * string, char **data, int nb_of_lines, bool is_set){
    char *help_string = malloc(sizeof (char) * strlen(string)+1);
    strcpy(help_string,string);
    const char delim[4] = " ";
    char *token;
    int counter = 0;
    bool error = false;
    token = strtok(help_string, delim);//skips C
    char *ptr;
    while (token!= NULL && error == false) {
        if(counter == 2){ // checks if second word (not counter C) is number
            if(!is_number(token)) {
                error = true;
                break;
            }
            int number = (int) strtol(token,&ptr,10);
            if (number <= 0) {
                error = true;
                break;
            }
            if(number > nb_of_lines){error = true;break;}//breaks because it would segfault on following if , checks if the number is greater than the number of lines
            if(is_set){ // if the operation is on set
                if (data[number][0] != 'S' && data[number][0] != 'U'){ // if first word is not U or S, error sets to true and subsequently, the function returns false
                    error = true;
                    break;
                }

            }
            else if (data[number][0] != 'R') {
                error = true;
                break;
            }

        }
        token = strtok(NULL, delim); // skips the name of function
        counter++;
    }
    if (counter != 3) error = true; // if there is  not  1 argument, returns false
    free(help_string);
    if (error) return false;
    return true;
}
// checks operations with 2 arguments
bool check_op_two(char * string, char **data, int nb_of_lines, bool is_set){
    char *help_string = malloc(sizeof (char) * strlen(string)+1);
    strcpy(help_string,string);
    const char delim[4] = " ";
    char *token;
    int counter = 0;
    bool error = false;
    token = strtok(help_string, delim);//skips C
    char *ptr;
    while (token!= NULL && error == false) {
        if(counter == 2 || counter == 3){ // if 1st or 2nd arguments are not number, error sets to true
            if(!is_number(token)) {
                error = true;
                break;
            }
            int number = (int) strtol(token,&ptr,10);
            if(number > nb_of_lines){error = true;break;}//breaks cuz it would segfault on following if,  checks if the number is greater than the number of lines
            if(is_set){ // same as previous function
                if (data[number][0] != 'S') {
                    error = true;
                    break;
                }

            }
            else if (data[number][0] != 'R') {
                error = true;
                break;
            }
        }
        token = strtok(NULL, delim); // skips the name of function
        counter++;
    }
    if (counter != 4) error = true; // if there are not  2 arguments, returns false
    free(help_string);
    if (error) return false;
    return true;
}
// checks operations with 3 arguments
bool check_op_three(char * string, char **data, int nb_of_lines){
    char *help_string = malloc(sizeof (char) * strlen(string)+1);
    strcpy(help_string,string);
    const char delim[4] = " ";
    char *token;
    int counter = 0;
    bool error = false;
    token = strtok(help_string, delim);//skips C
    char *ptr;
    while (token!= NULL && error == false) {
        if(counter == 2){// if 1st argument is not number, error sets to true
            if(!is_number(token)) {
                error = true;
                break;
            }
            int number = (int) strtol(token,&ptr,10);
            if(number > nb_of_lines){error = true;break;}//breaks cuz it would segfault on following if
            if (data[number][0] != 'R') { // first argument must point to a relation
                error = true;
                break;
            }
        }
        if(counter == 3 || counter == 4){ // if 2nd or 3rd arguments are not number, error sets to true
            if(!is_number(token)) {
                error = true;
                break;
            }
            int number = (int) strtol(token,&ptr,10);
            if(number > nb_of_lines){error = true;break;}//breaks cuz it would segfault on following if
            if (data[number][0] != 'S') { // 2nd and 3rd arguments must point to sets
                error = true;
                break;
            }
        }
        token = strtok(NULL, delim); // skips the name of function
        counter++;
    }
    if (counter != 5) error = true; // there must be only 3 arguments
    free(help_string);
    if (error) return false;
    return true;
}
//checks the validity of operation
bool check_operation(char * line,char **data, int counter){
    char *help_string = malloc(sizeof (char) * strlen(line)+1);
    strcpy(help_string,line);
    const char delim[4] = " ";
    char *token;
    token = strtok(help_string, delim);//skips C
    token = strtok(NULL, delim);
    if (token == NULL) { // if only C is present, the syntax is wrong, returns false
        free(help_string);
        fprintf(stderr,"wrong command syntax");
        return false;
    }
    if (!strcmp(token,"empty") || !strcmp(token,"card") || !strcmp(token,"complement")){ //if 2nd word is one of these words, calls function that check it
        bool set  = true;
        if(!check_op_one(line, data, counter,set)){//these functions are on sets and require 1 argument
            fprintf(stderr,"wrong command syntax");
            free(help_string);
            return false;
        }
    }
        //if 2nd word is one of these words, calls function that check it
    else if (!strcmp(token,"union") || !strcmp(token,"intersect") || !strcmp(token,"minus") || !strcmp(token,"subseteq") || !strcmp(token,"subset") || !strcmp(token,"equals")){
        bool set = true;
        if(!check_op_two(line, data, counter,set)){//these functions are on sets and require 2 arguments
            fprintf(stderr,"wrong command syntax");
            free(help_string);
            return false;
        }
    }
    else if (!strcmp(token,"reflexive") || !strcmp(token,"symmetric") || !strcmp(token,"antisymmetric") || !strcmp(token,"transitive") || !strcmp(token,"function") || !strcmp(token,"domain") || !strcmp(token,"codomain")){
        bool set = false;
        if(!check_op_one(line, data, counter,set)){//these functions are on relations and require 1 argument
            fprintf(stderr,"wrong command syntax");
            free(help_string);
            return false;
        }
    }

    else if (!strcmp(token,"injective") || !strcmp(token,"surjective") || !strcmp(token,"bijective")){
        if(!check_op_three(line, data, counter)){
            fprintf(stderr,"wrong command syntax");//these functions are on relations and require 3 argument
            free(help_string);
            return false;
        }
    }
    else {
        fprintf(stderr,"Wrong command syntax2"); // the 2nd word is not an operation, returns false
        free(help_string);
        return false;
    }
    free(help_string);
    return true;
}
//calls operation functions
bool print_commands(char *line,char **data){
    //using help string because strtok changes the string it is using
    //and need line intact for further use
    char *help_string = malloc(sizeof (char) * strlen(line)+1);
    strcpy(help_string,line);
    const char delim[4] = " ";
    Args arguments;
    if (!find_args(help_string,&arguments)){ // finds the arguments contained on the line
        free(help_string);
        return false;
    }
    char *token;
    token = strtok(help_string, delim);
    token = strtok(NULL, delim); // if only C is present, returns false
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
        (union_f(data, arguments));
    }
    else if (!strcmp(token,"intersect")) {
        intersect(data,arguments);
    }
    else if (!strcmp(token,"minus")) {
        minus(data,arguments);
    }
    else if (!strcmp(token,"subseteq")) {
        if(!subseteq(data, arguments)){
            free(help_string);
            return false;
        }
    }
    else if (!strcmp(token,"subset")) {
        subset(data , arguments);
    }
    else if (!strcmp(token,"equals")) {
        equals(data, arguments);
    }
    else if (!strcmp(token,"reflexive")) {
        reflexive(data,arguments);
    }
    else if (!strcmp(token,"symmetric")) {
        symmetric(data, arguments);
    }
    else if (!strcmp(token,"antisymmetric")) {
        antisymmetric(data,arguments);
    }
    else if (!strcmp(token,"transitive")) {
        transitive(data, arguments);
    }
    else if (!strcmp(token,"function")) {
        function(data, arguments);
    }
    else if (!strcmp(token,"domain")) {
        domain(data, arguments);
    }
    else if (!strcmp(token,"codomain")) {
        codomain(data, arguments);
    }
    else if (!strcmp(token,"injective")) {
        injective(data, arguments);
    }
    else if (!strcmp(token,"surjective")) {
        surjetive(data, arguments);
    }
    else if (!strcmp(token,"bijective")) {
        bijective(data, arguments);
    }
    else{
        fprintf(stderr, "wrong command");
        free(help_string);
        return false;
    }
    free(help_string);
    return true;
}

void print_results(char ** data, int counter, int first_op_index){
    //prints universum, all  sets and relations
    for (int index = 1; index < first_op_index;index++){
        printf("%s", data[index]);
    }
    bool is_first = true;
    //prints outcomes of operations
    for (int index = first_op_index; index <counter+1;index++){
        if(!is_first)printf("\n"); // new line after each operation except for 1st
        is_first = false;
        print_commands(data[index],data);
    }
}

bool check_document(FILE *fp,char  **argv, Universum universum){
    char *line = NULL; //for getline function
    size_t len = 0; //for getline function
    bool is_universum = false; //to check if universum is present
    bool error = false;
    bool is_operation = false;
    int first_operation_index= 0; //the first occurrence of an operation
    int counter = 0; // number of lines
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr,"File is empty");
        return false;}
    char **data = NULL;
    int index = 1;
    data = (char **) malloc((index + 1) * sizeof(*data));
    data[0] = malloc(sizeof (char));
    bool is_RS =false; // to check if relation or set is present
    while ((getline(&line, &len, fp) != -1) && error == false){ // reads 1 line from the document, stores it in line, stores it's length in len
        //allocs memory for data and load the current line
        data = (char **) realloc(data, (index + 1) * sizeof(*data)); // reallocs the column size to match the current iteration
        if ( data == NULL){
            free(line);
            fclose(fp);
            fprintf(stderr,"error allocating memory");
            return false;
        }
        data[index] = (char *)malloc(sizeof(char)*  strlen(line)+1); // allocates a row equal to the size of line
        if ( data == NULL){
            free(line);
            fclose(fp);
            fprintf(stderr,"error allocating memory");
            return false;
        }
        strcpy(data[index], line);//copies the line to previously allocated space in 2d array
        index++;
        counter++;
        if(counter> 1000){ //checks for the limit of 1000 lines
            fprintf(stderr,"too many lines!");
            fclose(fp);
            free(line);
            if(universum.universum != NULL)free(universum.universum);
            for(int j = 0; j <counter+1;j++){
                free(data[j]);
            }
            free(data);
            return false;
        }
        switch (line[0]) {//checks the first character of line
            case 'U':
                if (!is_universum) {
                    universum.universum = malloc(sizeof (char)* strlen(line)+1);//stores the line to struct universum
                    strcpy(universum.universum,line);
                    is_universum = true; // sets to true because universe is present
                    if(!check_words(line)){ error = true;break;} // checks if the line does not contain any invalid characters
                }
                else{
                    fprintf(stderr,"More than 1 universum");
                    error = true;
                }
                break;
            case 'S':
                if(is_operation){ //operation has to be AFTER sets
                    fprintf(stderr,"wrong order");
                    error = true;
                    break;}
                if(!is_universum){error = true;break;} // set has to be AFTER universum
                if(!check_words(line)){ error = true;break;}// checks if the elements are correct
                if(!check_set_with_uni(line,universum)){//checks if elements are in universum
                    error = true;
                    break;
                }
                is_RS = true; // sets to true because set is present
                break;
            case 'R':
                if(is_operation){ // operations have to be AFTER relations
                    fprintf(stderr,"wrong order");
                    error = true;
                    break;}
                if(!is_universum){ // relations have to be AFTER universum
                    error = true;
                    break;}
                if(!check_rel_with_uni(line,universum)){ error = true;break;}//checks if elements are in universum
                if (!check_relation(line)){ // checks if syntax of relation is valid
                    error = true;
                    break;}
                is_RS = true;  // sets to true because relation is present
                break;
            case 'C':
                if(!is_RS){ //operation has to be AFTER sets and relations
                    error = true;
                    break;
                }
                if(!is_universum){//operation has to be AFTER universum
                    error = true;
                    break;}
                if (!is_operation){ // sets the index to current iteration if it is first operation occurrence
                    first_operation_index=counter;
                    is_operation = true;
                }
                if(!check_operation(line,data,counter)){ // checks the syntax of operation
                    error = true;
                    break;
                }
                break;
            default:
                fprintf(stderr,"wrong identifier");
                free(universum.universum);
                fclose(fp);
                free(line);
                for(int j = 0; j <counter+1;j++){
                    free(data[j]);
                }
                free(data);
                return false;
        }
    }
    if(!first_operation_index || !is_universum){ // error sets to true if no universum or operations are present
        fprintf(stderr,"Element missing");
        error=true;
    }
    if(!error){ // calls print function if no errors are present
        print_results(data,counter, first_operation_index);
    }
    if(universum.universum != NULL)free(universum.universum);
    fclose(fp);
    free(line);

    for(int j = 0; j <counter+1;j++){
        free(data[j]);
    }

    free(data);
    if(!is_RS && !error){ //if no relations or sets are present, returns false
        fprintf(stderr,"no Set or Relation");
        return false;
    }
    if(error) return false;
    return true;
}

int main(int argc, char **argv) {
    if(argc > 2){
        fprintf(stderr,"too many arguments");
        return 2;
    }
    if (argv[1] == NULL){
        fprintf(stderr,"wrong argument");
        return 2;
    }
    FILE *fp = NULL;
    Universum universum = {NULL};
    if(!check_document(fp,argv,universum)) return 2;
    return 0;
}