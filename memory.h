#include <efi.h>
#include <efilib.h>

void * malloc(UINTN poolSize)
{
    EFI_STATUS status;
    void * handle;
    Print(L"allocating memory pool\n");
    status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, poolSize, &handle);

    if(status == EFI_OUT_OF_RESOURCES)
    {
        Print(L"out of resources for pool\n");
        return 0;
    }
    else if(status == EFI_INVALID_PARAMETER)
    {
        Print(L"invalid pool type\n");
        return 0;
    }
    else
    {
        Print(L"memory pool successfully allocated\n");
        return handle;
    }
}

void free(void * pool)
{
    EFI_STATUS status;
    Print(L"freeing memory pool\n");
    status = uefi_call_wrapper(BS->FreePool, 1, pool);

    if(status == EFI_INVALID_PARAMETER)
    {
        Print(L"invalid pool pointer\n");
    }
    else
    {
        Print(L"memory pool successfully freed\n");
    }
}