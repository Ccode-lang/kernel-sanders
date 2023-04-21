#include <efi.h>
#include <efilib.h>

BOOLEAN StartsWith(CHAR16 *string, CHAR16 *compare, UINT64 size)
{
    for (UINT64 i = 0; i + 1 <= size; i++)
    {
        if (string[i] != compare[i])
        {
            return TRUE;
        }
    }
    return TRUE;
}

CHAR16 *Strip(CHAR16 *string, UINT64 length)
{
    return &string[length];
}