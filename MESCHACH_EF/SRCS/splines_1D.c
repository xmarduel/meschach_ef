
/** file splines_1D.c
 *
 * Contains the structure of SPLINES in 1 dimension
 *
 */

#include "MESCHACH/INCLUDES/matrix2.h"

#include "MESCHACH_EF/INCLUDES/splines_1D.h"

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

static SPLINE_1D *spline1D_calc    (SPLINE_1D  *spl, const GEOM_1D *geom);

static SPLINE_1D *spline1D_calc_S3 (SPLINE_1D  *spl, const GEOM_1D *geom);
static SPLINE_1D *spline1D_calc_S5 (SPLINE_1D  *spl, const GEOM_1D *geom);

static SPLINE_1D_BASIC *spline1D_basic_calc_S3 ( SPLINE_1D_BASIC* spl, const VEC *local_mesh);
static SPLINE_1D_BASIC *spline1D_basic_calc_S5 ( SPLINE_1D_BASIC* spl, const VEC *local_mesh);

static VEC* spline1D_basic_get_local_mesh__from_periodic_total_mesh    (const SPLINE_1D  *spl, int i, const VEC *total_mesh, VEC *local_mesh);
static VEC* spline1D_basic_get_local_mesh__from_non_periodic_total_mesh(const SPLINE_1D  *spl, int i, const VEC *total_mesh, VEC *local_mesh);

static VEC *total_mesh_get(const GEOM_1D *geom);

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

SPLINE_1D *spline1D_get (const ELT_1D *elt, const GEOM_1D *geom)
{
   int i;
   SPLINE_1D* spline = NEW(SPLINE_1D);

   if ( strcmp(elt->name_ef,"S2") == 0 )
   {
      strcpy(spline->type, "S2");
   }
   else
   if ( strcmp(elt->name_ef,"S3") == 0 )
   {
      strcpy(spline->type, "S3");
   }
   else
   if ( strcmp(elt->name_ef,"S5") == 0 )
   {
      strcpy(spline->type, "S5");
   }
   else
   {
      error(E_UNKNOWN, "spline1D_get"); return NULL;
   }


   spline->dim = geom->NBSOMM;
   
   /* alloc mem */
   spline->spline_basic = (SPLINE_1D_BASIC*)calloc(spline->dim, sizeof(SPLINE_1D_BASIC));
   /* alloc mem */
   for ( i=0; i<spline->dim; i++)
   {
      spline1D_basic_init( spline->spline_basic + i , elt->name_ef); 
   }

   /* and calculate all the datas defining a spline */
   spline1D_calc (spline, geom);
      
   /* finito */
   return spline;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

int         spline1D_free(SPLINE_1D  *spl)
{
   int i;

   for (i=0; i<spl->dim; i++)
   {
      spline1D_basic_free(spl->spline_basic + i);
   }

   free(spl);
   
   return 1;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */


static SPLINE_1D *spline1D_calc (SPLINE_1D  *spline, const GEOM_1D *geom)
{
   if ( strcmp(spline->type,"S3") == 0 )
   {
      return spline1D_calc_S3 ( spline, geom);
   }
   else
   if ( strcmp(spline->type,"S5") == 0 )
   {
      return spline1D_calc_S5 ( spline, geom);
   }
   else
   {
      error(E_UNKNOWN, "spline1D_calc");
      return NULL;
   }

   return spline;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

static VEC* spline1D_basic_get_local_mesh__from_periodic_total_mesh_S3(const SPLINE_1D  *spl, int i, const VEC *total_mesh, VEC *local_mesh)
{
   if ( i==0 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[2] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[1] ;
   }
   else
   if ( i==1 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[1] ;
      local_mesh->ve[3] = total_mesh->ve[2] ;
   }
   else
   if ( i==spl->dim -1 )
   {
      local_mesh->ve[0] = total_mesh->ve[i-2] ;
      local_mesh->ve[1] = total_mesh->ve[i-1] ;
      local_mesh->ve[2] = total_mesh->ve[i-0] ;
      local_mesh->ve[3] = total_mesh->ve[0]   ;
   }
   else
   {
      local_mesh->ve[0] = total_mesh->ve[i-2] ;
      local_mesh->ve[1] = total_mesh->ve[i-1] ;
      local_mesh->ve[2] = total_mesh->ve[i-0] ;
      local_mesh->ve[3] = total_mesh->ve[i+1] ;
   }

   return local_mesh;
}

/*----------------------------------------------------------------------------------------------- */

static VEC* spline1D_basic_get_local_mesh__from_non_periodic_total_mesh_S3(const SPLINE_1D  *spl, int i, const VEC *total_mesh, VEC *local_mesh)
{
   if (i==0)
   {
      local_mesh->ve[0] = total_mesh->ve[0] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[0] ;
   }
   else
   if ( i==1 )
   {
      local_mesh->ve[0] = total_mesh->ve[0] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[1] ;
   }
   else
   if ( i==2 )
   {
      local_mesh->ve[0] = total_mesh->ve[0] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[1] ;
      local_mesh->ve[3] = total_mesh->ve[2] ;
   }
   else
   if ( i==spl->dim -1 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[3] = total_mesh->ve[total_mesh->dim -1] ;
   }
   else
   if ( i==spl->dim -2 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[3] = total_mesh->ve[total_mesh->dim -1] ;
   }
   else
   if ( i==spl->dim -3 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -3] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[3] = total_mesh->ve[total_mesh->dim -1] ;
   }
   else
   {
      local_mesh->ve[0] = total_mesh->ve[i-3] ;
      local_mesh->ve[1] = total_mesh->ve[i-2] ;
      local_mesh->ve[2] = total_mesh->ve[i-1] ;
      local_mesh->ve[3] = total_mesh->ve[i-0] ;
   }

   return local_mesh;
}

/*----------------------------------------------------------------------------------------------- */

static SPLINE_1D *spline1D_calc_S3 (SPLINE_1D  *spl, const GEOM_1D *geom)
{
   int i;

   VEC *total_mesh = total_mesh_get(geom);
   VEC *local_mesh = v_get(4);
      
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      for ( i=0; i<spl->dim; i++)
      {
         spline1D_basic_get_local_mesh__from_periodic_total_mesh(spl, i, total_mesh, local_mesh);

         spline1D_basic_calc( spl->spline_basic +i , "S3", local_mesh);
      }
   }
   else
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      for ( i=0; i<spl->dim; i++)
      {
         spline1D_basic_get_local_mesh__from_non_periodic_total_mesh(spl, i, total_mesh, local_mesh);

         spline1D_basic_calc( spl->spline_basic +i , "S3", local_mesh);
      }
   }
   
   V_FREE(local_mesh);
   V_FREE(total_mesh);
   
   /* finito */
   return spl;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

static VEC* spline1D_basic_get_local_mesh__from_periodic_total_mesh_S5(const SPLINE_1D  *spl, int i, const VEC *total_mesh, VEC *local_mesh)
{
   if (i==0)
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -3] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[3] = total_mesh->ve[0] ;
      local_mesh->ve[4] = total_mesh->ve[1] ;
      local_mesh->ve[5] = total_mesh->ve[2] ;
   }
   else
   if ( i==1 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[2] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[1] ;
      local_mesh->ve[4] = total_mesh->ve[2] ;
      local_mesh->ve[5] = total_mesh->ve[3] ;
   }
   else
   if ( i==2 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[1] ;
      local_mesh->ve[3] = total_mesh->ve[2] ;
      local_mesh->ve[4] = total_mesh->ve[3] ;
      local_mesh->ve[5] = total_mesh->ve[4] ;
   }
   else
   if ( i==spl->dim -1 )
   {
      local_mesh->ve[0] = total_mesh->ve[i-3] ;
      local_mesh->ve[1] = total_mesh->ve[i-2] ;
      local_mesh->ve[2] = total_mesh->ve[i-1] ;
      local_mesh->ve[3] = total_mesh->ve[i-0] ;
      local_mesh->ve[4] = total_mesh->ve[0] ;
      local_mesh->ve[5] = total_mesh->ve[1] ;
   }
   else
   if ( i==spl->dim -2 )
   {
      local_mesh->ve[0] = total_mesh->ve[i-3] ;
      local_mesh->ve[1] = total_mesh->ve[i-2] ;
      local_mesh->ve[2] = total_mesh->ve[i-1] ;
      local_mesh->ve[3] = total_mesh->ve[i-0] ;
      local_mesh->ve[4] = total_mesh->ve[i+1] ;
      local_mesh->ve[5] = total_mesh->ve[0] ;
   }
   else
   {
      local_mesh->ve[0] = total_mesh->ve[i-3] ;
      local_mesh->ve[1] = total_mesh->ve[i-2] ;
      local_mesh->ve[2] = total_mesh->ve[i-1] ;
      local_mesh->ve[3] = total_mesh->ve[i+0] ;
      local_mesh->ve[4] = total_mesh->ve[i+1] ;
      local_mesh->ve[5] = total_mesh->ve[i+2] ;
   }

   return local_mesh;
}

/*----------------------------------------------------------------------------------------------- */

static VEC* spline1D_basic_get_local_mesh__from_non_periodic_total_mesh_S5(const SPLINE_1D  *spl, int i, const VEC *total_mesh, VEC *local_mesh)
{
   if (i==0)
   {
      local_mesh->ve[0] = total_mesh->ve[0] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[0] ;
      local_mesh->ve[4] = total_mesh->ve[0] ;
      local_mesh->ve[5] = total_mesh->ve[0] ;
   }
   else
   if ( i==1 )
   {
      local_mesh->ve[0] = total_mesh->ve[0] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[0] ;
      local_mesh->ve[4] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[1] ;
   }
   else
   if ( i==2 )
   {
      local_mesh->ve[0] = total_mesh->ve[0] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[0] ;
      local_mesh->ve[4] = total_mesh->ve[1] ;
      local_mesh->ve[3] = total_mesh->ve[2] ;
   }
   else
   if ( i==3 )
   {
      local_mesh->ve[0] = total_mesh->ve[0] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[0] ;
      local_mesh->ve[3] = total_mesh->ve[1] ;
      local_mesh->ve[4] = total_mesh->ve[2] ;
      local_mesh->ve[3] = total_mesh->ve[3] ;
   }
   else
   if ( i==4 )
   {
      local_mesh->ve[0] = total_mesh->ve[0] ;
      local_mesh->ve[1] = total_mesh->ve[0] ;
      local_mesh->ve[2] = total_mesh->ve[1] ;
      local_mesh->ve[3] = total_mesh->ve[2] ;
      local_mesh->ve[4] = total_mesh->ve[3] ;
      local_mesh->ve[3] = total_mesh->ve[4] ;
   }
   else
   if ( i==spl->dim -1 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[3] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[4] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[5] = total_mesh->ve[total_mesh->dim -1] ;
   }
   else
   if ( i==spl->dim -2 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[3] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[4] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[5] = total_mesh->ve[total_mesh->dim -1] ;
   }
   else
   if ( i==spl->dim -3 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -3] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[3] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[4] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[5] = total_mesh->ve[total_mesh->dim -1] ;
   }
   else
   if ( i==spl->dim -4 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -4] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -3] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[3] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[4] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[5] = total_mesh->ve[total_mesh->dim -1] ;
   }
   else
   if ( i==spl->dim -5 )
   {
      local_mesh->ve[0] = total_mesh->ve[total_mesh->dim -5] ;
      local_mesh->ve[1] = total_mesh->ve[total_mesh->dim -4] ;
      local_mesh->ve[2] = total_mesh->ve[total_mesh->dim -3] ;
      local_mesh->ve[3] = total_mesh->ve[total_mesh->dim -2] ;
      local_mesh->ve[4] = total_mesh->ve[total_mesh->dim -1] ;
      local_mesh->ve[5] = total_mesh->ve[total_mesh->dim -1] ;
   }
   else
   {
      local_mesh->ve[0] = total_mesh->ve[i-5] ;
      local_mesh->ve[1] = total_mesh->ve[i-4] ;
      local_mesh->ve[2] = total_mesh->ve[i-3] ;
      local_mesh->ve[3] = total_mesh->ve[i-2] ;
      local_mesh->ve[4] = total_mesh->ve[i-1] ;
      local_mesh->ve[5] = total_mesh->ve[i-0] ;
   }
   
   return local_mesh;
}

/*----------------------------------------------------------------------------------------------- */

static SPLINE_1D *spline1D_calc_S5 (SPLINE_1D  *spl, const GEOM_1D *geom)
{
   int i;
   
   VEC *total_mesh = total_mesh_get(geom);
   VEC *local_mesh = v_get(6);

   if ( geom->periodicity == PERIODIC_MESHe )
   {
      for ( i=0; i<spl->dim; i++)
      {
         spline1D_basic_get_local_mesh__from_periodic_total_mesh(spl, i, total_mesh, local_mesh);
         
         spline1D_basic_calc( spl->spline_basic +i , "S5", local_mesh);
      }         
   }
   else
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      for ( i=0; i<spl->dim; i++)
      {
         spline1D_basic_get_local_mesh__from_non_periodic_total_mesh(spl, i, total_mesh, local_mesh);

         spline1D_basic_calc( spl->spline_basic +i , "S5", local_mesh);
      }
   }

   V_FREE(local_mesh);
   V_FREE(total_mesh);
   
   /* finito */
   return spl;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

SPLINE_1D_BASIC *spline1D_basic_get(const ELT_1D *elt)
{
   SPLINE_1D_BASIC *spl = NEW(SPLINE_1D_BASIC);

   return spline1D_basic_init(spl, elt->name_ef);
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

int   spline1D_basic_free(SPLINE_1D_BASIC  *spl)
{
   M_FREE( spl->COEFFS_POLYNOMES  );
   V_FREE( spl->LOCAL_MESH        );

   free(spl);

   return EXIT_SUCCESS;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

SPLINE_1D_BASIC *spline1D_basic_init(SPLINE_1D_BASIC *spl, const char *name_ef)
{
   if ( strcmp(name_ef,"S2") == 0 )
   {
      spl->COEFFS_POLYNOMES = m_get(3,3);
      spl->LOCAL_MESH       = v_get(3);
   }
   else
   if ( strcmp(name_ef,"S3") == 0 )
   {
      spl->COEFFS_POLYNOMES = m_get(4,4);
      spl->LOCAL_MESH       = v_get(4);
   }
   else
   if ( strcmp(name_ef,"S5") == 0 )
   {
      spl->COEFFS_POLYNOMES = m_get(6,6);
      spl->LOCAL_MESH       = v_get(6);
   }
   else
   {
      return NULL;
   }

   return spl;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

SPLINE_1D_BASIC *spline1D_basic_calc ( SPLINE_1D_BASIC* spline_basic, const char *name_ef, const VEC *local_mesh)
{
   if ( strcmp(name_ef,"S3") == 0 )
   {
      return spline1D_basic_calc_S3 ( spline_basic, local_mesh);
   }
   else
   if ( strcmp(name_ef,"S5") == 0 )
   {
      return spline1D_basic_calc_S5 ( spline_basic, local_mesh);
   }
   else
   {
      error(E_UNKNOWN, "spline1D_basic_calc");
      return NULL;
   }

   return spline_basic;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

static VEC* spline1D_basic_get_local_mesh__from_periodic_total_mesh(const SPLINE_1D  *spl, int i, const VEC *total_mesh, VEC *local_mesh)
{
   if ( strcmp(spl->type,"S3") == 0 )
   {
      return spline1D_basic_get_local_mesh__from_periodic_total_mesh_S3(spl, i, total_mesh, local_mesh);
   }
   else
   if ( strcmp(spl->type,"S5") == 0 )
   {
      return spline1D_basic_get_local_mesh__from_periodic_total_mesh_S5(spl, i, total_mesh, local_mesh);
   }
   else
   {
      error(E_UNKNOWN, "spline1D_basic_get_local_mesh__from_periodic_total_mesh");
      return NULL;
   }

   return NULL;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

static VEC* spline1D_basic_get_local_mesh__from_non_periodic_total_mesh(const SPLINE_1D  *spl, int i, const VEC *total_mesh, VEC *local_mesh)
{
   if ( strcmp(spl->type,"S3") == 0 )
   {
      return spline1D_basic_get_local_mesh__from_non_periodic_total_mesh_S3(spl, i, total_mesh, local_mesh);
   }
   else
   if ( strcmp(spl->type,"S5") == 0 )
   {
      return spline1D_basic_get_local_mesh__from_non_periodic_total_mesh_S5(spl, i, total_mesh, local_mesh);
   }
   else
   {
      error(E_UNKNOWN, "spline1D_basic_get_local_mesh__from_non_periodic_total_mesh");
      return NULL;
   }

   return NULL;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

static SPLINE_1D_BASIC *spline1D_basic_calc_S3 ( SPLINE_1D_BASIC* spline_basic, const VEC *local_mesh )
{
   MAT  *mat;
   PERM *piv;
   VEC  *vec;
   VEC  *sol;
   
   int i;
   double y[5]    = { 0.0 , 0.0 , 4.0 , 0.0 , 0.0 };
   double y_xx[5] = { 0.0 , 0.0 , 0.0 , 0.0 , 0.0 };

   /*
   y_xx  s.t.    y''(X0) = 0.0
                 y''(x4) = 0.0
   y     s.t     y(X0)   = 0.0
                 y(X2)   = 4.0
                 y(X4)   = 0.0
   */

   assert ( spline_basic->LOCAL_MESH->dim     == 4 );
   assert ( spline_basic->COEFFS_POLYNOMES->m == 4 );
   assert ( spline_basic->COEFFS_POLYNOMES->n == 4 );
   
   /* for the resolution : eval y_xx[1:3] with y1 and y3 <- sol */
   mat  = m_get(5,5);
   piv = px_get(5);
   vec  = v_get(5);
   sol  = v_get(5);

   /*  mat * sol = vec   with vec = [ y1 ; y''1 ; y''2 ; y''3 ; y3 ] */
      
   /* y'(0) = 0 -> eq with y1 : 2*ho*y''(Xo) + ho*y''(X1) - 6/ho*(y1 -y0) + 6*y'(Xo) = 0  -> ho*y''(X1) - 6/ho*(y1) = 0 */
   mat->me[0][0] =  6.0/(local_mesh->ve[0]);
   mat->me[0][1] =     -(local_mesh->ve[0]);
   /* */
   mat->me[1][0] =  6.0/(local_mesh->ve[0]) + 6.0/(local_mesh->ve[1]);
   mat->me[1][1] =  2.0*(local_mesh->ve[0]+local_mesh->ve[1]);
   mat->me[1][2] =                        (local_mesh->ve[1]);
   /* */
   mat->me[2][0] = -6.0/(local_mesh->ve[1]);
   mat->me[2][1] =      (local_mesh->ve[1]);
   mat->me[2][2] =  2.0*(local_mesh->ve[1]+local_mesh->ve[2]);
   mat->me[2][3] =                        (local_mesh->ve[2]);
   mat->me[2][4] = -6.0/(local_mesh->ve[2]);
   /* */
   mat->me[3][2] =      (local_mesh->ve[2]);
   mat->me[3][3] =  2.0*(local_mesh->ve[2]+local_mesh->ve[3]);
   mat->me[3][4] =  6.0/(local_mesh->ve[2]) + 6.0/(local_mesh->ve[3]);
   /* */
   /* y'(x4) = 0 -> eq with y4 : h3*y''(X3) + 2*h3*y''(X4) + 6/h3*(y4 -y3) + 6*y'(X4) = 0  -> h3*y''(X3) + 6/h3*(-y3) = 0 */
   mat->me[4][3] =     -(local_mesh->ve[3]);
   mat->me[4][4] =  6.0/(local_mesh->ve[3]);
   /* */
   /* */
   vec->ve[0] =  0.0;
   vec->ve[1] = -6.0/local_mesh->ve[0]*( y[1]-y[0] ) + 6.0/local_mesh->ve[1]*( y[2]-y[1] );
   vec->ve[2] = -6.0/local_mesh->ve[1]*( y[2]-y[1] ) + 6.0/local_mesh->ve[2]*( y[3]-y[2] );
   vec->ve[3] = -6.0/local_mesh->ve[2]*( y[3]-y[2] ) + 6.0/local_mesh->ve[3]*( y[4]-y[3] );
   vec->ve[4] =  0.0;
   /* */
   /* */
   /* solve and find y_xx[1], y_xx[2], y_xx[3] */
   /* */
   LUfactor(mat, piv);
   LUsolve(mat, piv, vec, sol);
   
   /* */
   y[1]    = sol->ve[0];
   y_xx[1] = sol->ve[1];
   y_xx[2] = sol->ve[2];
   y_xx[3] = sol->ve[3];
   y[3]    = sol->ve[4];
       
   /* get the coeff of the function */
   for (i=0; i<4; i++)
   {
      spline_basic->COEFFS_POLYNOMES->me[i][0] = 1.0/(6.0*local_mesh->ve[i]) * ( y_xx[i+1] - y_xx[i] );
      spline_basic->COEFFS_POLYNOMES->me[i][1] = 0.5 * y_xx[i];
      spline_basic->COEFFS_POLYNOMES->me[i][2] = 1.0/(local_mesh->ve[i]) * ( y[i+1] - y[i] ) - (1.0/6.0)*local_mesh->ve[i] * ( y_xx[i+1] + 2*y_xx[i] );
      spline_basic->COEFFS_POLYNOMES->me[i][3] = y[i];
      
      spline_basic->LOCAL_MESH->ve[i] = local_mesh->ve[i];
   }
   

   M_FREE(mat);
   PX_FREE(piv);
   V_FREE(vec);
   V_FREE(sol);

   /* finito */
   return spline_basic;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

static SPLINE_1D_BASIC *spline1D_basic_calc_S5 ( SPLINE_1D_BASIC* spline_basic, const VEC *local_mesh )
{
   MAT  *mat;
   PERM *piv;
   VEC  *vec;
   VEC  *sol;

   double y[7]    = { 0.0 , 0.0 , 4.0 , 0.0 , 0.0 };
   double y_xx[7] = { 0.0 , 0.0 , 0.0 , 0.0 , 0.0 };

   assert ( spline_basic->LOCAL_MESH->dim == 6 );

   error(E_UNKNOWN, "spline1D_basic_set_S5");

   return spline_basic;
}
/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

static VEC * total_mesh_get(const GEOM_1D *geom)
{
   VEC * total_mesh = v_get(geom->NBELMT);
   int i;

   for (i=0; i<geom->NBSOMM; i++)
   {
      total_mesh->ve[i] = geom->XSOMM->ve[i+1] - geom->XSOMM->ve[i] ;
   }

   return total_mesh;
}

/*----------------------------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------------------------- */

