#include "libprivoxy.h"
#include "miscutil.h"
#include <assert.h>

char g_privoxy_config_full_path[1024] = { 0 };
extern HMODULE g_hLibPrivoxyModule = NULL;
extern int g_terminate;
extern void close_privoxy_listening_socket();

LIBPRIVOXY_API int __stdcall StartPrivoxy(char *config_full_path)
{
	g_terminate = 0;

	strcpy_s(g_privoxy_config_full_path, 1024, config_full_path);

	// start privoxy
	WinMain( NULL,NULL,NULL, 0);

	return 0;
}

LIBPRIVOXY_API void __stdcall StopPrivoxy()
{
	g_terminate = 1;
	close_privoxy_listening_socket();
}

LIBPRIVOXY_API int __stdcall IsRunning()
{
	return 1 == g_terminate ? 0 : 1;
}