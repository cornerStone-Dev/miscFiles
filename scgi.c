// scgi.c

#include "scgi.h"

/*******************************************************************************
 * SECTION Internal Structures
*******************************************************************************/

typedef struct Pair {
	u64 x;
	u64 y;
} Pair;

/*******************************************************************************
 * SECTION Internal Funtions
*******************************************************************************/

static s32
stringCompare(u8 *str1, u8 *str2)
{
	s32 c1, c2;
	
	while(1){
		c1=*str1;
		str1+=1;
		c2=*str2;
		str2+=1;
		c1-=c2;
		if( (c1!=0) || (c2==0) ){
			return c1;
		}
	}
}

static u8*
skipHeaderValue(u8 *b)
{
	while(*b!=0)
	{
		b++;
	}
	// skip terminating null
	b++;
	return b;
}

// returns pair
// x: unsigned integer value converted from text
// y: pointer to next u8 to process
static Pair
getUnsignedNumber(u8 *b)
{
	Pair p = {0};
	
	while( (*b >= '0') && (*b <= '9') )
	{
		p.x = (p.x * 10) + ((u64)*b - '0');
		b++;
	}
	
	p.y = (u64)b;
	return p;
}

/*******************************************************************************
 * SECTION Main Funtion Interface
*******************************************************************************/

s32
scgiProcessRequest(scgiInfo *scgi, u8 *request)
{
	Pair p;
	s32 retVal = scgiUnprocessedHeaders;
	
	// reset scgiInfo to zero
	{
		u64 *memory = (u64*)scgi;
		u64 size = sizeof(scgiInfo)/8;
		u64 i = 0;
		do {
			memory[i++] = 0;
		} while(i < size);
	}
	
	// process parameter length (first part of netstring)
	p = getUnsignedNumber(request);
	request = (u8*)p.y;
	// the start of content (if any) is past the parameters and ':' and ','
	scgi->contentStart = request + p.x + 1 + 1;
	
	// move past ':'
	request++;
	
	if (stringCompare((u8*)"CONTENT_LENGTH", request) != 0)
	{
		return scgiErrorContentLengthMissing;
	}
	
	// move past header name
	request += sizeof "CONTENT_LENGTH";
	if (*request != 0)
	{
		// content length is always a number, so read it
		p = getUnsignedNumber(request);
		// returns 0 is there is nothing, store result
		scgi->contentLength = p.x;
		request = (u8*)p.y;
	}
	
	request = skipHeaderValue(request);
	if (*request == ',')
	{
		retVal = scgiOK;
		goto end;
	}
	
	/// header code here, order matters!
	
	#define X(name, label) \
	/* see if header is there */ \
	if (stringCompare((u8*)#label, request) == 0) \
	{ \
		/* move past header name */ \
		request += sizeof #label; \
		/* record location if anything is there */ \
		if (*request != 0) \
		{ \
			 scgi->name = request; \
		} \
		/* skip the header value content */ \
		request = skipHeaderValue(request); \
		/* see if the end of the scgi headers is reached */ \
		if (*request == ',') \
		{ \
			retVal = scgiOK; \
			goto end; \
		} \
	}
	LIST_OF_HEADERS
	#undef X

	end:
#ifndef NDEBUG
	{
		printf("contentLength = %ld\n", scgi->contentLength);
		#define X(name, label) printf( #name " = %s\n", scgi->name);
		LIST_OF_HEADERS
		#undef X
	}
#endif
	
	return retVal;
}

/*******************************************************************************
 * SECTION Debug Funtion Interface
*******************************************************************************/
#ifndef NDEBUG
u8*
scgiDebugPrintReturnCode(s32 code)
{
	switch(code){
		case scgiOK:
		return (u8*)"scgiOK: normal return value.";
		break;
		
		case scgiErrorContentLengthMissing:
		return (u8*)"scgiErrorContentLengthMissing: Content Length Header is "
			"missing. SCGI protocol error.";
		break;
		
		case scgiUnprocessedHeaders:
		return (u8*)"scgiUnprocessedHeaders: Some headers were not processed."
			"The end of the netstring was not detected.";
		break;
		
		default:
		return (u8*)"Not a valid scgiProcessRequest return code.";
	}
}
#endif




















