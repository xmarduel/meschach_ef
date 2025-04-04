
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
		Files for matrix computations

	Givens operations file. Contains routines for calculating and
	applying givens rotations for/to vectors and also to matrices by
	row and by column.
*/


#include	"matrix.h"
#include    "matrix2.h"

/* givens -- returns c,s parameters for Givens rotation to
		eliminate y in the vector [ x y ]' */
extern  void	givens(double  x, double y, Real *c, Real *s)
{
	Real	norm;

	norm = sqrt(x*x+y*y);
	if ( norm == 0.0 )
	{	*c = 1.0;	*s = 0.0;	}	/* identity */
	else
	{	*c = x/norm;	*s = y/norm;	}
}

/* rot_vec -- apply Givens rotation to x's i & k components */
extern  VEC	*rot_vec(VEC *x, u_int i, u_int k, double c, double s, VEC *out)
{
	Real	temp;

	if ( x==VNULL )
		error(E_NULL,"rot_vec");
	if ( i >= x->dim || k >= x->dim )
		error(E_RANGE,"rot_vec");
	out = v_copy(x,out);

	/* temp = c*out->ve[i] + s*out->ve[k]; */
	temp = c*v_entry(out,i) + s*v_entry(out,k);
	/* out->ve[k] = -s*out->ve[i] + c*out->ve[k]; */
	v_set_val(out,k,-s*v_entry(out,i)+c*v_entry(out,k));
	/* out->ve[i] = temp; */
	v_set_val(out,i,temp);

	return (out);
}

/* rot_rows -- premultiply mat by givens rotation described by c,s */
extern  MAT	*rot_rows(MAT *mat,u_int i, u_int k, double c, double s, MAT *out)
{
	u_int	j;
	Real	temp;

	if ( mat==(MAT *)NULL )
		error(E_NULL,"rot_rows");
	if ( i >= mat->m || k >= mat->m )
		error(E_RANGE,"rot_rows");
	out = m_copy(mat,out);

	for ( j=0; j<mat->n; j++ )
	{
		/* temp = c*out->me[i][j] + s*out->me[k][j]; */
		temp = c*m_entry(out,i,j) + s*m_entry(out,k,j);
		/* out->me[k][j] = -s*out->me[i][j] + c*out->me[k][j]; */
		m_set_val(out,k,j, -s*m_entry(out,i,j) + c*m_entry(out,k,j));
		/* out->me[i][j] = temp; */
		m_set_val(out,i,j, temp);
	}

	return (out);
}

/* rot_cols -- postmultiply mat by givens rotation described by c,s */
extern  MAT	*rot_cols(MAT *mat,u_int i, u_int k, double c, double s, MAT *out)
{
	u_int	j;
	Real	temp;

	if ( mat==(MAT *)NULL )
		error(E_NULL,"rot_cols");
	if ( i >= mat->n || k >= mat->n )
		error(E_RANGE,"rot_cols");
	out = m_copy(mat,out);

	for ( j=0; j<mat->m; j++ )
	{
		/* temp = c*out->me[j][i] + s*out->me[j][k]; */
		temp = c*m_entry(out,j,i) + s*m_entry(out,j,k);
		/* out->me[j][k] = -s*out->me[j][i] + c*out->me[j][k]; */
		m_set_val(out,j,k, -s*m_entry(out,j,i) + c*m_entry(out,j,k));
		/* out->me[j][i] = temp; */
		m_set_val(out,j,i,temp);
	}

	return (out);
}

