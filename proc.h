#pragma once
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>

DWORD GetProcessId(const wchar_t* proc_name);

uintptr_t GetModuleBaseAddress(DWORD proc_id, const wchar_t* module_name);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);



