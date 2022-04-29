/********NOTE********NOTE********NOTE*********
 This translation from FORTRAN leaves all code
 identical, except any pointers into the work
 space are reduced by 1, to follow C indexing.
**********************************************/

#include <limits.h>
/* meschach headers */
#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_ADDS/INCLUDES/gpskmesh.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"

/** \file gpskmesh.c
 *
 * Contains the function to call "gpskmesh" main function (band width reduction)
 *
 */

static int

gpskca(long , long *, long *, long *, int   , long  , long *, long *, long *, long *, long *),
gpskcb(long , long *, long *, long *, long  , long  , long *, long *, long *, long *, long *, long *, long *, long *, long *, long *),
gpskcj(long , long *, long *, long *, long  , long *, long  , long  , int   , long  , long *, long *, long *, long *),
gpskcg(long , long *, long *, long *, long  , long  , long  , long *, long *, long *, long  , long  , long *, long *, long *, int   , int  *, long *),
gpskcf(long , long  , long  , long *, long *, long *, int   ),
gpskce(long , long  , long  , long  , long  , long *, long *, long *, long  , long *, long *, long *, long *, int  *, long *),
gpskco(long , long *),
gpskcp(long , long *, long *),
gpskcq(long , long *, long *),
gpskcn(long , long *, long *),
gpskch(long , long *, long *, long *, long *, long  , long *, long  , long *, long *, long *, long *),
gpskci(long , long  , long  , long *, long *, long *, long *, long *),
gpskcl(long , long *, long *, long *, long *, long *, long *, long *, long *, long *),
gpskck(long , long *, long *, long *, long  , long *, long *, long  , long  , long *, long *, long *, long *),
gpskcm(long , long *, long *, long *, long *, long *, long *, long *, long *, long *),
gpskcd(long , long *, long *, long *, long  , long  , long  , long  , long *, long *, long *, long *,long *),
gpskcc(long , long *, long *, long *, long  , long  , long  , long *, long *, long *, long *, long *),
gpstrf(long , long *, long *, long *, long *, long *);

static int  setsign(long, long *,long *, long *);
static char *challoc(unsigned,unsigned);

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static int setconnectivity(const SPMAT *LHS, long *connect, long nz);
static int gpskmesh(const SPMAT *LHS, PERM *P, PERM *INVP, int optpro );

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

PERM* sp_bandwr_meschach(const SPMAT* A, PERM* P, PERM *INVP, BANDWRt_OPTION_MESCHACH option)
{
   int rc,type;
   const int type_prof = 1; /* value for gpskmesh */
   const int type_band = 0; /* value for gpskmesh */

   if ( A    == SMNULL ) error(E_NULL, "sp_bandwr_meschach");
   if ( P    == PNULL  ) error(E_NULL, "sp_bandwr_meschach");
   if ( INVP == PNULL  ) error(E_NULL, "sp_bandwr_meschach");

   if ( INVP->size != P->size ) error(E_SIZES,"sp_bandwr_meschach");
   if ( INVP->size != A->m    ) error(E_SIZES,"sp_bandwr_meschach");
   
   switch(option)
   {
      case BANDWRe_MESCHACH_BAND:     printf("   b a n d w i d t h    r e d u c t i o n"); type = type_band; break;
      case BANDWRe_MESCHACH_PROFILE:  printf("   p r o f i l e    r e d u c t i o n");     type = type_prof; break;
      default: warning(WARN_UNKNOWN, "sp_bandwr_meschach");                                type = type_band;
   }

   rc = gpskmesh(A, INVP, P, type);

   return P;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static int   gpskmesh(const SPMAT *LHS, PERM *P, PERM *INVP, int optpro)
{
   /* n is the maximum number of equations to process */
   long     i, nz, cstart, connec, degree, bandwd, profil, permut, space, gpswln, gpswrk, liwork;
   long     *iwork;
   int      error;
   long n = (long)(LHS->m);
   
   /* convert meschach matrix to gps connectivity */
   sp_compact((SPMAT*)LHS, 0.0);

   /* non-zeros of the symmetric part of  LHS */
   nz = sp_nonzeros_symmetric_part(LHS);

   
   liwork= 2L*nz + 8L*n + 4L;

   if(liwork > UINT_MAX)
   {
        printf("\n TVAASGPS: Can't allocate memory block of size %ld",liwork);
        return(1);
   }

   if  (n < 1L  ||  nz <  1L)
   {
      printf("\nimproper input values for number of rows or number of nonzeroes\n n and nz must be positive.");
      return(1);
   }
   
   iwork=(long *)challoc( (unsigned)liwork, sizeof(long));

   /* set matrix connectivity */
   setconnectivity(LHS, iwork, nz);

   
   cstart = 1;
   permut = cstart + (n + 1);
   connec = permut + n;
   /*     ... assign space within workspace
          ... build full adjacency (connectivity) structure required for
          the reordering routines.  integer workspace is partitioned as

          column starts         (   n+1  )  (lower triangle)
          permutation from gps  (    n   )
          adjacency structure   (2*nz - n)  (nz = nonzeroes in lower
          triangle and diagonal)
          node degrees          (    n   )  (full structure)
          gps workspace         ( 6*n + 3)

          the 2*nz locations consisting of the adjacency structure
          and the vector of degrees are remapped into two vectors
          of length  nz  by subroutine  gpsrpk, and used in that
          form by the routines which reformat the real data.
          */
   degree = connec + (2*nz - n);
   gpswrk = degree + n;
   gpswln = liwork - gpswrk + 1;

   error = gpstrf(n, &iwork[cstart-1], &iwork[connec-1], &iwork[gpswrk-1], &iwork[degree-1], &iwork[permut-1]);

   if  (error)
   {
      printf("\n Program terminated by error");
      return(1);
   }
   /*  ... call gibbs-poole-stockmeyer-king code to compute a desirable reordering for the matrix  */
   error= gpskca (n, &iwork[degree-1], &iwork[cstart-1], &iwork[connec-1],
                  optpro, gpswln, &iwork[permut-1], &iwork[gpswrk-1], &bandwd, &profil, &space);
   /*
   printf("\n Bandwidth= %ld  Profile= %ld  Error= %d  Space= %ld", bandwd,profil,error,space);
   */
   for(i=0; i<n; i++)
   {
      P->pe[i] = iwork[permut+i-1] - 1;  /* back to C indexing */

      /* check values */
      if( P->pe[i]>n )
      {
         error=9999;
      }
   }

   if (!error)
   {
      INVP = px_inv(P, INVP);
   }
   
   if (error)
   {
      printf("\n Error in gpsk = %d  Space=%ld",error,space);
   }
   
   free(iwork);
   return(error);
}

int  gpstrf(long n, long *cstart, long *connec, long *cstrt2, long *degree, long *permut)
{
   /*     ==================================================================
          transform connectivity data from form as
          lower triangle by columns
          (upper triangle by rows)
          to complete connectivity structure.

          this subroutine will check the data to ensure that only the
          lower triangle is present.  repeated elements are allowed,
          but no attempt will be made to detect them and reduce storage.
          ==================================================================

          ... (actual dimension for cstart and cstrt2 is  n+1.  actual
          dimension for connec is  2*nz - n)
          */
   long    i, j, k, kstart, nk, kstop, rk, rj, istart,
   jstart, jk;
   /* ==================================================================

     ... determine strict lower triangle degree of each node  */
   for(j=0;j<n;j++)  degree[j] = cstart[j+1] - cstart[j];
   /*     ... determine degree in full adjacency structure */

   for(j=0;j<n;j++)
   {
      kstart = cstart[j];
      kstop  = cstart[j+1];
      if  (kstart > kstop) continue;
      for(k=kstart;k<kstop;k++)
      {
         i = connec[k-1]-1;
         if(i == j ) continue;
         if(i < j)
         {
            printf("\ndata out of triangle i(=%ld) should be greater than  j(=%ld).",i,j);
            return(1);
         }

         if  (i > n)
         {
            printf("\nrow index out of range i = %ld j = %ld",i,j);
            return(1);
         }
         degree[i]++;
      }
   }

   /*     ... compute row starts in full adjacency structure */

   cstrt2[0] = 1;
   if  (n <= 1)  return(0);

   for(i=1;i<n;i++)   cstrt2[i] = cstrt2[i-1] + degree[i-1];

   /*     ... expand lower adjacency structure into space for full adjacency
          structure.  real workspace is left alone.
          loops run backward, from last column to first, and last row
          to first within column, to ensure that no overwriting occurs
          and that the lower triangle is contiguous as the first group
          of entries within a column of the full structure.
          */
   for(rj=1;rj<=n;rj++)
   {
      j = n - rj + 1;
      jstart = cstrt2[j-1];
      kstart = cstart[j-1];
      nk     = cstart[j] - kstart;
      if  (nk <= 0)  continue;

      for(rk=1;rk<=nk;rk++)
      {
         k = kstart + (nk - rk);
         i = connec[k-1];
         jk = jstart + (nk - rk);
         connec[jk-1] = i;

         /*  ... build upper triangle;*/

         if  (i != j)
         {
            istart = cstrt2[i-1];
            connec[istart-1] = j;
            cstrt2[i-1] = istart + 1;
         }
      }
      cstrt2[j-1] = jstart + nk;
   }
   /*  ... reset row start pointers for full connectivity structure
     and initialize permutation vector for gps     */
   for(j=0;j<n;j++)
   {
      cstart[j] = cstrt2[j] - degree[j];
      permut[j] = j+1;
   }
   cstart[n] = cstart[n-1] + degree[n-1];

   return(0);
}


int  gpskca(long n, long *degree, long *rstart, long *connec, int   optpro,
              long wrklen, long *permut, long *work, long *bandwd,
              long *profil, long *space)
{
   /*   ==================================================================
        ==================================================================
        =                                                                =
        = b a n d w i d t h    or    p r o f i l e    r e d u c t i o n  =
        =        for a sparse and (structurally) symmetric matrix,       =
        =                         using either                           =
        =                                                                =
        =   the gibbs-poole-stockmeyer algorithm (bandwidth reduction)   =
        =                               or                               =
        =          the gibbs-king algorithm (profile reduction)          =
        =                                                                =
        ==================================================================
        ==================================================================
        =     this code supersedes toms algorithms 508 and 509 in the    =
        =     collected algorithms of the acm (calgo).                   =
        ==================================================================
        ==================================================================*/

   /*------------------------------------------------------------------
     input parameters:
     ----- ----------

     n      -- the dimension of the matrix

     degree,
     rstart,
     connec -- describe the structure of the sparse matrix.
     degree(i) specifies the number of non-zero
     off-diagonal entries in the  i-th  row of the
     sparse matrix.  the column indices of these
     entries are given in consecutive locations in
     connec, starting at location  rstart[i).
     in other words, the indices of the non-zero
     off-diagonal elements of the  i-th  row are found
     in:
     connec [rstart[i)),
     connec [rstart[i) + 1),
     . . .
     connec [rstart[i) + degree[i) - 1)

     dimensions:
     rstart is dimension  n  (or longer).
     degree is dimension  n  (or longer).
     connec is dimension roughly the number of non-
     zero entries in the matrix.

     optpro -- .true. if reducing the profile of the matrix
     is more important than reducing the
     bandwidth
     .false. if bandwidth reduction is most important

     wrklen -- the  actual  length of the vector  work  as supplied
     by the user.  see the discussion of the workspace
     'work'  below for typical storage requirements.
     the value of  wrklen  will be used to ensure that
     the routine will not use more storage than is
     available.  if not enough space is given in  work
     to permit a solution to be found, the  error  flag
     will be set and further computation stopped.


     input and output parameter:
     ----- --- ------ ---------

     permut -- on input, an alternative reordering for the
     rows and columns of the matrix.  permut[i) gives
     the position in which row and column  i  should
     be placed to reduce the bandwidth or the profile.
     if the user has no alternative to the natural
     ordering implicit in  degree,  rstart  and  connec,
     he should initialize  permut  to be the identity
     permutation  permut[i) = i .

     on output,  permut  will contain the permutation
     for reordering the rows and columns which reduces
     the bandwidth and/or profile.  the result will be
     the reordering found by 'gpskca' or the reordering
     given by the user in 'permut', whichever does the
     job better.


     output parameters:
     ------ ----------

     work   -- a temporary storage vector, of length somewhat
     greater than  3n.  the space beyond  3n  required
     is problem-dependent.  any problem can be solved
     in  6n+3  locations.
     most problems can be reordered with  4n  locations
     in 'work'.  if space is not a constraint, provide
     6n+3  locations in 'work'.  otherwise, provide as
     much more than  3n  as is convenient and check the
     error flag and space required parameters (see below)

     on output, the 1st  n  locations of work will be
     a listing of the original row and column indices as
     they appear in the computed reordering.
     locations  n+1, ... , 2n  of  work  will contain
     the new positions for the equations in the order
     found by gpskca.  thus, the two vectors are inverse
     permutations of each other.  if the ordering
     found by this algorithm is better than the user-
     supplied order, the second permutation vector is
     identical to the result returned in  'permut'.

     bandwd -- the bandwidth of the matrix when rows and columns
     are reordered in the ordering returned in  permut.

     profil -- the profile of the matrix when rows and columns are
     reordered in the ordering returned in  permut.

     error  -- will be equal to zero if a new numbering could be
     found in the space provided.  otherwise,  error
     will be set to a positive error code (see table
     given below).  if the reordering algorithm has been
     stopped by lack of workspace, the space parameter
     will be set to the number of additional locations
     required to complete at least the next phase of
     the algorithm.

     whenever a non-zero value for  error  is given
     permut  will retain the values provided by the user
     and the scalars  bandwd  and  profil  will be set to
     outrageous values.  it is the user's responsibility
     to check the status of  error.

     space  -- will indicate either how much space the reordering
     actually required or how much space will be
     required to complete the next phase of the
     reordering algorithm.  the possible outcomes are ..

     error = 0          space is the minimal value for
     wrklen  required to reorder
     this matrix again.

     error <> 0         space is the minimum number
     due to lack of     of extra workspace required
     workspace          to continue the reordering
     algorithm on this matrix.

     error <> 0         space = -1
     due to error
     in data structures


     ==================================================================*/

   /*   ----------------------
        e r r o r    c o d e s
        ----------------------

        error codes have the form  0xy  or  1xy.

        errors of the form  1xy  result from inadequate workspace.

        errors of the form  0xy  are internal program checks, which
        most likely occur because the connectivity structure of the
        matrix is represented incorrectly (e.g., the degree of
        a node is not correct  or  node i is connected to node j,
        but not conversely).

        the last digit (y) is mainly useful for debugging the
        the reordering algorithm.  the middle digit  (x)  indicates
        how much of the algorithm has been performed.
        the table below gives the correspondence between the
        values of  x   and the structure of the algorithm.
        x = 0     initial processing
        x = 1     computing pseudo-diameter  (algorithm i)
        x = 2     transition between algorithm i and ii
        x = 3     combining level structures (algorithm ii)
        x = 4     transition between algorithm ii and iii
        x = 5     bandwidth numbering (algorithm iiia)
        x = 6     profile numbering (algorithm iiib)
        x = 7     final bandwidth/profile computation

        ==================================================================

        ---------------------    ---------------
        a l t e r n a t i v e    v e r s i o n s
        ---------------------    ---------------

        short integer version

        on machines with two or more precisions for integers,
        all of the input arrays except 'rstart' can be converted
        to the shorter precision as long as that shorter precision
        allows numbers as large as 'n'.  a version of this code
        suitable for use on ibm computers (integer * 2) is embedded
        as comments in this code.  all such comments have the
        characters 'cibm' in the first four columns, and precede the
        equivalent standard code which they would replace.

        connectivity compatibility version

        the original (1976) toms code  'reduce'  used a less storage
        efficient format for the connectivity table  'connec'.
        the 1976 code used a rectangular matrix of dimensions
        n  by  maxdgr,  where  maxdgr  is at least as large as
        the maximum degree of any node in the graph of the matrix.
        the format used in the current code is often substantially
        more efficient.  however, for users for whom conversion will
        be difficult or impossible, two alternatives are ..
        1.  simply note that changing the order of subscripts
        in a rectangular connection table will enable you
        to use the new version.  this subroutine will accept a
        rectangular connection table of dimensions
        maxdgr by n,
        provided that  rstart[i)  is set to  (i-1)*maxdgr + 1.
        2.  the author will make available a variant version
        'gpskra', which expects the adjacency matrix or
        connectivity table in the same form as did  'reduce'.
        this version can be obtained by writing to ..
        john gregg lewis
        boeing computer services company
        mail stop 9c-01
        p.o. box 24346
        seattle, wa 98124
        please include a description of the computing
        environment on which you will be using the code.

        ==================================================================*/

   long     i, inc1, inc2, avail, nxtnum, lowdg, stnode, nleft,
   tree1, tree2, depth, empty, stotal, reqd, cspace,
   lvllst, lvlptr, active, rvnode, width1, width2, mxdg;
   int       error;
   int       revrs1, oneis1;

   /*    ==================================================================

         << number any degree zero nodes >>

         while << some nodes yet unnumbered >> do
         << find a pseudo-diameter of the matrix graph >>
         << convert form of level trees >>
         << combine level trees into one level structure >>
         << convert form of level structure >>
         if optpro then
         << renumber by king algorithm >>
         else
         << renumber by reverse cuthill-mckee algorithm >>

         ==================================================================*/

   /*     ... initialize counters, then number any nodes of degree  0.
          the list of nodes, by new number, will be built in place at
          the front of the work area. */

   nxtnum = 1;
   *space = 2*n;

   mxdg = 0;
   for(i=0; i<n; i++)
   {
      if( degree[i] < 0 )
      {
         *space=-1;
         setsign(n,degree,bandwd,profil);
         return(1);
      }
      if( degree[i]==0 )
      {
         work[nxtnum-1] = i+1;
         nxtnum++;
      }
      else
      if  ( degree[i] > mxdg )
      {
         mxdg = degree[i];
      }
   }


   /* ==============================
      ... while  nxtnum <= n  do ...
      ==============================*/

   while( nxtnum <= n )
   {

      /*... find an unnumbered node of minimal degree */

      lowdg = mxdg + 1;
      stnode = 0;
      for(i=0;i<n;i++)
      {
         if( (degree[i] > 0) && (degree[i] < lowdg) )
         {
            lowdg = degree[i];
            stnode = i+1;
         }
      }

      if ( stnode == 0 )
      {
         *space=-1;
         setsign(n,degree,bandwd,profil);
         return(22222);
      }

      /* ... set up pointers for three lists in work area, then look
         for pseudo-diameter, beginning with stnode. */

      avail = (wrklen - nxtnum + 1) / 3;
      nleft = n - nxtnum + 1;
      *space = max(*space, nxtnum + 3*n - 1);
      
      if ( avail < n )
      {
         *space=-1;
         setsign(n,degree,bandwd,profil);
         return(101);
      }
      
      error=gpskcb (n, degree, rstart, connec, avail, nleft,
                    &stnode, &rvnode, &work[nxtnum-1], &tree1, &tree2,
                    &active, &depth, &width1, &width2, space);
      
      if ( error != 0 )
      {
         setsign(n,degree,bandwd,profil);
         return(error);
      }
      *space = max(*space, nxtnum + 3*(active+depth+1) - 1);

      /* ... dynamic space check for most of remainder of algorithm */

      reqd = max (nxtnum + 2*n + 3*depth - 1, 3*n + 2*depth + 1);
      *space = max (*space, reqd);
      
      if  ( wrklen < reqd )
      {
         *space=-1;
         setsign(n,degree,bandwd,profil);
         return(102);
      }

      /*  ... output from gpskcb is a pair of level trees, in the form
          of lists of nodes by level.  convert this to two lists of
          of level number by node.  at the same time pack
          storage so that one of the level tree vectors is at the
          back end of the work area.   */

      lvlptr = nxtnum + avail - depth;
      error=gpskce (n, avail, active, depth, wrklen, &work[nxtnum-1],
                    &work[lvlptr-1], &work[0], nxtnum, &tree1,
                    &tree2, &width1, &width2, &oneis1, space);
      
      if ( error != 0 )
      {
         setsign(n,degree,bandwd,profil);
         return(error);
      }

      if (( tree1 != wrklen - n +1  ) || (tree2 != nxtnum))
      {
         *space=-1;
         setsign(n,degree,bandwd,profil);
         return(2);
      }

      /* ... combine the two level trees into a more general
         level structure. */
      avail = wrklen - nxtnum + 1 - 2*n - 3*depth;
      stotal = n + nxtnum;
      empty = stotal + depth;
      inc1 = tree1 - depth;
      inc2 = inc1 - depth;

      error=gpskcg (n, degree, rstart, connec, active, width1,
                    width2, &work[tree1-1], &work[tree2-1], &work[empty-1],
                    avail, depth, &work[inc1-1], &work[inc2-1],
                    &work[stotal-1], oneis1, &revrs1, &cspace);
      
      if ( error != 0 )
      {
         setsign(n,degree,bandwd,profil);
         return(error);
      }
      *space = max(*space, nxtnum + cspace - 1);

      /* ... combined level structure is represented by gpskcg as
         a vector of level numbers.  for renumbering phase,
         convert this also to the inverse permutation. */

      lvlptr = tree1 - (depth+1);
      lvllst = lvlptr - active;
      if ( stotal + depth > lvlptr )
      {
         *space=-1;
         setsign(n,degree,bandwd,profil);
         return(4);
      }

      error=gpskci (n, active, depth, &work[tree1-1], &work[lvllst-1], &work[lvlptr-1], &work[stotal-1], space);
      
      if  (error != 0)
      {
         setsign(n,degree,bandwd,profil);
         return(error);
      }

      /* ... now renumber all members of this component using
         either a reverse cuthill-mckee or a king strategy,
         as profile or bandwidth reduction is more important. */
      if (!optpro )
      {
         error=gpskcj (n, degree, rstart, connec, active,
                       &work[nxtnum-1], stnode, rvnode, revrs1, depth,
                       &work[lvllst-1], &work[lvlptr-1], &work[tree1-1],
                       space);
         
         if ( error != 0 )
         {
            setsign(n,degree,bandwd,profil);
            return(error);
         }
         nxtnum +=  active;
      }
      else
      {
         error=gpskck (n, degree, rstart, connec, lvllst-1, &nxtnum,
                       &work[0], active, depth, &work[lvllst-1],
                       &work[lvlptr-1], &work[tree1-1], space);
         
         if ( error != 0 )
         {
            setsign(n,degree,bandwd,profil);
            return(error);
         }
      }
      /*=========================================================
        ... end of while loop ... repeat if graph is disconnected
        =========================================================*/
   }
   /* check whether initial numbering or final numbering provides better results */

   if(wrklen < 2*n)
   {
      setsign(n,degree,bandwd,profil);
      *space = 2*n - wrklen;
      return(10);
   }

   if(!optpro)
     error=gpskcl (n, degree, rstart, connec, &work[0], &work[n],
                   permut, bandwd, profil, space);
   else
     error=gpskcm (n, degree, rstart, connec, &work[0], &work[n],
                   permut, bandwd, profil, space);

   return(error);
}

int setsign(long dim, long *vector, long *flag1, long *flag2)
{
   register int ii;

   *flag1=-1;
   *flag2=-1;
   
   for(ii=0; ii<dim; ii++)
   {
      if( vector[ii] < 0 )
      {
         vector[ii] = -vector[ii];
      }
   }
   return(0);
}

int   gpskcb  (long n, long *degree, long *rstart, long *connec,
               long avail, long nleft,
               long *stnode, long *rvnode, long *work, long *forwd,
               long *bestbk, long *nnodes, long *depth, long *fwidth,
               long *bwidth, long *space)
{

   /*==================================================================

         find a pseudo-diameter of the matrix graph ...

         << build a level tree from stnode >>
         repeat
         << build a level tree from each node 'bknode' in the
         deepest level of  stnode's tree >>
         << replace 'stnode' with 'bknode' if a deeper and
         narrower tree was found. >>
         until
         << no further improvement made >>

         ... heuristic above differs from the algorithm published in
         siam j. numerical analysis, but matches the code
         distributed by toms.


         parameters :

         n, degree, rstart & connec  describe the matrix structure

         work   -- working space, of length  3*avail, used to store
         three level trees.

         output:
         stnode is initially the number of a node to be used to
         start the process, to be the root of the first tree.
         on output, stnode is the end of the pseudo-diameter whose
         level tree is narrowest.

         rvnode will be the other end of the pseudo-diameter.

         nnodes will be the number of nodes in this connected
         componnent of the matrix graph, i.e., the length of
         the level trees.

         depth  -- the depth of the level trees being returned,
         i.e., the length of the pseudo-diameter.

         ==================================================================*/

   /*     structure of workspace ...

          ---------------------------------------------------------------
          : numbered :  tlist1  ptr1  :  tlist2  ptr2  :  tlist3  ptr3  :
          ---------------------------------------------------------------

          tlisti is a list of nodes of length  'active'
          ptri   is a list of pointers into tlisti, of length  'depth+1'

          The variable work was declared as (avail,3) in the original
          FORTRAN, so in C pointers are calculated to rows of length
          avail.
          ==================================================================*/

   long    backwd, mxdpth, width, fdepth, lstlvl,
   nlast, t, i, bknode;
   int     error;
   int     improv;

   /*... build initial level tree from 'stnode'.  find out how many
     nodes lie in the current connected component.  */

   *forwd = 1;
   backwd = 2;
   *bestbk = 3;
   error= gpskcc (n, degree, rstart, connec, *stnode, avail, nleft,
                  &work[0], nnodes, depth, &width, space);
   if ( error != 0 )  return(error);
   mxdpth = avail - *nnodes - 1;
   /* ==========================================
        repeat until no deeper trees are found ...
        ==========================================*/
   do{
      *fwidth = width;
      fdepth = *depth;
      lstlvl = avail - *depth + 1;
      i= avail*(*forwd-1)+lstlvl - 1;
      nlast = work[i-1] - work[i];
      lstlvl = work[i];
      i= avail*(*forwd-1)+lstlvl - 1;
      *bwidth = n+1;

      /*... sort the deepest level of 'tree1' tree into increasing
        order of node degree.   */

      error=gpskcq (nlast, &work[i], degree);
      if  (error != 0){
         *space=-1;
         return(11);
      }

      /* ... build level tree from nodes in 'lstlvl' until a deeper
         and narrower tree is found or the list is exhausted.  */

      improv = 0;
      for(i=0;i<nlast;i++){
         bknode = work[(*forwd-1)*avail+lstlvl+i-1 ];
         error=gpskcd (n, degree, rstart, connec, bknode, avail,
                       *nnodes, mxdpth, &work[(backwd-1)*avail], depth, &width,
                       bwidth, space);
         if ( error != 0 ) return(error);

         if ( *depth > fdepth ){
            /* ... new deeper tree ... make it new 'tree1' tree
               and break out of 'do' loop.  */
            improv = 1;
            t = *forwd;
            *forwd = backwd;
            backwd = t;
            *stnode = bknode;
            break;
         }
         /* ... else check for narrower tree. */
         if ( width < *bwidth ){
            t = *bestbk;
            *bestbk = backwd;
            backwd = t;
            *bwidth = width;
            *rvnode = bknode;
         }
      }
   }
   while ( improv );

   *depth = fdepth;
   return(0);
}

int   gpskcc(long n, long *degree, long *rstart, long *connec, long stnode,
             long avail, long nleft, long *list, long *active, long *depth,
             long *width, long *space){

   /* ==================================================================
      build the level tree rooted at 'stnode' in the space provided in
      list.  check for overrun of space allocation.
      ==================================================================

      input ...

      n, degree, rstart, connec -- describe the matrix structure
      stnode -- the root of the level tree.
      avail  -- the length of the working space available
      nleft  -- the number of nodes yet to be numbered
      list   -- the working space.
      output ...
      active -- the number of nodes in the component
      depth  -- the depth of the level tree rooted at  stnode.
      width  -- the width of the level tree rooted at  stnode.
      error  -- zero unless storage was insufficient.
      ------------------------------------------------------------------*/

   long lstart, nlevel, front, j, newnod, ptr, cdgree,
   lfront, listj;
   int      error;

   /*... build the level tree using  list  as a queue and leaving
     the nodes in place.  this generates the nodes ordered by level
     put pointers to the beginning of each level, building from
     the back of the work area. */

   *active = 1;
   *depth = 0;
   *width = 0;
   error = 0;
   lstart = 1;
   front = 1;
   list [*active-1] = stnode;
   degree [stnode-1] = -degree [stnode-1];
   list [avail-1]  = 1;
   nlevel = avail;
   /* ... repeat until queue becomes empty or we run out of space.
      ------------------------------------------------------------*/
   do{
      if ( front >= lstart ){

         /*... first node of level.  update pointers.*/

         lstart = *active + 1;
         *width = max(*width, lstart - list[nlevel-1]);
         nlevel--;
         (*depth)++;
         if ( nlevel <= *active ){
            *space = 3 * ( (nleft+1-*active)*(*depth) / nleft + (nleft+1-*active) );
            return(110);
         }
         list[nlevel-1] = lstart;
      }
      /*... find all neighbors of current node, add them to queue.*/
      lfront = list [front-1];
      ptr = rstart [lfront-1];
      cdgree = -degree [lfront-1];
      if (cdgree <= 0){
         *space=-1;
         return(12);
      }
      for(j=0;j<cdgree;j++){
         newnod = connec[ptr-1]-1;   /* C indexing done for newnod */
         ptr++;

         /*... add to queue only nodes not already in queue*/
         if ( degree[newnod] > 0 ){
            degree [newnod] = -degree [newnod];
            (*active)++;
            if ( nlevel <= *active ){
               *space = 3*( (nleft+1-*active)*(*depth) / nleft + (nleft+1-*active) );
               return(110);
            }
            if ( *active > nleft  ){
               *space=-1;
               return(12);
            }
            list [*active-1] = newnod+1;
         }
      }

      front++;
      /*   ... is queue empty?
               -------------------*/
   }
   while( front <= *active );

   /*... yes, the tree is built.  undo our markings.*/

   for(j=0;j<*active;j++){
      listj = list[j]-1;
      degree [listj] = -degree [listj];
   }

   return(0);
}

int   gpskcd   (long n, long *degree, long *rstart, long *connec,
                long stnode, long avail, long active, long mxdpth,
                long *list, long *depth, long *width, long *maxwid,
                long *space){

   /* ==================================================================;
      build the level tree rooted at 'stnode' in the space provided in
      list.  overflow check needed only on depth of tree.

      build the level tree to completion only if the width of all
      levels is smaller than 'maxwid'.  if a wider level is found
      terminate the construction.
      ==================================================================

      input ...
      n, degree, rstart, connec -- describe the matrix structure
      stnode -- the root of the level tree.
      avail  -- the length of the working space available
      nleft  -- the number of nodes yet to be numbered
      active -- the number of nodes in the component
      mxdpth -- maximum depth of level tree possible in
      allotted working space
      list   -- the working space.
      output ...
      depth  -- the depth of the level tree rooted at  stnode.
      width  -- the width of the level tree rooted at  stnode.
      maxwid -- limit on width of the tree.  tree will not be
      used if width of any level is as great as
      maxwid, so construction of tree need not
      continue if any level that wide is found.
      error  -- zero unless storage was insufficient.

      ------------------------------------------------------------------*/

   long    lstart, nlevel, front, j, newnod, ptr, back,
           sptr,fptr, lfront, listj;
   int     error;

   /* ... build the level tree using  list  as a queue and leaving
      the nodes in place.  this generates the nodes ordered by level
      put pointers to the beginning of each level, building from
      the back of the work area.*/

   back = 1;
   *depth = 0;
   *width = 0;
   error = 0;
   lstart = 1;
   front = 1;
   list [back-1] = stnode;
   degree [stnode-1] = -degree [stnode-1];
   list [avail-1]  = 1;
   nlevel = avail;

   /*... repeat until queue becomes empty or we run out of space.
     ------------------------------------------------------------*/

   do{
      if ( front >= lstart ){

         /* ... first node of level.  update pointers.*/

         lstart = back + 1;
         *width = max (*width, lstart - list[nlevel-1]);
         if  ( *width >= *maxwid ){
            /*... abort generation of tree because it is already too wide */
            *width = n + 1;
            *depth = 0;
            for(j=0;j<back;j++){
               listj = list[j]-1;
               degree [listj] = -degree [listj];
            }
            return(0);
         }
         nlevel--;
         (*depth)++;
         if ( *depth > mxdpth ){
            *space = 3 * ( (active+1-back)*(*depth) / active + (active+1-back) );
            return(111);
         }
         list[nlevel-1] = lstart;
         /*... find all neighbors of current node, add them to queue.*/
      }
      lfront = list[front-1];
      sptr = rstart[lfront-1];
      fptr = sptr - degree[lfront-1] - 1; 
      for(ptr=sptr;ptr<=fptr;ptr++){
         newnod = connec[ptr-1];
         /*... add to queue only nodes not already in queue*/
         if ( degree[newnod-1] > 0 ){
            degree[newnod-1] = -degree[newnod-1];
            back++;
            list[back-1] = newnod;
         }
      }

      front++;
      /*         ... is queue empty?
                 -------------------*/
   }
   while ( front <= back );

   /* ... yes, the tree is built.  undo our markings.*/
   if (back != active){
      *space=-1;
      return(13);
   }
   for(j=0;j<back;j++){
      listj = list[j]-1;
      degree[listj] = -degree[listj];
   }

   return(0);
}

int   gpskce(long n, long avail, long active, long depth, long wrklen,
             long *lvllst, long *lvlptr, long *work, long nxtnum,
             long *tree1, long *tree2, long *width1, long *width2,
             int   *oneis1, long *space){

   /* ==================================================================

      transition between algorithm i and algorithm ii of
      the gibbs-poole-stockmeyer paper.

      in this implementation algorithm i represents level trees as
      lists of nodes ordered by level.  algorithm ii appears to require
      level numbers indexed by node -- vectors for efficiency.
      this subroutine changes the level tree representation to that
      required by algorithm ii.  note that the first algorithm can be
      carried out with the level number vector format, probably requring
      more computation time, but perhaps less storage.

      input:  two level trees, as level lists and level pointers,
      found in two of the three columns of the arrays 'lvllst'
      and 'lvlptr'

      output: two level trees, as vectors of level numbers,
      one packed to the front, one to the rear of the working
      area 'work'.  note that 'work', 'lvllst' and 'lvlptr'
      share common locations.

      ================================================================

      ... structure of workspace

      input .. (output from gpskcb)

      --------------------------------------------------------------
      : numbered : tlist1  ptr1  :  tlist2  ptr2  :  tlist3  ptr3  :
      --------------------------------------------------------------

      output .. (goes to combin)

      --------------------------------------------------------------
      : numbered :  tree2  :           ...               :  tree1  :
      --------------------------------------------------------------

      ==================================================================*/
/*****NOTE****
For the C version, pointers are calculated into this space:
 example   lvlptr+0=lvlptr(1,1)  lvlptr+avail=lvlptr(1,2)
*/
   long     i, btree, ftree, fwidth, bwidth;
   int      error;


   /* ... check that we have enough room to do the necessary unpacking */
   if (3*avail > wrklen){
      *space=-1;
      return(20);
   }
   if (avail < n){
      *space = 3 * (n - avail);
      return(120);
   }
   /*... input has three possible cases:
     lvllst(*,1) is empty
     lvllst(*,2) is empty
     lvllst(*,3) is empty */
   ftree = *tree1;
   btree = *tree2;
   fwidth = *width1;
   bwidth = *width2;
   *tree1 = wrklen - n + 1 ;    
   *tree2 = nxtnum ;
   if ( (ftree != 1) && (btree != 1) ){
      /*... case 1:  1st slot is empty.  unpack 3 into 1, 2 into 3 */

      if (ftree == 2){
         *oneis1 = 1;
         *width2 = bwidth;
         *width1 = fwidth;
      }
      else{
         *oneis1 = 0;
         *width1 = bwidth;
         *width2 = fwidth;
      }
      error=gpskcf (n, active, depth, &lvllst[2*avail], &lvlptr[2*avail],
                    &work[*tree2-1], *oneis1);
      error= gpskcf (n, active, depth, &lvllst[avail], &lvlptr[avail],
                     &work[*tree1-1], !(*oneis1) );
      return(0);
   }
   if ( (ftree != 2) && (btree != 2) ){
      /*... case 2:  2nd slot is empty.  to enable complete
        repacking, move 3 into 2, then fall into next case */

      for(i=0;i<active;i++)   lvllst[avail+i] = lvllst[2*avail+i];
      for(i=-1;i<depth;i++)   lvlptr[avail+i] = lvlptr[2*avail+i];
   }
   /* ... case 3:  slot 3 is empty.  move 1 into 3, then 2 into 1. */

   if (ftree != 1){
      *oneis1 = 0;
      *width1 = bwidth;
      *width2 = fwidth;
   }
   else{
      *oneis1 = 1;
      *width1 = fwidth;
      *width2 = bwidth;
   }
   error=gpskcf (n, active, depth, &lvllst[0], &lvlptr[0],
                 &work[*tree1-1], !(*oneis1) );
   error=gpskcf (n, active, depth, &lvllst[avail], &lvlptr[avail],
                &work[*tree2-1], *oneis1);
   return(0);
}

int   gpskcf  (long n, long active, long depth, long *lvllst,
               long *lvlptr, long *lvlnum, int   revers){

   /*==================================================================

     convert level structure representation from a list of nodes
     grouped by level to a vector giving level number for each node.

     lvllst, lvlptr -- list of lists

     lvlnum -- output vector of level numbers

     revers -- if .true., number level structure from back end
     instead of from front

     ==================================================================*/

   long     i, level, lstart, lend, xlevel, plstrt, lvllsi;

   if  (active != n){
      /*... if not all nodes of graph are active, mask out the
        nodes which are not active */
      for(i=0;i<n;i++)   lvlnum[i] = 0;
   }
   for(level=1;level<=depth;level++){
      xlevel = level;
      plstrt = depth - level + 1;
      if (revers) xlevel = plstrt;
      lstart = lvlptr[plstrt-1];
      lend = lvlptr[plstrt - 2]-1;
      for(i = lstart;i<=lend;i++){
         lvllsi = lvllst[i-1] - 1;
         lvlnum[lvllsi] = xlevel;
      }
   }
   return(0);
}

int   gpskcg(long n, long *degree, long *rstart, long *connec, long active,
             long width1, long width2, long *tree1, long *tree2, long *work,
             long wrklen, long depth, long *inc1, long *inc2, long *total,
             int   oneis1, int   *revrs1, long *space){

   /* ==================================================================

      combine the two rooted level trees into a single level structure
      which may have smaller width than either of the trees.  the new
      structure is not necessarily a rooted structure.

      parameters:

      n, degree, rstart, connec -- give the dimension and structure
      of the sparse symmetric matrix
      active -- the number of nodes in this connected component of
      the matrix graph
      tree1  -- on input, one of the input level trees.  on
      output, the combined level structure
      tree2  -- the second input level tree
      width1 -- the maximum width of a level in tree1
      width2 -- the maximum width of a level in tree2
      work   -- a working area of length 'wrklen'
      inc1,  -- vectors of length 'depth'
      inc2,
      total
      oneis1 -- indicates whether tree1 or tree2 represents the
      forward tree or the backwards tree of phase 1.
      used to mimic arbitrary tie-breaking procedure of
      original gibbs-poole-stockmeyer code.
      revrs1 -- output parameter indicating whether a backwards
      ordering was used for the largest component of
      the reduced graph
      error  -- non-zero only if failure of space allocation or
      data structure error found
      space -- minimum space required to rerun or complete phase.
      ------------------------------------------------------------------

      ==================================================================

      << remove all nodes of pseudo-diameters >>
      << find connected components of reduced graph >>
      << combine level trees, component by component >>

      ==================================================================

      structure of workspace ...

      ------------------------------------------------------------------
      : numbered : tree2 : total : nodes : start : size : inc1 : inc2 :
      ------------------------------------------------------------------

      --------
      tree1 :
      --------

      numbered  is the set of  numbered nodes (probably empty)

      tree1 and tree1 are level trees (length n)
      total, inc1 and inc2  are vectors of node counts per level
      (length 'depth')
      nodes is the set of nodes in the reduced graph (the nodes
      not on any shortest path from one end of the
      pseudodiameter to the other)
      start, size are pointers into 'nodes', one of each for
      each connected component of the reduced graph.
      the sizes of nodes, start and size are not known apriori.

      ==================================================================*/
   long     i, size, avail, cstop, start, compon, tree1i, pcstrt,
   cstart, mxinc1, mxinc2, compns, mxcomp, offdia,
   csize, pcsize, worki, tworki;
   int      error;

   /* ------------------------------------------------------------------
      ... find all shortest paths from start to finish.  remove nodes on
      these paths and in other connected components of full graph
      from further consideration.  sign of entries in tree1 is used
      as a mask. */

   offdia = active;
   for(i=0;i<depth;i++)   total[i] = 0;

   for(i=0;i<n;i++){
      if((tree1[i] == tree2[i]) && (tree1[i] != 0)){
         tree1i = tree1[i]-1;
         total[tree1i]++;
         tree1[i] = - tree1[i];
         offdia--;
      }
   }

   if ( offdia == 0 ){
      /*... default when the reduced graph is empty */
      *revrs1=1;
      *space=2*n;
      return(0);
   }
   if ( offdia < 0 ){
      *space=-1;
      return(30);
   }
   /*... find connected components of graph induced by the nodes not
     removed.  'mxcomp' is the largest number of components
     representable in the working space available.*/

   avail = wrklen - offdia;
   mxcomp = avail/2;
   start = offdia + 1;
   size = start + mxcomp ;

   if  (mxcomp <= 0){
      *space=2-avail;
      return(131);
   }
   error= gpskch (n, degree, rstart, connec, tree1, offdia, work,
                  mxcomp, &work[start-1], &work[size-1], &compns,
                  space);
   if ( error != 0 ){
      *space=-1;
      return(error);
   }
   /*... record space actually used  (not including  numbered ) */
   *space = 2*n + 3*(depth) + 2*compns + offdia;

   /* ... sort the component start pointers into increasing order
      of size of component */

   if (compns > 1)  error=gpskcn (compns, &work[size-1], &work[start-1]);
   if  (error != 0){
      *space=-1;
      return(32);
   }

   /*... for each component in turn, choose to use the ordering of the
     'forward' tree1 or of the 'backward' tree2 to number the nodes
     in this component.  the numbering is chosen to minimize the
     maximum increment to any level. */
   for(compon=1;compon<=compns;compon++){
      pcstrt = start + compon -1;
      cstart = work[pcstrt-1];
      pcsize = size + compon - 1 ;
      csize = work[pcsize-1];
      cstop  = cstart + csize - 1;
      if ( ( csize < 0 ) || ( csize > offdia ) ){
         *space=-1;
         return(31);
      }

      for(i=0;i<depth;i++){
         inc1[i] = 0;
         inc2[i] = 0;
      }
      mxinc1 = 0;
      mxinc2 = 0;

      for(i=cstart;i<=cstop;i++){
         worki = work[i-1]-1;     /* C indexing */
         tworki = -tree1[worki]-1;   /* "  */
         inc1[tworki]++;
         tworki =  tree2[worki]-1;   /* "  */
         inc2[tworki]++;
      }
      /*... baroque tests below duplicate the gibbs-poole-stockmeyer-
        crane program, *** not *** the published algorithm. */

      for(i=0;i<depth;i++){
         if ((inc1[i] == 0L) && (inc2[i] == 0L)) continue;
         if  (mxinc1  <  total[i] + inc1[i])  mxinc1 = total[i] + inc1[i];
         if  (mxinc2  <  total[i] + inc2[i])  mxinc2 = total[i] + inc2[i];
      }
      /* ... use ordering of narrower tree unless it increases
         width more than wider tree.  in case of tie, use tree 2! */

      if ( (mxinc1 > mxinc2)  ||
          ( (mxinc1 == mxinc2) && ( (width1 > width2) ||
            ( (width1 == width2) && oneis1) ) ) ){
         if ( compon == 1 )  *revrs1 = oneis1;
         for(i=cstart;i<=cstop;i++){
            worki = work[i-1]-1;
            tree1[worki] = - tree2[worki];
         }
         for(i=0;i<depth;i++){
            total[i]+= inc2[i];
         }
      }
      else{
         if ( compon == 1 )  *revrs1 = ! oneis1;
         for(i=0;i<depth;i++){
            total[i]+= inc1[i];
         }
      }
   }
   return(0);
}

int   gpskch   (long n, long *degree, long *rstart, long *connec,
               long *status, long nreduc, long *work, long mxcomp,
               long *start, long *size, long *compns, long *space){

   /*==================================================================

     find the connected components of the graph induced by the set
     of nodes with positive 'status'.  we shall build the list of
     connected components in 'work', with a list of pointers
     to the beginning nodes of components located in 'start'

     parameters ...

     n      -- dimension of the original matrix
     degree, rstart, connec -- the structure of the original matrix

     status -- derived from a level tree. positive entries indicate
     active nodes.  nodes with status <= 0 are ignored.
     nreduc -- the number of active nodes

     work   -- work space, used as a queue to build connected
     components in place.

     mxcomp -- maximum number of components allowed by current
     space allocation.  must not be violated.

     start  -- pointer to beginning of  i-th  connected component

     size   -- size of each component

     compns -- number of components actually found

     error  -- should be zero on return unless we have too little
     space or we encounter an error in the data structure

     space  -- maximum amount of workspace used / needed

     ==================================================================*/

     long    i, j, free, jptr, node, jnode, front, cdgree, root;

     /* ------------------------------------------------------------------
        repeat
        << find an unassigned node and start a new component >>
        repeat
        << add all new neighbors of front node to queue, >>
        << remove front node.                            >>
        until <<queue empty>>
        until << all nodes assigned >>   */

   free   = 1;
   *compns = 0;
   root   = 1;

   /* ... start of outer repeat loop */

   /* ... find an unassigned node */
   do{
      for(i=root;i<=n;i++){
         if (status[i-1] <= 0) continue;
         node = i; break;
      }
      if(i == n+1){
         *space=-1;
         return(34);
      }
      /*         ... start new component */

      (*compns)++;
      root = node +1;
      if (*compns > mxcomp){
         *space=nreduc-free+1;
         return(130);
      }
      start[(*compns)-1] = free;
      work[free-1] = node;
      status[node-1] = -status[node-1];
      front = free;
      free++;
      /*   ... inner repeat until queue becomes empty */
      do{
         node = work[front-1]-1;      /* C indexing */
         front++;

         jptr = rstart[node]-1;
         cdgree = degree[node];
         for(j=0;j<cdgree;j++){
            jnode = connec[jptr]-1;
            jptr++;
            if (status[jnode] < 0) continue;
            if (status[jnode] == 0){
               *space=-1;
               return(33);
            }
            status[jnode] = -status[jnode];
            work[free-1] = jnode+1;
            free++;
         }
      }
      while(front < free);

      /* ... end of inner repeat.  compute size of component and
         see if there are more nodes to be assigned  */

      size[*compns-1] = free - start[*compns-1];
   }
   while(free <= nreduc);
   if (free != nreduc+1){
      *space=-1;
      return(35);
   }
   return(0);
}
int  gpskci(long n, long active, long depth, long *lstruc, long *lvllst,
            long *lvlptr, long *ltotal, long *space){

   /* ==================================================================
      transitional subroutine, algorithm ii to iiia or iiib.

      convert level structure given as vector of level numbers for nodes
      to structure as list of nodes by level

      n, active, depth -- problem sizes
      lstruc -- input level structure
      lvllst, lvlptr -- output level structure
      ltotal -- number of nodes at each level (precomputed
      ===============================================================

      structure of workspace ..

      input (from combin) ..

      ------------------------------------------------------------------
      :  numbered  :  ..(n)..  :  total  :         ...        :  tree  :
      ------------------------------------------------------------------

      output (to gpskcj or gpskck) ..

      ------------------------------------------------------------------
      :  numbered  :       ...             :  tlist  :  tptr  :  tree  :
      ------------------------------------------------------------------

      here, numbered is the set of nodes in numbered components
      total is a vector of length 'depth' giving the number
      of nodes in each level of the 'tree'.
      tlist, tptr are lists of nodes of the tree, arranged
      by level.  tlist is of length 'active', tptr 'depth+1'.

      =================================================================*/

   long     i, acount, start, level, plevel;

   /*   ... establish starting and ending pointers for each level */
   start = 1;
   for(i=0;i<depth;i++){
      lvlptr[i] = start;
      start += ltotal[i];
      ltotal[i] = start;
   }
   lvlptr[depth] = start;

   acount = 0;
   for(i=0;i<n;i++){
      if(lstruc[i]>0){
         *space=-1;
         return(40);
      }
      if (lstruc[i]==0) continue;
      level= -lstruc[i];
      lstruc[i] = level;
      plevel = lvlptr[level-1] - 1;
      lvllst[plevel] = i+1;
      lvlptr[level-1]++;
      acount++;
      if (lvlptr[level-1] > ltotal[level-1]){
         *space=-1;
         return(41);
      }
   }
   /* ... reset starting pointers */

   lvlptr[0] = 1;
   for(i=0;i<depth;i++)   lvlptr[i+1] = ltotal[i];
   return(0);
}

int   gpskcj (long n, long *degree, long *rstart, long *connec,
              long ncompn, long *invnum, long snode1, long snode2,
              int  revrs1, long depth, long *lvllst, long *lvlptr,
              long *lvlnum, long *space){

   /* ==================================================================

      number the nodes in a generalized level structure according
      to a generalization of the cuthill mckee strategy.

      n      -- dimension of original problem
      degree, rstart, connec -- give structure of sparse and
      symmetric matrix
      ncompn -- number of nodes in this component of matrix graph
      invnum -- will become a list of the original nodes in the order
      which reduces the bandwidth of the matrix.
      nxtnum -- the next index to be assigned (1 for first component)
      revrs1 -- if .true., first component of reduced graph was numbered
      backwards.
      lvllst -- list of nodes in level tree ordered by level.
      lvlptr -- position of initial node in each level of lvllst.
      lvlnum -- level number of each node in component


      ==================================================================

      numbering requires two queues, which can be build in place
      in invnum.


      ==================================================================
      a l g o r i t h m    s t r u c t u r e
      ==================================================================

      << set queue1 to be the set containing only the start node. >>

      for level = 1 to depth do

      begin
      loop

      repeat
      begin
      << cnode <- front of queue1                        >>
      << add unnumbered neighbors of cnode to the back   >>
      << of queue1 or queue2 (use queue1 if neighbor     >>
      << at same level, queue2 if at next level).  sort  >>
      << the newly queued nodes into increasing order of >>
      << degree.  number cnode, delete it from queue1.   >>
      end
      until
      << queue1 is empty >>

      exit if << all nodes at this level numbered >>

      begin
      << find the unnumbered node of minimal degree at this >>
      << level, restart queue1 with this node.              >>
      end

      end << loop loop >>

      << promote queue2 to be initial queue1 for next iteration >>
      << of  for  loop.                                         >>

      end <<for loop>>

      ==================================================================

      structure of workspace ..

      --------------------------------------------------------------
      : numbered :  queue1  :  queue2  : ... : tlist : tptr : tree :
      --------------------------------------------------------------

      on completion, we have only a new, longer numbered set.

      ==================================================================*/
   long     i, bq1, bq2, fq1, inc, cptr, cnode,
   inode, level, nleft, lstart, lwidth, queue1,
   queue2, cdgree, xlevel, stnode, ilevel, sq1, sq2,
   nsort, lowdg, bptr, lvllsc, lvllsb, invnmi;
   int       error,
             forwrd, rlevel;

   /* ------------------------------------------------------------------*/

   /*... gibbs-poole-stockmeyer heuristic choice of order */

   if  (degree[snode1-1] <= degree[snode2-1]){
      forwrd = revrs1;
      stnode = snode1;
   }
   else{
      forwrd = !revrs1;
      stnode = snode2;
   }
   /*     ... set up initial queues at front of 'invnum' for forwrd order,
          at back for reversed order.  */

   if (!forwrd){
      inc = -1;
      queue1 = ncompn;
   }
   else{
      inc = +1;
      queue1 = 1;
   }
   invnum[queue1-1] = stnode;
   if (lvlnum[stnode-1] == depth) rlevel=1;
   else rlevel=0;
   lvlnum[stnode-1] = 0;
   fq1 = queue1;
   bq1 = queue1 + inc;

     /*    -------------------------------
           number nodes level by level ...
           -------------------------------  */

     for(xlevel=1;xlevel<=depth;xlevel++){
        level = xlevel ;
        if  (rlevel)  level = depth - xlevel + 1;

        lstart = lvlptr[level-1];
        lwidth = lvlptr[level] - lstart;
        nleft = lwidth;
        queue2 = queue1 + inc*lwidth;
        bq2 = queue2;

        /* ==============================================================
           ... 'loop' construct begins at statement 1000
           the inner 'repeat' will be done as many times as
           is necessary to number all the nodes at this level.
           ==============================================================*/

        while(1){

           /* ==========================================================
              ... repeat ... until queue1 becomes empty
              take node from front of queue1, find each of its
              neighbors which have not yet been numbered, and
              add the neighbors to queue1 or queue2 according to
              their levels.
              ========================================================== */
           do{
              cnode = invnum[fq1-1];
              fq1+= inc;
              sq1 = bq1;
              sq2 = bq2;
              nleft--;
              cptr = rstart[cnode-1];
              cdgree = degree[cnode-1];
              for(i=0;i<cdgree;i++){
                 inode = connec[cptr-1];
                 cptr++;
                 ilevel = lvlnum[inode-1];
                 if (ilevel == 0)  continue;
                 lvlnum[inode-1] = 0;
                 if ( ilevel != level ){
                    if  (labs(level-ilevel) != 1){
                       *space=-1;
                       return(54);
                    }
                    invnum[bq2-1] = inode;
                    bq2+=inc;
                   }
                 else{
                    invnum[bq1-1] = inode;
                    bq1+= inc;
                 }
              }

              /*  ==================================================
                  ... sort the nodes just added to queue1 and queue2
                  separately into increasing order of degree.
                  ================================================== */
              if  (labs (bq1 - sq1) > 1){
                 nsort = labs (bq1 - sq1);
                 if(!forwrd){
                    error=gpskcp (nsort, &invnum[bq1], degree);
                    if  (error != 0){
                       *space=-1;
                       return(56);
                    }
                 }
                 else{
                    error=gpskcq (nsort, &invnum[sq1-1], degree);
                    if  (error != 0){
                       *space=-1;
                       return(56);
                    }
                 }
              }
              if  (labs (bq2 - sq2) > 1){
                 nsort = labs (bq2 - sq2);
                 if  (!forwrd){
                    error=gpskcp (nsort, &invnum[bq2], degree);
                    if  (error != 0){
                       *space=-1;
                       return(56);
                    }
                 }
                 else{
                    error= gpskcq (nsort, &invnum[sq2-1], degree);
                    if  (error != 0){
                       *space=-1;
                       return(56);
                    }
                 }
              }
              /* ... end of repeat loop */
           }
           while (fq1 != bq1);

           /* ==============================================================
              ... queue1 is now empty ...
              if there are any unnumbered nodes left at this level,
              find the one of minimal degree and return to the
              repeat loop above.
              ============================================================== */

           if  ((bq1 == queue2) && (nleft == 0)) break;

           if ((nleft <= 0) || (nleft != inc * (queue2 - bq1))){
              *space=-1;
              return(52);
           }

           lowdg = n + 1;
           bptr  = n + 1;
           cptr  = lstart - 1;
           for(i=1;i<=nleft;i++){
              while(1){
                 cptr++;
                 lvllsc = lvllst[cptr-1] -1;          /* C indexing */
                 if (lvlnum[lvllsc] == level) break;
                 if (lvlnum[lvllsc] != 0){
                    *space=-1;
                    return(53);
                 }
              }
              if  (degree[lvllsc] < lowdg){
                 lowdg = degree[lvllsc];
                 bptr  = cptr;
              }
           }

           /* ... minimal degree unnumbered node found ... */

           if  (bptr > n){
              *space=-1;
              return(55);
           }
           lvllsb = lvllst[bptr-1];
           invnum[bq1-1] = lvllsb;
           lvlnum[lvllsb-1] = 0;
           bq1+= inc;
        }

        /*  =============================================
            ... advance queue pointers to make queue2 the
            new queue1 for the next iteration.
            ============================================= */

        queue1 = queue2;
        fq1 = queue1;
        bq1 = bq2;
        if  ((bq1 == fq1) && (xlevel < depth)){
           *space=-1;
           return(51);
        }
     }

   /*... change sign of degree to mark these nodes as 'numbered' */
   for(i=0;i<ncompn;i++){
      invnmi = invnum[i]-1;
      degree[invnmi] = -degree[invnmi];
   }
   return(0);
}

int   gpskck(long n, long *degree, long *rstart, long *connec, long wrklen,
             long *nxtnum, long *work, long ncompn, long depth, long *lvllst,
             long *lvlptr, long *lvlnum, long *space){

   /* ==================================================================

      number nodes in a generalized level structure according to
      a generalization of the king algorithm, which reduces
      the profile of the sparse symmetric matrix.

      ---------------------

      code uses a priority queue to choose the next node to be numbered
      the priority queue is represented by a simple linear-linked list
      to save space.  this will require more searching than a fully
      linked representation, but the data manipulation is simpler.

      -------------------

      << establish priority queue 'active' for level 1 nodes >>

      for i = 1 to depth do
      << set queue 'queued' to be empty, list 'next' to be >>
      << set of nodes at next level.                       >>

      for j = 1 to 'nodes at this level' do
      << find first node in active with minimal connections >>
      << to 'next'.  number this node and remove him from   >>
      << 'active'.  for each node in 'next' which connected >>
      << to this node, move it to 'queued' and remove it    >>
      << from 'next'.                                       >>

      << set new queue 'active' to be 'queued' followed by any >>
      << nodes still in 'next'.                                >>

      ==================================================================

      data structure assumptions:
      the first 'nxtnum-1' elements of  work  are already in use.
      the level structure 'lvllst' is contiguous with  work, that is,
      it resides in elements  wrklen+1, ...  of  work.  'lvlptr' and
      'lvlnum' are also embedded in work, behind 'lvllst'.  the
      three vectors are passed separately to clarify the indexing,
      but the queues developed will be allowed to overrun 'lvllst'
      as needed.

      ... build the first 'active' queue starting w1 locations from
      the front of the current working area  (w1 is the width of the
      first level).  build the first 'queued' queue starting from
      the back of work space.  the list 'next' will be realized
      implicitly in 'lvlnum' as:
      lvlnum[i] > 0   <== level number of node.  'next' is
      set with lvlnum[i] = level+1
      lvlnum[i] = 0   <== i-th node is in 'queued' or is
      not in this component of graph,
      or has just been numbered.
      lvlnum[i] < 0   <== i-th node is in 'active' and is
      connected to -lvlnum[i] nodes in
      'next'.

      ==================================================================

      structure of workspace ..

      --------------------------------------------------------------
      : numbered : done : active : alevel : ... : queued : lvllst :
      --------------------------------------------------------------

      -------------------
      lvlptr : lvlnum :
      -------------------

      in the above,
      numbered is the set of nodes already numbered from
      previous components and earlier levels of this component.
      done, active, alevel  are vectors of length the width of
      the current level.  active is a set of indices into
      alevel.  as the nodes in alevel are numbered, they
      are placed into 'done'.
      queued is a queue of nodes in the 'next' level, which
      grows from the start of the 'next' level in lvllst
      forwards toward 'alevel'.  queued is of length no more
      than the width of the next level.
      lvllst is the list of unnumbered nodes in the tree,
      arranged by level.
      ================================================================== */
   long     i, j, k, ptr, jptr, kptr, lptr, mptr, pptr, rptr,
   mpptr, jnode, knode, cnode, level, lowdg, unused,
   mxque, nnext, astart, mindg, lstart, lwidth, active,
   queueb, queued, qcount, nconnc, nactiv, cdgree,
   ldgree, nfinal, jdgree, strtic, added, twrkln,
   lvllsl, connej, conner, astptr, actptr, activi,
   astrti, queuei, acpptr;
   int        error;

   /* ------------------------------------------------------------------ */

   twrkln = wrklen + ncompn + n + depth + 1;
   unused = twrkln;

   astart = lvlptr[0];
   lwidth = lvlptr[1] - astart;
   astart = wrklen + 1;  
   active = *nxtnum + lwidth + 1;
   nactiv = lwidth;
   nfinal = *nxtnum + ncompn;

   nnext = lvlptr[2] - lvlptr[1];
   queued = wrklen;
   queueb = queued;
   mxque = active + lwidth;

   /* ... build first priority queue 'active' */

   lowdg = - (n + 1);
   lptr = lvlptr[0];
   for(i=1;i<=lwidth;i++){
      nconnc = 0;
      lvllsl= lvllst[lptr-1] -1 ;      /* C indexing */
      jptr = rstart[lvllsl]-1;
      ldgree = degree[lvllsl];
      for(j=0;j<ldgree;j++){
         connej = connec[jptr];
         if ( lvlnum[connej-1] == 2 )  nconnc--;
         jptr++;
      }

      activi = active + i - 1;
      work[activi-1] = i;
      lvlnum[lvllsl] = nconnc;
        lowdg = max(lowdg, nconnc);
      lptr++;
   }
   work[active-2] = 0;

   /* -----------------------------------
      now number nodes level by level ...
      ----------------------------------- */

   for(level=1;level<=depth;level++){

      /* ... number all nodes in this level */

      for(i=1;i<=lwidth;i++){
         pptr = -1;
         ptr = work[active-2];
         if (nnext != 0){
            /* ... if nodes remain in next, find the earliest node
               in active of minimal degree.  */

            mindg = -(n+1);
            for(j=0;j<nactiv;j++){
               astptr = astart + ptr;
               cnode = work[astptr-1]-1;
               if ( lvlnum[cnode] == lowdg )  break;
               if ( lvlnum[cnode] > mindg ){
                  mpptr = pptr;
                  mptr = ptr;
                  mindg = lvlnum[cnode];
               }
               pptr = ptr;
               actptr = active + ptr;
               ptr = work[actptr-1];
            }
            /* ... establish  ptr  as first min degree node
               pptr as predecessor in list.  */
            if(j>=nactiv){
               ptr = mptr;
               pptr = mpptr;
            }
            astptr = astart + ptr;
            cnode = work[astptr-1]-1;
            lowdg = lvlnum[cnode];
            lvlnum[cnode] = 0;
            jptr = rstart[cnode];
            /* ... update connection counts for all nodes which
               connect to  cnode's  neighbors in  next. */

            cdgree = degree[cnode];
            strtic = queueb;
            for(j=0;j<cdgree;j++){
               jnode = connec[jptr-1]-1;
               jptr++;
               if( lvlnum[jnode] != level+1 ) continue;
               if (queueb < mxque){
                  *space=nactiv+nnext;
                  return(160);
               }
               work[queueb-1] = jnode+1;
               queueb--;
               nnext--;
               lvlnum[jnode] = 0;
               if  (nactiv == 1)  continue;
               kptr = rstart[jnode]-1;
               jdgree = degree[jnode];
               for(k=0;k<jdgree;k++){
                  knode = connec[kptr]-1;
                  kptr++;
                  if (lvlnum[knode] >= 0)  continue;
                  lvlnum[knode]++;
                  if  (lowdg < lvlnum[knode]) lowdg = lvlnum[knode];
               }
            }

            /* ... to mimic the algorithm as implemented by gibbs,
               sort the nodes just added to the queue into
               increasing order of original index. (but, because
               the queue is stored backwards in memory, the sort
               routine is called for decreasing index.) */

            /* treat  0, 1 or 2  nodes added as special cases */

            added = strtic - queueb;
            if  (added - 2 >=0){
               if(added==2){
                  if (work[strtic-2] <= work[strtic-1]){
                     jnode = work[strtic-1];
                     work[strtic-1] = work[strtic-2];
                     work[strtic-2] = jnode;
                  }
               }
               else{
                  error=gpskco (added, &work[queueb]);
                  if  (error != 0) return(64);
               }
            }
         }
         /* ... number this node and delete it from 'active'.;
            mark it unavailable by changing sign of degree */

         nactiv--;
         astptr = astart + ptr;
         cnode = work[astptr-1] -1;
         work[*nxtnum-1] = cnode+1;
         degree[cnode] = -degree[cnode];
         (*nxtnum)++;

         /*  ... delete link to this node from list */

         acpptr = active + pptr;
         actptr = active + ptr;
         work[acpptr-1] = work[actptr-1];
      }
      /* ... now move the queue 'queued' forward, at the same
         time computing connection counts for its elements.
         then do the same for the remaining nodes in 'next'.  */

      unused = min (unused, queueb - mxque);
      if ( *nxtnum != active-1 )  return(61);
      if ( level != depth ){
         lstart = lvlptr[level];
         lwidth = lvlptr[level+1] - lstart;
         active = *nxtnum + lwidth + 1;
         astart = active + lwidth ;
         nactiv = lwidth;
         mxque = astart + lwidth;
         if ( mxque > queueb + 1 ){
            *space=nactiv+nnext;
            return(160);
         }
         unused = min (unused, queueb - mxque + 1);

         qcount = queued - queueb;
         lowdg = -n-1;
         work[active-2] = 0;
         ptr = lstart;
         for(i=1;i<=lwidth;i++){
            /*   ... choose next node from either 'queued' or 'next'  */
            if (i <= qcount ){
               queuei = queued + 1 - i;
               cnode = work[queuei-1] -1;
            }
            else{
               do{
                  cnode = lvllst[ptr-1] -1;
                  ptr++;
                  if ( ptr > lvlptr[level+1] )  return(62);
               }
               while(lvlnum[cnode] <= 0);
            }
            if ( level+1 != depth ){
               rptr = rstart[cnode];
               nconnc = 0;
               jdgree = degree[cnode];
               for(j=0;j<jdgree;j++){
                  conner = connec[rptr-1];
                  if ( lvlnum[conner-1] == level+2 )  nconnc--;
                  rptr++;
               }
               lvlnum[cnode] = nconnc;
               lowdg = max(lowdg, nconnc);
            }
            /*  ... add cnode to new 'active' queue  */

            activi = active + (i - 1);
            astrti = astart + (i - 1);
            work[activi-1] = i;
            work[astrti-1] = cnode +1;
         }

         if (depth != level+1 ){
            nnext = lvlptr[level+2] - lvlptr[level+1];
            queued = lstart - 1 + lwidth + wrklen;
            queueb = queued;
         }
         else    nnext = 0;
      }
   }
   if  (*nxtnum != nfinal) return(63);
   *space = max (*space, twrkln - unused);
   return(0);
}

int   gpskcl (long n, long *degree, long *rstart, long *connec,
               long *invnum, long *newnum,
               long *oldnum, long *bandwd, long *profil, long *space){
   /* ==================================================================
      compute the bandwidth and profile for the renumbering given
      by 'invnum' and also for the renumbering given by 'oldnum'.
      'newnum' will be a permutation vector copy of the node
      list 'invnum'.
      ==================================================================*/

   long     i, j, jptr, idgree, oldbnd, oldpro, newbnd, newpro,
   oldrwd, newrwd, oldorg, neworg, jnode, invnmi;

   /* ------------------------------------------------------------------*/

   /* ... create newnum as a permutation vector */

   for(i=0;i<n;i++){
      invnmi = invnum [i];
      newnum[invnmi-1] = i+1;
   }

   /* ... compute profile and bandwidth for both the old and the new
      orderings. */

   oldbnd = 0;
   oldpro = 0;
   newbnd = 0;
   newpro = 0;

   for(i=0;i<n;i++){
      if (degree[i] == 0)  continue;
      if (degree[i] > 0){
         *space=-1;
         return(70);
      }
      idgree = -degree[i];
      degree[i] = idgree;
      neworg = newnum[i];
      oldorg = oldnum[i];
      newrwd = 0;
      oldrwd = 0;
      jptr = rstart [i] - 1;

      /* ... find neighbor which is numbered farthest ahead of the
         current node.*/

      for(j=0;j<idgree;j++){
         jnode = connec[jptr] - 1;
         jptr++;
         newrwd = max (newrwd, neworg - newnum[jnode]);
         oldrwd = max (oldrwd, oldorg - oldnum[jnode]);
      }
      newpro +=  newrwd;
      newbnd = max (newbnd, newrwd);
      oldpro +=  oldrwd;
      oldbnd = max (oldbnd, oldrwd);
   }

   /* ------------------------------------------------------------------------ */
   printf("\n  Original matrix had Bandwidth= %ld Profile=%ld. ",oldbnd,oldpro);
   printf("\n  Computed order  had Bandwidth= %ld Profile=%ld. ",newbnd,newpro);
   printf("\n");
   /* ------------------------------------------------------------------------ */

   /*... if new ordering has better bandwidth than old ordering,
     replace old ordering by new ordering */

   if  (newbnd <= oldbnd){
      *bandwd = newbnd;
      *profil = newpro;
      for(i=0;i<n;i++)   oldnum[i] = newnum[i];
   }
   else{
      /* ... retain old ordering */

      *bandwd = oldbnd;
      *profil = oldpro;
   }
   return(0);
}

int   gpskcm(long n, long *degree, long *rstart, long *connec, long *invnum,
         long *newnum, long *oldnum, long *bandwd, long *profil, long *space){

   /*==================================================================


     compute the bandwidth and profile for the renumbering given
     by 'invnum', by the reverse of numbering 'invnum', and also
     by the renumbering given in 'oldnum'.
     'newnum' will be a permutation vector copy of the node
     list 'invnum'.

     ==================================================================*/

   long     i, j, jptr, idgree, oldbnd, oldpro, newbnd, newpro,
   oldrwd, newrwd, oldorg, neworg, jnode, nrvbnd, nrvpro,
   nrvorg, nrvrwd, invnmi, nmip1;

   /*    ------------------------------------------------------------------*/

   /* ... create newnum as a permutation vector */

   for(i=0;i<n;i++){
      invnmi = invnum [i];
      newnum[invnmi-1] = i+1;
   }

   /*     ... compute profile and bandwidth for both the old and the new
          orderings.    */

   oldbnd = 0;
   oldpro = 0;
   newbnd = 0;
   newpro = 0;
   nrvbnd = 0;
   nrvpro = 0;

   for(i=0;i<n;i++){
      if (degree[i] == 0)  continue;
      if (degree[i] > 0){
         *space=-1;
         return(71);
      }
      idgree = -degree[i];
      degree[i] = idgree;
      newrwd = 0;
      oldrwd = 0;
      nrvrwd = 0;
      neworg = newnum[i];
      oldorg = oldnum[i];
      nrvorg = n - newnum[i] + 1;
      jptr = rstart [i] - 1;

      /*... find neighbor which is numbered farthest ahead of the
        current node.  */

      for(j=0;j<idgree;j++){
         jnode = connec[jptr] - 1;
         jptr++;
         newrwd = max (newrwd, neworg - newnum[jnode]);
         oldrwd = max (oldrwd, oldorg - oldnum[jnode]);
         nrvrwd = max (nrvrwd, nrvorg - n + newnum[jnode] - 1);
      }

      newpro +=  newrwd;
      newbnd = max (newbnd, newrwd);
      nrvpro +=  nrvrwd;
      nrvbnd = max (nrvbnd, nrvrwd);
      oldpro +=  oldrwd;
      oldbnd = max (oldbnd, oldrwd);
   }

printf("\n  Original matrix had Bandwidth= %ld Profile=%ld. ",oldbnd,oldpro);
printf("\n  Computed order  had Bandwidth= %ld Profile=%ld. ",newbnd,newpro);
printf("\n  Reversed order  had Bandwidth= %ld Profile=%ld. ",nrvbnd,nrvpro);
printf("\n");
/*... if new ordering has better bandwidth than old ordering,
     replace old ordering by new ordering */

   if  ((newpro <= oldpro)  && (newpro <= nrvpro)){
      *bandwd = newbnd;
      *profil = newpro;
      for(i=0;i<n;i++)              oldnum[i] = newnum[i];
   }
   else{
      /*... check new reversed ordering for best profile */

      if  (nrvpro <= oldpro){
         *bandwd = nrvbnd;
         *profil = nrvpro;
         for(i=1;i<=n;i++){
            oldnum[i-1] = n - newnum[i-1] + 1;
            if  (i > n/2)  continue;
            j = invnum[i-1];
            nmip1 = (n +1) - i  ;
            invnum[i-1] = invnum[nmip1-1];
            invnum[nmip1-1] = j;
         }
      }
      else{
         /*     ... retain old ordering */

         *bandwd = oldbnd;
         *profil = oldpro;
      }
   }
   return(0);
}

int   gpskcn   (long n, long *key, long *data){

   /*==================================================================

        i n s e r t i o n    s o r t

        input:
        n    -- number of elements to be sorted
        key  -- an array of length  n  containing the values
        which are to be sorted
        data -- a second array of length  n  containing data
        associated with the individual keys.

        output:
        key  -- will be arranged so that values are in decreasing
        order
        data -- rearranged to correspond to rearranged keys
        error -- will be zero unless the program is malfunctioning,
        in which case it will be equal to 1.


        ==================================================================*/

   /*     ------------------------------------------------------------------*/

   long     i, j, d, k, ip1, jm1;

   /*    ------------------------------------------------------------------*/

   if (n == 1)  return(0);
   if  (n <= 0) return(1);

   /* ... insertion sort ... for i := n-1 step -1 to 1 do ... */

   i = n - 2;
   ip1 = n-1;
   while(i>=0){
      if ( key [i] < key [ip1] ){

         /* ... out of order ... move up to correct place */

         k = key [i];
         d = data [i];
         j = ip1;
         jm1 = i;

         /* ... repeat ... until 'correct place for k found' */
         do{
            key [jm1] = key [j];
            data [jm1] = data [j];
            jm1 = j;
            j++;
            if(j >= n) break;
         }
         while(key[j] > k);

         key[jm1] = k;
         data[jm1] = d;
      }
      ip1 = i;
      i--;
   }
   return(0);
}

int   gpskco   (long n, long *key){
/*     ==================================================================

     i n s e r t i o n    s o r t

     input:
         n    -- number of elements to be sorted
         key  -- an array of length  n  containing the values
                 which are to be sorted

     output:
         key  -- will be arranged so that values are in decreasing
                 order

     ==================================================================*/

/*  ------------------------------------------------------------------*/

long     i, j, k, ip1, jm1;

/*   ------------------------------------------------------------------*/

      if (n == 1)  return(0);
      if  (n <= 0)  return(1);

   /*  ... insertion sort ... for i := n-1 step -1 to 1 do ... */

  i = n - 2;
  ip1 = n-1;
  while(i>=0){
     if ( key [i] < key[ip1] ){
        /* ... out of order ... move up to correct place */
        k = key [i];
        j = ip1;
        jm1 = i;
        /* ... repeat ... until 'correct place for k found' */
        do{
           key[jm1] = key[j];
           jm1 = j;
           j++;
           if(j >= n) break;
        }
        while (key[j] > k);

        key[jm1] = k;
     }
     ip1 = i;
     i--;
  }
 return(0);
}

int   gpskcp  (long n, long *index, long *degree){

/*    ==================================================================

     i n s e r t i o n      s o r t

     input:
         n    -- number of elements to be sorted
         index  -- an array of length  n  containing the indices
                 whose degrees are to be sorted
         degree -- an  nvec  vector, giving the degrees of nodes
                   which are to be sorted.

     output:
         index  -- will be arranged so that values are in decreasing
                 order
         error -- will be zero unless the program is malfunctioning,
                  in which case it will be equal to 1.

     ==================================================================*/

/*  ------------------------------------------------------------------*/

long     i, j, v, ip1, jm1, indexi, indxi1, indexj;

/*    ------------------------------------------------------------------*/

      if (n == 1)  return(0);
      if  (n <= 0) return(1);

/*   ------------------------------------------------------------------
     insertion sort the entire file
     ------------------------------------------------------------------*/


     /* ... insertion sort ... for i := n-1 step -1 to 1 do ... */

  i = n - 2;
  ip1 = n-1;
  while(i>=0){
     indexi = index [i] -1;
     indxi1 = index [ip1] -1;
     if ( degree[indexi] < degree[indxi1] ){

        /* ... out of order ... move up to correct place */

        v = degree [indexi];
        j = ip1;
        jm1 = i;
        indexj = index[j];

        /* ... repeat ... until 'correct place for v found' */
        do{
           index[jm1] = indexj;
           jm1 = j;
           j++;
           if(j >= n) break;
           indexj = index[j];
        }
        while(degree[indexj -1] > v);

        index[jm1] = indexi +1;
     }
     ip1 = i;
     i--;
  }
  return(0);
}

int   gpskcq (long n, long *index, long *degree){

/*   ==================================================================

     i n s e r t i o n      s o r t

     input:
         n    -- number of elements to be sorted
         index  -- an array of length  n  containing the indices
                 whose degrees are to be sorted
         degree -- an  nvec  vector, giving the degrees of nodes
                   which are to be sorted.

     output:
         index  -- will be arranged so that values are in increasing
                   order
         error -- will be zero unless the program is malfunctioning,
                  in which case it will be equal to 1.

     ==================================================================*/

long     i, j, v, indexi, indxi1, indexj, ip1, jm1;

/*------------------------------------------------------------------*/

      if (n == 1)  return(0);
      if  (n <= 0) return(1);

/*  ------------------------------------------------------------------
    insertion sort the entire file;
    ------------------------------------------------------------------*/


     /*... insertion sort ... for i := n-1 step -1 to 1 do ... */

  i = n - 1;
  ip1 = n;
  while(i>0){
     indexi = index [i-1] ;
     indxi1 = index[ip1-1];
     if ( degree[indexi-1] > degree[indxi1-1] ){
        /*... out of order ... move up to correct place */
        v = degree[indexi-1];
        j = ip1;
        jm1 = i;
        indexj = index[j-1];

        /*... repeat ... until 'correct place for v found' */
        do{
           index[jm1-1] = indexj;
           jm1 = j;
           j++;
           if (j > n) break;
           indexj = index[j-1];
        }
        while (degree[indexj-1] < v);

        index[jm1-1] = indexi;
     }
     ip1 = i;
     i--;
  }

  return(0);
}

char *challoc(unsigned bcount,unsigned bsize)
{
   char *st;

   if( NULL==(st=(char*)malloc(bcount*bsize)) )
   {
      printf("\n\nUnable to allocate required memory.\n");
      exit(1);
   }
   return(st);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static int setconnectivity(const SPMAT *LHS, long *iwork, long nz)
{
   int k=0;
   int i,j;
   
   long n = (long)(LHS->m);

   long cstart = 1;
   long permut = cstart + (n + 1);
   long connec = permut + n;
   /*  scan matrix for connec = row id of non zero element and cstart=offset of ith row in connec
      NOTE all contents are FORTRAN index style 1,...,n   */
   iwork[cstart-1] = 1;

   for(i=0; i<n; i++)
   {
      SPROW *r = LHS->row +i;
      for (j=0; j<r->len; j++)
      {
         if ( r->elt[j].col < i )
         {
            continue;
         }

         if( r->elt[j].val != 0.0 )
         {
            iwork[connec+k-1]=r->elt[j].col+1;
            k++;
         }
      }
      iwork[cstart+i] = k+1;
   }

   /*     ... check that column start data is consistent with stated  nz */
   if  (iwork[cstart+n-1] != nz+1L )
   {
      printf("\nerror in input data:\n nz=%ld  but column start = %ld",nz,iwork[cstart+n-1]);
      return(1);
   }

   return 0;
}
