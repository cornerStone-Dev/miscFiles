// scgi.h

#ifndef SCGI_H
#define SCGI_H
//#define NDEBUG
/*******************************************************************************
 * SECTION includes and typdefs
*******************************************************************************/
#ifndef NDEBUG
#include <stdio.h>
#endif
#include <stdint.h>
typedef uint8_t   u8;
typedef int8_t    s8;
typedef uint16_t  u16;
typedef int16_t   s16;
typedef uint32_t  u32;
typedef int32_t   s32;
typedef uint64_t  u64;
typedef int64_t   s64;
typedef float     f32;
typedef double    f64;

/*******************************************************************************
 * SECTION Header Interface X MACRO
*******************************************************************************/
// interface for code
// X(headerName, nginxHeaderLabel)
#define LIST_OF_HEADERS \
	X(utility, U) \
	X(requestMethod, M)

/*******************************************************************************
 * SECTION Interface struct, enum for return values, and functions
*******************************************************************************/
// Needs to be a multiple of 8 bytes in size
typedef struct scgiInfo {
	u64  contentLength;
	u8  *contentStart;
#define X(name, label) u8  *name;
LIST_OF_HEADERS
#undef X
} scgiInfo;

enum scgiReturnCode{
	scgiErrorContentLengthMissing = -1,
	scgiOK = 0,
	scgiUnprocessedHeaders = 1
};

// returns scgiReturnCode, you can use the debug print below to print out
// the context information.
// If function is successful will have filled in all header locations into
// the provided struct.
s32
scgiProcessRequest(
	scgiInfo *scgi, // pointer to memory for scgi struct
	u8 *request);   // pointer to first byte of scgi request
	

#ifndef NDEBUG
u8*
scgiDebugPrintReturnCode(s32 code);
#endif

#endif
