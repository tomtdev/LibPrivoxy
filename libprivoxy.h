#ifndef _LIBPRIVOXY_EXPORT_H
#define _LIBPRIVOXY_EXPORT_H

/** @brief Many people want to integrate Privoxy into their own projects, 
* I made some changes to the Privoxy codes to compiled into a DLL or static library, 
* so that you can better integrate it into their own projects. https://www.sockscap64.com
*
* @Author: Taro, https://www.sockscap64.com
*/

#ifdef LIBPRIVOXY_EXPORTS
#define LIBPRIVOXY_API __declspec(dllexport)
#else
#define LIBPRIVOXY_API __declspec(dllimport)
#endif

extern "C" LIBPRIVOXY_API int __stdcall StartPrivoxy(char *config_full_path);

extern "C" LIBPRIVOXY_API void __stdcall stop_privoxy();

extern "C" LIBPRIVOXY_API int __stdcall is_privoxy_started();
#endif