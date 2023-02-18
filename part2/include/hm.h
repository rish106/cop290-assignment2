#define _XOPEN_SOURCE 600
#include "mythread.h"
#include "list.h"
#include <string.h>
#define SZ 4096

struct hashmap_element_s {
  char *key;
  void *data;
};

struct hashmap_s {
  struct list* table[SZ];
  struct lock* lk[SZ];
};

// Hash function
unsigned int hash(const char *key) {
    unsigned int hash_value = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash_value = (hash_value * 37 + key[i]) % SZ;
    }
    return hash_value;
}

// Initialize a hashmap
int hashmap_create(struct hashmap_s *const out_hashmap) {
    for (int i = 0; i < SZ; i++) {
        out_hashmap->table[i] = list_new();
        out_hashmap->lk[i] = lock_new();
    }
    return 1;
}

// Set value of the key as data in hashmap. You can use any method to resolve conflicts. Also write your own hashing function
int hashmap_put(struct hashmap_s *const hashmap, const char* key, void* data) {
    struct hashmap_element_s* new_item = (struct hashmap_element_s*)malloc(sizeof(struct hashmap_element_s));
    new_item->key = (char*)key;
    new_item->data = data;
    struct listentry* e = hashmap->table[hash(key)]->head;
    while (e != NULL) {
        if (!strcmp(((struct hashmap_element_s*)(e->data))->key, key)) {
            ((struct hashmap_element_s*)(e->data))->data = data;
            return 1;
        } else {
            e = e->next;
        }
    }
    list_add(hashmap->table[hash(key)], new_item);
    return 1;
}

// Fetch value of a key from hashmap
void* hashmap_get(struct hashmap_s *const hashmap, const char* key) {
    struct listentry* e = hashmap->table[hash(key)]->head;
    while (e != NULL && strcmp(((struct hashmap_element_s*)(e->data))->key, key)) {
        e = e->next;
    }
    if (e == NULL) {
        return NULL;
    } else {
        return ((struct hashmap_element_s*)(e->data))->data;
    }
}

// Execute argument function on each key-value pair in hashmap
void hashmap_iterator(struct hashmap_s* const hashmap, int (*f)(struct hashmap_element_s *const)) {
    for (int i = 0; i < SZ; i++) {
        struct listentry* e = hashmap->table[i]->head;
        while (e != NULL) {
            f((struct hashmap_element_s* const)(e->data));
            e = e->next;
        }
    }
}

// Acquire lock on a hashmap slot
int acquire_bucket(struct hashmap_s *const hashmap, const char* key) {
    lock_acquire((struct lock*)(hashmap->lk[hash(key)]));
    return 1;
}

// Release acquired lock
int release_bucket(struct hashmap_s *const hashmap, const char* key) {
    return lock_release((struct lock*)(hashmap->lk[hash(key)]));
}
