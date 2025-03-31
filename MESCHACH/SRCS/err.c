/**************************************************************************
**
** Copyright (C) 1993 David E. Steward & Zbigniew Leyk, all rights reserved.
**
**			     Meschach Library
** 
** This Meschach Library is provided "as is" without any express 
** or implied warranty of any kind with respect to this software. 
** In particular the authors shall not be liable for any direct, 
** indirect, special, incidental or consequential damages arising 
** in any way from use of the software.
** 
** Everyone is granted permission to copy, modify and redistribute this
** Meschach Library, provided:
**  1.  All copies contain this copyright notice.
**  2.  All modified copies shall carry a notice stating who
**      made the last modification and the date of such modification.
**  3.  No charge is made for this software or works derived from it.  
**      This clause shall not be construed as constraining other software
**      distributed on the same medium as this software, nor is a
**      distribution fee considered a charge.
**
***************************************************************************/


/*
  File with basic error-handling operations
  Based on previous version on Zilog
  System 8000 setret() etc.
  Ported to Pyramid 9810 late 1987
  */

#include        "err.h"


#ifdef SYSV
/* AT&T System V */
#include	<sys/signal.h>
#else
/* something else -- assume BSD or ANSI C */
#include	<signal.h>
#endif


static	char	*err_mesg[] =
{	
   "unknown error",		    	               /* 0 */
   "sizes of objects don't match",        	       /* 1 */
   "index out of bounds",		               /* 2 */
   "can't allocate memory",		               /* 3 */
   "singular matrix",			               /* 4 */
   "matrix not positive definite",	               /* 5 */
   "incorrect format input",		               /* 6 */
   "bad input file/device",		               /* 7 */
   "NULL objects passed",		               /* 8 */
   "matrix not square",			               /* 9 */
   "object out of range",		               /* 10 */
   "can't do operation in situ for non-square matrix", /* 11 */
   "can't do operation in situ",		       /* 12 */
   "excessive number of iterations",       	       /* 13 */
   "convergence criterion failed",	               /* 14 */
   "bad starting value",			       /* 15 */
   "floating exception",			       /* 16 */
   "internal inconsistency (data structure)",          /* 17 */
   "unexpected end-of-file",		               /* 18 */
   "shared vectors (cannot release them)",             /* 19 */  
   "negative argument",			               /* 20 */
   "cannot overwrite object",                          /* 21 */
   /* >>> xavier */
   "file not found"                                    /* 22 */
   /* <<< xavier */
};

#define	MAXERR	(sizeof(err_mesg)/sizeof(char *))

static char *warn_mesg[] = {
   "unknown warning",				        /* 0 */
   "wrong type number (use macro TYPE_*)",	        /* 1 */
   "no corresponding mem_stat_mark",		        /* 2 */
   "computed norm of a residual is less than 0",        /* 3 */
   "resizing a shared vector"			        /* 4 */
};

#define MAXWARN  (sizeof(warn_mesg)/sizeof(char *))



#define	MAX_ERRS	100
#define	MAX_WARNS	100

jmp_buf	restart;

extern jmp_buf* jmp_buf__get(void)
{
   return &restart;
}

int list_number = 0;

extern int list_number__get(void)
{
   return list_number;
}

/* array of pointers to lists of errors */

typedef struct
{
   char **listp;    /* pointer to a list of errors */
   unsigned len;    /* length of the list */
}  Err_list;

static Err_list     err_list[ERR_LIST_MAX_LEN] =
{
   {err_mesg , MAXERR }	/* basic errors list */
};

static Err_list     warn_list[ERR_LIST_MAX_LEN] =
{
   {warn_mesg, MAXWARN}	/* basic warnings list */
};

static int err_list_end  = ERR_LIST_MAX_LEN;   /* number of elements in err_list */
static int warn_list_end = ERR_LIST_MAX_LEN;   /* number of elements in err_list */

/* attach a new list of errors pointed by err_ptr
   or change a previous one;
   list_len is the number of elements in the list;
   list_num is the list number;
   list_start is the number of the first error in the enum of error
   warn == FALSE - errors (stop the program),
   warn == TRUE - warnings (continue the program);
   Note: lists numbered 0 and 1 are attached automatically,
   you do not need to do it
*/
extern  int err_list_attach(int list_num, int list_len, char **err_ptr)
{
   if (list_num < 0 || list_len <= 0 ||  err_ptr == (char **)NULL ) 
   {
      return -1;
   }
   
   if (list_num >= ERR_LIST_MAX_LEN)
   {
      fprintf(stderr,"\n file \"%s\": %s %s\n",
	"err.c", "increase the value of ERR_LIST_MAX_LEN", "in matrix.h and zmatdef.h");

      if ( ! isatty(fileno(stdout)) )
      {
         fprintf(stderr,"\n file \"%s\": %s %s\n",
            "err.c", "increase the value of ERR_LIST_MAX_LEN", "in matrix.h and zmatdef.h");
      }

      printf("Exiting program\n");
      exit(0);
   }

   if ( err_list[list_num].listp != (char **)NULL &&  err_list[list_num].listp != err_ptr )
   {
      free( (char *)err_list[list_num].listp );
   }

   err_list[list_num].listp = err_ptr;
   err_list[list_num].len   = list_len;
   //err_list[list_num].warn  = warn;

   if ( list_num > err_list_end )
   {
      err_list_end = list_num+1;
   }
   
   return list_num;
}

extern  int warn_list_attach(int list_num, int list_len, char **warn_ptr)
{
   if (list_num < 0 || list_len <= 0 ||  warn_ptr == (char **)NULL) 
   {
      return -1;
   }
   
   if (list_num >= ERR_LIST_MAX_LEN)
   {
      fprintf(stderr,"\n file \"%s\": %s %s\n",
              "err.c", "increase the value of ERR_LIST_MAX_LEN", "in matrix.h and zmatdef.h");

      if ( ! isatty(fileno(stdout)) )
      {
         fprintf(stderr,"\n file \"%s\": %s %s\n",
                 "err.c", "increase the value of ERR_LIST_MAX_LEN", "in matrix.h and zmatdef.h");
      }

      printf("Exiting program\n");
      exit(0);
   }

   if (warn_list[list_num].listp != (char **)NULL &&  warn_list[list_num].listp != warn_ptr)
   {
      free((char *)warn_list[list_num].listp);
   }
   warn_list[list_num].listp = warn_ptr;
   warn_list[list_num].len   = list_len;
   //warn_list[list_num].warn  = warn;

   if (list_num > warn_list_end )
   {
      warn_list_end = list_num+1;
   }

   return list_num;
}


/* release the error list numbered list_num */
extern  int err_list_free(int list_num)
{
   if (list_num < 0 || list_num >= err_list_end) return -1;

   if (err_list[list_num].listp != (char **)NULL)
   {
      err_list[list_num].listp = (char **)NULL;
      err_list[list_num].len   = 0;
      //err_list[list_num].warn  = 0;
   }
   return 0;
}

extern  int warn_list_free(int list_num)
{
   if (list_num < 0 || list_num >= warn_list_end) return -1;

   if (warn_list[list_num].listp != (char **)NULL)
   {
      warn_list[list_num].listp = (char **)NULL;
      warn_list[list_num].len   = 0;
      //warn_list[list_num].warn  = 0;
   }
   return 0;
}


/* check if list_num is attached;
   return FALSE if not;
   return TRUE if yes
*/
extern  int err_is_list_attached(int list_num)
{
   if (list_num < 0 || list_num >= err_list_end)
     return FALSE;
   
   if (err_list[list_num].listp != (char **)NULL)
     return TRUE;
   
   return FALSE;
}

extern  int warn_is_list_attached(int list_num)
{
   if (list_num < 0 || list_num >= warn_list_end)
      return FALSE;

   if (warn_list[list_num].listp != (char **)NULL)
      return TRUE;

   return FALSE;
}

/* other local variables */

/* static	int	err_flag = EF_EXIT, num_errs = 0, cnt_errs = 1; */
static	int	err_flag = EF_JUMP  , num_errs  = 0, cnt_errs  = 1;
static	int    warn_flag = EF_SILENT, num_warns = 0, cnt_warns = 1;

/* set_err_flag -- sets err_flag -- returns old err_flag */
extern  int	set_err_flag(int flag)
{
   int	tmp = err_flag;
   err_flag = flag;

   return tmp;
}

extern  int	set_warn_flag(int flag)
{
   int	tmp = warn_flag;
   warn_flag = flag;

   return tmp;
}

/* count_errs -- sets cnt_errs (TRUE/FALSE) & returns old value */
extern  int	count_errs(int flag)
{
   int	tmp = cnt_errs;
   cnt_errs = flag;

   return tmp;
}

extern  int	count_warns(int flag)
{
   int	tmp = cnt_warns;
   cnt_warns = flag;

   return tmp;
}

/* ev_err -- reports error (err_num) in file "file" at line "line_num" and
   returns to user error handler;
   list_num is an error list number (0 is the basic list 
   pointed by err_mesg, 1 is the basic list of warnings)
 */
extern  int	ev_err(char *file, int err_num, int line_num, char *fn_name, int list_num)
{
   if ( err_num < 0 ) err_num = 0;
   
   if ( list_num < 0 || list_num >= err_list_end || err_list[list_num].listp == (char **)NULL )
   {
      fprintf(stderr, "\n Not (properly) attached list of errors: list_num = %d\n", list_num);

      fprintf(stderr, " Call \"err_list_attach\" in your program\n");

      if ( ! isatty(fileno(stdout)) )
      {
	 fprintf(stderr, "\n Not (properly) attached list of errors: list_num = %d\n", list_num);
	 fprintf(stderr," Call \"err_list_attach\" in your program\n");
      }
      
      fprintf(stdout,"-->no attached list -> exit \n");
      exit(0);
   }
   
   if ( err_num >= err_list[list_num].len ) err_num = 0;
   
   if ( cnt_errs && ++num_errs >= MAX_ERRS )	/* too many errors */
   {
      fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n",
	      file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");

      if ( ! isatty(fileno(stdout)) )
      {
         fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n",
		file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");
      }

      printf("Sorry, too many errors: %d  --> exit\n",num_errs);
      exit(0);
   }
   
   /*
   if ( err_list[list_num].warn )
   {
       switch ( err_flag )
       {
	   case EF_SILENT:
              break;
	   default:
	      fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n\n",
		   file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");
	      if ( ! isatty(fileno(stdout)) )
	           fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n\n",
		       file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");
	      break;
       }
   }
   else */
   {
      switch ( err_flag )
      {
         case EF_SILENT:
            list_number = list_num;
            fprintf(stdout,"--> err_flag = EF_SILENT -> ....  \n");
	    longjmp(restart,(err_num==0)? -1 : err_num);
	    break;
         case EF_ABORT:
	    fprintf(stdout,"--> err_flag = EF_ABORT -> abort \n");
            fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n",
		   file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");
	    if ( ! isatty(fileno(stdout)) )
	       fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n",
		       file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");
	    abort();
	    break;
         case EF_JUMP:
            list_number = list_num;
            /*fprintf(stdout,"--> err_flag = EF_JUMP -> longjmp  \n");*/
	    fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n",
		   file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");
	    if ( ! isatty(fileno(stdout)) )
	       fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n",
		       file,line_num,err_list[list_num].listp[err_num],isascii(*fn_name) ? fn_name : "???");
	    longjmp(restart,(err_num==0)? -1 : err_num);
	    break;
         default:
            fprintf(stdout,"--> err_flag = EF_DEFAULT -> exit  \n");
	    fprintf(stderr,"\n\"%s\", line %d: %s in function %s()\n\n",
		   file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");
	    if ( ! isatty(fileno(stdout)) )
	       fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n\n",
		       file,line_num,err_list[list_num].listp[err_num], isascii(*fn_name) ? fn_name : "???");
	   
            break;
       }
   }
   
   /* ensure exit if fall through */
   /*
    if ( ! err_list[list_num].warn )  exit(0);
   */
   
   return 0;
}

extern  int	ev_warn(char *file, int warn_num, int line_num, char *fn_name, int list_num)
{
   if ( warn_num < 0 ) warn_num = 0;

   if (list_num < 0 || list_num >= warn_list_end || warn_list[list_num].listp == (char **)NULL)
   {
      fprintf(stderr, "\n Not (properly) attached list of warnings: list_num = %d\n", list_num);
      fprintf(stderr," Call \"warn_list_attach\" in your program\n");
      
      if ( ! isatty(fileno(stdout)) )
      {
         fprintf(stderr, "\n Not (properly) attached list of warnings: list_num = %d\n", list_num);
         fprintf(stderr," Call \"warn_list_attach\" in your program\n");
      }
      
      printf("\nExiting program\n");
      exit(0);
   }

   if ( warn_num >= warn_list[list_num].len )  warn_num = 0;

   if ( cnt_errs && ++num_warns >= MAX_WARNS )	/* too many errors */
   {
      fprintf(stderr,"\n\"%s\", line %d: **warning** %s in function %s()\n",
              file,line_num,err_list[list_num].listp[warn_num], isascii(*fn_name) ? fn_name : "???");

      if ( ! isatty(fileno(stdout)) )
      {
         fprintf(stdout,"\n\"%s\", line %d: **warning** %s in function %s()\n",
                 file,line_num,err_list[list_num].listp[warn_num], isascii(*fn_name) ? fn_name : "???");
      }

      printf("Sorry, too many errors: %d\n",num_warns);
      printf("Exiting program\n");
      exit(0);
   }

   
   switch ( warn_flag )
   {
      case EF_JUMP:
         list_number = list_num;
         fprintf(stdout,"--> warn_flag = EF_JUMP -> longjmp  \n");
         fprintf(stderr,"\n\"%s\", line %d: **warning** %s in function %s()\n",
                 file,line_num,warn_list[list_num].listp[warn_num], isascii(*fn_name) ? fn_name : "???");

         if ( ! isatty(fileno(stdout)) )
            fprintf(stdout,"\n\"%s\", line %d: %s in function %s()\n",
                    file,line_num,warn_list[list_num].listp[warn_num],isascii(*fn_name) ? fn_name : "???");

         longjmp(restart,(warn_num==0)? -1 : warn_num);
         break;

      case EF_SILENT:
         /*fprintf(stdout,"--> warn_flag = EF_SILENT -> nix  \n");*/
         fprintf(stderr,"\n\"%s\", line %d: **warning** %s in function %s()\n\n",
                    file,line_num,warn_list[list_num].listp[warn_num], isascii(*fn_name) ? fn_name : "???");

         if ( ! isatty(fileno(stdout)) )
         {
            fprintf(stdout,"\n\"%s\", line %d: **warning** %s in function %s()\n\n",
                    file,line_num,warn_list[list_num].listp[warn_num], isascii(*fn_name) ? fn_name : "???");
         }
         break;

      default:
         /* fprintf(stdout,"--> warn_flag = EF_DEFAULT -> nix  \n"); */
         break;
   }

   /* ensure exit if fall through */
   /*
    if ( warn_list[list_num].warn == 0 )  exit(0);
   */
   
   return 0;
}


/* float_error -- catches floating arithmetic signals */
void	float_error(void)
{
   signal(SIGFPE, catch_FPE);
   /* fprintf(stderr,"SIGFPE: signal #%d\n",num); */
   /* fprintf(stderr,"errno = %d\n",errno); */
   ev_err("???.c",E_SIGNAL,0,"???",0);
}

/* catch_signal -- sets up float_error() to catch SIGFPE's */
extern  void	catch_FPE(int num)
{
   ev_err("???.c",E_SIGNAL,0,"???",0);
}

