
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH
//
/////////////////////////////////////////////////////////

#ifndef SPARSEH
#define SPARSEH 

#include        "matrix.h"

/* basic sparse types */

//typedef struct row_elt	{
//	int	col, nxt_row, nxt_idx;
//	Real	val;
//		} row_elt;

//typedef struct SPROW {
//	int	len, maxlen, diag;
//	row_elt	*elt;		/* elt[maxlen] */
//		} SPROW;

typedef struct SPMAT {
	%immutable;
        int	m, n;
        %mutable;
//      int  max_m, max_n;
//	char	flag_col, flag_diag;
//	SPROW	*row;		/* row[max_m] */
//	int	*start_row;	/* start_row[max_n] */
//	int	*start_idx;	/* start_idx[max_n] */
	      } SPMAT;

/* Note that the first allocated entry in column j is start_row[j];
	This starts the chain down the columns using the nxt_row and nxt_idx
	fields of each entry in each row. */

//typedef struct pair { int pos;	Real val; } pair;

//typedef struct SPVEC {
//	int	dim, max_dim;
//	pair	*elt;		/* elt[max_dim] */
//	       } SPVEC;

#define	SMNULL	((SPMAT*)NULL)
#define	SVNULL	((SPVEC*)NULL)

/* Macro for speedup */
#define	sprow_idx2(r,c,hint)	\
	( ( (hint) >= 0 && (hint) < (r)->len && \
	   (r)->elt[hint].col == (c)) ? (hint) : sprow_idx((r),(c)) )



/* memory functions */

//int sp_get_vars(int m,int n,int deg,...);
//int sp_resize_vars(int m,int n,...);
//int sp_free_vars(SPMAT **,...);


SPMAT	*sp_get(int,int,int), *sp_copy(SPMAT *),
	*sp_copy2(SPMAT *,SPMAT *),
	*sp_zero(SPMAT *), *sp_resize(SPMAT *,int,int),
	*sp_compact(SPMAT *,double);
double	sp_get_val(SPMAT *,int,int), sp_set_val(SPMAT *,int,int,double);
VEC	*sp_mv_mlt(SPMAT *,VEC *,VEC *), *sp_vm_mlt(SPMAT *,VEC *,VEC *);
int	sp_free(SPMAT *);

/* Access path operations */
SPMAT	*sp_col_access(SPMAT *);
SPMAT	*sp_diag_access(SPMAT *);
int     chk_col_access(SPMAT *);

/* Input/output operations */
SPMAT	*sp_finput(FILE *);
void	sp_foutput(FILE *,SPMAT *), sp_foutput2(FILE *,SPMAT *);

/* algebraic operations */
SPMAT *sp_smlt(SPMAT *A,double alpha,SPMAT *B),
      *sp_add(SPMAT *A,SPMAT *B,SPMAT *C),
      *sp_sub(SPMAT *A,SPMAT *B,SPMAT *C),
      *sp_mltadd(SPMAT *A,SPMAT *B,double alpha,SPMAT *C);

/* sparse row operations */
SPROW	*sprow_get(int), *sprow_xpd(SPROW *r,int n,int type),
        *sprow_resize(SPROW *r,int n,int type),
	*sprow_merge(SPROW *,SPROW *,SPROW *,int type),
        *sprow_copy(SPROW *,SPROW *,SPROW *,int type),
	*sprow_mltadd(SPROW *,SPROW *,double,int,SPROW *,int type);
SPROW *sprow_add(SPROW *r1,SPROW *r2, int j0,SPROW *r_out, int type), 
        *sprow_sub(SPROW *r1,SPROW *r2, int j0,SPROW *r_out, int type), 
        *sprow_smlt(SPROW *r1,double alpha, int j0,SPROW *r_out, int type);
double	sprow_set_val(SPROW *,int,double);
int      sprow_free(SPROW *);
int	sprow_idx(SPROW *,int);
void	sprow_foutput(FILE *,SPROW *);

/* from "extras.h" */
SPROW	*sprow_copy2(SPROW *r1, SPROW *r_out, int type);
double   sprow_get_val(SPROW *r1, int j);

/* dump */
void    sp_dump(FILE *fp, SPMAT *A);
void    sprow_dump(FILE *fp, SPROW *r);
MAT	*sp_m2dense(SPMAT *A,MAT *out);


/* MACROS */

#define	sp_input()	sp_finput(stdin)
#define	sp_output(A)	sp_foutput(stdout,(A))
#define	sp_output2(A)	sp_foutput2(stdout,(A))
#define	row_mltadd(r1,r2,alpha,out)	sprow_mltadd(r1,r2,alpha,0,out)
#define	out_row(r)	sprow_foutput(stdout,(r))

#define SP_FREE(A)    ( sp_free((A)),  (A)=(SPMAT *)NULL) 
#define SPROW_FREE(r) ( sprow_free((r)), (r)= (SPROW *)NULL)

/* utility for index computations -- ensures index returned >= 0 */
#define	fixindex(idx)	((idx) == -1 ? (error(E_BOUNDS,"fixindex"),0) : \
			 (idx) < 0 ? -((idx)+2) : (idx))


/*  NOT USED */

/* loop over the columns in a row */
/*
#define	loop_cols(r,e,code) \
    do { int _r_idx; row_elt *e; SPROW *_t_row;			\
	  _t_row = (r); e = &(_t_row->elt);				\
	  for ( _r_idx = 0; _r_idx < _t_row->len; _r_idx++, e++ )	\
	  {  code;  }  }  while ( 0 )
*/
/* loop over the rows in a column */
/*
#define	loop_cols(A,col,e,code) \
    do { int _r_num, _r_idx, _c; SPROW *_r; row_elt *e;		\
	  if ( ! (A)->flag_col )	sp_col_access((A));		\
	  col_num = (col);						\
	  if ( col_num < 0 || col_num >= A->n )				\
	      error(E_BOUNDS,"loop_cols");				\
          _r_num = (A)->start_row[_c]; _r_idx = (A)->start_idx[_c];	\
	  while ( _r_num >= 0 )  {					\
	      _r = &((A)->row[_r_num]);					\
              _r_idx = sprow_idx2(_r,_c,_r_idx);			\
              if ( _r_idx < 0 )  continue;				\
	      e = &(_r->elt[_r_idx]);	code;				\
	      _r_num = e->nxt_row;	_r_idx = e->nxt_idx;		\
	      } } while ( 0 )

*/

#endif

