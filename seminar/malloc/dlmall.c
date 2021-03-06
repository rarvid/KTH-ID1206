#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(head))
#define MIN(size) (((size) > 8)?(size):8)
#define LIMIT(size) (MIN(0) + HEAD + size)
#define MAGIC(memory) ((head*)memory - 1)
#define HIDE(block) (void*)((head*)block + 1)
#define ALIGN 8
#define ARENA 64 * 1024

typedef struct head {
    uint16_t bfree; // 2 bytes, the status of block before
    uint16_t bsize; // 2 bytes, size of block before
    uint16_t free;  // 2 bytes, status of current block
    uint16_t size;  // 2 bytes, size of current block
    struct head *next;  // 8 bytes pointer
    struct head *prev;  // 8 bytes pointer
} head;

head *after(head *block) {
    // return neighbouring after block
    return (head *)((char*)block + block->size + HEAD);
}

head *before(head *block) {
    // return neighbouring before block
    return (head *)((char*)block - block->bsize - HEAD);
}

head *split(head *block, int size) {
    int rsize = block->size - size - HEAD;
    // calculate size of remainig after block
    block->size = size;
    // change the current block size to the new smaller size

    head *splt = after(block);
    splt->bfree = block->free;
    splt->bsize = block->size;
    splt->size = rsize;
    splt->free = TRUE;
    // define splitted of block header

    head *aft = after(splt);
    aft->bsize = splt->size;
    // update next block as well

    return splt;
}

head *arena = NULL;

head *new() {

    if (arena != NULL) {
        printf("one arena alread allocated \n");
        return NULL;
    }

    // define new block
    head *new = mmap(NULL, ARENA, 
                    PROT_READ | PROT_WRITE, 
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (new == MAP_FAILED) {
        printf("mmap failed: error %d\n", errno);
        return NULL;
    }

    // make room for head and dummy
    uint size = ARENA - 2 * HEAD;

    new->bfree = FALSE;
    new->bsize = 0;
    new->free = TRUE;
    new->size = size;

    head *sentinel = after(new);
    
    // only touch the status fields
    sentinel->bfree = TRUE;
    // sentinel->bsize = new->size;
    sentinel->free = FALSE;
    // sentinel->size = 0;

    arena = (head *)new;
    return new;
}

head *flist;

void detach(head *block) {
    if (block->next != NULL){
        block->next->prev = block->prev;    
    }
    if (block->prev != NULL){
        block->prev->next = block->next;
    } else {
        flist = block->next;
    }
    block->next = NULL;
    block->prev = NULL;
}

void insert_reg(head *block) {
    block->next = flist;
    block->prev = NULL;
    if (flist != NULL) {
        flist->prev = block;
    }
    flist = block;
}

void insert_sorted(head *block) {
    head *flcp = flist;
    if(flcp == NULL) {
        insert_reg(block);
    }
    else if(block->size <= flcp->size) {
        insert_reg(block);
    } else {
        while (flcp->next != NULL && block->size > flcp->next->size) {
            flcp = flcp->next;
        }
        block->next = flcp->next;
        block->prev = flcp;
        if(flcp->next == NULL) {
            flcp->next = block; 
        }
        else {
            flcp->next->prev = block;
            flcp->next = block;
        }
    }
}

void insert(head *block,int bit){
    if(bit == 1) {
        insert_sorted(block);
    }
    if(bit == 0) {
        insert_reg(block);
    }
}

int adjust(int request) {
    // check if requested block is smaller than 8
    if(request <= MIN(0)){
        return 8;
    }
    if ((request % ALIGN) != 0) {
        return request + (ALIGN - (request % ALIGN));
    } 
    return request;
}

head *find(int size, int bit) {
    if(arena == NULL) {
        head *mem = new();
        flist = mem;
    }
    if(flist == NULL) {
        return NULL;
    }
    

    head *block = flist;
    while(block != NULL) {
        if(block->size >= size) {
            if(block->size >= LIMIT(size)){
                head *off = split(block, size);
                block->free = FALSE;
                after(block)->bfree = FALSE;
                detach(block);
                insert(off, bit);
                return block;
            } else {
                block->free = FALSE;
                after(block)->bfree = FALSE;
                detach(block);
                return block;
            }
        }
        block = block->next;
    }
    return NULL;
}

head *merge(head *block) {
    head *aft = after(block);

    if(block->bfree) {
        head *bef = before(block);
        detach(bef);
        int newsize = block->size + bef->size + HEAD;
        aft->bsize = newsize;
        bef->size = newsize;
        block = bef;
    }

    if(aft->free) {
        detach(aft);
        int newsize = block->size + aft->size + HEAD;
        head *aftaft = after(aft);
        aftaft->bsize = newsize;
        block->size = newsize;
    }
    return block;
}

void *dalloc(size_t request, int bit) {
    if(request <= 0) {
        return NULL;
    }
    int size = adjust(request);
    head *taken = find(size, bit);
    if(taken == NULL){
        return NULL;
    } else {
        return HIDE(taken);
    }
}

void dfree(void *memory, int m, int bit) {

    if(memory != NULL) {
        head *block = MAGIC(memory);

        if(m == 1) {
        block = merge(block);
        }
        head *aft = after(block);
        block->free = TRUE;
        aft->bfree = TRUE;   

        insert(block, bit);
    }
    return;
}

void sanity(){

    head *sanflist = flist;
    printf("                        \n\n*****************************FREE LIST PRINT OUT*******************\n\n");
    while(sanflist != NULL) {
        printf("free list block: %15p  |   bfree:  %2d  |   bsize:  %5d  |   size:  %5d  |   free:   %2d  |   next:   %15p  |   prev:   %15p\n",
        sanflist,
        sanflist->bfree, 
        sanflist->bsize, 
        sanflist->size, 
        sanflist->free, 
        sanflist->next, 
        sanflist->prev);
        sanflist = sanflist->next;
    }

    head *sanarena = arena;
    int size = 0;
    printf("                           \n\n*****************************ARENA PRINT OUT*******************\n\n");
    do {
        printf("free list block: %15p  |   bfree:  %2d  |   bsize:  %5d  |   size:  %5d  |   free:   %2d  |   next:   %15p  |   prev:   %15p\n",
        sanarena,
        sanarena->bfree, 
        sanarena->bsize, 
        sanarena->size, 
        sanarena->free, 
        sanarena->next, 
        sanarena->prev);
        size = sanarena->size;
        sanarena = after(sanarena);
    } while(size != 0);


}

void get_block_count() {
    head *cpFL = flist;
    int nofFL = 0;
    while(cpFL != NULL){
        nofFL++;
        cpFL = cpFL->next;
    }
    head *cpA = arena;
    int size = 0;
    int nofA = 0;
    do{
        size = cpA->size;
        nofA++;
        cpA = after(cpA);
    }while(size !=0);

    printf("\t%d\t%d\n", nofFL, nofA);
}
