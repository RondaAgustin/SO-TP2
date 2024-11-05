// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_list.h"


int cmp(int o1, int o2){
    return o1 == o2 ? 0 : 1;
}

int main(int argc, char const *argv[])
{
    ListCircularADT list = list_create();

    int data_length = 4;
    int data[] = {2, 2, 5, 2};

    for (int i = 0; i < data_length; i++){
        list_add(list, data[i]);
    }

    // if (list_size(list) != data_length)
    //     printf("Incorrect size\n");



    for (size_t i = 0; i < data_length; i++){
        printf("%d\n", list_next(list));
    }


    data_length += 2;
    printf("\n\n");

    for (size_t i = 0; i < data_length; i++){
        printf("%d\n", list_next(list));
    }

    list_destroy(list, NULL);
    
    return 0;
}
