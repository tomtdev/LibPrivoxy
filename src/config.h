#ifndef _LIBPRIVOXY_CONFIG_
#define _LIBPRIVOXY_CONFIG_

#define VERSION "3.0.26"
#define CODE_STATUS "stable"
#define VERSION_MAJOR 3
#define VERSION_MINOR 0
#define VERSION_POINT 26

#define HAVE_SNPRINTF 1

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp  _strnicmp
#define strdup _strdup
#endif

#ifdef LIBPRIVOXY
#define exit libprivoxy_exit
#endif

#define FEATURE_CONNECTION_KEEP_ALIVE

#endif

/*
  Local Variables:
  tab-width: 3
  end:
*/
