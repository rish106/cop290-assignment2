#include <stdlib.h>
#ifndef LIST_H
#define LIST_H

struct list {
    struct listentry* head;
    struct listentry* tail;
};

struct listentry {
    void *data;
    struct listentry *prev;
    struct listentry *next;
};

// Return an initialized list
struct list* list_new() {
    struct list* mlist = (struct list*) malloc(sizeof(struct list));
    mlist->head = NULL;
    mlist->tail = NULL;
    return mlist;
}

// Return an initialized listentry
struct listentry* listentry_new() {
    struct listentry* res = (struct listentry*) malloc(sizeof(struct listentry));
    res->data = NULL;
    res->prev = NULL;
    res->next = NULL;
    return res;
}

// Check if list is empty or not
int is_empty(struct list* l) {
    if (l->head == NULL) {
        return 1;
    }
    return 0;
}

// Remove an item from the list
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

// Add an item to the list
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
