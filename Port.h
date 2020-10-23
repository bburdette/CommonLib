#ifndef PortTypes_h
#define PortTypes_h

#ifdef WIN32

#ifndef DONT_INCLUDE_WINDOWS_H
#include <windows.h>
#endif

#define PI 3.1415926535897932384626433832795

#pragma warning (disable: 4244 4305 4786)

typedef __int64 int64_t;
//#define int64_t __int64

#include <stdlib.h>

inline __int64 atoll(const char *aC) { return _atoi64(aC); }

#else  // WIN32
// not win32...
#include <stdlib.h>
#endif  // WIN32

#define int16 short
#define int32 int
#define uint32 unsigned int
#define bigtime_t __int64

// #define MULTICHAR(a,b,c,d) ((d) | ((c) << 8) | ((b) << 16) | ((a) << 24))

#include <assert.h>
#include <string.h>

inline unsigned int MultiChar(const char *aC)
{
	assert(strlen(aC) == 4);
	unsigned int lIRc = ((aC[3]) | ((aC[2]) << 8) | ((aC[1]) << 16) | ((aC[0]) << 24));
	return lIRc;
}

// milliseconds since program start (wall clock time).
// defined in GameSDL as of now.
extern int MsClock();

#endif
