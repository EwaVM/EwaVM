#include <core.h>

namespace ewasvm
{

    void *wa_malloc(size_t size)
    {
        void *res = malloc(size);
        return res;
    }

    void *wa_realloc(void *p, size_t nsize)
    {
        void *res = realloc(p, nsize);
        return res;
    }

    void wa_free(void *p)
    {
        free(p);
    }

    void *wa_calloc(size_t size)
    {
        return calloc(size, 1);
    }

};