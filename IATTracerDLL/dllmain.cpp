// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "framework.h"


#include <iostream>
#include "Util.h"

HMODULE WINAPI HookLoadLibraryW(_In_ LPCWSTR lpLibFileName);
HMODULE WINAPI HookLoadLibraryA(_In_ LPCSTR lpLibFileName);


typedef HANDLE (WINAPI* OrgCreateFileA)(_In_ LPCSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile);
typedef HANDLE (WINAPI* OrgCreateFileW)(_In_ LPCWSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile);
OrgCreateFileA orgCreateFileA = nullptr;
OrgCreateFileW orgCreateFileW = nullptr;
HANDLE WINAPI HookCreateFileA(_In_ LPCSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile);
HANDLE WINAPI HookCreateFileW(_In_ LPCWSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile);

typedef int (WINAPI*OrgMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType);
typedef int (WINAPI*OrgMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType);
OrgMessageBoxA orgMsgBoxA = nullptr;
OrgMessageBoxW orgMsgBoxW = nullptr;
int WINAPI HookMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType);
int WINAPI HookMessageBoxW( _In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType);



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {

        FILE* fp;
        if (AllocConsole())
        {
            std::wcout.imbue(std::locale("kor")); // locale 설정
            freopen_s(&fp, "CONIN$", "rb", stdin);
            freopen_s(&fp, "CONOUT$", "wb", stdout);
            freopen_s(&fp, "CONOUT$", "wb", stderr);

            // cout/cin/cerr을 사용하려면 아래의 주석을 지우면 된다.
            std::ios::sync_with_stdio();
        }

        PVOID pOldProc;
        Util::HookIAT("KERNELBASE.dll", "LoadLibraryA", HookLoadLibraryA, &pOldProc);
        Util::HookIAT("KERNELBASE.dll", "LoadLibraryW", HookLoadLibraryW, &pOldProc);

        Util::HookIAT("Kernel32.dll", "CreateFileA", HookCreateFileA, (LPVOID*)&orgCreateFileA);
        Util::HookIAT("Kernel32.dll", "CreateFileW", HookCreateFileW, (LPVOID*)&orgCreateFileW);

        Util::HookIAT("User32.dll", "MessageBoxA", HookMessageBoxA, (LPVOID*)&orgMsgBoxA);
        Util::HookIAT("User32.dll", "MessageBoxW", HookMessageBoxW, (LPVOID*)&orgMsgBoxW);


    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


HMODULE WINAPI HookLoadLibraryW(LPCWSTR lpLibFileName)
{
    std::wcout << "lpLibFileName [" << lpLibFileName << "]" << std::endl;

    return HMODULE();
}

HMODULE WINAPI HookLoadLibraryA(LPCSTR lpLibFileName)
{
    std::cout << "lpLibFileName [" << lpLibFileName << "]" << std::endl;

    return HMODULE();
}

HANDLE WINAPI HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    std::cout << "lpFileName [" << lpFileName << "]" << std::endl;
    return orgCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE WINAPI HookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    std::wcout << L"lpFileName [" << lpFileName << L"]" << std::endl;

    return orgCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

int WINAPI HookMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    std::cout << "lpText [" << lpText << "]" << std::endl;

    return orgMsgBoxA(hWnd, lpText, lpCaption, uType);
    //return ::MessageBoxA(hWnd, lpText, lpCaption, uType);
}

int WINAPI HookMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    std::wcout << L"lpText [" << lpText <<  L"]" << std::endl;

    return orgMsgBoxW(hWnd, lpText, lpCaption, uType);
    //return ::MessageBoxW(hWnd, lpText, lpCaption, uType);
}
