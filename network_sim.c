#include <stdio.h>
#include <string.h>

#include "link_and_router.h"

int DEBUG = 1;

#define DEFAULT_NETWORK_SIZE 5

void DistanceVector(struct router list_input[DEFAULT_NETWORK_SIZE]);

/*Returns the first index where an empty element is detected in the provided linkTable[]*/
int findLink(struct link *table_input);

int main(int agrc , char **argv) {

    char user_input[DEFAULT_STR_SIZE];
    struct router router_list[DEFAULT_NETWORK_SIZE];
    char input_list[DEFAULT_NETWORK_SIZE][DEFAULT_STR_SIZE];

    memset(router_list, 0, sizeof(router_list));

    // 
    do {
        printf("Enter router name: ");
        fgets(user_input , DEFAULT_STR_SIZE , stdin);

        /*Remove \n char from string*/
        user_input[strcspn(user_input , "\n")] = 0;

        
        for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
            if(strcmp(user_input,"START") /* Check for null or empty string*/
            && (router_list[i].router_name == NULL || *router_list[i].router_name == '\0')) {
                
                /*If found an empty spot, put router in there*/
                strcpy(router_list[i].router_name , user_input);
                strcpy(input_list[i] , user_input);

                /*Printing value of router_list[i] just to make sure*/
                if(DEBUG) {
                    printf("Name enetered: %s\n" , router_list[i].router_name);
                }
                break;
            }
        }

        
        
    /*Will change this to START later*/
} while (strcmp(user_input,"START"));

/*Make new and blank router connections for each routers*/
for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
    for(int j=0 ; j<DEFAULT_NETWORK_SIZE ; j++) {
        /*If found 2 different router then set cost to inf*/
        if(strcmp(router_list[i].router_name , router_list[j].router_name)) {
            strcpy(router_list[i].link_table[j].destination , router_list[j].router_name);
            router_list[i].link_table[j].distance_to_dest = -1;       

        } else {
            /*If found 2 router of same name then set cost to 0*/
            strcpy(router_list[i].link_table[j].destination , router_list[i].router_name);
            strcpy(router_list[i].link_table[j].next_hop , router_list[j].router_name);
            router_list[i].link_table[j].distance_to_dest = 0;

        }
    }
}

/*Printing the link table of each router*/

for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
    if(!strcmp(router_list[i].router_name , "")) {
        break;
    }
    printf("Router %s link table\n" , router_list[i].router_name);
    printf("\n");

    for(int j=0 ; j<DEFAULT_NETWORK_SIZE ; j++) {
        if(!strcmp(router_list[i].link_table[j].destination , "")) {
            continue;
        }
        printf("dest: %s\t hop: %s\t cost: %d\n" , 
        router_list[i].link_table[j].destination , router_list[i].link_table[j].next_hop , 
        router_list[i].link_table[j].distance_to_dest);
    }
}



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

    if(user_input == "UPDATE") {
        continue;
    };
    
    char* tokenised;
    
    /*Tokenise the inputs for processing*/
    tokenised = strtok(user_input , " ");
    
    if(!strcmp(user_input,"PRINT")) {
        for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
            if(!strcmp(router_list[i].router_name , "")) {
                break;
            }
            printf("Router %s link table\n" , router_list[i].router_name);
            printf("\n");

            for(int j=0 ; j<DEFAULT_NETWORK_SIZE ; j++) {
                if(!strcmp(router_list[i].link_table[j].destination , "")) {
                    continue;
                }
                printf("dest: %s\t hop: %s\t cost: %d\n" , 
                router_list[i].link_table[j].destination , router_list[i].link_table[j].next_hop , 
                router_list[i].link_table[j].distance_to_dest);
            }
        }
    }
    
    
    /*Find source input*/
    for(i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
        if(!strcmp(router_list[i].router_name , tokenised)) {

            /*Find dest input*/
            tokenised = strtok(NULL , " ");
            for(int j=0 ; j<DEFAULT_NETWORK_SIZE ; j++) {
                if(!strcmp(router_list[i].link_table[j].destination , tokenised)) {
                    /*Update the cost and neighbor accordingly*/
                    strcpy(router_list[i].link_table[j].next_hop , tokenised);
                    tokenised = strtok(NULL , " ");
                    router_list[i].link_table[j].distance_to_dest = (*tokenised)-'0';
                }
            }

        }
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




int findLink(struct link *table_input) {
    int i;
    for(i=0 ; i<DEFAULT_TABLE_SIZE ; i++) {
        if(*table_input[i].destination == '\0') {
            return i;
        }
    }

    printf("From findLink(): No empty indexes found\n");
    return -1;
};