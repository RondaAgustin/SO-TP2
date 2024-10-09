#include "test_list.h"


int cmp(int o1, int o2){
    return o1 == o2 ? 0 : 1;
}

int main(int argc, char const *argv[])
{
    ListADT list = list_create();

    int data_length = 9;
    int data[] = {2, 2, 5, 6, 1, 9, 0, 12, 2};

    for (int i = 0; i < data_length; i++)
    {
        list_add(list, data[i]);
    }

    if (list_size(list) != data_length)
        printf("Incorrect size\n");

    list_remove_all(list, 2, cmp);


    ListIteratorADT iterator = list_iterator(list);
    while (list_iterator_has_next(iterator))
    {
        printf("Elemento: %i\n", list_iterator_next(iterator));
    }
    
    list_destroy(list, NULL);
    list_iterator_destroy(iterator);
    

    return 0;
}
