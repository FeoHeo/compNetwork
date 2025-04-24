#include <stdio.h>

int main() {

    int arr[5];

    arr[0] = 1;

    for(int i=0 ; i<5 ; i++) {
        if(&arr[i] != NULL) {
            printf("%d\n",arr[i]);
        } 
    }

    return 0;
}