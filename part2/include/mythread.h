#ifndef THREAD_H
#define THREAD_H

#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include<signal.h>
#include<sys/time.h>
#include<unistd.h>
#include "list.h"
#define MEM 16384

ucontext_t main_ctx;
struct list* l;
struct listentry* curr_ctx;

// Initialize threads list
void mythread_init() {
    l = list_new();
    curr_ctx = listentry_new();
    curr_ctx->data = &main_ctx;
}

// Create a new thread
ucontext_t* mythread_create(void func(void*), void* arg) {
    ucontext_t* ctx = (ucontext_t*) malloc(sizeof(ucontext_t));
    getcontext(ctx);
    ctx->uc_link = &main_ctx;
    ctx->uc_stack.ss_sp = malloc(MEM);
    ctx->uc_stack.ss_size = MEM;
    makecontext(ctx, (void (*)(void))func, 1, arg);
    list_add(l, ctx);
    curr_ctx->next = l->head;
    return ctx;
}

// Waits for other thread to complete. It is used in case of dependent threads.
void mythread_join() {
    struct listentry* e = l->head;
    // setcontext((ucontext_t*)(e->data));
    while (!is_empty(l)) {
        curr_ctx = e;
        swapcontext(&main_ctx, (ucontext_t*)(e->data));
        e = e->next;
        if (e == NULL) {
            list_rm(l, l->head);
        } else {
            list_rm(l, e->prev);
        }
    }
}

// Perform context switching here
void mythread_yield() {
    struct listentry* prev_ctx = listentry_new();
    prev_ctx->data = curr_ctx->data;
    curr_ctx = curr_ctx->next;
    if (curr_ctx == NULL) {
        curr_ctx = l->head;
    }
    swapcontext((ucontext_t*)(prev_ctx->data), (ucontext_t*)(curr_ctx->data));
}

struct lock {
	ucontext_t* ctx;
};

// return an initialized lock object
struct lock* lock_new() {
    struct lock* lk = (struct lock*) malloc(sizeof(struct lock));
    lk->ctx = NULL;
    return lk;
}

// Set lock. Yield if lock is acquired by some other thread.
void lock_acquire(struct lock* lk) {
    while (lk->ctx != NULL) {
        mythread_yield();
    }
    lk->ctx = (ucontext_t*)(curr_ctx->data);
}

// Release lock
int lock_release(struct lock* lk) {
    lk->ctx = NULL;
    return 1;
}

#endif
