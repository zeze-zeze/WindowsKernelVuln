#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\SE64"

HANDLE hDevice;

struct DATA
{
    int Port;
    int Value;
    int haha;
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
    DATA data;
    data.Port = 0xcf9;
    data.Value = 0xe;
    DeviceIoControl(hDevice, 0x80002014, &data, 12, NULL, 0, &dwWrite, NULL);

    system("pause");
    return 0;
}
