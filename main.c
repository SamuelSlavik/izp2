#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct list_item {
    struct list_item *next;
    char *value;

} list_item;

typedef struct list {
    list_item *top;
    int size;
} list;

typedef struct {
    char * identifier;
    int first_id;
    int second_id;
}Operation;

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
        printf("%s", token);
        token = strtok(NULL, s);
    }
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
int main(int argc, char **argv) {

    if (argv[1] == NULL){
        fprintf(stderr,"Error: No argument given");
        return -1;
    }

    if(argc > 2){
        fprintf(stderr,"Error: Too much arguments");
        return -1;
    }

    //printf("%d", argc);
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    list universum =  create_list();
    list s = create_list();
    list r = create_list();
    /*line = readline(fp);*/

    fp = fopen(argv[1], "r");
    if (fp == NULL){
        fprintf(stderr, "Error: Could not open file");
        return -1;
    } 
    

    
    char **data = NULL;
    int index = 1;
    while ((read = getline(&line, &len, fp) != -1)){
        

        data = (char **) realloc(data, (index + 1) * sizeof(*data));
        
        data[index] = (char *)malloc(2 * sizeof(char));
        
        printf("%d", index);
        
        //data[index]= malloc (sizeof(char)* strlen(line));
        //strcpy(data[index], line);
        index++;
        //printf("line is %s \n", data[index]);
        switch (line[0]) {
            case 'U':
                read_words(&universum,line);
                //printf("%s",line);
                //printf("%d", index);
                
                break;
            case 'S':
                read_words(&s,line);
                //printf("%d", index);
               
                break;
            case 'R':
            read_words(&r,line);
                //printf("%d", index);
                
                break;

                break;
            case 'C':

                break;
            default:
                fprintf(stderr,"wrong identifier");
                return 2;


        }
        //printf("%s\n", line);
        //index++;
        //read_words(&universum,line);
    }
    
    fclose(fp);

    free(data[index]);
    
    if(line){
        free(line);
        
    } 
    
    
    /*
    add_item(&universum, "Apple");
    add_item(&universum, "Pear");
    add_item(&universum, "Chery");
     */
    destroy_list(&universum);
    return 0;
}
