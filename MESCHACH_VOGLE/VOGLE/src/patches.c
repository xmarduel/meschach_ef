#include "vogle.h"

static int	ntsegs, tsegs = 10, nusegs, usegs = 10, 
		ntcurves = 10, nucurves = 10, 
		ntiter = 1, nuiter = 1,
		makeprec_called = 0;

static Matrix	tbasis, ubasis, et, eu,
		ones =	{
				{1.0, 1.0, 1.0, 1.0},
				{1.0, 1.0, 1.0, 1.0},
				{1.0, 1.0, 1.0, 1.0},
				{1.0, 1.0, 1.0, 1.0}
			};
			

void		premulttensor(Tensor c, Matrix a, Tensor b),
 		multtensor(Tensor c, Matrix a, Tensor b), 
		copytensor(Tensor b, Tensor a),
		copytensortrans(Tensor b, Tensor a),
		drpatch(Tensor R, int ntcurves, int nucurves, int ntsegs, int nusegs, int ntiter, int nuiter),
		drcurve(int n, Matrix r), 
		transformtensor(Tensor S, Matrix m);

static	void	makeprec(void), 
		replace(Tensor a, Matrix b, int k),
		iterate(Tensor R, int n),
		extract(Matrix b, Tensor a, int intk);

static	float	addemup(Matrix m);

/*
 * patchbasis
 *
 *	Specify the two basis matrices for a patch
 */
void
patchbasis(Matrix tb, Matrix ub)
{
   if(!vdevice.initialised)
      verror(VERR_UNINIT, "patchbasis");

   copymatrix(tbasis, tb);
   copytranspose(ubasis, ub);
}

/*
 * patchprecision
 *
 *	Specify the lower limit on the number of line segments used
 * to draw a curve as part of a patch. The actual number used varies
 * with the number of curve segments in the "other" direction.
 */
void
patchprecision(int tseg, int useg)
{
   if (!vdevice.initialised)
      verror(VERR_UNINIT, "patchprecision");

   if(tseg > 0 && useg > 0)
   {
      tsegs = tseg;
      usegs = useg;
   }
   else
   {
      verror(VERR_NUMSEGS, "patchprecision");
   }

   /*
    * Set up the difference matrices
    */
   makeprec();
}

/*
 * patchcurves
 *
 *	Specify the number of curves to be drawn in each direction
 *	on a patch.
 */
void
patchcurves(int nt, int nu)
{

   if (!vdevice.initialised)
      verror(VERR_UNINIT, "patchcurves");

   if(nt > 0 && nu > 0)
   {
      ntcurves = nt;
      nucurves = nu;
   }
   else
   {
      verror(VERR_NUMCRVS, "patchcurves");
   }
   
   /*
    * Set up the difference matrices
    */
   makeprec();
}

/*
 * makeprec
 *
 *	Makes up the two precision matrices for a patch
 */
static	void
makeprec(void)
{
   float	n2, n3;

   /*
    * Find ntsegs, nusegs, ntiter, nuiter....
    * ie. the actual number of curve segments of a tcurve,
    *     the actual number of curve segments of a ucurve,
    *     and the number of times to iterate in each direction.
    */

   ntsegs = tsegs;
   ntiter = ntsegs / (nucurves - 1);
   if (ntsegs > ntiter * (nucurves - 1))
   {
      ntsegs = (++ntiter) * (nucurves - 1);
   }
   
   nusegs = usegs;
   nuiter = nusegs / (ntcurves - 1);
   if (nusegs > nuiter * (ntcurves - 1))
   {
      nusegs = (++nuiter) * (ntcurves - 1);
   }
   
   /*
    * Doing the t precision matrix.....
    */
   identmatrix(et);
   n2 = (float)(ntsegs * ntsegs);
   n3 = (float)(ntsegs * n2);

   et[0][0] = et[2][2] = et[3][3] = 0.0;
   et[1][0] = 1.0 / n3;
   et[1][1] = 1.0 / n2;
   et[2][0] = et[3][0] = 6.0 / n3;
   et[2][1] = 2.0 / n2;
   et[1][2] = 1.0 / (float)ntsegs;
   et[0][3] = 1.0;

   /*
    * Make the Transpose of eu
    */
   identmatrix(eu);
   n2 = (float)(nusegs * nusegs);
   n3 = (float)(nusegs * n2);

   eu[0][0] = eu[2][2] = eu[3][3] = 0.0;
   eu[0][1] = 1.0 / n3;
   eu[1][1] = 1.0 / n2;
   eu[0][2] = eu[0][3] = 6.0 / n3;
   eu[1][2] = 2.0 / n2;
   eu[2][1] = 1.0 / (float)nusegs;
   eu[3][0] = 1.0;

   makeprec_called = 1;
}


/*
 * patch
 *
 *	Draws a bicubic patch. (ie. all the w coords a 1 and the
                           *	basis matrices don't change that)
 */
void
patch(Matrix geomx, Matrix geomy, Matrix geomz)
{
   rpatch(geomx, geomy, geomz, ones);
}

/*
 * rpatch
 *
 *	Draws rational bicubic patches.
 *
 *	Reference: J. H. Clark, Parametric Curves, Surfaces and volumes in
 *	computer graphics and computer aided Geometric Design.
 *	Technical report No. 221, Nov 1981.
 *	Computer Systems Lab. Dept's of Elecrical Eng. and Computer Science,
 *	Standford University, Standford, California 94305.
 */
void
rpatch(Matrix geomx, Matrix geomy, Matrix geomz, Matrix geomw)
{

   Tensor	S, R;
   Matrix	tmp, tmp2;
   float	xlast, ylast, zlast;
   int	i, j, sync;
   Token	*tok;

   if (!vdevice.initialised)
      verror(VERR_UNINIT, "patch");

   /*
      *  Form S = et . tbasis . Gtensor . ubasisT . euT
    */

   if (!makeprec_called)
      makeprec();

   mult4x4(tmp, et, tbasis);
   mult4x4(tmp2, ubasis, eu);

   /*
    * Load the geometry matrices into S.
    */
   for (i = 0; i < 4; i++)
   for (j = 0; j < 4; j++)
   {
      S[0][i][j] = geomx[i][j];
      S[1][i][j] = geomy[i][j];
      S[2][i][j] = geomz[i][j];
      S[3][i][j] = geomw[i][j];
   }

   premulttensor(R, tbasis, S);
   multtensor(S, ubasis, R);

   /*
    * Find the last point on the curve.
    */
   xlast = addemup(S[0]);
   ylast = addemup(S[1]);
   zlast = addemup(S[2]);

   /*
    * Multiply the precision matrices in.
    */
   premulttensor(R, et, S);
   multtensor(S, eu, R);

   if (vdevice.inobject)
   {
      tok = newtokens(74);
      tok[0].i = RPATCH;
      tok[1].f = xlast;
      tok[2].f = ylast;
      tok[3].f = zlast;
      tok[4].i = ntcurves;
      tok[5].i = nucurves;
      tok[6].i = ntsegs;
      tok[7].i = nusegs;
      tok[8].i = ntiter;
      tok[9].i = nuiter;

      tok += 10;
      for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++)
      {
         (tok++)->f = S[0][i][j];
         (tok++)->f = S[1][i][j];
         (tok++)->f = S[2][i][j];
         (tok++)->f = S[3][i][j];
      }

      return;
   }

   /*
    * Multiply by the current transformation....
    */
   transformtensor(S, vdevice.transmat->m);

   /*
    * Draw the patch....
    */
   if ((sync = vdevice.sync))
   {
      vdevice.sync = 0;
   }
   
   drpatch(S, ntcurves, nucurves, ntsegs, nusegs, ntiter, nuiter);

   if (sync)
   {
      vdevice.sync = 1;
      (*vdevice.dev.Vsync)();
   }

   /*
    * Set the current (untransformed) world spot....
    */
   vdevice.pos->cpW[V_X] = xlast;
   vdevice.pos->cpW[V_Y] = ylast;
   vdevice.pos->cpW[V_Z] = zlast;
}

/*
 * transformtensor
 *
 *	Transform the tensor S by the matrix m
 */
void
transformtensor(Tensor S, Matrix m)
{
   Matrix	tmp, tmp2;
   int	i;

   for (i = 0; i < 4; i++)
   {
      extract(tmp, S, i);
      mult4x4(tmp2, tmp, m);
      replace(S, tmp2, i);
   }
}

/*
 * replace
 *
 *	Does the reverse of extract.
 */
static	void
replace(Tensor a, Matrix b, int k)
{
   int	i, j;

   /*
    * Not unwound because it only gets called once per patch.
    */
   for (i = 0; i < 4; i++)
   for (j = 0; j < 4; j++)
      a[j][i][k] = b[i][j];
}

/*
 * drpatch
 *
 *	Actually does the work of drawing a patch.
 */
void
drpatch(Tensor R, int ntcurves, int nucurves, int ntsegs, int nusegs, int ntiter, int nuiter)
{
   Tensor	S;
   Matrix	tmp;
   int	i;

   /*
    *  Copy R transposed into S
    */
   copytensortrans(S, R);

   for (i = 0; i < ntcurves; i++)
   {
      extract(tmp, R, 0);
      drcurve(ntsegs, tmp);
      iterate(R, nuiter);
   }

   /*
    * Now using S...
    */
   for (i = 0; i < nucurves; i++)
   {
      extract(tmp, S, 0);
      drcurve(nusegs, tmp);
      iterate(S, ntiter);
   }
}

/*
 * iterate
 *
 *	Iterates the forward difference tensor R
 */
static	void
iterate(Tensor R, int n)
{
   int	it;

   /*
    * Anyone for an unwound loop or two???
    *
    *	for (it = 0; it < n; it++) {
    *		for (i = 0; i < 4; i++)
    *			for (j = 0; j < 4; j++)
    *				for (k = 0; k < 3; k++)
    *					R[i][j][k] += R[i][j][k+1];
    *	}
    */
   for (it = 0; it < n; it++)
   {
      R[0][0][0] += R[0][0][1];
      R[0][0][1] += R[0][0][2];
      R[0][0][2] += R[0][0][3];

      R[0][1][0] += R[0][1][1];
      R[0][1][1] += R[0][1][2];
      R[0][1][2] += R[0][1][3];

      R[0][2][0] += R[0][2][1];
      R[0][2][1] += R[0][2][2];
      R[0][2][2] += R[0][2][3];

      R[0][3][0] += R[0][3][1];
      R[0][3][1] += R[0][3][2];
      R[0][3][2] += R[0][3][3];

      R[1][0][0] += R[1][0][1];
      R[1][0][1] += R[1][0][2];
      R[1][0][2] += R[1][0][3];

      R[1][1][0] += R[1][1][1];
      R[1][1][1] += R[1][1][2];
      R[1][1][2] += R[1][1][3];

      R[1][2][0] += R[1][2][1];
      R[1][2][1] += R[1][2][2];
      R[1][2][2] += R[1][2][3];

      R[1][3][0] += R[1][3][1];
      R[1][3][1] += R[1][3][2];
      R[1][3][2] += R[1][3][3];

      R[2][0][0] += R[2][0][1];
      R[2][0][1] += R[2][0][2];
      R[2][0][2] += R[2][0][3];

      R[2][1][0] += R[2][1][1];
      R[2][1][1] += R[2][1][2];
      R[2][1][2] += R[2][1][3];

      R[2][2][0] += R[2][2][1];
      R[2][2][1] += R[2][2][2];
      R[2][2][2] += R[2][2][3];

      R[2][3][0] += R[2][3][1];
      R[2][3][1] += R[2][3][2];
      R[2][3][2] += R[2][3][3];

      R[3][0][0] += R[3][0][1];
      R[3][0][1] += R[3][0][2];
      R[3][0][2] += R[3][0][3];

      R[3][1][0] += R[3][1][1];
      R[3][1][1] += R[3][1][2];
      R[3][1][2] += R[3][1][3];

      R[3][2][0] += R[3][2][1];
      R[3][2][1] += R[3][2][2];
      R[3][2][2] += R[3][2][3];

      R[3][3][0] += R[3][3][1];
      R[3][3][1] += R[3][3][2];
      R[3][3][2] += R[3][3][3];
   }
}

/*
 * Extract the k'th column of the tensor a into the matrix b.
 */
static	void
extract(Matrix b, Tensor a, int k)
{
   b[0][0] = a[0][0][k];
   b[0][1] = a[1][0][k];
   b[0][2] = a[2][0][k];
   b[0][3] = a[3][0][k];

   b[1][0] = a[0][1][k];
   b[1][1] = a[1][1][k];
   b[1][2] = a[2][1][k];
   b[1][3] = a[3][1][k];

   b[2][0] = a[0][2][k];
   b[2][1] = a[1][2][k];
   b[2][2] = a[2][2][k];
   b[2][3] = a[3][2][k];

   b[3][0] = a[0][3][k];
   b[3][1] = a[1][3][k];
   b[3][2] = a[2][3][k];
   b[3][3] = a[3][3][k];
}

static	float
addemup(Matrix m)
{
   return (m[0][0] + m[0][1] + m[0][2] + m[0][3] +
           m[1][0] + m[1][1] + m[1][2] + m[1][3] +
           m[2][0] + m[2][1] + m[2][2] + m[2][3] +
           m[3][0] + m[3][1] + m[3][2] + m[3][3]);
}
