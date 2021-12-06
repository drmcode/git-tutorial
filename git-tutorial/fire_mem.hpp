#ifndef FIRE_MEM_HPP
#define FIRE_MEM_HPP

#include "commTypes.hpp"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LV_MEM_SIZE    (128U * 1024U)

typedef union
{
    struct
    {
        uint32_t used : 1;    // 1: if the entry is used
        uint32_t d_size : 31; // Size off the data (1 means 4 bytes)
    } s;
    uint32_t header; // The header (used + d_size)
} mem_header_t;

typedef struct
{
    mem_header_t header;
    uint8_t data; /*First data byte in the allocated data (Just for easily create a pointer)*/
} mem_ent_t;

typedef struct
{
    uint32_t total_size; /**< Total heap size */
    uint32_t free_cnt;
    uint32_t free_size; /**< Size of available memory */
    uint32_t free_biggest_size;
    uint32_t used_cnt;
    uint8_t used_pct; /**< Percentage used */
    uint8_t frag_pct; /**< Amount of fragmentation */
} mem_monitor_t;


#ifdef __GNUC__
#if defined(__x86_64__)
#define LV_MEM_ENV64
#endif
#endif

class MemMgr
{
    private:
        void mem_align(uint32_t &size);
        uint8_t ent_isOutOfBoudary(mem_ent_t *e);
        mem_ent_t * ent_get_next(mem_ent_t * act_e);
        void ent_trunc(mem_ent_t * e, uint32_t size);
        void * ent_alloc(mem_ent_t * e, uint32_t size);
        uint32_t mem_get_size(const void * data);
        void ent_garbage_collect(mem_ent_t *e);
        MemMgr();
        ~MemMgr();
    public:
        static MemMgr* const getInstance();
        void * mem_alloc(uint32_t size);
        void mem_free(const void *data);
        void * mem_realloc(void * data_p, uint32_t new_size);
        void mem_defrag();
        void mem_monitor(mem_monitor_t * mon_p);
    private:
        uint32_t * work_mem;
        uint32_t * addr_end;
};

#ifdef __cplusplus
}
#endif

#endif
