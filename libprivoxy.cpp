#include "LibPrivoxy/stdafx.h"
#include "libprivoxy.h"
#include "miscutil.h"
#include <assert.h>

/** @brief Many people want to integrate Privoxy into their own projects, 
* I made some changes to the Privoxy codes to compiled into a DLL or static library, 
* so that you can better integrate it into their own projects. https://www.sockscap64.com
*
* @Author: Taro, https://www.sockscap64.com
*/

static bool g_privoxy_service_started = false;
extern "C" char g_privoxy_config_full_path[1024] = { 0 };
extern "C" HMODULE g_hLibPrivoxyModule = NULL;
extern "C" int g_terminate;

extern "C" void close_privoxy_listening_socket();

extern "C" LIBPRIVOXY_API int __stdcall StartPrivoxy(char *config_full_path)
{
	g_terminate = 0;

	strcpy_s(g_privoxy_config_full_path, 1024, config_full_path);

	// start privoxy
	WinMain( NULL,NULL,NULL, 0);

	g_privoxy_service_started = true;

	return 0;
}

extern "C" LIBPRIVOXY_API void __stdcall stop_privoxy()
{
	//close_privoxy_listening_socket();

	g_terminate = 1;

	g_privoxy_service_started = false;
}

extern "C" LIBPRIVOXY_API int __stdcall is_privoxy_started()
{
	return g_privoxy_service_started;
}
