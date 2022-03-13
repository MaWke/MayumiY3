#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <stdexcept>
#include <vector>

namespace Process
{
	enum class Modules {
		CLIENTDLL,
		ENGINEDLL
	};

	HANDLE& getProcessHandle(DWORD procId);
	bool closeProcessHandle(DWORD procId);

	DWORD getProcessID();
	MODULEENTRY32 getModuleEntry(Modules mod);
	uintptr_t getModuleAddr(Modules mod);
	uintptr_t findPattern(Modules mod, const char* patternCombo, std::vector<int> offsets = {}, int extra = 0);

	//
	//ReadProcessMemory and WriteProcessMemory templates
	//
	template <class T>
	T readMem(uintptr_t addr)
	{
		static HANDLE pHandle = getProcessHandle(getProcessID());
		T read;
		ReadProcessMemory(pHandle, (LPCVOID)addr, &read, sizeof(read), NULL);
		return read;
	}

	template <class T>
	bool writeMem(uintptr_t addr, T write)
	{
		static HANDLE pHandle = getProcessHandle(getProcessID());
		return WriteProcessMemory(pHandle, (LPVOID)addr, &write, sizeof(write), NULL);
	}
}

