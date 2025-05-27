#include <stdio.h>
#include <string.h>

#include "link_and_router.h"

int DEBUG = 1;

void DistanceVector();

int main(int agrc , char **argv) {

    char router_name_input[16];
    struct router router_list[8];

    do {
        printf("Enter router name: ");
        fgets(router_name_input , 16 , stdin);

        /*Remove \n char from string*/
        router_name_input[strcspn(router_name_input , "\n")] = 0;

        
        int i;
        for(i=0 ; i<8 ; i++) {
            if(strcmp(router_name_input,"END") /* Check for null or empty string*/
            && (router_list[i].router_name == NULL || *router_list[i].router_name == '\0')) {
                
                /*If found an empty spot, put router in there*/
                strcpy(router_list[i].router_name , router_name_input);

                /*Printing value of router_list[i] just to make sure*/
                if(DEBUG) {
                    printf("Name enetered: %s\n" , router_list[i].router_name);
                }
                break;
            }
        }

        // if(!strcmp(router_name_input,"testPrint")) {
        //     for(i=0 ; i<8 ; i++) {
        //         if(*router_list[i].router_name != '\0') {
        //             printf("Name %d: %s\n" , i , router_list[i].router_name);
        //         }
        //     }
        // }


    /*Will change this to START later*/
    } while (strcmp(router_name_input,"END"));


    return 0;
}