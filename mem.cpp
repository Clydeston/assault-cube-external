#include "mem.h"

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	// creating byte pointer - creating new byte array on heap, assigning address of array to nopArray ptr
	BYTE* nopArray = new BYTE[size];
	// setting each byte in array to 0x90, 0x90 = no operation instruction
	memset(nopArray, 0x90, size);

	// patching values from nop array into memory destination
	PatchEx(dst, nopArray, size, hProcess);
	// deleting nop array from heap
	delete[] nopArray;
}