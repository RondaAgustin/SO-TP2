#ifndef LIST_H
#define LIST_H


#include <lib.h>

typedef struct ListCDT* ListADT;
typedef struct ListIteratorCDT* ListIteratorADT;
typedef void * DataType;


ListADT list_create();
void list_add(ListADT list, DataType data);
void list_remove(ListADT list, DataType data, int (*cmp)(const DataType, const DataType));
void list_remove_all(ListADT list, DataType data, int (*cmp)(const DataType, const DataType));
void list_destroy(ListADT list, void (*free_func)(void *));
uint64_t list_size(ListADT list);


ListIteratorADT list_iterator(ListADT list);
int list_iterator_has_next(ListIteratorADT iterator);
DataType list_iterator_next(ListIteratorADT iterator);
void list_iterator_destroy(ListIteratorADT iterator);

#endif