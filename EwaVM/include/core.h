#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <limits.h>
#include <string.h>

#include <stdarg.h>

#include "../sljit/sljit_src/sljitLir.h"

namespace EwaVM
{
    // allocator.cpp
    void *wa_malloc(size_t size);
    void *wa_realloc(void *p, size_t nsize);
    void wa_free(void *p);
    void *wa_calloc(size_t size);

    // std.cpp
    void wa_debug(const char *fmt, ...);
    void wa_abort();
    void wa_assert(int cond, const char *failInfo);

    // dynarr.cpp
    struct dynarr
    {
        uint32_t len;
        uint32_t cap;
        uint32_t elemSize;
        uint8_t data[1];
    };

    // dynarr.cpp
    void dynarr_init(struct dynarr **arr, uint32_t elemSize);
    void *dynarr_push(struct dynarr **buf, int count);
    void *dynarr_pop(struct dynarr **buf, int count);
    void dynarr_free(struct dynarr **arr);

    // pool can allocate small memory many times and free all in one call.
    struct pool
    {
        struct dynarr *chunks; // chunks ,type (char *)
        int cur;
        int used;
        int chunk_size;
    };

    // pool.cpp
    void pool_init(struct pool *p, int chunk_size);
    char *pool_alloc(struct pool *p, int size);
    void pool_free(struct pool *p);

    // bytes.cpp
    uint64_t read_LEB(uint8_t *bytes, uint32_t *pos, uint32_t maxbits);
    uint64_t read_LEB(uint8_t *bytes, uint32_t *pos, uint32_t maxbits);
    uint64_t read_LEB_signed(uint8_t *bytes, uint32_t *pos, uint32_t maxbits);
    uint32_t read_uint32(uint8_t *bytes, uint32_t *pos);
};

using namespace EwaVM;

// fast utils for dynarr
#define dynarr_pop_type(dynarr1, typename) (typename *)dynarr_pop((struct dynarr **)dynarr1, 1)
#define dynarr_push_type(dynarr1, typename) (typename *)dynarr_push((struct dynarr **)dynarr1, 1)
#define dynarr_get(dynarr, typename, index) (((typename *)dynarr->data) + index)

#define ReturnIfErr(expr)                       \
    {                                           \
        char *err = expr;                       \
        if (err != NULL)                        \
        {                                       \
            wa_debug("error occure:%s\n", err); \
            return err;                         \
        }                                       \
    }
