// dllmain.c : Defines the entry point for the DLL application.
#include <windows.h>

HMODULE g_hLibPrivoxyModule;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (DLL_PROCESS_ATTACH == ul_reason_for_call) {
		g_hLibPrivoxyModule = hModule;
	}
	else if (DLL_PROCESS_DETACH == ul_reason_for_call) {
	}
	else if (DLL_THREAD_ATTACH == ul_reason_for_call) {
	}
	else if (DLL_THREAD_DETACH == ul_reason_for_call) {
	}

	return TRUE;
}

