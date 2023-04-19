#include <efi.h>
#include <efilib.h>
#include "memory.h"
#include "filesystem.h"

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    Print(L"Hello, world!\n");
    EFI_FILE_HANDLE Volume = GetVolume(ImageHandle);
    CHAR16 * fileContent = (CHAR16 *)ReadFileTooBuffer(L"Hello.txt", Volume);
    Print(fileContent);
    uefi_free(fileContent);
    return EFI_SUCCESS;
}