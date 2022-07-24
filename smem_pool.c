#include "smem_pool.h"
#include "stdint.h"
#include "stdio.h"

typedef struct{
    _psize_t_   size;
    uint8_t **  handle;
}smp_chunk_t;

static uint8_t      static_pool [STATIC_POOL_SIZE] = { 0 };
static smp_chunk_t  chunks      [STATIC_POOL_CHUNKS] = { 0 };
static _psize_t_    static_pool_top     = 0;
static _csize_t_    static_chunk_count  = 0;

smp_status_t smp_malloc(void ** p_mem, _psize_t_ size){
    if (static_chunk_count >= STATIC_POOL_CHUNKS) return SMP_ERROR;
    if (STATIC_POOL_SIZE - static_pool_top < size) return SMP_ERROR;

    for (_csize_t_ iter = 0; iter < STATIC_POOL_CHUNKS; iter++){
        if (chunks[iter].size == 0){
            chunks[iter].handle = p_mem;
            chunks[iter].size = size;
            *p_mem = static_pool + static_pool_top;
            static_pool_top += size;
            static_chunk_count++;

#ifdef SMP_DEBUG
            printf("Malloc OK for %d bytes\r\n", size);
            printf("\tStatic Memory top at: %d\r\n", static_pool_top);
            printf("\tStatic Chunks count: %d\r\n", static_chunk_count);
#endif

            return SMP_OK;
        }
    }

    return SMP_ERROR;
}

smp_status_t smp_free(uint8_t ** p_mem){
    smp_chunk_t * target = chunks;
    for (; target->handle != p_mem; target++);


    uint8_t * detached_pool = *p_mem + target->size;
    _psize_t_ detached_pool_size = static_pool + static_pool_top - detached_pool;

    if (detached_pool_size <= target->size)
        memcpy(*p_mem, detached_pool, detached_pool_size);
    else{
        for (_psize_t_ ready = 0; ready < detached_pool_size; ){
            if ((detached_pool_size - ready) > target->size)
                memcpy(*p_mem + ready, detached_pool + ready, target->size);
            else
                memcpy(*p_mem + ready, detached_pool + ready, detached_pool_size - ready);
            ready += target->size;
        }
    }

    for (_csize_t_ iter = 0; iter < STATIC_POOL_CHUNKS; iter++){
        if (chunks[iter].handle == (uint8_t **)0) continue;
        if (*(chunks[iter].handle) > *p_mem)
            *(chunks[iter].handle) -= target->size;
    }

    static_chunk_count--;
    static_pool_top -= target->size;
    
#ifdef SMP_DEBUG
    printf("Free OK for %d bytes\r\n", target->size);
    printf("\tStatic Memory top at: %d\r\n", static_pool_top);
    printf("\tStatic Chunks count: %d\r\n", static_chunk_count);
#endif
    
    target->handle = (uint8_t **)0;
    target->size = 0;
    
    return SMP_OK;
}