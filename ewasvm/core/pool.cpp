#include <core.h>

namespace ewasvm
{

    void pool_init(struct pool *p, int chunk_size)
    {
        dynarr_init(&p->chunks, sizeof(char *));

        *dynarr_push_type(&p->chunks, char *) = (char *)wa_calloc(chunk_size);

        p->cur = 0;
        p->used = 0;
        p->chunk_size = chunk_size;
    }

    // alloc size must less than chunk_size
    char *pool_alloc(struct pool *p, int size)
    {
        wa_assert(size <= p->chunk_size, "pool_alloc require size<chunk_size");

        char *r;
        if (p->used + size <= p->chunk_size)
        {
            r = *dynarr_get(p->chunks, char *, p->cur) + p->used;
            p->used += size;
        }
        else
        {
            r = (char *)wa_calloc(p->chunk_size);
            *dynarr_push_type(&p->chunks, char *) = r;
            p->cur = p->chunks->len - 1;
            p->used = size;
        }

        return r;
    }

    void pool_free(struct pool *p)
    {
        for (int i = 0; i < p->chunks->len; i++)
        {
            wa_free(*dynarr_get(p->chunks, char *, i));
        }
        dynarr_free(&p->chunks);
    }

}