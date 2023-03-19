#include "msrexec.hpp"
#include "vdm.hpp"
#include <iostream>

using ex_alloc_pool_t = void* (*)(std::uint32_t, std::size_t);
using dbg_print_t = void (*)(const char*, ...);

int __cdecl main(int argc, char** argv)
{
    writemsr_t _write_msr = [&](std::uint32_t reg, std::uintptr_t value) -> bool
    {
        struct DATA_9C402088
        {
            unsigned long Register;
            unsigned long Value_low;
            unsigned long Value_high;
        };
        DWORD dwWrite;
        HANDLE hDevice =
            CreateFile(L"\\\\.\\WiseHDInfo", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
        DATA_9C402088 data;
        data.Register = reg;
        data.Value_low = value & 0xffffffff;
        data.Value_high = (value >> 32) & 0xffffffff;
        DeviceIoControl(hDevice, 0x9C402088, &data, sizeof(data), NULL, 0, &dwWrite, NULL);
        return true;
    };

    vdm::msrexec_ctx msrexec(_write_msr);
    msrexec.exec(
        [&](void* krnl_base, get_system_routine_t get_kroutine) -> void
        {
            const auto dbg_print = reinterpret_cast<dbg_print_t>(get_kroutine(krnl_base, "DbgPrint"));

            const auto ex_alloc_pool = reinterpret_cast<ex_alloc_pool_t>(get_kroutine(krnl_base, "ExAllocatePool"));

            dbg_print("> allocated pool -> 0x%p\n", ex_alloc_pool(NULL, 0x1000));
            dbg_print("> cr4 -> 0x%p\n", __readcr4());
            dbg_print("> hello world!\n");
        });

    std::printf("completed tests...\n");
    std::getchar();
}