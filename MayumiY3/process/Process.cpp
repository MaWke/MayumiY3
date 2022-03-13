#include "Process.h"

#include <vector>
#include <corecrt_wstring.h>
#include <memory>
#include <string.h>

/*															*\
	*													*
	*					MAYUMIY3						*
	*		HARDCODED PROCESS AND MODULE NAMES			*
	*													*
\*															*/

//@todo could do the optimization here so everything that can be is calculated only once
//currently just using "static inptr_t moduleAddr = getModuleAddr()" everywhere
//						  ^^

namespace process {
	const std::wstring processName = L"csgo.exe";
}

//
//MODULE NAMES
//
static std::wstring moduleName(Process::Modules mod)
{
	switch(mod)
	{
	case Process::Modules::CLIENTDLL:
		return L"client.dll";
	case Process::Modules::ENGINEDLL:
		return L"engine.dll";
	default:
		throw std::runtime_error("Unknown module name!");
	}
}
//-----------------------------------------------------------------------

//
//Returns process ID
//
DWORD Process::getProcessID() 
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);		//snapshot of processes

	if (hSnap == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Process: couldn't take snapshot of processes");

	PROCESSENTRY32 procEntry{};
	procEntry.dwSize = sizeof(procEntry);

	Process32First(hSnap, &procEntry);
	do
	{
		if (!_wcsicmp(procEntry.szExeFile, process::processName.c_str())) {
			DWORD procId = procEntry.th32ProcessID;	
			CloseHandle(hSnap);
			return procId;
		}
	} while (Process32Next(hSnap, &procEntry));
	CloseHandle(hSnap);		//Couldn't find the process.
	throw std::runtime_error("Process: couldn't find the process");		
}
//-----------------------------------------------------------------------

//
//Returns the MODULEENTRY32 of a module
//
MODULEENTRY32 Process::getModuleEntry(Modules mod)
{
	const std::wstring modName = moduleName(mod);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, getProcessID());	//snapshot of modules
	if (hSnap == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Process: couldn't take snapshot of modules");

	MODULEENTRY32 modEntry{};
	modEntry.dwSize = sizeof(modEntry);

	Module32First(hSnap, &modEntry);
	do
	{
		if (!_wcsicmp(modEntry.szModule, modName.c_str())) {
			CloseHandle(hSnap);	//Module found!
			return modEntry;
		}
	} while (Module32Next(hSnap, &modEntry));

	CloseHandle(hSnap);	//Couldn't find the module.
	throw std::runtime_error("Process: couldn't find the module");
}
//-----------------------------------------------------------------------

//
//Returns the address of a module
//
uintptr_t Process::getModuleAddr(Modules mod)
{
	MODULEENTRY32 modEntry = getModuleEntry(mod);
	return (uintptr_t)modEntry.modBaseAddr;
}
//-----------------------------------------------------------------------

//
//Opens a handle to the process and returns it
//
HANDLE& Process::getProcessHandle(DWORD procId)
{
	static HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
	return pHandle;
}
//-----------------------------------------------------------------------
//
//Pattern input parser
//made by https://github.com/GH-Rake
static void parsePattern(const char* combo, char* pattern, char* mask)
{
	char lastChar = ' ';
	unsigned int j = 0;

	for (unsigned int i = 0; i < strlen(combo); i++)
	{
		if ((combo[i] == '?' || combo[i] == '*') && (lastChar != '?' && lastChar != '*'))
		{
			pattern[j] = mask[j] = '?';
			j++;
		}

		else if (isspace(lastChar))
		{
			pattern[j] = lastChar = (char)strtol(&combo[i], 0, 16);
			mask[j] = 'x';
			j++;
		}
		lastChar = combo[i];
	}
	pattern[j] = mask[j] = '\0';
}

//
//Pattern scanning
//
static bool scanRegionForPattern(const char* pattern, const char* mask, const std::unique_ptr<char[]>& buffer, intptr_t readSize, uintptr_t* offsetInRegion)
{
	for (int i = 0; i < readSize; ++i) {
		bool found = true;
		for (int j = 0; j < (int)strlen(pattern); ++j)
			if (pattern[j] != buffer[i + j] && mask[j] != '?') {
				found = false;
				break;
			}
		if (found) {
			*offsetInRegion = i;
			return true;
		}
	}
	return false;
}

//
//Returns location of a pattern in a module
//
uintptr_t Process::findPattern(Modules mod, const char* patternCombo, std::vector<int> offsets, int extra)
{
	//Parse input
	char pattern[100];
	char mask[100];
	parsePattern(patternCombo, pattern, mask);

	HANDLE pHandle = getProcessHandle(getProcessID());
	MODULEENTRY32 modEntry = getModuleEntry(mod);
	uintptr_t modSize = modEntry.modBaseSize;
	uintptr_t modBaseAddr = (intptr_t)modEntry.modBaseAddr;

	MEMORY_BASIC_INFORMATION mbi;
	mbi.RegionSize = 0x1000;	//Scan one region at a time

	for (uintptr_t current = modBaseAddr; current < modBaseAddr + modSize; current += mbi.RegionSize)
		//Loop through every region and scan valid memory for the pattern
	{
		VirtualQueryEx(pHandle, (LPCVOID)current, &mbi, sizeof(mbi));
		if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS) continue;

		//save memory's protect settings
		DWORD oldProtect;
		if (!VirtualProtectEx(pHandle, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldProtect)) continue;

		//read memory
		auto buffer = std::unique_ptr<char[]>(new char[mbi.RegionSize]);
		SIZE_T readSize;
		ReadProcessMemory(pHandle, mbi.BaseAddress, buffer.get(), mbi.RegionSize, &readSize);

		//set old protect settings back
		VirtualProtectEx(pHandle, mbi.BaseAddress, mbi.RegionSize, oldProtect, &oldProtect);

		uintptr_t offsetInRegion;
		if (scanRegionForPattern(pattern, mask, buffer, readSize, &offsetInRegion)) {
			uintptr_t result = current + offsetInRegion;
			if (!offsets.empty())
				for(int offset : offsets)
				result = readMem<int>(result + offset);

			return result + extra;
		}
	}
	
	throw std::runtime_error("Can't find " + pattern[5]);
}
//-----------------------------------------------------------------------

//
//Closes the handle to the process
//If the function succeeds, the return value is true
//
bool Process::closeProcessHandle(DWORD procId)
{
	if(CloseHandle(getProcessHandle(procId)))
		return true;
	return false;
}
//-----------------------------------------------------------------------
