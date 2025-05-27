#ifndef LINK_AND_ROUTER_H
#define LINK_AND_ROUTER_H

/*Struct for each link in the link table*/
struct link {
    char destination[16];
    char next_hop[16];
    int distance_to_dest;
};

/*Struct for each router in the network*/
struct router {
    char router_name[16];
    struct link link_table[64];
};





#endif