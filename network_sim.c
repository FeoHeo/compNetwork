#include <stdio.h>
#include <string.h>

#include "link_and_router.h"

int DEBUG = 1;

#define DEFAULT_NETWORK_SIZE 5

void DistanceVector(struct router list_input[DEFAULT_NETWORK_SIZE]);

/*Returns the first index where an empty element is detected in the provided linkTable[]*/
int findEmptyLink(struct link *table_input);

int main(int agrc , char **argv) {

    char user_input[DEFAULT_STR_SIZE];
    struct router router_list[DEFAULT_NETWORK_SIZE];

    // 
    do {
        printf("Enter router name: ");
        fgets(user_input , DEFAULT_STR_SIZE , stdin);

        /*Remove \n char from string*/
        user_input[strcspn(user_input , "\n")] = 0;

        
        int i;
        for(i=0 ; i<8 ; i++) {
            if(strcmp(user_input,"START") /* Check for null or empty string*/
            && (router_list[i].router_name == NULL || *router_list[i].router_name == '\0')) {
                
                /*If found an empty spot, put router in there*/
                strcpy(router_list[i].router_name , user_input);

                /*Printing value of router_list[i] just to make sure*/
                if(DEBUG) {
                    printf("Name enetered: %s\n" , router_list[i].router_name);
                }
                break;
            }
        }

        
        
    /*Will change this to START later*/
} while (strcmp(user_input,"START"));

    /*Process inputs for links*/
    do {
        int i;
        printf("Enter router links: \n");
        fgets(user_input , DEFAULT_STR_SIZE , stdin);
        /*Remove \n char from string*/
        user_input[strcspn(user_input , "\n")] = 0;
        
        if(user_input == "END") {
            break;
        };
        
        char* tokenised;
        
        /*Tokenise the inputs for processing*/
        tokenised = strtok(user_input , " ");
        
        if(!strcmp(user_input,"testPrint")) {
            for(i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
                if(strcmp(router_list[i].router_name , "")) {
                    printf("Name %d: %s\n" , i , router_list[i].router_name);
                }
            }
            continue;
        }
        
        /*Set distance to self to 0*/
        for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
            strcpy(router_list[i].link_table[0].next_hop , router_list[i].router_name);
            strcpy(router_list[i].link_table[0].destination , router_list[i].router_name);
            router_list[i].link_table[0].distance_to_dest = 0;
        }
        
        /*Input the neighboring links*/
        for(i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
            if(!strcmp(router_list[i].router_name , tokenised));
            {
                    int empty_index = findEmptyLink(router_list[i].link_table);
                    
                    /* int empty_index = 0; using this for testing, remove later*/
                    
                    tokenised = strtok(NULL , " ");
                    strcpy(router_list[i].link_table[empty_index].destination , tokenised);
                    strcpy(router_list[i].link_table[empty_index].next_hop , tokenised);

                    tokenised = strtok(NULL , " ");
                    int to_int = *tokenised - '0';
                    router_list[i].link_table[empty_index].distance_to_dest = to_int;
                    break;
                }
        }
        
        if(DEBUG) {
            printf("Info: \nRouter: %s\nLink %s - %d\n  " , router_list[0].router_name , router_list[0].link_table[0].destination , router_list[0].link_table[0].distance_to_dest);
        }
        
    } while(strcmp(user_input , "END"));

    printf("Calculating Distance matrix\n");
    
    DistanceVector(router_list);
    
    
    return 0;
}

void DistanceVector(struct router list_input[DEFAULT_NETWORK_SIZE]) {
    // Go through all routers
    for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
        
    }
}




int findEmptyLink(struct link *table_input) {
    int i;
    for(i=0 ; i<DEFAULT_TABLE_SIZE ; i++) {
        if(*table_input[i].destination == '\0') {
            return i;
        }
    }

    printf("From findEmptyLink(): No empty indexes found\n");
    return -1;
};