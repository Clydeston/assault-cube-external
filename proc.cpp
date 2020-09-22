#include "proc.h"

/*
Returning uintptr_t so the functions will still return the correct values on any architecture x86 or x64 architecture independent variable
*/

DWORD GetProcessId(const wchar_t* proc_name) 
{
	DWORD procID = 0;
	// snapshot of all running processes
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// only fail return value - checking for it
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		// entry from process list 
		PROCESSENTRY32 procEntry;
		// size of the structure in bytes - must initialize size otherwise Process32First fails 
		procEntry.dwSize = sizeof(procEntry);

		// first process in system processes snapshotcc
		if (Process32First(hSnap, &procEntry))
		{
			// do while loop to find the correct process by name
			do
			{
				/*
				_wcsicmp - case insesitive string comparison
				szExeFile = name of executable file for the process
				returns 0 if string 1 is identical to string 2 
				*/
				if (!_wcsicmp(procEntry.szExeFile, proc_name))
				{
					// process Id of process
					procID = procEntry.th32ProcessID;
					break;
				}
				// information from next process in system snapshot
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	// closing open object handle
	CloseHandle(hSnap);
	// returns process id
	return procID;
}

uintptr_t GetModuleBaseAddress(DWORD proc_id, const wchar_t* module_name) 
{
	uintptr_t modBaseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, module_name))
				{
					modBaseAddress = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}		
	}
	CloseHandle(hSnap);
	return modBaseAddress;
}

//hproc = process handle ptr = base pointer offset = vector of various offsets for multi level pointer
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t address = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)address, &address, sizeof(address), 0);
		address += offsets[i];
	}
	return address;
}
