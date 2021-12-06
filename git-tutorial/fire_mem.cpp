#include "fire_mem.hpp"
#include <string.h>
#include "sp_debug.hpp"

#define MATH_MIN(a, b) ((a)<(b)?(a):(b))
//#define MATH_MAX(a, b) ((a)>(b)?(a):(b))
#define MEM_TRACE(text) SP_TRACE(1, text)

MemMgr::MemMgr()
{
    static uint32_t work_mem_int[LV_MEM_SIZE / sizeof(uint32_t)];
    MemMgr::work_mem = work_mem_int;
    MemMgr::addr_end = &work_mem_int[LV_MEM_SIZE / sizeof(uint32_t)];

    mem_ent_t * initMem = (mem_ent_t *)(MemMgr::work_mem);
    initMem->header.s.used = 0;
    initMem->header.s.d_size = LV_MEM_SIZE - sizeof(mem_header_t);
}

MemMgr::~MemMgr(){}

MemMgr* const MemMgr::getInstance()
{
    static MemMgr mem;
    return &mem;
}

void * MemMgr::mem_alloc(uint32_t size)
{
    void * alloc = nullptr;
    mem_ent_t * e = nullptr;

    if (0 == size)
      return nullptr;

    MemMgr::mem_align(size);

    do{
        e = MemMgr::ent_get_next(e);

        if(e){
            alloc = MemMgr::ent_alloc(e, size);
        }
    }while(!e and alloc == nullptr);

    if(alloc)
    {
        memset(alloc, 0xFFFF, size);
    }

    return alloc;
}

void MemMgr::mem_free(const void * data)
{
    memset((void *)data, 0xFFFF, MemMgr::mem_get_size(data));

    mem_ent_t * e = (mem_ent_t *)((uint8_t *)data - sizeof(mem_header_t));
    e->header.s.used = 0;

    MemMgr::ent_garbage_collect(e);
}

void * MemMgr::mem_realloc(void * data_p, uint32_t new_size)
{
    mem_ent_t * e = nullptr;

    if (!data_p){
        e = (mem_ent_t *)((uint8_t *)data_p - sizeof(mem_header_t));
        if (e->header.s.used == 0){
            data_p = nullptr;
        }
    }

    auto old_size = mem_get_size(data_p);
    if(old_size == new_size) return data_p;

    if(new_size < old_size){
        ent_trunc(e, new_size);
        return &e->data;
    }

    auto new_p = mem_alloc(new_size);

    if(new_p and data_p){
        if(0 != old_size){
            memcpy(new_p, data_p, MATH_MIN(new_size, old_size));
            mem_free(data_p);
        }
    }

    return new_p;
}


void MemMgr::mem_align(uint32_t &size)
{
    if(!size) return;

#ifdef LV_MEM_ENV64
    if(size & 0x7)
    {
        size = size & (~0x7);
        size += 8;
    }
#else
    if(sizeof & 0x3)
    {
        size = size & (~0x3)
        size +=4;
    }
#endif
}

mem_ent_t * MemMgr::ent_get_next(mem_ent_t * act_e)
{
    mem_ent_t * next_e = nullptr;

    if(!act_e)
    {
        next_e = (mem_ent_t *)MemMgr::work_mem;
    }
    else{
        auto data = &act_e->data;
        next_e = (mem_ent_t *)&data[act_e->header.s.d_size];

        if(ent_isOutOfBoudary(next_e))
            next_e = nullptr;
    }

    return next_e;
}

void * MemMgr::ent_alloc(mem_ent_t * e, uint32_t size)
{
    void * alloc = nullptr;

    if ((e->header.s.used == 0) and e->header.s.d_size >= size)
    {
        MemMgr::ent_trunc(e, size);
        e->header.s.used = 1;
        alloc = &e->data;
    }

    return alloc;
}

void MemMgr::ent_trunc(mem_ent_t * e, uint32_t size)
{
    if(nullptr == e)
        return;

    MemMgr::mem_align(size);

    if (e->header.s.d_size == (size + sizeof(mem_header_t)))
    {
        size = e->header.s.d_size;
    }

    if(e->header.s.d_size != size)
    {
        uint8_t * e_data = &e->data;
        mem_ent_t * after_new_e = (mem_ent_t *)&e_data[size];
        after_new_e->header.s.used = 0;
        after_new_e->header.s.d_size = e->header.s.d_size - size - sizeof(mem_header_t);
    }

    e->header.s.d_size = size;
}

uint32_t MemMgr::mem_get_size(const void * data)
{
    if(nullptr == data) return 0;

    auto e = (mem_ent_t*)((uint8_t *)data - sizeof(mem_header_t));

    return e->header.s.d_size;
}

void MemMgr::ent_garbage_collect(mem_ent_t *e)
{
    if(nullptr == e) return;

//    mem_ent_t * e_next;

    auto e_next = MemMgr::ent_get_next(e);
    while(!e_next){
        if(e_next->header.s.used == 0){
            e->header.s.d_size += e_next->header.s.d_size + sizeof(mem_header_t);
        }else{
            break;
        }

        e_next = MemMgr::ent_get_next(e_next);
    }
}

void MemMgr::mem_defrag()
{
    auto e_free = MemMgr::ent_get_next(nullptr);

    while(true){
        while(e_free){
            if(e_free->header.s.used != 0){
                e_free = MemMgr::ent_get_next(e_free);
            }else{
                break;
            }
        }

        if(!e_free) return;

        auto e_next = MemMgr::ent_get_next(e_free);

        while(e_next){
            if(e_next->header.s.used == 0){
                e_free->header.s.d_size += e_next->header.s.d_size + sizeof(mem_header_t);
            }else{
                break;
            }
            e_next = MemMgr::ent_get_next(e_next);
        }

        if(!e_next) return;

        e_free = e_next;
    }
}

void MemMgr::mem_monitor(mem_monitor_t * mon_p)
{
    if(!mon_p) return;

    memset(mon_p, 0, sizeof(mem_monitor_t));

    auto e = MemMgr::ent_get_next(nullptr);
    MEM_TRACE("used "<<e->header.s.used<<", d_size "<<e->header.s.d_size);

    uint32_t count = 0;
    while(e){        
        MEM_TRACE("count "<<count<<", used "<<e->header.s.used<<", d_size "<<e->header.s.d_size);
        if(e->header.s.used == 0){
            mon_p->free_cnt++;
            mon_p->free_size += e->header.s.d_size;

            if(e->header.s.d_size > mon_p->free_biggest_size){
                mon_p->free_biggest_size = e->header.s.d_size;
            }
        }else{
            mon_p->used_cnt++;
        }

        e = MemMgr::ent_get_next(e);
        count++;
    }

    mon_p->total_size = LV_MEM_SIZE;
    mon_p->used_pct = 100 - (100U * mon_p->free_size)/mon_p->total_size;
    mon_p->frag_pct = (uint32_t)mon_p->free_biggest_size *100U / mon_p->free_size;
    mon_p->frag_pct = 100 - mon_p->frag_pct;
}


uint8_t MemMgr::ent_isOutOfBoudary(mem_ent_t *e)
{
    if (!e) return 0;

    //out of boudary
    if ((uint32_t *)e >= (uint32_t *)&MemMgr::work_mem[LV_MEM_SIZE])
    {
        MEM_TRACE("out of boudary, e : "<<(uint32_t *)e<<", boudary of mem: "<<MemMgr::addr_end);
        return 1;
    }

    return 0;
}


