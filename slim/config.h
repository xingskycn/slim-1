// -------------------------------------------------------------
// Copyright (C) 2004- Adam Petrone

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
#pragma once

// setup compiler defines
#if defined( __clang__ )
	#define PLATFORM_COMPILER "clang"
	#define PLATFORM_COMPILER_VERSION CONCAT_PERIOD( STRINGIZE(__clang_major__), STRINGIZE(__clang_minor__))
	#define PLATFORM_FANCY_FUNCTION __PRETTY_FUNCTION__
#elif defined( __GNUC__ )
	#define PLATFORM_COMPILER "gcc"
	#define PLATFORM_COMPILER_VERSION CONCAT_PERIOD( STRINGIZE(__GNUC__), STRINGIZE(__GNUC_MINOR__) )
	#define PLATFORM_FANCY_FUNCTION __PRETTY_FUNCTION__
#elif defined( _MSC_VER )
	#define PLATFORM_COMPILER "msvc"
	#if _MSC_VER < 1300
		#define PLATFORM_COMPILER_VERSION STRINGIZE(12.0)
	#elif _MSC_VER == 1300
		#define PLATFORM_COMPILER_VERSION STRINGIZE(13.0) // Visual Studio 2002
	#elif _MSC_VER == 1310
		#define PLATFORM_COMPILER_VERSION STRINGIZE(13.1)
	#elif _MSC_VER == 1400
		#define PLATFORM_COMPILER_VERSION STRINGIZE(14.0) // Visual Studio 2005
	#elif _MSC_VER == 1500
		#define PLATFORM_COMPILER_VERSION STRINGIZE(15.0) // Visual Studio 2008
	#elif _MSC_VER == 1600
		#define PLATFORM_COMPILER_VERSION STRINGIZE(16.0) // Visual Studio 2010
	#elif _MSC_VER == 1700
		#define PLATFORM_COMPILER_VERSION STRINGIZE(17.0) // Visual Studio 2012
	#else
		#define PLATFORM_COMPILER_VERSION "Unknown version of msvc"
	#endif
	#define PLATFORM_FANCY_FUNCTION __FUNCSIG__
#endif

// Define the following macros for this platform:
// PLATFORM_NAME - a string describing the OS at compile time (win32, linux, macosx)
#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN 1
	#endif

	#ifndef WIN32_EXTRA_LEAN
		#define WIN32_EXTRA_LEAN 1
	#endif

	// this is copied from Wine to prevent inclusion of unused definitions
	#define NOSERVICE
	#define NOMCX
	#define NOIME
	#define NOCOMM
	#define NOKANJI
	#define NORPC
	#define NOPROXYSTUB
	#define NOIMAGE
	#define NOTAPE

	#define PLATFORM_NAME "windows"
	#define PLATFORM_WINDOWS 1
#elif __linux__
	#if RASPBERRYPI
		// specifically built for RaspberryPi
		#define PLATFORM_RASPBERRYPI 1
		#define PLATFORM_NAME "raspberrypi"
	#else
		// generic flavor
		#define PLATFORM_NAME "linux"
	#endif

	#define PLATFORM_LINUX 1
#elif __APPLE__
	#include <TargetConditionals.h>

	#define PLATFORM_APPLE 1

	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#define PLATFORM_NAME "iphoneos"
		#define PLATFORM_IS_MOBILE 1
		#define PLATFORM_IOS 1
	#else
		#define PLATFORM_NAME "macosx"
		#define PLATFORM_MACOSX 1
	#endif
#elif __ANDROID__
	#define PLATFORM_NAME "Android"
	#define PLATFORM_ANDROID 1
	#define PLATFORM_IS_MOBILE 1
#else
	#error Unknown platform!
#endif