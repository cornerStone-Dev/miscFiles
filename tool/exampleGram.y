/*
 * grammar for a c-like language that is context free
*/

%include{
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

#include <stdio.h>
#define NDEBUG
#include <assert.h>

#define YYNOERRORRECOVERY 1
typedef struct tokenData {
	u8  *string;
	u64  data;
} tokenData;

typedef struct terminalData {
	u8  *siblings;
	u32  lineNumber;
	u32  stringLength;
	u8  *children;
} terminalData;

typedef struct context {
	u8  *string;
	u32  stringLength;
	u32  lineNumber;
} context;

#define YYSTACKDEPTH 150
}

%extra_context {context *c}

%token_type   {tokenData}
%default_type {terminalData}

%syntax_error {
	printf("%d\n", c->lineNumber);
}

program ::= terminal_list(A).
{
	printf("parse done, input exhausted.\n");
	c->lineNumber = A.lineNumber;
}

terminal_list ::= TERMINAL.
terminal_list ::= terminal_list TERMINAL.


%code{
s32
exampleParse(void *yyp,                   /* The parser */
	int yymajor,                 /* The major token code number */
	tokenData *yyminorp       /* The value for the token */
	)
{
	tokenData yyminor = *yyminorp;
	Parse(yyp, yymajor, yyminor);
	return 0;
}
}
