#undef TRACE_SYSTEM
#define TRACE_SYSTEM blocktrace

#if !defined(_TRACE_BLOCK_EVENT_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_BLOCK_EVENT_H

#include <linux/tracepoint.h>

/*
 * The TRACE_EVENT macro is broken up into 5 parts.
 *
 * name: name of the trace point. This is also how to enable the tracepoint.
 *   A function called trace_foo_bar() will be created.
 *
 * proto: the prototype of the function trace_foo_bar()
 *   Here it is trace_foo_bar(char *foo, int bar).
 *
 * args:  must match the arguments in the prototype.
 *    Here it is simply "foo, bar".
 *
 * struct:  This defines the way the data will be stored in the ring buffer.
 *    There are currently two types of elements. __field and __array.
 *    a __field is broken up into (type, name). Where type can be any
 *    primitive type (integer, long or pointer). __field_struct() can
 *    be any static complex data value (struct, union, but not an array).
 *    For an array. there are three fields. (type, name, size). The
 *    type of elements in the array, the name of the field and the size
 *    of the array.
 *
 *    __array( char, foo, 10) is the same as saying   char foo[10].
 *
 * fast_assign: This is a C like function that is used to store the items
 *    into the ring buffer.
 *
 * printk: This is a way to print out the data in pretty print. This is
 *    useful if the system crashes and you are logging via a serial line,
 *    the data can be printed to the console using this "printk" method.
 *
 * Note, that for both the assign and the printk, __entry is the handler
 * to the data structure in the ring buffer, and is defined by the
 * TP_STRUCT__entry.
 */
TRACE_EVENT(blockIO,

	TP_PROTO(int maj,int min,char RW,unsigned long long sector,unsigned int size),

	TP_ARGS(maj,min,RW,sector,size),

	TP_STRUCT__entry(
		__field(	int,	maj			)
		__field(	int,	min			)
		__field(	char,	RW			)
		__field(	unsigned long long,	sector			)
		__field(	unsigned int,	size			)
	),

	TP_fast_assign(
		__entry->maj	= maj;
		__entry->min	= min;
		__entry->RW	= RW;
		__entry->sector	= sector;
		__entry->size	= size;
		
	),

	TP_printk("%d,%d %c %llu + %u", __entry->maj, __entry->min,__entry->RW,__entry->sector,__entry->size)
);
#endif

/***** NOTICE! The #if protection ends here. *****/


/*
 * There are several ways I could have done this. If I left out the
 * TRACE_INCLUDE_PATH, then it would default to the kernel source
 * include/trace/events directory.
 *
 * I could specify a path from the define_trace.h file back to this
 * file.
 *
 * #define TRACE_INCLUDE_PATH ../../samples/trace_events
 *
 * But the safest and easiest way to simply make it use the directory
 * that the file is in is to add in the Makefile:
 *
 * CFLAGS_trace-events-sample.o := -I$(src)
 *
 * This will make sure the current path is part of the include
 * structure for our file so that define_trace.h can find it.
 *
 * I could have made only the top level directory the include:
 *
 * CFLAGS_trace-events-sample.o := -I$(PWD)
 *
 * And then let the path to this directory be the TRACE_INCLUDE_PATH:
 *
 * #define TRACE_INCLUDE_PATH samples/trace_events
 *
 * But then if something defines "samples" or "trace_events" as a macro
 * then we could risk that being converted too, and give us an unexpected
 * result.
 */
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_PATH .
/*
 * TRACE_INCLUDE_FILE is not needed if the filename and TRACE_SYSTEM are equal
 */
#define TRACE_INCLUDE_FILE trace-block-events
#include <trace/define_trace.h>
