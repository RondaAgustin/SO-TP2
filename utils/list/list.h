#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>


typedef struct ListCDT* ListADT;
typedef struct ListIteratorCDT* ListIteratorADT;
typedef int DataType;


ListADT list_create();
void list_add(ListADT list, DataType data);
void list_remove(ListADT list, DataType data, int (*cmp)(const DataType, const DataType));
void list_remove_all(ListADT list, DataType data, int (*cmp)(const DataType, const DataType));
void list_destroy(ListADT list, void (*free_func)(void *));
size_t list_size(ListADT list);


ListIteratorADT list_iterator(ListADT list);
int list_iterator_has_next(ListIteratorADT iterator);
DataType list_iterator_next(ListIteratorADT iterator);
void list_iterator_destroy(ListIteratorADT iterator);

#endif