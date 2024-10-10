#ifndef __CIRCULAR_LIST_H__
#define __CIRCULAR_LIST_H__

#include <stddef.h>
#include <stdint.h>

typedef void* DataType;
typedef struct ListCircularCDT *ListCircularADT;

ListCircularADT list_create();
void list_add(ListCircularADT list, DataType data);
void list_remove(ListCircularADT list, DataType data, int (*cmp)(const DataType, const DataType));
void list_remove_all(ListCircularADT list, DataType data, int (*cmp)(const DataType, const DataType));
uint64_t list_size(ListCircularADT list);
void list_destroy(ListCircularADT list, void (*free_func)(void *));
DataType list_next(ListCircularADT list);

#endif