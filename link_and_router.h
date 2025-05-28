#ifndef LINK_AND_ROUTER_H
#define LINK_AND_ROUTER_H

#define DEFAULT_TABLE_SIZE 64
#define DEFAULT_STR_SIZE 16

/*Struct for each link in the link table*/
struct link {
    char destination[DEFAULT_STR_SIZE];
    char next_hop[DEFAULT_STR_SIZE];
    int distance_to_dest;
};

/*Struct for each router in the network*/
struct router {
    char router_name[DEFAULT_STR_SIZE];
    struct link link_table[DEFAULT_TABLE_SIZE];
};





#endif