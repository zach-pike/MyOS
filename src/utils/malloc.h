#pragma once

#include "std.h"

#define ZMALLOC_MEM_SIZE 1000 * 1000
#define NULL 0L

// Allocate 256 Kb for memory
char memory[ZMALLOC_MEM_SIZE];

// Size type
typedef unsigned long size_t;

struct Block {
    size_t size;
    int free;
    struct Block* next;
};

// Start of list
struct Block* freeList = (void*)memory;

void Zmalloc_init() {
    freeList->free = 1;
    freeList->next = NULL;
    freeList->size = ZMALLOC_MEM_SIZE - sizeof(struct Block);
}

void Zmalloc_split(struct Block* fitting_slot, size_t size) {
    struct Block* new = (void*)((void*)fitting_slot+size+sizeof(struct Block));
    new->size=(fitting_slot->size)-size-sizeof(struct Block);
    new->free=1;
    new->next=fitting_slot->next;
    fitting_slot->size=size;
    fitting_slot->free=0;
    fitting_slot->next=new;
}

void* Zmalloc(size_t n_bytes) {
    if (!(freeList->size)) Zmalloc_init();

    // Find a chunk that is just the size we need and is free
    
    struct Block* current_block = freeList;
    unsigned foundBlock = 0;


    // Attempt to find big enouth block size
    for (;;) {
        if (current_block->size >= n_bytes && current_block->free == 1) {
            foundBlock = 1;
            break;
        }

        if (current_block->next == NULL) break;
        current_block = current_block->next;
    }

    if (foundBlock != 1) return NULL;

    current_block->free = 0;
    Zmalloc_split(current_block, n_bytes);

    // Returns a pointer to memory
    return current_block + sizeof(struct Block);
}

float getPercentFree() {
    float notalloc = 0;

    struct Block* block = freeList;

    for (;;) {
        if (block->free == 1) {
            notalloc += (sizeof(struct Block) + block->size);
        }

        if (block->next == NULL) break;
        block = block->next;
    }

    return (notalloc / (float)ZMALLOC_MEM_SIZE) * 100;
}
