#include <efi.h>
#include <efilib.h>
#include "memory.h"
#include "filesystem.h"

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
    Print(L"Hello, world!\n");
    EFI_FILE_HANDLE Volume = GetVolume(ImageHandle);
    CHAR8 *fileContent = (CHAR8 *)ReadFileTooBuffer(L"hello.txt", Volume);
    AsciiPrint(fileContent);
    uefi_free(fileContent);

    while (1)
    {
        ;
    }

    return EFI_SUCCESS;
}