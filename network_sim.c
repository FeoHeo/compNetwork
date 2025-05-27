#include <stdio.h>
#include <string.h>

#include "link_and_router.h"

int DEBUG = 1;

void DistanceVector();

int main(int agrc , char **argv) {

    char user_input[16];
    struct router router_list[8];

    // 
    do {
        printf("Enter router name: ");
        fgets(user_input , 16 , stdin);

        /*Remove \n char from string*/
        user_input[strcspn(user_input , "\n")] = 0;

        
        int i;
        for(i=0 ; i<8 ; i++) {
            if(strcmp(user_input,"END") /* Check for null or empty string*/
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

        /*if(!strcmp(user_input,"testPrint")) {
            for(i=0 ; i<8 ; i++) {
                if(*router_list[i].router_name != '\0') {
                    printf("Name %d: %s\n" , i , router_list[i].router_name);
                }
            }
        }*/


    /*Will change this to START later*/
    } while (strcmp(user_input,"START"));

    /*Process inputs for links*/
    do {
        printf("Enter router links: \n");
        fgets(user_input , 16 , stdin);
        /*Remove \n char from string*/
        user_input[strcspn(user_input , "\n")] = 0;
        
        char* tokenised;

        /*Tokenise the inputs for processing*/
        tokenised = strtok(user_input , " ");

        if(DEBUG) {
            printf("Atempt to add connection at: %s\n",tokenised);
        };

    } while(strcmp(user_input , "END"));


    return 0;
}