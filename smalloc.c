
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#define HEAP_MAX 1024*10

typedef struct MemChunkHeader{
    size_t size;
    struct MemChunkHeader *next;
} MemChunkHeader;

typedef struct MemControlBlock { 
    int32_t is_available; 
    int32_t size;
} MemControlBlock;

int32_t has_initialized = 0;
void *managed_memory_start;
void *last_valid_address;
MemChunkHeader top_header = {0, NULL};

void smalloc_init(){
    last_valid_address = sbrk(0);
    managed_memory_start = last_valid_address;
    has_initialized = 1;  
}

void free_brk(void *firstbyte) { 
    void *p;
    MemControlBlock *mcb;
    size_t acc_empty = 0;
    mcb = firstbyte - sizeof(MemControlBlock);
    mcb->is_available = 1;
    for(p = managed_memory_start; p != last_valid_address; p = p + mcb->size){
        mcb = (MemControlBlock *)p;
        if(mcb->is_available){
            acc_empty = acc_empty + mcb->size;
        }else{
            acc_empty = 0;
        }
    }
    if(acc_empty>0) {
        sbrk(0 - acc_empty);
        last_valid_address = sbrk(0);
    }
    return;  
} 

void free(void *firstbyte){
    MemChunkHeader *p;
    MemChunkHeader *tmp;
    if(!firstbyte)
        return;
    if(firstbyte >= managed_memory_start && firstbyte <= last_valid_address){
        free_brk(firstbyte);
        return;
    }
    p = &top_header;
    while((void *)(p->next) + sizeof(MemChunkHeader) != firstbyte)
        p = p->next;
    tmp = (p->next)->next;
    munmap(p->next, (p->next)->size);
    p->next = tmp;
    return;
}


void *smalloc_brk(size_t numbytes) { 
    void *current_location;
    MemControlBlock *current_location_mcb;
    void *memory_location;
    numbytes = numbytes + sizeof(MemControlBlock);
    memory_location = 0;
    current_location = managed_memory_start;
    if(numbytes > HEAP_MAX)
        return NULL;
    while(current_location != last_valid_address) {
        current_location_mcb = (MemControlBlock *)current_location;
        if(current_location_mcb->is_available){
            if(current_location_mcb->size >= numbytes){
                current_location_mcb->is_available = 0;
                memory_location = current_location;
                break;
            }
        }
        current_location = current_location + current_location_mcb->size;
    }
    if(! memory_location){
        if(numbytes > (size_t) managed_memory_start + (size_t)HEAP_MAX - (size_t)last_valid_address)
            return NULL;
        if(sbrk(numbytes) == (void *) -1)
            return NULL; 
        memory_location = last_valid_address;
        last_valid_address = last_valid_address + numbytes;
        current_location_mcb = memory_location;
        current_location_mcb->is_available = 0;
        current_location_mcb->size = numbytes;
    }
    memory_location = memory_location + sizeof(MemControlBlock);
    return memory_location;
}

void *smalloc(size_t numbytes) { 
    void *smalloc_brk_result;
    void *smalloc_result;
    MemChunkHeader *p_chunk_header;
    if(!has_initialized)
        smalloc_init();
    if(!numbytes)
        return NULL;
    smalloc_brk_result = smalloc_brk(numbytes);
    if(smalloc_brk_result)
        return smalloc_brk_result;
    numbytes = numbytes + sizeof(MemChunkHeader);
    p_chunk_header = &top_header;
    while(p_chunk_header->next)
        p_chunk_header = p_chunk_header->next;
    smalloc_result = mmap(NULL, numbytes, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if(!smalloc_result)
        return NULL;
    p_chunk_header->next = smalloc_result;
    ((MemChunkHeader *)smalloc_result)->next = NULL;
    ((MemChunkHeader *)smalloc_result)->size = numbytes;
    smalloc_result = smalloc_result + sizeof(MemChunkHeader);
    return smalloc_result;
}

void *scalloc(int32_t n, size_t size){
    void * pmem;
    pmem = smalloc(n*size);
    if(!pmem)
        return NULL;
    memset(pmem, 0, n*size);
    return pmem;
}

void *srealloc(void *src, size_t size){
    void *dst;
    void *p;
    size_t src_size;
    if(src >= managed_memory_start && src <=last_valid_address){
        p = src - sizeof(MemControlBlock);
        src_size = ((MemControlBlock *)p)->size - sizeof(MemControlBlock);
    }else{
        p = src - sizeof(MemChunkHeader);
        src_size = ((MemChunkHeader *)p)->size - sizeof(MemChunkHeader);
    }
    if(size <= src_size)
        return src;
    dst = smalloc(size);
    if(!dst)
        return NULL;
    memcpy(dst, src, src_size);
    free(src);
    return dst;
}

void print_mem_control_blocks(){
    void *p;
    MemControlBlock *mcb;
    printf("%p:managed_memory_start\n", managed_memory_start);
    for(p = managed_memory_start; p != last_valid_address; p = p + mcb->size){
        mcb = (MemControlBlock *)p;
        printf("%p\t", p);
        printf("%p\t", p + mcb->size);
        printf("%d\t", mcb->size);
        printf("%d\n", mcb->is_available);
    }
    printf("%p:sbrk(0)\n", sbrk(0));
    printf("(heapsize:%ld)\n", sbrk(0)-managed_memory_start);
}

void print_mem_chanks(){
    MemChunkHeader *p;
    for(p = top_header.next; p!=NULL; p = p->next)
        printf("%p\t%zu\t(mmaped)\n", (void *)p, p->size);
}


