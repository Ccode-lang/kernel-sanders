#include <efi.h>
#include <efilib.h>
#include "filesystem.h"
#include "memory.h"

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    Print(L"Hello, world!\n");
    EFI_FILE_HANDLE Volume = GetVolume(ImageHandle);
    CHAR16 * fileContent = (CHAR16 *)ReadFileTooBuffer(Volume);
    Print(fileContent);
    free(fileContent);
    return EFI_SUCCESS;
}