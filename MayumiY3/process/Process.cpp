#include "Process.h"
#include "../Options.h"

#include <TlHelp32.h>
#include <vector>
#include <corecrt_wstring.h>

/*															*\
	*													*
	*					MAYUMIY3						*
	*		HARDCODED PROCESS AND MODULE NAMES			*
	*													*
\*															*/

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
//Returns the address of a module
//
uintptr_t Process::getModuleAddr(Modules mod)
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
			return (uintptr_t)modEntry.modBaseAddr;
		}
	} while (Module32Next(hSnap, &modEntry));

	CloseHandle(hSnap);	//Couldn't find the module.
	throw std::runtime_error("Process: couldn't find module");
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
