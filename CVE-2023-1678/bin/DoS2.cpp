#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\MyDrivers0_0_1"

HANDLE hDevice;

struct DATA_9C40A0D8
{
    int Port;
    int Data;
};
void outbyte_9C40A0D8(int Port, int Data)
{
    DWORD dwWrite;
    DATA_9C40A0D8 data;
    data.Port = Port;
    data.Data = Data;
    DeviceIoControl(hDevice, 0x9C40A0D8, &data, sizeof(data), NULL, 0, &dwWrite, NULL);
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

    outbyte_9C40A0D8(0xcf9, 0xe);

    system("pause");
    return 0;
}
