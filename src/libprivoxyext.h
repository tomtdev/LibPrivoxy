#ifndef _LIBPRIVOXYEXT_H_
#define _LIBPRIVOXYEXT_H_

#ifdef WIN32
	int	gettimeofday(struct timeval *tp, void *tzp);
#endif

	void libprivoxy_exit(int _Code);
	int __stdcall libprivoxy_FreeConsole(void);
#endif