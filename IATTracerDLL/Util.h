#pragma once
#include <Windows.h>
#include <iostream>

#define PtrFromRva( base, rva ) ( ( ( PBYTE ) base ) + rva )

namespace Util
{
	BOOL HookIAT(const char* szModuleName, const char* szFuncName, PVOID pNewFunc, PVOID* pOldFunc);
	BOOL ShowIAT();
}
