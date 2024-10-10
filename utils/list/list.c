#include "list.h"

typedef struct Node {
    DataType data;
    struct Node *next;
} Node;

struct ListCDT {
    Node *head;
    size_t size;
};

struct ListIteratorCDT {
    Node *current;
};



ListADT list_create(void) {
    ListADT list = malloc(sizeof(ListADT));
    if (list == NULL) return NULL;
    
    list->head = NULL;
    list->size = 0;

    return list;
}

static Node *create_node(DataType data) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) return NULL;
    
    node->data = data;
    node->next = NULL;
    return node;
}

void list_add(ListADT list, DataType data) {
    Node *new_node = create_node(data);
    if (new_node == NULL) return; 

    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) 
            current = current->next;
        
        current->next = new_node;
    }
    list->size++;
}

// Elimina el primer elemento que coincida con data
void list_remove(ListADT list, DataType data, int (*cmp)(const DataType, const DataType)) {
    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (cmp(current->data, data) == 0) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            list->size--;
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Elimina todos los elementos que coincidan con data
void list_remove_all(ListADT list, DataType data, int (*cmp)(const DataType, const DataType)) {
    Node *current = list->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (cmp(current->data, data) == 0) {
            Node *delete_node = current;
            if (previous == NULL) {
                list->head = current->next; 
            } else {
                previous->next = current->next; 
            }
            current = current->next;  
            free(delete_node);
            list->size--; 
        } else {
            previous = current; 
            current = current->next;
        }
    }
}

size_t list_size(ListADT list) {
    return list->size;
}

// Libera la memoria de la lista
void list_destroy(ListADT list, void (*free_func)(void *)) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        
        if (free_func != NULL) 
            free_func(current->data);
        
        free(current);
        current = next;
    }
    free(list);
}

ListIteratorADT list_iterator(ListADT list){
    if (list == NULL) return NULL;
    
    ListIteratorADT iterator = malloc(sizeof(ListIteratorADT));
    if (iterator == NULL) 
        return NULL;

    iterator->current = list->head;

    return iterator;
}


int list_iterator_has_next(ListIteratorADT iterator){
    return iterator->current != NULL;
}

DataType list_iterator_next(ListIteratorADT iterator){
    if (!list_iterator_has_next(iterator)) return NULL;
    DataType data = iterator->current->data;
    iterator->current = iterator->current->next;
    return data;
}

void list_iterator_destroy(ListIteratorADT iterator){
    free(iterator);
}