
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH
//
///////////////////////////////////////////////////////// 

#ifndef	MATRIXH
#define	MATRIXH	


/* vector definition */
typedef	struct	{
		%immutable;
                u_int	dim;
                %mutable;
//              max_dim;
//		Real	*ve;
		} VEC;

/* matrix definition */
typedef	struct	{
		%immutable;
                u_int	m, n;
                %mutable;
//		u_int	max_m, max_n, max_size;
//		Real	**me,*base;	/* base is base of alloc'd mem */
		} MAT;

/* band matrix definition */
typedef struct {
//             MAT   *mat;       /* matrix */
               %immutable;
               int   lb,ub;    /* lower and upper bandwidth */
               %mutable;
               } BAND;


/* permutation definition */
typedef	struct	{
		%immutable;
                u_int	size;
                %mutable;
//              max_size, *pe;
		} PERM;

/* integer vector definition */
typedef struct	{
		%immutable;
                u_int	dim;
                %mutable;
//              max_dim;
//		int	*ive;
	        } IVEC;



void	m_version( void );



#define	NEW(type)	((type *)calloc((size_t)1,(size_t)sizeof(type)))
#define	NEW_A(num,type)	((type *)calloc((size_t)(num),(size_t)sizeof(type)))
#define	RENEW(var,num,type) \
    ((var)=(type *)((var) ? \
		    realloc((char *)(var),(size_t)((num)*sizeof(type))) : \
		    calloc((size_t)(num),(size_t)sizeof(type))))

#define	MEMCOPY(from,to,n_items,type) \
 MEM_COPY((char *)(from),(char *)(to),(unsigned)(n_items)*sizeof(type))


#ifndef max
#define	max(a,b)	((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define	min(a,b)	((a) > (b) ? (b) : (a))
#endif


#undef TRUE
#define	TRUE	1
#undef FALSE
#define	FALSE	0


#define MAXLINE 81


extern	VEC *v_get(int), *v_resize(VEC *,int);
extern	MAT *m_get(int,int), *m_resize(MAT *,int,int);
extern	PERM *px_get(int), *px_resize(PERM *,int);
extern	IVEC *iv_get(int), *iv_resize(IVEC *,int);
extern  BAND *bd_get(int,int,int), *bd_resize(BAND *,int,int,int);

extern  int iv_free(IVEC *);
extern  int	m_free(MAT *), v_free(VEC *), px_free(PERM *);
extern  int bd_free(BAND *);

extern  VEC *v_count(VEC *x); 

extern  IVEC	*iiv_finput(FILE *fp, IVEC *iv); 
extern  IVEC	*biv_finput(FILE *fp, IVEC *iv);

extern  MAT     *im_finput(FILE *fp, MAT *mat);
extern  MAT     *bm_finput(FILE *fp, MAT *mat);
extern  PERM    *ipx_finput(FILE *fp, PERM *px);
extern  PERM    *bpx_finput(FILE *fp, PERM *px);
extern  VEC *ifin_vec(FILE *fp, VEC *vec);
extern  VEC     *bfin_vec(FILE *fp, VEC *vec);
extern  char	*setformat(const char    *f_string);

extern  MAT	*bifactor(MAT *A,MAT *U,MAT *V);

#define	M_FREE(mat)	( m_free(mat),	(mat)=(MAT *)NULL )
#define V_FREE(vec)	( v_free(vec),	(vec)=(VEC *)NULL )
#define	PX_FREE(px)	( px_free(px),	(px)=(PERM *)NULL )
#define	IV_FREE(iv)	( iv_free(iv),	(iv)=(IVEC *)NULL )
#define	BD_FREE(bd)	( bd_free(bd),	(bd)=(BAND *)NULL )



#define MAXDIM  	1000000


#ifdef DEBUG

#define	v_entry(x,i)	(((i) < 0 || (i) >= (x)->dim) ? \
			 error(E_BOUNDS,"v_entry"), 0.0 : (x)->ve[i] )
#define	v_set_val(x,i,val) ((x)->ve[i] = ((i) < 0 || (i) >= (x)->dim) ? \
			    error(E_BOUNDS,"v_set_val"), 0.0 : (val))
#define	v_add_val(x,i,val) ((x)->ve[i] += ((i) < 0 || (i) >= (x)->dim) ? \
			    error(E_BOUNDS,"v_add_val"), 0.0 : (val))
#define	v_sub_val(x,i,val) ((x)->ve[i] -= ((i) < 0 || (i) >= (x)->dim) ? \
			    error(E_BOUNDS,"v_sub_val"), 0.0 : (val))
#define	m_entry(A,i,j)	(((i) < 0 || (i) >= (A)->m || \
			  (j) < 0 || (j) >= (A)->n) ? \
			 error(E_BOUNDS,"m_entry"), 0.0 : (A)->me[i][j] )
#define	m_set_val(A,i,j,val) ((A)->me[i][j] = ((i) < 0 || (i) >= (A)->m || \
					       (j) < 0 || (j) >= (A)->n) ? \
			      error(E_BOUNDS,"m_set_val"), 0.0 : (val) )
#define	m_add_val(A,i,j,val) ((A)->me[i][j] += ((i) < 0 || (i) >= (A)->m || \
						(j) < 0 || (j) >= (A)->n) ? \
			      error(E_BOUNDS,"m_add_val"), 0.0 : (val) )
#define	m_sub_val(A,i,j,val) ((A)->me[i][j] -= ((i) < 0 || (i) >= (A)->m || \
						(j) < 0 || (j) >= (A)->n) ? \
			      error(E_BOUNDS,"m_sub_val"), 0.0 : (val) )
#else

#define	v_entry(x,i)		((x)->ve[i])
#define	v_set_val(x,i,val)	((x)->ve[i]  = (val))
#define	v_add_val(x,i,val)	((x)->ve[i] += (val))
#define	v_sub_val(x,i,val)	((x)->ve[i] -= (val))
#define	m_entry(A,i,j)		((A)->me[i][j])
#define	m_set_val(A,i,j,val)	((A)->me[i][j]  = (val) )
#define	m_add_val(A,i,j,val)	((A)->me[i][j] += (val) )
#define	m_sub_val(A,i,j,val)	((A)->me[i][j] -= (val) )

#endif


void v_foutput(FILE *fp,VEC *x),
     m_foutput(FILE *fp,MAT *A),
    px_foutput(FILE *fp,PERM *px);
void iv_foutput(FILE *fp,IVEC *ix);

VEC *v_finput(FILE *fp,VEC *out);
MAT *m_finput(FILE *fp,MAT *out);
PERM *px_finput(FILE *fp,PERM *out);
IVEC *iv_finput(FILE *fp,IVEC *out);

int fy_or_n(FILE *fp,const char *s);
int yn_dflt(int val);
int fin_int(FILE *fp,const char *s,int low,int high);
double fin_double(FILE *fp,const char *s,int low, int high);

int skipjunk(FILE *fp);

#define	v_output(vec)	v_foutput(stdout,vec)
#define	v_input(vec)	v_finput(stdin,vec)
#define	m_output(mat)	m_foutput(stdout,mat)
#define	m_input(mat)	m_finput(stdin,mat)
#define	px_output(px)	px_foutput(stdout,px)
#define	px_input(px)	px_finput(stdin,px)
#define	iv_output(iv)	iv_foutput(stdout,iv)
#define	iv_input(iv)	iv_finput(stdin,iv)

#define	finput(fp,prompt,fmt,var) \
	( ( isatty(fileno(fp)) ? fprintf(stderr,prompt) : skipjunk(fp) ), \
							fscanf(fp,fmt,var) )
#define	input(prompt,fmt,var)	finput(stdin,prompt,fmt,var)
#define	fprompter(fp,prompt) \
	( isatty(fileno(fp)) ? fprintf(stderr,prompt) : skipjunk(fp) )
#define	prompter(prompt)	fprompter(stdin,prompt)
#define	y_or_n(s)	fy_or_n(stdin,s)
#define	in_int(s,lo,hi)	fin_int(stdin,s,lo,hi)
#define	in_double(s,lo,hi)	fin_double(stdin,s,lo,hi)


extern	MAT	*_m_copy(MAT *in,MAT *out,u_int i0,u_int j0),
		* m_move(MAT *in, int, int, int, int, MAT *out, int, int),
		*vm_move(VEC *in, int, MAT *out, int, int, int, int);
extern	VEC	*_v_copy(VEC *in,VEC *out,u_int i0),
		* v_move(VEC *in, int, int, VEC *out, int),
		*mv_move(MAT *in, int, int, int, int, VEC *out, int);
extern	PERM	*px_copy(PERM *in,PERM *out);
extern	IVEC	*iv_copy(IVEC *in,IVEC *out),
		*iv_move(IVEC *in, int, int, IVEC *out, int);
extern  BAND    *bd_copy(BAND *in,BAND *out);


#define	m_copy(in,out)	_m_copy(in,out,0,0)
#define	v_copy(in,out)	_v_copy(in,out,0)


extern	VEC     *v_zero(VEC *), *v_rand(VEC *), *v_ones(VEC *);
extern	MAT     *m_zero(MAT *), *m_ident(MAT *), *m_rand(MAT *),
						*m_ones(MAT *);
extern	PERM    *px_ident(PERM *);
extern  IVEC    *iv_zero(IVEC *);

VEC *bd_mv_mlt(BAND *A, VEC *x, VEC *out);

extern	VEC	*sv_mlt(double,VEC *,VEC *),
		*mv_mlt(MAT *,VEC *,VEC *),
		*vm_mlt(MAT *,VEC *,VEC *),
		*v_add(VEC *,VEC *,VEC *),
                *v_sub(VEC *,VEC *,VEC *),
		*px_vec(PERM *,VEC *,VEC *),
		*pxinv_vec(PERM *,VEC *,VEC *),	
		*v_mltadd(VEC *,VEC *,double,VEC *),
		*v_map(double (*f)(double),VEC *,VEC *),  
		*_v_map(double (*f)(void *,double),void *,VEC *,VEC *),
		*v_lincomb(int,VEC **,Real *,VEC *);                              
                //*v_linlist(VEC *out,VEC *v1,double a1,...);
                                             

extern	double	v_min(VEC *, int *), 	
                v_max(VEC *, int *), 
                v_sum(VEC *);

extern	VEC	*v_star(VEC *, VEC *, VEC *),
		*v_slash(VEC *, VEC *, VEC *),
		*v_sort(VEC *, PERM *);

extern VEC	*v_conv(VEC *x1, VEC *x2, VEC *out);
extern VEC	*v_pconv(VEC *x1, VEC *x2, VEC *out);

extern	double	_in_prod(VEC *x,VEC *y,u_int i0), 
                __ip__(Real *,Real *,int);

extern	void	__mltadd__(Real *,Real *,double,int),
		__add__(Real *,Real *,Real *,int),
		__sub__(Real *,Real *,Real *,int),
                __smlt__(Real *,double,Real *,int),
		__zero__(Real *,int);

#define	in_prod(a,b)	_in_prod(a,b,0)

extern	double	_v_norm1(VEC *x,VEC *scale),   
                _v_norm2(VEC *x,VEC *scale),
		_v_norm_inf(VEC *x,VEC *scale);

extern double m_norm1(MAT *A), m_norm_inf(MAT *A), m_norm_frob(MAT *A);


#define	v_norm1(x)	_v_norm1(x,VNULL)
#define	v_norm2(x)	_v_norm2(x,VNULL)
#define	v_norm_inf(x)	_v_norm_inf(x,VNULL)


extern	MAT	*sm_mlt(double s,MAT *A,MAT *out),
		*m_mlt(MAT *A,MAT *B,MAT *out),
		*mmtr_mlt(MAT *A,MAT *B,MAT *out),
		*mtrm_mlt(MAT *A,MAT *B,MAT *out),
		*m_add(MAT *A,MAT *B,MAT *out),	
		*m_sub(MAT *A,MAT *B,MAT *out),
		*sub_mat(MAT *A,u_int,u_int,u_int,u_int,MAT *out),
		*m_transp(MAT *A,MAT *out),
		*ms_mltadd(MAT *A,MAT *B,double s,MAT *out);   


extern  BAND    *bd_transp(BAND *in, BAND *out); 
extern	MAT	*px_rows(PERM *px,MAT *A,MAT *out),
		*px_cols(PERM *px,MAT *A,MAT *out),
		*swap_rows(MAT *,int,int,int,int),
		*swap_cols(MAT *,int,int,int,int),
		*_set_col(MAT *A,u_int i,VEC *out,u_int j0),
		*_set_row(MAT *A,u_int j,VEC *out,u_int i0);

extern	VEC	*get_row(MAT *,u_int,VEC *),
		*get_col(MAT *,u_int,VEC *),
		*sub_vec(VEC *,int,int,VEC *),
		*mv_mltadd(VEC *x,VEC *y,MAT *A,double s,VEC *out),
		*vm_mltadd(VEC *x,VEC *y,MAT *A,double s,VEC *out);

#define	set_row(mat,row,vec)	_set_row(mat,row,vec,0) 
#define	set_col(mat,col,vec)	_set_col(mat,col,vec,0)



extern	PERM	*px_mlt(PERM *px1,PERM *px2,PERM *out),
		*px_inv(PERM *px,PERM *out),
		*px_transp(PERM *px,u_int i,u_int j);

extern	int	px_sign(PERM *);


extern	IVEC	*iv_add(IVEC *ix,IVEC *iy,IVEC *out),
		*iv_sub(IVEC *ix,IVEC *iy,IVEC *out),
		*iv_sort(IVEC *ix, PERM *order);

double	square(double x), 
  cube(double x), 
  mrand(void);

void	smrand(int seed), 
  mrandlist(Real *x, int len); 

void    m_dump(FILE *fp,MAT *a), px_dump(FILE *,PERM *px),
        v_dump(FILE *fp,VEC *x), iv_dump(FILE *fp, IVEC *ix);

MAT *band2mat(BAND *bA, MAT *A);
BAND *mat2band(MAT *A, int lb,int ub, BAND *bA);


#define	VNULL	((VEC *)NULL)
#define	MNULL	((MAT *)NULL)
#define	PNULL	((PERM *)NULL)
#define	IVNULL	((IVEC *)NULL)
#define BDNULL  ((BAND *)NULL)



//int v_get_vars(int dim,...);
//int iv_get_vars(int dim,...);
//int m_get_vars(int m,int n,...);
//int px_get_vars(int dim,...);

//int v_resize_vars(int new_dim,...);
//int iv_resize_vars(int new_dim,...);
//int m_resize_vars(int m,int n,...);
//int px_resize_vars(int new_dim,...);

//int v_free_vars(VEC **,...);
//int iv_free_vars(IVEC **,...);
//int px_free_vars(PERM **,...);
//int m_free_vars(MAT **,...);

//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{

#undef	M_FREE
#undef  V_FREE
#undef	PX_FREE
#undef	IV_FREE
#undef	BD_FREE
#undef  SP_FREE
#undef	ZV_FREE
#undef	ZM_FREE

void M_FREE(MAT* mat)
{ 
   m_free(mat);
   (mat)=(MAT *)NULL;
}
void V_FREE(VEC* vec)
{ 
   v_free(vec);
   (vec)=(VEC *)NULL;
}
void PX_FREE(PERM* px)
{ 
   px_free(px);
   (px)=(PERM *)NULL;
}
void IV_FREE(IVEC* iv)
{ 
   iv_free(iv);
   (iv)=(IVEC *)NULL;
}
void BD_FREE(BAND* bd)
{ 
   bd_free(bd);
   (bd)=(BAND *)NULL;
}
void SP_FREE(SPMAT* A)
{
  sp_free(A);
  A =(SPMAT *)NULL;
} 
void	ZV_FREE(ZVEC* x)
{
 zv_free(x);
 x = ZVNULL;
}
void	ZM_FREE(ZMAT* A)
{
 zm_free(A);
 A = ZMNULL;
}

#undef in_prod
double	in_prod(VEC *a, VEC *b)
{
 return _in_prod(a,b,0);
}

#undef	v_entry
#undef	v_set_val
#undef	v_add_val
#undef	v_sub_val
#undef	m_entry
#undef	m_set_val
#undef	m_add_val
#undef	m_sub_val

int 	iv_entry(IVEC* x, int i)
{
 return ( ( i < 0 || i >= x->dim ) ?  error(E_BOUNDS,"iv_entry"), 0 : x->ive[i] );
}
int 	iv_get_val(IVEC* x, int i)
{
 return ( ( i < 0 || i >= x->dim ) ?  error(E_BOUNDS,"iv_set_val"), 0 : x->ive[i] );
}
void	iv_set_val(IVEC* x, int i, int val)
{
 x->ive[i]  = ( ( i < 0 || i >= x->dim ) ? error(E_BOUNDS,"iv_set_val"), 0 : val );
}
void	iv_add_val(IVEC* x, int i, int val)
{
 x->ive[i] += ( ( i < 0 || i >= x->dim ) ? error(E_BOUNDS,"iv_add_val"), 0 : val );
}
void	iv_sub_val(IVEC* x, int i, int val)
{
 x->ive[i] -= ( ( i < 0 || i >= x->dim ) ? error(E_BOUNDS,"iv_sub_val"), 0 : val );
}


double 	v_entry(VEC* x, int i)
{
 return ( ( i < 0 || i >= x->dim ) ?  error(E_BOUNDS,"v_entry"), 0.0 : x->ve[i] );
}
double 	v_get_val(VEC* x, int i)
{
 return ( ( i < 0 || i >= x->dim ) ?  error(E_BOUNDS,"v_get_val"), 0.0 : x->ve[i] );
}
void	v_set_val(VEC* x, int i, double val)
{
 x->ve[i]  = ( ( i < 0 || i >= x->dim) ? error(E_BOUNDS,"v_set_val"), 0.0 : (val));
}
void	v_add_val(VEC* x, int i, double val)
{
 x->ve[i] += ( ( i < 0 || i >= x->dim) ? error(E_BOUNDS,"v_add_val"), 0.0 : (val));
}
void	v_sub_val(VEC* x, int i, double val)
{
 x->ve[i] -= ( ( i < 0 || i >= x->dim ) ? error(E_BOUNDS,"v_sub_val"), 0.0 : (val));
}


double	m_entry(MAT* A, int i, int j)
{
 return ( (i < 0 || i >= A->m ||  j < 0 || j >= A->n) ? error(E_BOUNDS,"m_entry"), 0.0 : A->me[i][j] );
}
double	m_get_val(MAT* A, int i, int j)
{
 return ( (i < 0 || i >= A->m ||  j < 0 || j >= A->n) ? error(E_BOUNDS,"m_get_val"), 0.0 : A->me[i][j] );
}
void	m_set_val(MAT* A, int i, int j, double val)
{
 A->me[i][j]  = ( ( i < 0 || i >= A->m || j < 0 || j >= A->n ) ? error(E_BOUNDS,"m_set_val"), 0.0 : (val) );
}
void	m_add_val(MAT* A, int i, int j, double val)
{
 A->me[i][j] += ( ( i < 0 || i >= A->m || j < 0 || j >= A->n ) ? error(E_BOUNDS,"m_add_val"), 0.0 : (val) );
}
void	m_sub_val(MAT* A, int i, int j, double val)
{
 A->me[i][j] -= ( ( i < 0 || i >= A->m || j < 0 || j >= A->n ) ? error(E_BOUNDS,"m_sub_val"), 0.0 : (val) );
}



int	px_get_val(PERM* P, int i)
{
 return ( (i < 0 || i >= P->size ) ? error(E_BOUNDS,"px_get_val"), 0 : P->pe[i] );
}
/*
void	px_set_val(PERM* P, int i, int val)
{
 P->pe[i]  = ( ( i < 0 || i >= A->m ) ? error(E_BOUNDS,"px_set_val"), 0 : (val) );
}
*/

#undef	v_norm1
#undef	v_norm2
#undef	v_norm_inf

double	v_norm1(VEC* x)
{
	return _v_norm1(x,VNULL);
}
double	v_norm2(VEC* x)
{
	return _v_norm2(x,VNULL);
}
double v_norm_inf(VEC* x)
{
	return _v_norm_inf(x,VNULL);
}



#undef	set_row
#undef	set_col

MAT*	set_row(MAT* mat, int row, VEC* vec)
{
	return _set_row(mat,row,vec,0);
} 
MAT*	set_col(MAT* mat, int col, VEC* vec)
{
	return _set_col(mat,col,vec,0);
}



/////////////////// 
IVEC* iv_null(void)
{
   return (IVEC*)NULL;
}

VEC* v_null(void)
{
   return (VEC*)NULL;
}

MAT* m_null(void)
{
   return (MAT*)NULL;
}

SPMAT* sp_null(void)
{
   return (SPMAT*)NULL;
}

PERM* px_null(void)
{
   return (PERM*)NULL;
} 

#undef v_copy
#undef m_copy

VEC* v_copy(VEC* v1, VEC* v2)
{
	return _v_copy(v1,v2,0);
}
MAT* m_copy(MAT* m1, MAT* m2)
{
	return _m_copy(m1,m2,0,0);
}
 
%}


#endif
