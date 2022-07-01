/* machine.h.  Generated automatically by configure.  */
/* Any machine specific stuff goes here */
/* Add details necessary for your own installation here! */

/* RCS id: $Id: machine.h,v 1.6 2015/02/07 20:07:04 xavier Exp $ */

/* This is for use with "configure" -- if you are not using configure
	then use machine.van for the "vanilla" version of machine.h */

/* Note special macros: ANSI_C (ANSI C syntax)
			SEGMENTED (segmented memory machine e.g. MS-DOS)
			MALLOCDECL (declared if malloc() etc have
					been declared) */


#ifndef MACHINEHEADER
#define MACHINEHEADER

/* #undef const */

/* #undef MALLOCDECL */
#define NOT_SEGMENTED 1
/* #undef HAVE_MEMORY_H */
/* #undef HAVE_COMPLEX_H */
#define HAVE_MALLOC_H 1
#define STDC_HEADERS 1
#define HAVE_BCOPY 1
#define HAVE_BZERO 1
#define CHAR0ISDBL0 1
#define WORDS_BIGENDIAN 1
/* #undef U_INT_DEF */
/* #define VARARGS 1 */
#define ANSI_C 1
#define HAVE_PROTOTYPES 1
/* #undef HAVE_PROTOTYPES_IN_STRUCT */

/* for inclusion into C++ files */
#ifdef __cplusplus
#define ANSI_C 1
#ifndef HAVE_PROTOTYPES 
#define HAVE_PROTOTYPES 1
#endif
#ifndef PROTOTYPES_IN_STRUCT
#define PROTOTYPES_IN_STRUCT 1
#endif

#undef HAVE_MALLOC_H 

#endif /* __cplusplus */

/* example usage: VEC *PROTO(v_get,(int dim)); */
#ifdef HAVE_PROTOTYPES
#define	PROTO(name,args)	name args
#else
#define PROTO(name,args)	name()
#endif /* HAVE_PROTOTYPES */
#ifdef HAVE_PROTOTYPES_IN_STRUCT
/* PROTO_() is to be used instead of PROTO() in struct's and typedef's */
#define	PROTO_(name,args)	name args
#else
#define PROTO_(name,args)	name()
#endif /* HAVE_PROTOTYPES_IN_STRUCT */

/* for basic or larger versions */
#define COMPLEX 1
#define SPARSE 1

/* for loop unrolling */
/* #undef VUNROLL */
/* #undef MUNROLL */

/* for segmented memory */
#ifndef NOT_SEGMENTED
#define	SEGMENTED
#endif

#include <sys/types.h>

//#define PLATFORM_LINUX 1
//#define PLATFORM_WINDOWS 1
#define PLATFORM_DARWIN 1

/* if the system has malloc.h */
#ifdef HAVE_MALLOC_H
#define	MALLOCDECL	1

#ifdef PLATFORM_LINUX
#include <malloc.h>
#else
#ifdef PLATFORM_WINDOWS
#include	<malloc.h>
#else
#include <sys/malloc.h>
#endif
#endif

#endif

/* any compiler should have this header */
/* if not, change it */
#include <stdio.h>

/* and this one ...*/
#ifdef PLATFORM_LINUX
#include <sys/signal.h>
#else
#ifdef PLATFORM_WINDOWS
#include <signal.h>
#else
#include <sys/signal.h>
#endif
#endif

/* Check for ANSI C memmove and memset */
#ifdef STDC_HEADERS

/* standard copy & zero functions */
#define	MEM_COPY(from,to,size)	memmove((to),(from),(size))
#define	MEM_ZERO(where,size)	memset((where),'\0',(size))

/*#define	MEM_COPY(from,to,size)	bcopy((char *)(from),(char*)(to),(int)(size)) */
/*#define	MEM_ZERO(where,size)	bzero((char *)(where),(int)(size)) */


#ifndef ANSI_C
#define ANSI_C 1
#endif

#ifndef PROTOTYPES_IN_STRUCT
#define PROTOTYPES_IN_STRUCT 1
#endif

#endif

/* standard headers */
#ifdef ANSI_C
#include	<stdlib.h>
#include	<stddef.h>
#include	<string.h>
#include	<float.h>
#include	<math.h>
#include	<ctype.h>

#include <assert.h>
#endif


/* if have bcopy & bzero and no alternatives yet known, use them */
#ifdef HAVE_BCOPY
#ifndef MEM_COPY
/* nonstandard copy function */
#define	MEM_COPY(from,to,size)	bcopy((char *)(from),(char *)(to),(int)(size))
#endif
#endif

#ifdef HAVE_BZERO
#ifndef MEM_ZERO
/* nonstandard zero function */
#define	MEM_ZERO(where,size)	bzero((char *)(where),(int)(size))
#endif
#endif

/* if the system has complex.h */
#ifdef HAVE_COMPLEX_H
#include	<complex.h>
#endif

/* If prototypes are available & ANSI_C not yet defined, then define it,
	but don't include any header files as the proper ANSI C headers
        aren't here */
#ifdef HAVE_PROTOTYPES
#ifndef ANSI_C
#define ANSI_C  1
#endif
#endif


/* floating point precision */

/* you can choose single, double or long double (if available) precision */

#define FLOAT        1
#define DOUBLE 		2
#define LONG_DOUBLE 	3

/* #undef REAL_FLT */
/* #undef REAL_DBL */
/* #undef REAL_QUA */

/* if nothing is defined, choose double precision */
#ifndef REAL_QUA
#ifndef REAL_DBL
#ifndef REAL_FLT
#define REAL_DBL 1
#endif
#endif
#endif

/* single precision */
#ifdef REAL_FLT
#define Real float
#define real float
#define REAL FLOAT
#endif

/* double precision */
#ifdef REAL_DBL
#define Real double
#define real double
#define REAL DOUBLE
#endif

/* long double precision */
#ifdef REAL_QUA
#define Real long double
#define real long double
#define REAL LONG_DOUBLE
#endif

/* machine epsilon or unit roundoff error */
/* This is correct on most IEEE Real precision systems */
#ifdef DBL_EPSILON
#if REAL == DOUBLE
#define	MACHEPS	DBL_EPSILON
#elif REAL == FLOAT
#define	MACHEPS	FLT_EPSILON
#elif REAL == LONG_DOUBLE
#define MACHEPS LDBL_EPSILON
#endif
#endif

#define F_MACHEPS 1.19209e-07
#define D_MACHEPS 2.22045e-16
#define L_MACHEPS 2.22045e-19

#ifndef MACHEPS
#if REAL == DOUBLE
#define	MACHEPS	D_MACHEPS
#elif REAL == FLOAT  
#define MACHEPS F_MACHEPS
#elif REAL == LONG_DOUBLE
#define MACHEPS L_MACHEPS
#endif
#endif

/* #undef M_MACHEPS */

/********************
#ifdef DBL_EPSILON
#define	MACHEPS	DBL_EPSILON
#endif
#ifdef M_MACHEPS
#ifndef MACHEPS
#define MACHEPS	M_MACHEPS
#endif
#endif
********************/

#define	M_MAX_INT 2147483647
#ifdef	M_MAX_INT
#ifndef MAX_RAND
#define	MAX_RAND ((double)(M_MAX_INT))
#endif
#endif

#ifdef ANSI_C
extern	int	isatty(int);
#endif

#endif

