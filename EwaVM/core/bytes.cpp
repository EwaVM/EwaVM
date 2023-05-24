#include <core.h>

namespace EwaVM
{

    static uint64_t read_LEB_(uint8_t *bytes, uint32_t *pos, uint32_t maxbits, int sign)
    {
        uint64_t result = 0;
        uint32_t shift = 0;
        uint32_t bcnt = 0;
        uint32_t startpos = *pos;
        uint64_t byte;

        while (1)
        {
            byte = bytes[*pos];
            *pos += 1;
            result |= ((byte & 0x7f) << shift);
            shift += 7;
            if ((byte & 0x80) == 0)
            {
                break;
            }
            bcnt += 1;
            if (bcnt > (maxbits + 7 - 1) / 7)
            {
                wa_debug("Unsigned LEB at byte %d overflow", startpos);
            }
        }
        if (sign && (shift < maxbits) && (byte & 0x40))
        {
            // Sign extend
            result |= -(1 << shift);
        }
        return result;
    }

    uint64_t read_LEB(uint8_t *bytes, uint32_t *pos, uint32_t maxbits)
    {
        return read_LEB_(bytes, pos, maxbits, 0);
    }

    uint64_t read_LEB_signed(uint8_t *bytes, uint32_t *pos, uint32_t maxbits)
    {
        return read_LEB_(bytes, pos, maxbits, 1);
    }

    uint32_t read_uint32(uint8_t *bytes, uint32_t *pos)
    {
        *pos += 4;
        return ((uint32_t *)(bytes + *pos - 4))[0];
    }

}