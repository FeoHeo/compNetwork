#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "link_and_router.h"

int DEBUG = 1;

#define INF 999
#define DEFAULT_NETWORK_SIZE 5

void DistanceVector(struct router list_input[DEFAULT_NETWORK_SIZE]);

void printAll(struct router list_input[DEFAULT_NETWORK_SIZE]);


// Returns the dest that need updating, caller need to calculate the value based on the value on the neighbor's table
// If no value need to be updated then return blank ""
void tableCompare(struct router router_1_input , struct router router_2_input , char* buffer_input);

// Find the name_input from router_input inside router_input and return the index to that in link_table
int findLink(struct router router_input , char name_input[DEFAULT_STR_SIZE]);


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
            router_list[i].link_table[j].distance_to_dest = INF;       

        } else {
            /*If found 2 router of same name then set cost to 0*/
            strcpy(router_list[i].link_table[j].destination , router_list[i].router_name);
            strcpy(router_list[i].link_table[j].next_hop , router_list[j].router_name);
            router_list[i].link_table[j].distance_to_dest = 0;

        }
    }
}

// Print table

printAll(router_list);



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
                    router_list[i].link_table[j].distance_to_dest = atoi(tokenised);
                }
            }

        }
    }
    
    
} while(strcmp(user_input , "END"));

    printAll(router_list);

    printf("Calculating Distance matrix\n");
    
    DistanceVector(router_list);
    
    
    return 0;
}

void DistanceVector(struct router list_input[DEFAULT_NETWORK_SIZE]) {
    
    int updateState = 0;
    
    do {
        updateState = 0;
        // Go through all routers
        for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
            // Go through all possible destination
            if(!strcmp(list_input[i].router_name , "")) {
                continue;
            }
            for(int j=0 ; j<DEFAULT_NETWORK_SIZE ; j++) {
                // Compare with neighbors on every possible connection
                if(!strcmp(list_input[j].router_name , "")) {
                    continue;
                }
                if(i==j) {
                    continue;
                }
                char changeRouter[DEFAULT_STR_SIZE] = "";
                int condition = 0;
                tableCompare(list_input[i] , list_input[j] , changeRouter);
                printf("Test: %s\n" , changeRouter);
                if(strcmp(changeRouter , "")) {
                    int dest_index_from_src = findLink(list_input[i] , changeRouter);
                    int neigh_index_from_src = findLink(list_input[i] , list_input[j].router_name);
                    int dest_index_from_neigh = findLink(list_input[j] , changeRouter);
                    
                    // Update the cost to dest = to_neigh + neigh_to_dest
                    list_input[i].link_table[dest_index_from_src].distance_to_dest = 
                    list_input[i].link_table[neigh_index_from_src].distance_to_dest +
                    list_input[j].link_table[dest_index_from_neigh].distance_to_dest;
                    
                    // Update next_hop in src
                    strcpy(list_input[i].link_table[dest_index_from_src].next_hop , list_input[j].router_name);
                    printf("Incoming Change for %s\n" , list_input[i].router_name);
                    printAll(list_input);
                    updateState = 1;
                }
                
            }
            
        }
    } while (updateState);
}

void tableCompare(struct router list_1_input , struct router list_2_input , char* buffer_input) {
    int link_neigh_dest_index = -1;
    int link_src_neigh_index = -1;

    if(strcmp(list_1_input.router_name , list_2_input.router_name) == 0) {
        return;
    }


    for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
        printf("Accessing router: %s\t - %s\n" , list_1_input.router_name , list_2_input.router_name);
        // Find the index of each link
        // link from src to dest is already i
        link_src_neigh_index = findLink(list_1_input , list_2_input.router_name);
        link_neigh_dest_index = findLink(list_2_input , list_1_input.link_table[i].destination);

        printf("distance src-dest: %d\t neigh-dest: %d\t src-neigh: %d\n" , 
            list_1_input.link_table[i].distance_to_dest ,
            list_2_input.link_table[link_neigh_dest_index].distance_to_dest ,
            list_1_input.link_table[link_src_neigh_index].distance_to_dest);

        strcpy(buffer_input , list_1_input.link_table[i].destination);
        if(list_1_input.link_table[i].distance_to_dest < 
            (list_2_input.link_table[link_neigh_dest_index].distance_to_dest + 
            list_1_input.link_table[link_src_neigh_index].distance_to_dest)) {

                return;
            }
    }

    strcpy(buffer_input , "");
    return;
}


int findLink(struct router router_input , char name_input[DEFAULT_STR_SIZE]) {
    for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
        if(!strcmp(router_input.link_table[i].destination , name_input)) {
            return i;
        }
    }
    return -1;

}

void printAll(struct router list_input[DEFAULT_NETWORK_SIZE]) {
    /*Printing the link table of each router*/

    for(int i=0 ; i<DEFAULT_NETWORK_SIZE ; i++) {
        if(!strcmp(list_input[i].router_name , "")) {
            break;
        }
        printf("Router %s link table\n" , list_input[i].router_name);
        printf("\n");

        for(int j=0 ; j<DEFAULT_NETWORK_SIZE ; j++) {
            if(!strcmp(list_input[i].link_table[j].destination , "")) {
                continue;
            }
            printf("dest: %s\t hop: %s\t cost: %d\n" , 
            list_input[i].link_table[j].destination , list_input[i].link_table[j].next_hop , 
            list_input[i].link_table[j].distance_to_dest);
        }
    }

    return;
}

