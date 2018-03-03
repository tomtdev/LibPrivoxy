#ifndef _LIBPRIVOXY_EXPORT_H
#define _LIBPRIVOXY_EXPORT_H

#ifdef LIBPRIVOXY_EXPORTS
#define LIBPRIVOXY_API __declspec(dllexport)
#else
#define LIBPRIVOXY_API __declspec(dllimport)
#endif

LIBPRIVOXY_API int __stdcall StartPrivoxy(char *config_full_path);

LIBPRIVOXY_API void __stdcall StopPrivoxy();

LIBPRIVOXY_API int __stdcall IsRunning();

#endif