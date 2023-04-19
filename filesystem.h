#include <efi.h>
#include <efilib.h>
#include "memory.h"

EFI_FILE_HANDLE GetVolume(EFI_HANDLE image)
{
    EFI_LOADED_IMAGE *loaded_image = NULL;                  /* image interface */
    EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;      /* image interface GUID */
    EFI_FILE_IO_INTERFACE *IOVolume;                        /* file system interface */
    EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID; /* file system interface GUID */
    EFI_FILE_HANDLE Volume;                                 /* the volume's interface */

    /* get the loaded image protocol interface for our "image" */
    uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGuid, (void **)&loaded_image);
    /* get the volume handle */
    uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &fsGuid, (VOID *)&IOVolume);
    uefi_call_wrapper(IOVolume->OpenVolume, 2, IOVolume, &Volume);
    return Volume;
}

void *ReadFileTooBuffer(CHAR16 *FileName)
{
    EFI_FILE_HANDLE FileHandle;

    /* open the file */
    uefi_call_wrapper(Volume->Open, 5, Volume, &FileHandle, FileName, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

    UINT64 ReadSize = FileSize(FileHandle);
    UINT8 *Buffer = malloc(ReadSize);

    uefi_call_wrapper(FileHandle->Read, 3, FileHandle, &ReadSize, Buffer);

    /* close the file */
    uefi_call_wrapper(FileHandle->Close, 1, FileHandle);

    return Buffer;
}