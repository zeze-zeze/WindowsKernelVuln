﻿#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\ProcObsrvesx"

HANDLE hDevice;


int main(int argc, char* argv[])
{
    hDevice = CreateFile(SymLinkName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        printf("Get Driver Handle Error with Win32 error code: %x\n", GetLastError());
        system("pause");
        return 0;
    }

    DWORD dwWrite;
    int a = 0;
    while (1)
        DeviceIoControl(hDevice, 0x22e008, &a, 4, &a, 4, &dwWrite, NULL);

    system("pause");
    return 0;
}
