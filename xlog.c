// -------------------------------------------------------------
// Copyright (C) 2012- Adam Petrone

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
#include "xlog.h"
#include <string.h> // for memcpy


#ifdef __cplusplus
extern "C"
{
#endif

static xlog_t * system_log;

void xlog_default( unsigned int type, const char * message, const char * function, const char * file, int linenumber )
{
	if ( system_log != 0 )
	{
		xmutex_lock( &system_log->log_mutex );
		
		xlog_message( system_log, message, file, function, linenumber, type );
		
		xmutex_unlock( &system_log->log_mutex );
	}
} // xlog_default

void xlog_init( xlog_t * log )
{
	unsigned int i;
	xlog_handler_t * slot;

#if PARANOID
	if ( !log )
	{
		return;
	}
#endif

	log->total_handlers = 0;

	// clear all data
	for( i = 0; i < XLOG_MAX_HANDLERS; ++i )
	{
		slot = &log->log_handlers[ i ];
		slot->close = 0;
		slot->open = 0;
		slot->message = 0;
		slot->userdata = 0;
	}
} // xlog_init

void xlog_add_handler( xlog_t * log, xlog_handler_t * handler )
{
	xlog_handler_t * slot;
#if PARANOID
	if ( !log || !handler )
	{
		return;
	}
#endif

	if ( log->total_handlers == XLOG_MAX_HANDLERS )
	{
		return;
	}

	if ( handler->open && handler->close && handler->message )
	{
		slot = &log->log_handlers[ log->total_handlers ];
		memcpy( slot, handler, sizeof(xlog_handler_t) );

		++log->total_handlers;
	}
} // xlog_add_handler

void xlog_set_default_log( xlog_t * log )
{
	system_log = log;
} // xlog_set_default_log


int xlog_open( xlog_t * log )
{
	unsigned int i;
	xlog_handler_t * handler;
	int log_handlers_opened = 0;

#if PARANOID
	if ( !log )
	{
		return;
	}
#endif

	// initialize all log handlers
	for( i = 0; i < log->total_handlers; ++i )
	{
		handler = &log->log_handlers[ i ];
		if ( !handler->open( handler ) )
		{
			handler->open = 0;
		}
		else
		{
			++log_handlers_opened;
		}
	}
	
	xmutex_create( &log->log_mutex );

	return log_handlers_opened;
} // xlog_open

void xlog_close( xlog_t * log )
{
	unsigned int i;
	xlog_handler_t * handler;

#if PARANOID
	if ( !log )
	{
		return;
	}
#endif

	// close all log handlers
	for( i = 0; i < log->total_handlers; ++i )
	{
		handler = &log->log_handlers[ i ];
		handler->close( handler );
	}
	
	xmutex_destroy( &log->log_mutex );
} // xlog_close

void xlog_message( xlog_t * log, const char * message, const char * filename, const char * function, int line, int type )
{
	unsigned int i;
	xlog_handler_t * handler;

#if PARANOID
	if ( !log )
	{
		return;
	}
#endif

	// dispatch message to all handlers
	for( i = 0; i < log->total_handlers; ++i )
	{
		handler = &log->log_handlers[ i ];
		if ( handler->open && handler->message && handler->close )
		{
			handler->message( handler, message, filename, function, line, type );
		}
	}
} // xlog_message

#ifdef __cplusplus
}
#endif