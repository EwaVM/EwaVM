#include <core.h>

namespace EwaVM
{

    void wa_debug(const char *fmt, ...)
    {
#ifdef DEBUG
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
#endif
    }

    void wa_abort()
    {
        printf("aborted...\n");
        exit(1);
    }

    void wa_assert(int cond, const char *failInfo)
    {
        if (!cond)
        {
            wa_debug("assert fail due to %s\n", failInfo);
            wa_abort();
        }
    }

}