#pragma once
#include <Windows.h>


namespace mem
{

	template<typename T>
	T RPM(uintptr_t address, HANDLE handle)
	{
		T buffer;
		ReadProcessMemory(handle, (BYTE*)address, &buffer, sizeof(buffer), nullptr);
		return buffer;
	}

	template <typename T>
	void WPM(uintptr_t address, T buffer, HANDLE handle)
	{
		WriteProcessMemory(handle, (BYTE*)address, &buffer, sizeof(address), nullptr);
	}



	void PatchEx(BYTE* dest, BYTE* src, unsigned int size, HANDLE hProc);

	void NopEx(BYTE* dest, unsigned int size, HANDLE hProc);
}
