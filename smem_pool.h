#ifndef _SMEM_POOL_H_
#define _SMEM_POOL_H_

#include <stdint.h>

#define SMP_DEBUG

#define STATIC_POOL_SIZE    1024
#define STATIC_POOL_CHUNKS  20

#if (STATIC_POOL_SIZE <= 0xFF)
    typedef uint8_t     _psize_t_;
#elif (STATIC_POOL_SIZE <= 0xFFFF)
    typedef uint16_t    _psize_t_;
#elif (STATIC_POOL_SIZE <= 0xFFFFFFFF)
    typedef uint32_t    _psize_t_;
#elif (STATIC_POOL_SIZE <= 0xFFFFFFFFFFFFFFFF)
    typedef uint64_t    _psize_t_;
#endif

#if (STATIC_POOL_CHUNKS <= 0xFF)
    typedef uint8_t     _csize_t_;
#elif (STATIC_POOL_CHUNKS <= 0xFFFF)
    typedef uint16_t    _csize_t_;
#elif (STATIC_POOL_CHUNKS <= 0xFFFFFFFF)
    typedef uint32_t    _csize_t_;
#elif (STATIC_POOL_CHUNKS <= 0xFFFFFFFFFFFFFFFF)
    typedef uint64_t    _csize_t_;
#endif

typedef enum{
    SMP_OK      = 0x00,
    SMP_ERROR   = 0x01
}smp_status_t;

smp_status_t    smp_malloc(void ** p_mem, _psize_t_ size);
smp_status_t    smp_free(uint8_t ** p_mem);

#endif