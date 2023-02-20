#define _XOPEN_SOURCE 600
#include <string.h>
#define SZ 4096

/*!
This class creates the blueprint of each element of a hashmap.
*/
struct hashmap_element_s {
  char *key;
  void *data;
};

/*!
This class creates the blueprint of the hashmap data structure.
*/
struct hashmap_s {
  struct list* table[SZ];
  struct lock* lk[SZ];
};

/*!
This is a hashing function it converts the array of characters, key, to a certain value
in the range 0-4096.
*/
unsigned int hash(const char *key) {
    unsigned int hash_value = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash_value = (hash_value * 37 + key[i]) % SZ;
    }
    return hash_value;
}

/*!
This function initialises the hashmap by instantiating linked list to each hash value.
It also adds locks for each of the hash value, so that it can accessed by only one context.
*/
int hashmap_create(struct hashmap_s *const out_hashmap) {
    for (int i = 0; i < SZ; i++) {
        out_hashmap->table[i] = list_new();
        out_hashmap->lk[i] = lock_new();
    }
    return 1;
}

/*!
This function puts a given key value pair in the hashmap. First it checks if the key is already present,
and depending on that it adds the key or update the value corresponding to the present key.
*/
int hashmap_put(struct hashmap_s *const hashmap, const char* key, void* data) {
    struct listentry* e = hashmap->table[hash(key)]->head;
    while (e != NULL) {
        if (!strcmp(((struct hashmap_element_s*)(e->data))->key, key)) {
            ((struct hashmap_element_s*)(e->data))->data = data;
            return 1;
        } else {
            e = e->next;
        }
    }
    struct hashmap_element_s* new_item = (struct hashmap_element_s*)malloc(sizeof(struct hashmap_element_s));
    char* k = (char*)malloc(25*sizeof(char));
    strcpy(k,key);
    new_item->key = (char*)k;
    new_item->data = data;
    list_add(hashmap->table[hash(key)], new_item);
    return 1;
}

/*!
This function fetches the value corresponding to the key given and returns NULL,
if the key is not present.
*/
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

/*!
This function iterates over all the elements of the hashmap and applies
the function passed as parameter on the key,value pairs.
*/
void hashmap_iterator(struct hashmap_s* const hashmap, int (*f)(struct hashmap_element_s *const)) {
    for (int i = 0; i < SZ; i++) {
        struct listentry* e = hashmap->table[i]->head;
        while (e != NULL) {
            f((struct hashmap_element_s* const)(e->data));
            e = e->next;
        }
    }
}

/*!
This function locks the corresponding hash value in the hash map,
according to the key passed as parameter, so that other context cant use it.
*/
int acquire_bucket(struct hashmap_s *const hashmap, const char* key) {
    lock_acquire((struct lock*)(hashmap->lk[hash(key)]));
    return 1;
}

/*!
This function releases the lock from the hash value corresponding to
the key given so that it can used by other contexts.
*/
int release_bucket(struct hashmap_s *const hashmap, const char* key) {
    return lock_release((struct lock*)(hashmap->lk[hash(key)]));
}
