
/** \file boundary_conditions_3D.c
 * Contains the structure of boundary conditions in 3 dimensions
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"


/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_3D_XTYPE* Bc3D_XTYPE_get( const char* name );
static int Bc3D_XTYPE_free( BC_3D_XTYPE *MyBC );

static BC_3D_XTYPE* Bc3D_XTYPE_setFunction1         (BC_3D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
static BC_3D_XTYPE* Bc3D_XTYPE_setFunction2         (BC_3D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);

static Real         Bc3D_XTYPE_evalFunction1        (BC_3D_XTYPE* MyBC, int ref_s, int axe, Real x , Real y, Real z);
static Real         Bc3D_XTYPE_evalFunction2        (BC_3D_XTYPE* MyBC, int ref_s, int axe, Real x , Real y, Real z);

static BC_3D_TYPE   Bc3D_getBcType_NoWarn           (const BC_3D* MyBC, int axe, int ref);

static int Bc3D_checkBCdirichlet(const BC_3D *MyBC, int axe, int ref);
static int Bc3D_checkBCneumann  (const BC_3D *MyBC, int axe, int ref);
static int Bc3D_checkBCcauchy   (const BC_3D *MyBC, int axe, int ref);
static int Bc3D_checkBCrobin    (const BC_3D *MyBC, int axe, int ref);

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_3D_XTYPE*  Bc3D_XTYPE_get(const char* name)
{
   int i,j;
   BC_3D_XTYPE *My_BC;

   if ( (My_BC=NEW(BC_3D_XTYPE)) == (BC_3D_XTYPE *)NULL )
   {
      error(E_MEM, "Bc3D_XTYPE_get");
   }
   else if (mem_info_is_on())
   {
      /*mem_bytes_list(TYPE_BC_3D_XTYPE, 0, sizeof(BC_3D_XTYPE), MY_LIST4);*/
      /*mem_numvar_list(TYPE_BC_3D_XTYPE, 1, MY_LIST4);*/
   }

   /* on initialise BC_dirichlet, BC_neumann, BCcauchy et BC_robin */
   strncpy(My_BC->BCname, name, 16);
   My_BC->BCname[15] = '\0';

   My_BC->current_selected_axe = AXEe_X;

   for (i=0; i<3; i++)
   for (j=0; j<NBMAX_BC_3D_FUNCTIONS; j++)
   {
      Fun3D_init(&(My_BC->Fun1[i][j]));
      Fun3D_init(&(My_BC->Fun2[i][j]));
   }

   return My_BC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc3D_XTYPE_free( BC_3D_XTYPE *MyBC )
{
   if ( MyBC == (BC_3D_XTYPE *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         /*mem_bytes_list(TYPE_BC_3D_XTYPE, sizeof(BC_3D_XTYPE), 0, MY_LIST4);*/
         /*mem_numvar_list(TYPE_BC_3D_XTYPE, -1, MY_LIST4);*/
      }

      /* free the structure BC_1D */
      free(MyBC);

      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_3D_XTYPE* Bc3D_XTYPE_setFunction1      ( BC_3D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata )
{
   if ( MyBC == NULL )                  error(E_NULL, "Bc3D_XTYPE_setFunction1");

   if ( ref_s > NBMAX_BC_3D_FUNCTIONS )               error4(E_BC_WRONGIDXNUMBER, "Bc3D_XTYPE_setFunction1");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_XTYPE_setFunction1");

   /* set the function */
   Fun3D_setFunction(&(MyBC->Fun1[axe][ref_s]), type, phi, clientdata);

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_3D_XTYPE* Bc3D_XTYPE_setFunction2      ( BC_3D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata )
{
   if ( MyBC == NULL )                  error(E_NULL, "Bc3D_XTYPE_setFunction2");

   if ( ref_s > NBMAX_BC_3D_FUNCTIONS )               error4(E_BC_WRONGIDXNUMBER, "Bc3D_XTYPE_setFunction2");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_XTYPE_setFunction2");

   /* set the function */
   Fun3D_setFunction(&(MyBC->Fun2[axe][ref_s]), type, phi, clientdata);

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real        Bc3D_XTYPE_evalFunction1        (BC_3D_XTYPE* MyBC, int ref_s, int axe, Real x , Real y, Real z )
{
   Real val = 0;

   if ( MyBC == NULL )                  error(E_NULL, "Bc3D_XTYPE_evalFunction1");

   if ( ref_s > NBMAX_BC_3D_FUNCTIONS )               error4(E_BC_WRONGIDXNUMBER, "Bc3D_XTYPE_evalFunction1");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_XTYPE_evalFunction1");

   val = MyBC->Fun1[axe][ref_s].eval(&MyBC->Fun1[axe][ref_s], x, y, z, MyBC->tps );

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real        Bc3D_XTYPE_evalFunction2        (BC_3D_XTYPE* MyBC, int ref_s, int axe, Real x , Real y, Real z)
{
   Real val = 0;

   if ( MyBC == NULL )                  error(E_NULL, "Bc3D_XTYPE_evalFunction2");

   if ( ref_s > NBMAX_BC_3D_FUNCTIONS )               error4(E_BC_WRONGIDXNUMBER, "Bc3D_XTYPE_evalFunction2");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_XTYPE_evalFunction2");

   val = MyBC->Fun2[axe][ref_s].eval(&MyBC->Fun2[axe][ref_s], x, y, z, MyBC->tps );

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D*  Bc3D_get(void)
{
   BC_3D *My_BC;
   int i;

   if ( (My_BC=NEW(BC_3D))== (BC_3D *)NULL )
   {
      error(E_MEM, "Bc3D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_BC_3D, 0, sizeof(BC_3D), MY_LIST4);
      mem_numvar_list(TYPE_BC_3D, 1, MY_LIST4);
   }

   /* on initialise BC_dirichlet, BC_neumann, BCcauchy et BC_robin */
   /* ie set the pointer to the right functions          */
   My_BC->BC_dirichlet = Bc3D_XTYPE_get("dirichlet");
   My_BC->BC_neumann   = Bc3D_XTYPE_get("neumann");
   My_BC->BC_cauchy    = Bc3D_XTYPE_get("cauchy");
   My_BC->BC_robin     = Bc3D_XTYPE_get("robin");

   for (i=0; i<NBMAX_BC_3D_FUNCTIONS; i++)
   {
      My_BC->BC_MASK[AXEe_X][i] = BC_3De_UNKNOWN;
      My_BC->BC_MASK[AXEe_Y][i] = BC_3De_UNKNOWN;
      My_BC->BC_MASK[AXEe_Z][i] = BC_3De_UNKNOWN;
   }

   return My_BC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D* Bc3D_setup_from_params(const PARAMS *MyParams)
{
	BC_3D* MyBC = Bc3D_get();

	int  i=0;
	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		Bc3D_setBcType(MyBC, MyParams->bc_params.BcType[AXEe_X][i], /*ref_s*/i, AXEe_X );
		Bc3D_setBcType(MyBC, MyParams->bc_params.BcType[AXEe_Y][i], /*ref_s*/i, AXEe_Y );
		Bc3D_setBcType(MyBC, MyParams->bc_params.BcType[AXEe_Z][i], /*ref_s*/i, AXEe_Z );
	}


	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		//Bc3D_setCFunction(MyBC, BC_3De_DIRICHLET, /*ref_s*/i, AXEe_X, BCphi3D[MyParams->bc_params.BcDirichlet[AXEe_X][i]] );
		//Bc3D_setCFunction(MyBC, BC_3De_DIRICHLET, /*ref_s*/i, AXEe_Y, BCphi3D[MyParams->bc_params.BcDirichlet[AXEe_Y][i]] );
		//Bc3D_setCFunction(MyBC, BC_3De_DIRICHLET, /*ref_s*/i, AXEe_Z, BCphi3D[MyParams->bc_params.BcDirichlet[AXEe_Z][i]] );

      Bc3D_setLUAFunction(MyBC, BC_3De_DIRICHLET, /*ref_s*/i, AXEe_X, MyParams->bc_params.BcDirichlet[AXEe_X][i].fundef);
      Bc3D_setLUAFunction(MyBC, BC_3De_DIRICHLET, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BcDirichlet[AXEe_Y][i].fundef);
      Bc3D_setLUAFunction(MyBC, BC_3De_DIRICHLET, /*ref_s*/i, AXEe_Z, MyParams->bc_params.BcDirichlet[AXEe_Z][i].fundef);
	}



   for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		//Bc3D_setCFunction(MyBC, BC_3De_NEUMANN, /*ref_s*/i, AXEe_X, BCphi3D[MyParams->bc_params.BCNeumann[AXEe_X][i]] );
		//Bc3D_setCFunction(MyBC, BC_3De_NEUMANN, /*ref_s*/i, AXEe_Y, BCphi3D[MyParams->bc_params.BCNeumann[AXEe_Y][i]] );
		//Bc3D_setCFunction(MyBC, BC_3De_NEUMANN, /*ref_s*/i, AXEe_Z, BCphi3D[MyParams->bc_params.BCNeumann[AXEe_Z][i]] );

      Bc3D_setLUAFunction(MyBC, BC_3De_NEUMANN, /*ref_s*/i, AXEe_X, MyParams->bc_params.BCNeumann[AXEe_X][i].fundef);
      Bc3D_setLUAFunction(MyBC, BC_3De_NEUMANN, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BCNeumann[AXEe_Y][i].fundef);
      Bc3D_setLUAFunction(MyBC, BC_3De_NEUMANN, /*ref_s*/i, AXEe_Z, MyParams->bc_params.BCNeumann[AXEe_Z][i].fundef);
	}

   for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		//Bc3D_setCFunction(MyBC, BC_3De_CAUCHY, /*ref_s*/i, AXEe_X, BCphi3D[MyParams->bc_params.BcCauchy[AXEe_X][i]] );
		//Bc3D_setCFunction(MyBC, BC_3De_CAUCHY, /*ref_s*/i, AXEe_Y, BCphi3D[MyParams->bc_params.BcCauchy[AXEe_Y][i]] );
		//Bc3D_setCFunction(MyBC, BC_3De_CAUCHY, /*ref_s*/i, AXEe_Z, BCphi3D[MyParams->bc_params.BcCauchy[AXEe_Z][i]] );

      Bc3D_setLUAFunction(MyBC, BC_3De_CAUCHY, /*ref_s*/i, AXEe_X, MyParams->bc_params.BcCauchy[AXEe_X][i].fundef);
      Bc3D_setLUAFunction(MyBC, BC_3De_CAUCHY, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BcCauchy[AXEe_Y][i].fundef);
      Bc3D_setLUAFunction(MyBC, BC_3De_CAUCHY, /*ref_s*/i, AXEe_Z, MyParams->bc_params.BcCauchy[AXEe_Z][i].fundef);
	}

   for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		//Bc3D_setCFunction (MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_X, BCphi3D[MyParams->bc_params.BcRobin1[AXEe_X][i]]);
		//Bc3D_setCFunction2(MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_X, BCphi3D[MyParams->bc_params.BcRobin2[AXEe_X][i]]);

		//Bc3D_setCFunction (MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_Y, BCphi3D[MyParams->bc_params.BcRobin1[AXEe_Y][i]]);
		//Bc3D_setCFunction2(MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_Y, BCphi3D[MyParams->bc_params.BcRobin2[AXEe_Y][i]]);

		//Bc3D_setCFunction (MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_Z, BCphi3D[MyParams->bc_params.BcRobin1[AXEe_Z][i]]);
		//Bc3D_setCFunction2(MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_Z, BCphi3D[MyParams->bc_params.BcRobin2[AXEe_Z][i]]);

      Bc3D_setLUAFunction (MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_X, MyParams->bc_params.BcRobin1[AXEe_X][i].fundef);
      Bc3D_setLUAFunction2(MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_X, MyParams->bc_params.BcRobin2[AXEe_X][i].fundef);

      Bc3D_setLUAFunction (MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BcRobin1[AXEe_Y][i].fundef);
      Bc3D_setLUAFunction2(MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BcRobin2[AXEe_Y][i].fundef);

      Bc3D_setLUAFunction (MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_Z, MyParams->bc_params.BcRobin1[AXEe_Z][i].fundef);
      Bc3D_setLUAFunction2(MyBC, BC_3De_ROBIN, /*ref_s*/i, AXEe_Z, MyParams->bc_params.BcRobin2[AXEe_Z][i].fundef);
   }

	return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Bc3D_free( BC_3D *MyBC )
{
   if ( MyBC == (BC_3D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_BC_3D, sizeof(BC_3D), 0, MY_LIST4);
         mem_numvar_list(TYPE_BC_3D, -1, MY_LIST4);
      }

      /* free the structure BC_1D */
      BC_3D_XTYPE_FREE(MyBC->BC_dirichlet);
      BC_3D_XTYPE_FREE(MyBC->BC_neumann  );
      BC_3D_XTYPE_FREE(MyBC->BC_cauchy   );
      BC_3D_XTYPE_FREE(MyBC->BC_robin    );

      free(MyBC);

      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D        *Bc3D_setBcType            ( BC_3D* MyBC, BC_3D_TYPE type, int ref, int axe)
{
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_setBcType");

   if ( ref > NBMAX_BC_3D_FUNCTIONS )                 error4(E_BC_WRONGIDXNUMBER, "Bc3D_setBcType");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_setBcType");

	if ( type == BC_3De_UNKNOWN )
	{
		return MyBC;
	}

   if ( (type != BC_3De_DIRICHLET) && (type != BC_3De_NEUMANN) && (type != BC_3De_CAUCHY) ) error4(E_BC_WRONGBCTYPE, "Bc3D_setBcType");


   MyBC->BC_MASK[axe][ref] = type;

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D_TYPE    Bc3D_getBcType            ( const BC_3D* MyBC, int axe, int ref)
{
   if ( MyBC == NULL )                                error(E_NULL, "Bc3D_getBcType");

   if ( ref > NBMAX_BC_3D_FUNCTIONS )                 error4(E_BC_WRONGIDXNUMBER, "Bc3D_getBcType");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_getBcType");


   if ( BC_3De_UNKNOWN == MyBC->BC_MASK[axe][ref] )   warning4(W_BC_TYPENOTSET, "Bc3D_getBcType");

   return MyBC->BC_MASK[axe][ref];
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_3D_TYPE    Bc3D_getBcType_NoWarn         ( const BC_3D* MyBC, int axe, int ref)
{
   if ( MyBC == NULL )                                error(E_NULL, "Bc3D_getBcType_NoWarn");

   if ( ref > NBMAX_BC_3D_FUNCTIONS )                 error4(E_BC_WRONGIDXNUMBER, "Bc3D_getBcType_NoWarn");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_getBcType_NoWarn");

   return MyBC->BC_MASK[axe][ref];
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D *Bc3D_setFunction1      ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata )
{
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_setFunction1");

   if ( ref_s > NBMAX_BC_3D_FUNCTIONS )               error4(E_BC_WRONGIDXNUMBER, "Bc3D_setFunction1");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_setFunction1");


   if ( bctype == BC_3De_DIRICHLET )
   {
      Bc3D_XTYPE_setFunction1( MyBC->BC_dirichlet, ref_s, axe, type, phi, clientdata);
   }
   else
   if ( bctype == BC_3De_NEUMANN )
   {
      Bc3D_XTYPE_setFunction1( MyBC->BC_neumann  , ref_s, axe, type, phi, clientdata);
   }
   else
   if ( bctype == BC_3De_CAUCHY )
   {
      Bc3D_XTYPE_setFunction1( MyBC->BC_cauchy   , ref_s, axe, type, phi, clientdata);
   }
   else
   if ( bctype == BC_3De_ROBIN )
   {
      Bc3D_XTYPE_setFunction1( MyBC->BC_robin    , ref_s, axe, type, phi, clientdata);
   }
   else
   {
      error(E_UNKNOWN, "Bc3D_setFunction1");
   }

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D *Bc3D_setFunction2      ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_setFunction2");

   if ( ref_s > NBMAX_BC_3D_FUNCTIONS )               error4(E_BC_WRONGIDXNUMBER, "Bc3D_setFunction2");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_setFunction2");


   if ( bctype == BC_3De_DIRICHLET )
   {
      warning(W_BC_SECONDFUNCDIRICHLET, "Bc3D_setFunction2");
   }
   else
   if ( bctype == BC_3De_NEUMANN )
   {
      warning(W_BC_SECONDFUNCNEUMANN, "Bc3D_setFunction2");
   }
   else
   if ( bctype == BC_3De_CAUCHY )
   {
      warning(W_BC_SECONDFUNCCAUCHY, "Bc3D_setFunction2");
   }
   else
   if ( bctype == BC_3De_ROBIN )
   {
      Bc3D_XTYPE_setFunction2( MyBC->BC_robin    , ref_s, axe, type, phi, clientdata);
   }
   else
   {
      error(E_UNKNOWN, "Bc3D_setFunction2");
   }

   return MyBC;
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

BC_3D *Bc3D_setCFunction       ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_3D phi)
{
   return Bc3D_setFunction1( MyBC, bctype, ref_s, axe, FUN_C_STATIONNARY, phi, NULL);
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

BC_3D *Bc3D_setLUAFunction       ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, const char* def)
{
   void *L = make_lua_interpreter(def, "3D");

   return Bc3D_setFunction1( MyBC, bctype, ref_s, axe, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction3D, L);
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

BC_3D *Bc3D_setCFunction2      ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_3D phi )
{
   return Bc3D_setFunction2( MyBC, bctype, ref_s, axe, FUN_C_STATIONNARY, phi, NULL );
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

BC_3D *Bc3D_setLUAFunction2      ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, const char* def)
{
   void *L = make_lua_interpreter(def, "3D");

   return Bc3D_setFunction2( MyBC, bctype, ref_s, axe, FUN_C_STATIONNARY, FunctionForEvaluatingLuaFunction3D, L );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D *Bc3D_setCFunctionTransient  ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_4D phi)
{
   return Bc3D_setFunction1( MyBC, bctype, ref_s, axe, FUN_C_TRANSIENT, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D *Bc3D_setLUAFunctionTransient ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, const char* def)
{
   void *L = make_lua_interpreter(def, "3D_TR");

   return Bc3D_setFunction1( MyBC, bctype, ref_s, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction4D, L );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D *Bc3D_setCFunctionTransient2 ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_4D phi)
{
   return Bc3D_setFunction2( MyBC, bctype, ref_s, axe, FUN_C_TRANSIENT, phi, NULL );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D *Bc3D_setLUAFunctionTransient2 ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, const char* def)
{
   void *L = make_lua_interpreter(def, "3D_TR");

   return Bc3D_setFunction2( MyBC, bctype, ref_s, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction4D, L );
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

Real  Bc3D_evalFunction1  (const BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, Real x, Real y, Real z)
{
   Real val = 0.0;

   if ( MyBC == NULL ) error(E_NULL, "Bc3D_evalFunction1");

   if ( ref_s > NBMAX_BC_3D_FUNCTIONS )               error4(E_BC_WRONGIDXNUMBER, "Bc3D_evalFunction1");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_evalFunction1");


   if ( bctype == BC_3De_DIRICHLET )
   {
      val = Bc3D_XTYPE_evalFunction1( MyBC->BC_dirichlet, ref_s, axe, x, y, z );
   }
   else
   if ( bctype == BC_3De_NEUMANN )
   {
      val = Bc3D_XTYPE_evalFunction1( MyBC->BC_neumann  , ref_s, axe, x, y, z );
   }
   else
   if ( bctype == BC_3De_CAUCHY )
   {
      val = Bc3D_XTYPE_evalFunction1( MyBC->BC_cauchy   , ref_s, axe, x, y, z );
   }
   else
   if ( bctype == BC_3De_ROBIN )
   {
      val = Bc3D_XTYPE_evalFunction1( MyBC->BC_robin    , ref_s, axe, x, y, z );
   }
   else
   {
      error(E_UNKNOWN, "Bc3D_evalFunction1");
   }

   return val;
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

Real  Bc3D_evalFunction2  (const BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, Real x, Real y, Real z)
{
   Real val = 0.0;

   if ( MyBC == NULL ) error(E_NULL, "Bc3D_evalFunction2");

   if ( ref_s > NBMAX_BC_3D_FUNCTIONS )               error4(E_BC_WRONGIDXNUMBER, "Bc3D_evalFunction2");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_evalFunction2");


   if ( bctype == BC_3De_ROBIN )
   {
      val = Bc3D_XTYPE_evalFunction2( MyBC->BC_robin    , ref_s, axe, x, y, z );
   }
   else
   {
      error(E_UNKNOWN, "Bc3D_evalFunction2");
   }

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D* Bc3D_setTps ( BC_3D* MyBC, Real tps )
{
   if ( MyBC == NULL )     error (E_NULL, "Bc3D_setTps");

   MyBC->BC_dirichlet->tps = tps;
   MyBC->BC_neumann->tps = tps;
   MyBC->BC_cauchy->tps = tps;
   MyBC->BC_robin->tps = tps;

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_3D* Bc3D_setCurrentSelectedAxe ( BC_3D* MyBc, int axe)
{
   /* check */
   if ( MyBc == NULL )   error (E_NULL, "Bc3D_setCurrentSelectedAxe");
   /* check */
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_setCurrentSelectedAxe");

   MyBc->BC_dirichlet->current_selected_axe = axe;
   MyBc->BC_neumann->current_selected_axe = axe;
   MyBc->BC_cauchy->current_selected_axe = axe;
   MyBc->BC_robin->current_selected_axe = axe;

   return MyBc;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int           Bc3D_checkBC               ( const BC_3D* MyBC , int axe)
{
   int ref;
   BC_3D_TYPE type;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_checkBC");

   /* check if we se the functions */
   for (ref=0; ref<NBMAX_BC_3D_FUNCTIONS; ref++)
   {
      type = Bc3D_getBcType_NoWarn( MyBC, axe, ref);

      /* check that the function is set */
      switch(type)
      {
         case BC_3De_UNKNOWN:

            break;

         case BC_3De_DIRICHLET:

            Bc3D_checkBCdirichlet(MyBC, axe, ref);

            break;

         case BC_3De_NEUMANN:

            Bc3D_checkBCneumann(MyBC, axe, ref);

            break;

         case BC_3De_CAUCHY:

            Bc3D_checkBCcauchy(MyBC, axe, ref);

            break;

         case BC_3De_ROBIN:

            Bc3D_checkBCrobin(MyBC, axe, ref);

            break;
      }
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int  Bc3D_hasDirichletBC        ( const BC_3D* MyBC , int axe)
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_hasDirichletBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y)&&(axe !=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_hasDirichletBC");

   for (i=0; i<NBMAX_BC_3D_FUNCTIONS; i++)
   {
      if ( BC_3De_DIRICHLET == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int  Bc3D_hasNeumannBC          (const BC_3D* MyBC, int axe)
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_hasNeumannBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y)&&(axe !=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_hasNeumannBC");

   for (i=0; i<NBMAX_BC_3D_FUNCTIONS; i++)
   {
      if ( BC_3De_NEUMANN == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc3D_hasCauchyBC           ( const BC_3D* MyBC , int axe)
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_hasCauchyBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y)&&(axe !=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_hasCauchyBC");

   for (i=0; i<NBMAX_BC_3D_FUNCTIONS; i++)
   {
      if ( BC_3De_CAUCHY == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc3D_hasRobinBC           (const BC_3D* MyBC, int axe)
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_hasRobinBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y)&&(axe !=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_hasRobinBC");

   for (i=0; i<NBMAX_BC_3D_FUNCTIONS; i++)
   {
      if ( BC_3De_ROBIN == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc3D_getNumberOfBC          (const BC_3D* MyBC, int axe)
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_getNumberOfBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y)&&(axe !=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_getNumberOfBC");

   for (i=0; i<NBMAX_BC_3D_FUNCTIONS; i++)
   {
      if ( BC_3De_UNKNOWN != MyBC->BC_MASK[axe][i] )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc3D_getNumberOfBcOnSomms          ( const BC_3D* MyBC , int axe)
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_getNumberOfBcOnSomms");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y)&&(axe !=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_getNumberOfBcOnSomms");

   for (i=0; i<NBMAX_BC_3D_FUNCTIONS; i++)
   {
      if ( (BC_3De_DIRICHLET == MyBC->BC_MASK[axe][i]) || (BC_3De_CAUCHY == MyBC->BC_MASK[axe][i]) || (BC_3De_ROBIN == MyBC->BC_MASK[axe][i]) )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc3D_getNumberOfBcOnFaces          (const BC_3D* MyBC, int axe)
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc3D_getNumberOfBcOnFaces");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y)&&(axe !=AXEe_Z) ) error4(E_BC_WRONGAXENUMBER, "Bc3D_getNumberOfBcOnFaces");

   for (i=0; i<NBMAX_BC_3D_FUNCTIONS; i++)
   {
      if ( (BC_3De_NEUMANN == MyBC->BC_MASK[axe][i]) || (BC_3De_ROBIN == MyBC->BC_MASK[axe][i]) )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc3D_checkBCdirichlet(const BC_3D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_dirichlet->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyz  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyzt  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyz_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyzt_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_STATIONNARY:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyz_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_TRANSIENT:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyzt_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc3D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc3D_checkBCdirichlet");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc3D_checkBCneumann(const BC_3D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_neumann->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyz  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyzt  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyz_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyzt_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_STATIONNARY:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyz_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_TRANSIENT:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyzt_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc3D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc3D_checkBCneumann");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc3D_checkBCcauchy(const BC_3D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_cauchy->Fun1[AXEe_X][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyz  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyzt  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyz_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyzt_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_STATIONNARY:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyz_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_TRANSIENT:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyzt_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc3D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc3D_checkBCcauchy");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc3D_checkBCrobin(const BC_3D *MyBC, int axe, int ref)
{
   /* the type of function should be the same for the two robin's function on the same reference */
   if ( MyBC->BC_robin->Fun1[axe][ref].type != MyBC->BC_robin->Fun2[axe][ref].type )
   {
      error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");
      return EXIT_FAILURE;
   }

   /* check that the function are set , according to their type */
   switch( MyBC->BC_robin->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyz  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyz  == NULL )
         {   error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyzt  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyzt  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyz_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyz_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyzt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyzt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_LUA_STATIONNARY:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyz_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyz_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_LUA_TRANSIENT:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyzt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyzt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc3D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc3D_checkBCrobin");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
