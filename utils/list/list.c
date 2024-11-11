// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "list.h"

typedef struct Node {
    DataType data;
    struct Node *next;
} Node;

struct ListCircularCDT {
    Node *head;
    Node *current;
    size_t size;
};

ListCircularADT list_create() {
    ListCircularADT list = malloc(sizeof(struct ListCircularCDT));
    if (list == NULL) return NULL;
    
    list->head = NULL;
    list->current = NULL;
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

char list_add(ListCircularADT list, DataType data) {
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return 0;
    }

    if (list->head == NULL) {
        list->head = new_node;
        list->current = new_node;
        new_node->next = new_node; // Circular reference
    } else {
        Node *current = list->head;
        while (current->next != list->head) 
            current = current->next;

        current->next = new_node;
        new_node->next = list->head; // Circular reference
    }
    list->size++;
    return 1;
}

// Elimina el primer elemento que coincida con data
void list_remove(ListCircularADT list, DataType data, int (*cmp)(const DataType, const DataType)) {
    if (list->head == NULL) return; // Lista vacía

    Node *current = list->head;
    Node *previous = NULL;

    do {
        if (cmp(current->data, data) == 0) {
            if (current == list->current)
                list->current = current->next;
            
            
            if (previous == NULL) { // El nodo a eliminar es el primero
                // Si es el único nodo
                if (current->next == list->head) {
                    free(current);
                    list->head = NULL;
                } else {
                    Node *tail = list->head;
                    while (tail->next != list->head) 
                        tail = tail->next;

                    tail->next = current->next; // Ajusta la referencia del último nodo
                    list->head = current->next; // Actualiza la cabeza
                    free(current);
                }
            } else {
                previous->next = current->next;
                free(current);
            }
            list->size--;
            return;
        }
        previous = current;
        current = current->next;
    } while (current != list->head);
}

// Elimina todos los elementos que coincidan con data
void list_remove_all(ListCircularADT list, DataType data, int (*cmp)(const DataType, const DataType)) {
    if (list->head == NULL) return; // Lista vacía

    Node *current = list->head;
    Node *previous = NULL;

    do {
        if (cmp(current->data, data) == 0) {
            if (current == list->current)
                list->current = current->next;
                
            Node *delete_node = current;
            if (previous == NULL) { // El nodo a eliminar es el primero
                // Si es el único nodo
                if (current->next == list->head) {
                    list->head = NULL;
                } else {
                    Node *tail = list->head;
                    while (tail->next != list->head) 
                        tail = tail->next;

                    tail->next = current->next; // Ajusta la referencia del último nodo
                    list->head = current->next; // Actualiza la cabeza
                }
            } else {
                previous->next = current->next;
            }
            current = current->next; // Continúa con el siguiente nodo
            free(delete_node);
            list->size--;
        } else {
            previous = current;
            current = current->next;
        }
    } while (current != list->head);
}

uint64_t list_size(ListCircularADT list) {
    return list->size;
}

// Libera la memoria de la lista
void list_destroy(ListCircularADT list, void (*free_func)(void *)) {
    if (list->head == NULL) {
        free(list);
        return;
    }

    Node *current = list->head;
    Node *next;

    do {
        next = current->next;

        if (free_func != NULL) 
            free_func(current->data);
        
        free(current);
        current = next;
    } while (current != list->head);

    free(list);
}

DataType list_next(ListCircularADT list){
    if (list->current == NULL) return;

    DataType data = list->current->data;

    list->current = list->current->next;

    return data;
}