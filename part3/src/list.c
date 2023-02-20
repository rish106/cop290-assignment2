#include <stdlib.h>
#ifndef LIST_H
#define LIST_H

/*!
This class implements the blueprint of the linked list.
*/
struct list {
    struct listentry* head;
    struct listentry* tail;
};

/*!
This class implements the blueprint of each element of a linked list.
*/
struct listentry {
    void *data;
    struct listentry *prev;
    struct listentry *next;
};

/*!
This function initiates a new empty list whose head and tail are both NULL. It also 
allocates memory in heap for it.
*/
struct list* list_new() {
    struct list* mlist = (struct list*) malloc(sizeof(struct list));
    mlist->head = NULL;
    mlist->tail = NULL;
    return mlist;
}

/*!
This function creates a new list element and allocates memory in heap for it.
*/
struct listentry* listentry_new() {
    struct listentry* res = (struct listentry*) malloc(sizeof(struct listentry));
    res->data = NULL;
    res->prev = NULL;
    res->next = NULL;
    return res;
}

/*!
This function checks if the list is empty or not.
*/
int is_empty(struct list* l) {
    if (l->head == NULL) {
        return 1;
    }
    return 0;
}

/*!
This function removes the list item e from a given list and free 
the memory allocated to it.
*/
void list_rm(struct list* l, struct listentry* e) {
    if (l->head == e && l->tail == e) {
        l->head = NULL;
        l->tail = NULL;
    } else if (l->head == e) {
        l->head = e->next;
        l->head->prev = NULL;
    } else if (l->tail == e) {
        l->tail = e->prev;
        l->tail->next = NULL;
    } else {
        e->prev->next = e->next;
        e->next->prev = e->prev;
    }
    free(e);
}

/*!
This function adds a list item to the list l whose data attribute is given. 
*/
struct listentry* list_add(struct list* l, void* data) {
    struct listentry* myentry = listentry_new();
    myentry->data = data;
    if (is_empty(l)) {
        l->head = myentry;
        l->tail = myentry;
    } else {
        l->tail->next = myentry;
        myentry->prev = l->tail;
        l->tail = myentry;
    }
    return myentry;
}

#endif
