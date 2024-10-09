#ifndef LIST_H
#define LIST_H


#include <lib.h>

typedef struct ListCDT* ListADT;
typedef struct ListCircularIteratorCDT* ListCircularIteratorADT;
typedef void * DataType;


ListADT list_create();
void list_add(ListADT list, DataType data);
void list_remove(ListADT list, DataType data, int (*cmp)(const DataType, const DataType));
void list_remove_all(ListADT list, DataType data, int (*cmp)(const DataType, const DataType));
void list_destroy(ListADT list, void (*free_func)(void *));
uint64_t list_size(ListADT list);


ListCircularIteratorADT list_iterator(ListADT list);
DataType list_circular_iterator_next(ListCircularIteratorADT iterator);
void list_iterator_destroy(ListCircularIteratorADT iterator);

#endif