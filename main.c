#include <efi.h>
#include <efilib.h>
#include "strops.h"
#include "memory.h"
#include "filesystem.h"

SIMPLE_INPUT_INTERFACE *CI;

int ReadLine(CHAR16 *buf, int l)
{
    int i = 0;
    EFI_INPUT_KEY key;
    while (1)
    {
        if (!uefi_call_wrapper(CI->ReadKeyStroke, 2, CI, &key))
        {
            if (key.UnicodeChar == CHAR_LINEFEED || key.UnicodeChar == CHAR_CARRIAGE_RETURN)
            {
                Print(L"\n");
                break;
            }
            else if (key.UnicodeChar == CHAR_BACKSPACE || key.ScanCode == SCAN_DELETE)
            {
                if (i)
                    i--;
                Print(L"\b");
                buf[i] = 0;
                continue;
            }
            else if (key.ScanCode == SCAN_ESC)
            {
                buf[0] = 0;
                return 0;
            }
            else if (key.UnicodeChar && i < l - 1)
            {
                CHAR16 tmp[2];
                tmp[0] = key.UnicodeChar;
                tmp[1] = 0;
                Print(tmp);
                buf[i++] = key.UnicodeChar;
                buf[i] = 0;
            }
        }
    }
    return i;
}

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

    CI = SystemTable->ConIn;

    EFI_INPUT_KEY Key;
    CHAR16 CommandBuffer[256] = L"";

    void *tmp;
    BOOLEAN running = TRUE;

    Print(L">");
    while (running)
    {
        ReadLine(CommandBuffer, 256);
        if (StrCmp(CommandBuffer, L"!test\0") == 0)
        {
            Print(L"Test\n");
        }
        else if (StartsWith(CommandBuffer, L"!echo ", 6))
        {
            tmp = Strip(CommandBuffer, 6);
            Print((CHAR16 *)tmp);
            Print(L"\n");
        }
        else if (StrCmp(CommandBuffer, L"!exit\0") == 0)
        {
            Print(L"Test");
            running = FALSE;
        }
        else
        {
            StrCat(CommandBuffer, L".efi");
            if (FileExists(CommandBuffer, Volume))
            {
                void *buffer = ReadFileTooBuffer(CommandBuffer, Volume);
                void (*command)(void) = (void (*)())buffer;
                command();
                uefi_free(buffer);
            }
            else
            {
                Print(L"Unknown command.\n");
            }
        }
        Print(L">");
    }

    return EFI_SUCCESS;
}