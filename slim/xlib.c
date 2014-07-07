// -------------------------------------------------------------
// Copyright (C) 2013- Adam Petrone

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// -------------------------------------------------------------
#include "config.h"
#include "xlib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if PLATFORM_LINUX || PLATFORM_ANDROID
	#include <errno.h>
#endif

#if PLATFORM_WINDOWS
	#include <windows.h>
#elif PLATFORM_LINUX || PLATFORM_APPLE || PLATFORM_ANDROID
	#include <dlfcn.h>
#else
	#error Unknown platform!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PARANOID 0

int xlib_open( xlib_t * lib, const char * library_path )
{
#if PLATFORM_WINDOWS
	// cache and set a new error mode
	unsigned int previous_error_mode = SetErrorMode( SEM_FAILCRITICALERRORS );
#endif

#if PARANOID
	if ( !lib )
	{
		printf( "Lib state is invalid!\n" );
		return 0;
	}

#endif

#if PLATFORM_WINDOWS
	// try to open the library
	lib->handle = LoadLibraryA( library_path );

	// restore error mode
	SetErrorMode( previous_error_mode );
#elif PLATFORM_LINUX || PLATFORM_APPLE || PLATFORM_ANDROID
	lib->handle = dlopen( library_path, RTLD_LAZY );
#endif

	if ( lib->handle == 0 )
	{
#if PLATFORM_WINDOWS
		printf( "Unable to open library: %i\n", GetLastError() );
#endif
		return 0;
	}
	
	return 1;
} // xlib_open

void xlib_close( xlib_t * lib )
{
	if ( lib->handle )
	{
#if PLATFORM_WINDOWS
		FreeLibrary( lib->handle );
		lib->handle = 0;
#elif PLATFORM_LINUX || PLATFORM_APPLE || PLATFORM_ANDROID
		dlclose( lib->handle );
#endif
	}
} // xlib_close

void * xlib_find_symbol( xlib_t * lib, const char * procname )
{
#if PARANOID
	if ( !lib )
	{
		printf( "Lib state is invalid!\n" );
		return 0;
	}
#endif

#if PLATFORM_WINDOWS
	return GetProcAddress( (HMODULE)lib->handle, (LPSTR)procname );
#elif PLATFORM_LINUX || PLATFORM_APPLE || PLATFORM_ANDROID
	return dlsym( lib->handle, procname );
#endif
} // xlib_find_symbol

#ifdef __cplusplus
}; // extern "C"
#endif
