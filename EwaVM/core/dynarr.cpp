#include <core.h>

namespace EwaVM
{

    void dynarr_init(struct dynarr **arr, uint32_t elemSize)
    {
        if (*arr != NULL)
        {
            wa_free(*arr);
        }
        *arr = (struct dynarr *)wa_malloc(sizeof(struct dynarr) + elemSize);
        (*arr)->cap = 1;
        (*arr)->len = 0;
        (*arr)->elemSize = elemSize;
    }

    void *dynarr_push(struct dynarr **buf, int count)
    {
        struct dynarr *arr = *buf;
        uint32_t cap = arr->cap;
        uint32_t elemSize = arr->elemSize;
        void *newelem = NULL;
        while (cap <= arr->len + count)
        {
            cap = cap * 2;
        }
        if (arr->cap != cap)
        {
            arr = (struct dynarr *)wa_realloc(*buf, sizeof(struct dynarr) + elemSize * cap);
            *buf = arr;
            arr->cap = cap;
        }
        newelem = &arr->data[arr->len * elemSize];
        memset(newelem, 0, elemSize * count);
        arr->len += count;
        return newelem;
    }

    void *dynarr_pop(struct dynarr **buf, int count)
    {
        struct dynarr *arr = *buf;
        uint32_t elemSize = arr->elemSize;
        arr->len -= count;
        return arr->data + arr->len * elemSize;
    }

    void dynarr_free(struct dynarr **arr)
    {
        if (*arr != NULL)
        {
            wa_free(*arr);
        }
        *arr = NULL;
    }
}