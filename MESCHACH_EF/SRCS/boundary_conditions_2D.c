
/** \file boundary_conditions_2D.c
 * Contains the structure of boundary conditions in 2 dimensions
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"


/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_2D_XTYPE* Bc2D_XTYPE_get ( BC_2D_TYPE bc_type );
static int          Bc2D_XTYPE_free( BC_2D_XTYPE *MyBC );

static BC_2D_XTYPE* Bc2D_XTYPE_setFunction1  ( BC_2D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
static BC_2D_XTYPE* Bc2D_XTYPE_setFunction2  ( BC_2D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);

static Real         Bc2D_XTYPE_evalFunction1 ( BC_2D_XTYPE* MyBC, int ref_s, int axe, Real x, Real y);
static Real         Bc2D_XTYPE_evalFunction2 ( BC_2D_XTYPE* MyBC, int ref_s, int axe, Real x, Real y);

static BC_2D_TYPE   Bc2D_getBcType_NoWarn    ( const BC_2D* MyBC, int axe, int ref);

static int Bc2D_checkBCdirichlet(const BC_2D *MyBC, int axe, int ref);
static int Bc2D_checkBCneumann  (const BC_2D *MyBC, int axe, int ref);
static int Bc2D_checkBCcauchy   (const BC_2D *MyBC, int axe, int ref);
static int Bc2D_checkBCrobin    (const BC_2D *MyBC, int axe, int ref);

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_2D_XTYPE*  Bc2D_XTYPE_get(BC_2D_TYPE bc_type)
{
   int i,j;
   BC_2D_XTYPE *My_BC;

   if ( (My_BC=NEW(BC_2D_XTYPE)) == (BC_2D_XTYPE *)NULL )
      error(E_MEM, "Bc2D_XTYPE_get");
   else if (mem_info_is_on())
   {
      /*mem_bytes_list(TYPE_BC_2D_XTYPE, 0, sizeof(BC_2D_XTYPE), MY_LIST4);*/
      /*mem_numvar_list(TYPE_BC_2D_XTYPE, 1, MY_LIST4);*/
   }

   /* on initialise BC_dirichlet, BC_neumann, BCcauchy et BC_robin */
   My_BC->bc_type = bc_type;

   My_BC->current_selected_axe = AXEe_X;

   for (i=0; i<2; i++)
   for (j=0; j<NBMAX_BC_2D_FUNCTIONS; j++)
   {
      Fun2D_init(&(My_BC->Fun1[i][j]));
      Fun2D_init(&(My_BC->Fun2[i][j]));
   }

   return My_BC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc2D_XTYPE_free( BC_2D_XTYPE *MyBC )
{
   if ( MyBC == (BC_2D_XTYPE *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         /*mem_bytes_list(TYPE_BC_2D_XTYPE, sizeof(BC_2D_XTYPE), 0, MY_LIST4);*/
         /*mem_numvar_list(TYPE_BC_2D_XTYPE, -1, MY_LIST4);*/
      }

      /* free the structure BC_2D */
      free(MyBC);

      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_2D_XTYPE* Bc2D_XTYPE_setFunction1      ( BC_2D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata )
{
   if ( MyBC == NULL )                    error(E_NULL, "Bc2D_XTYPE_setFunction1");

   if ( ref_s > NBMAX_BC_2D_FUNCTIONS )    error4(E_BC_WRONGIDXNUMBER, "Bc2D_XTYPE_setFunction1");
   if ( (axe != AXEe_X)&&(axe != AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_XTYPE_setFunction1");

   /* set the function */
   Fun2D_setFunction(&(MyBC->Fun1[axe][ref_s]), type, phi, clientdata );

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_2D_XTYPE* Bc2D_XTYPE_setFunction2      ( BC_2D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata )
{
   if ( MyBC == NULL )                    error(E_NULL, "Bc2D_XTYPE_setFunction2");

   if ( ref_s > NBMAX_BC_2D_FUNCTIONS )    error4(E_BC_WRONGIDXNUMBER, "Bc2D_XTYPE_setFunction2");
   if ( (axe != AXEe_X)&&(axe != AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_XTYPE_setFunction2");

   /* set the function */
   Fun2D_setFunction(&(MyBC->Fun2[axe][ref_s]), type, phi, clientdata );

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real       Bc2D_XTYPE_evalFunction1         ( BC_2D_XTYPE* MyBC, int ref_s, int axe , Real x, Real y )
{
   Real val = 0;

   if ( MyBC == NULL )                  error(E_NULL, "Bc2D_XTYPE_evalFunction1");

   if ( ref_s > NBMAX_BC_2D_FUNCTIONS )      error4(E_BC_WRONGIDXNUMBER, "Bc2D_XTYPE_evalFunction1");
   if ( (axe != AXEe_X)&&(axe != AXEe_Y ) )  error4(E_BC_WRONGAXENUMBER, "Bc2D_XTYPE_evalFunction1");

   val = MyBC->Fun1[axe][ref_s].eval(&MyBC->Fun1[axe][ref_s], x, y, MyBC->tps );

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real       Bc2D_XTYPE_evalFunction2         ( BC_2D_XTYPE* MyBC, int ref_s, int axe , Real x, Real y )
{
   Real val = 0;

   if ( MyBC == NULL )                  error(E_NULL, "Bc2D_XTYPE_evalFunction2");

   if ( ref_s > NBMAX_BC_2D_FUNCTIONS )     error4(E_BC_WRONGIDXNUMBER, "Bc2D_XTYPE_evalFunction2");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y ) )  error4(E_BC_WRONGAXENUMBER, "Bc2D_XTYPE_evalFunction2");

   val = MyBC->Fun2[axe][ref_s].eval(&MyBC->Fun2[axe][ref_s], x, y, MyBC->tps );

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D*  Bc2D_get( void )
{
   BC_2D *My_BC;
   int i;

   if ( (My_BC=NEW(BC_2D)) == (BC_2D *)NULL )
   {
      error(E_MEM, "BC_2D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_BC_2D, 0, sizeof(BC_2D), MY_LIST4);
      mem_numvar_list(TYPE_BC_2D, 1, MY_LIST4);
   }

   /* on initialise BC_dirichlet, BC_neumann, BCcauchy et BC_robin */
   /* ie set the pointer to the right functions          */
   My_BC->BC_dirichlet = Bc2D_XTYPE_get(BC_2De_DIRICHLET);
   My_BC->BC_neumann   = Bc2D_XTYPE_get(BC_2De_NEUMANN);
   My_BC->BC_cauchy    = Bc2D_XTYPE_get(BC_2De_CAUCHY);
   My_BC->BC_robin     = Bc2D_XTYPE_get(BC_2De_ROBIN);

   for (i=0; i<NBMAX_BC_2D_FUNCTIONS; i++)
   {
      My_BC->BC_MASK[AXEe_X][i] = BC_2De_UNKNOWN;
      My_BC->BC_MASK[AXEe_Y][i] = BC_2De_UNKNOWN;
   }

   return My_BC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D *Bc2D_setup_from_params(const PARAMS *MyParams)
{
	BC_2D *MyBC = Bc2D_get();

	int  i=0;
	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		Bc2D_setBcType(MyBC, MyParams->bc_params.BcType[AXEe_X][i], /*ref_s*/i, AXEe_X );
		Bc2D_setBcType(MyBC, MyParams->bc_params.BcType[AXEe_Y][i], /*ref_s*/i, AXEe_Y );
	}

	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		//Bc2D_setCFunction(MyBC, BC_2De_DIRICHLET, /*ref_s*/i, AXEe_X, BCphi2D[MyParams->bc_params.BcDirichlet[AXEe_X][i]] );
		//Bc2D_setCFunction(MyBC, BC_2De_DIRICHLET, /*ref_s*/i, AXEe_Y, BCphi2D[MyParams->bc_params.BcDirichlet[AXEe_Y][i]] );

      Bc2D_setLUAFunction(MyBC, BC_2De_DIRICHLET, /*ref_s*/i, AXEe_X, MyParams->bc_params.BcDirichlet[AXEe_X][i].fundef );
      Bc2D_setLUAFunction(MyBC, BC_2De_DIRICHLET, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BcDirichlet[AXEe_Y][i].fundef );
	}

   for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		//Bc2D_setCFunction(MyBC, BC_2De_NEUMANN, /*ref_s*/i, AXEe_X, BCphi2D[MyParams->bc_params.BCNeumann[AXEe_X][i]] );
		//Bc2D_setCFunction(MyBC, BC_2De_NEUMANN, /*ref_s*/i, AXEe_Y, BCphi2D[MyParams->bc_params.BCNeumann[AXEe_Y][i]] );

      Bc2D_setLUAFunction(MyBC, BC_2De_NEUMANN, /*ref_s*/i, AXEe_X, MyParams->bc_params.BCNeumann[AXEe_X][i].fundef );
      Bc2D_setLUAFunction(MyBC, BC_2De_NEUMANN, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BCNeumann[AXEe_Y][i].fundef );
	}

   for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		//Bc2D_setCFunction(MyBC, BC_2De_CAUCHY, /*ref_s*/i, AXEe_X, BCphi2D[MyParams->bc_params.BcCauchy[AXEe_X][i]] );
		//Bc2D_setCFunction(MyBC, BC_2De_CAUCHY, /*ref_s*/i, AXEe_Y, BCphi2D[MyParams->bc_params.BcCauchy[AXEe_Y][i]] );

      Bc2D_setLUAFunction(MyBC, BC_2De_CAUCHY, /*ref_s*/i, AXEe_X, MyParams->bc_params.BcCauchy[AXEe_X][i].fundef );
      Bc2D_setLUAFunction(MyBC, BC_2De_CAUCHY, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BcCauchy[AXEe_Y][i].fundef );
	}
	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		//Bc2D_setCFunction (MyBC, BC_2De_ROBIN, /*ref_s*/i, AXEe_X, BCphi2D[MyParams->bc_params.BcRobin1[AXEe_X][i]]);
		//Bc2D_setCFunction2(MyBC, BC_2De_ROBIN, /*ref_s*/i, AXEe_X, BCphi2D[MyParams->bc_params.BcRobin2[AXEe_X][i]]);

		//Bc2D_setCFunction (MyBC, BC_2De_ROBIN, /*ref_s*/i, AXEe_Y, BCphi2D[MyParams->bc_params.BcRobin1[AXEe_Y][i]]);
		//Bc2D_setCFunction2(MyBC, BC_2De_ROBIN, /*ref_s*/i, AXEe_Y, BCphi2D[MyParams->bc_params.BcRobin2[AXEe_Y][i]]);

      Bc2D_setLUAFunction (MyBC, BC_2De_ROBIN, /*ref_s*/i, AXEe_X, MyParams->bc_params.BcRobin1[AXEe_X][i].fundef);
      Bc2D_setLUAFunction2(MyBC, BC_2De_ROBIN, /*ref_s*/i, AXEe_X, MyParams->bc_params.BcRobin2[AXEe_X][i].fundef);

      Bc2D_setLUAFunction (MyBC, BC_2De_ROBIN, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BcRobin1[AXEe_Y][i].fundef);
      Bc2D_setLUAFunction2(MyBC, BC_2De_ROBIN, /*ref_s*/i, AXEe_Y, MyParams->bc_params.BcRobin2[AXEe_Y][i].fundef);
	}

	return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Bc2D_free( BC_2D *MyBC )
{
   if ( MyBC == (BC_2D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_BC_2D, sizeof(BC_2D), 0, MY_LIST4);
         mem_numvar_list(TYPE_BC_2D, -1, MY_LIST4);
      }

      /* free the structure BC_2D */
      BC_2D_XTYPE_FREE(MyBC->BC_dirichlet);
      BC_2D_XTYPE_FREE(MyBC->BC_neumann  );
      BC_2D_XTYPE_FREE(MyBC->BC_cauchy   );
      BC_2D_XTYPE_FREE(MyBC->BC_robin    );

      free(MyBC);

      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D        *Bc2D_setBcType            ( BC_2D* MyBC, BC_2D_TYPE type, int ref, int axe )
{
   if ( MyBC == NULL )                      error(E_NULL, "Bc2D_setBcType");

   if ( ref > NBMAX_BC_2D_FUNCTIONS )       error4(E_BC_WRONGIDXNUMBER, "Bc2D_setBcType");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y ) )  error4(E_BC_WRONGAXENUMBER, "Bc2D_setBcType");

   if ( type == BC_2De_UNKNOWN )
   {
      return MyBC;
   }

   if ( (type != BC_2De_DIRICHLET) && (type != BC_2De_NEUMANN) && (type != BC_2De_CAUCHY) && (type != BC_2De_ROBIN)) error4(E_BC_WRONGBCTYPE, "Bc2D_setBcType");

   MyBC->BC_MASK[axe][ref] = type;

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D_TYPE    Bc2D_getBcType            ( const BC_2D* MyBC, int axe, int ref)
{
   if ( MyBC == NULL )                      error(E_NULL, "Bc2D_getBcType");

   if ( ref > NBMAX_BC_2D_FUNCTIONS )       error4(E_BC_WRONGIDXNUMBER, "Bc2D_getBcType");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y ) )  error4(E_BC_WRONGAXENUMBER, "Bc2D_getBcType");


   if ( BC_2De_UNKNOWN == MyBC->BC_MASK[axe][ref] ) warning4(W_BC_TYPENOTSET, "Bc2D_getBcType");

   return MyBC->BC_MASK[axe][ref];
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_2D_TYPE    Bc2D_getBcType_NoWarn            ( const BC_2D* MyBC, int axe, int ref)
{
   if ( MyBC == NULL )                       error(E_NULL, "Bc2D_getBcType_NoWarn");

   if ( ref > NBMAX_BC_2D_FUNCTIONS )        error4(E_BC_WRONGIDXNUMBER, "Bc2D_getBcType_NoWarn");
   if ( (axe != AXEe_X)&&(axe != AXEe_Y ) )  error4(E_BC_WRONGAXENUMBER, "Bc2D_getBcType_NoWarn");

   return MyBC->BC_MASK[axe][ref];
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D*  Bc2D_setFunction1      ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata )
{
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_setFunction1");


   if ( bctype == BC_2De_DIRICHLET )
   {
      Bc2D_XTYPE_setFunction1( MyBC->BC_dirichlet, ref_s, axe, type, phi, clientdata );
   }
   else
   if ( bctype == BC_2De_NEUMANN )
   {
      Bc2D_XTYPE_setFunction1( MyBC->BC_neumann  , ref_s, axe, type, phi, clientdata );
   }
   else
   if ( bctype == BC_2De_CAUCHY )
   {
      Bc2D_XTYPE_setFunction1( MyBC->BC_cauchy   , ref_s, axe, type, phi, clientdata );
   }
   else
   if ( bctype == BC_2De_ROBIN )
   {
      Bc2D_XTYPE_setFunction1( MyBC->BC_robin    , ref_s, axe, type, phi, clientdata );
   }
   else
   {
      error(E_UNKNOWN, "Bc2D_setFunction1");
   }

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D*  Bc2D_setFunction2      ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata )
{
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_setFunction2");


   if ( bctype == BC_2De_DIRICHLET )
   {
      warning(W_BC_SECONDFUNCDIRICHLET, "Bc2D_setFunction2");
   }
   else
   if ( bctype == BC_2De_NEUMANN )
   {
      warning(W_BC_SECONDFUNCNEUMANN, "Bc2D_setFunction2");
   }
   else
   if ( bctype == BC_2De_CAUCHY )
   {
      warning(W_BC_SECONDFUNCCAUCHY, "Bc2D_setFunction2");
   }
   else
   if ( bctype == BC_2De_ROBIN )
   {
      Bc2D_XTYPE_setFunction2( MyBC->BC_robin    , ref_s, axe, type, phi, clientdata );
   }
   else
   {
      error(E_UNKNOWN, "Bc2D_setFunction2");
   }

   return MyBC;
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

BC_2D *Bc2D_setCFunction      ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , FUNC_2D phi )
{
   return Bc2D_setFunction1( MyBC, bctype, ref_s, axe, FUN_C_STATIONNARY, phi, NULL );
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

BC_2D *Bc2D_setLUAFunction      ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , const char* def )
{
   void *L = make_lua_interpreter(def, "2D");

   return Bc2D_setFunction1( MyBC, bctype, ref_s, axe, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction2D, L );
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

BC_2D *Bc2D_setCFunction2      ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , FUNC_2D phi )
{
   return Bc2D_setFunction2( MyBC, bctype, ref_s, axe, FUN_C_STATIONNARY, phi, NULL );
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

BC_2D *Bc2D_setLUAFunction2   ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , const char* def )
{
   void *L = make_lua_interpreter(def, "2D");

   return Bc2D_setFunction2( MyBC, bctype, ref_s, axe, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction2D, L );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D *Bc2D_setCFunctionTransient ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , FUNC_3D phi )
{
   return Bc2D_setFunction1( MyBC, bctype, ref_s, axe, FUN_C_TRANSIENT, phi, NULL );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D *Bc2D_setLUAFunctionTransient ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , const char* def )
{
   void *L = make_lua_interpreter(def, "2D_TR");

   return Bc2D_setFunction1( MyBC, bctype, ref_s, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction3D, L );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D *Bc2D_setCFunctionTransient2 ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , FUNC_3D phi )
{
   return Bc2D_setFunction2( MyBC, bctype, ref_s, axe, FUN_C_TRANSIENT, phi, NULL );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D *Bc2D_setLUAFunctionTransient2 ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , const char* def )
{
   void *L = make_lua_interpreter(def, "2D_TR");

   return Bc2D_setFunction2( MyBC, bctype, ref_s, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction3D, L );
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

Real  Bc2D_evalFunction1  ( const BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , Real x, Real y)
{
   Real val = 0.0;

   if ( MyBC == NULL ) error(E_NULL, "Bc2D_evalFunction1");


   if ( bctype == BC_2De_DIRICHLET )
   {
      val = Bc2D_XTYPE_evalFunction1( MyBC->BC_dirichlet, ref_s, axe, x, y );
   }
   else
   if ( bctype == BC_2De_NEUMANN )
   {
      val = Bc2D_XTYPE_evalFunction1( MyBC->BC_neumann  , ref_s, axe, x, y );
   }
   else
   if ( bctype == BC_2De_CAUCHY )
   {
      val = Bc2D_XTYPE_evalFunction1( MyBC->BC_cauchy   , ref_s, axe, x, y );
   }
   else
   if ( bctype == BC_2De_ROBIN )
   {
      val = Bc2D_XTYPE_evalFunction1( MyBC->BC_robin    , ref_s, axe, x, y );
   }
   else
   {
      error(E_UNKNOWN, "Bc2D_evalFunction1");
   }

   return val;
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

Real  Bc2D_evalFunction2  ( const BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , Real x, Real y)
{
   Real val = 0.0;

   if ( MyBC == NULL ) error(E_NULL, "Bc2D_evalFunction2");


   if ( bctype == BC_2De_ROBIN )
   {
      val = Bc2D_XTYPE_evalFunction2( MyBC->BC_robin    , ref_s, axe, x, y );
   }
   else
   {
      error(E_UNKNOWN, "Bc2D_evalFunction2");
   }

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D *Bc2D_setTps  ( BC_2D* MyBC, Real tps )
{
   if ( MyBC == NULL )   error (E_NULL, "Bc2D_setTps");

   MyBC->BC_dirichlet->tps = tps;
   MyBC->BC_neumann->tps = tps;
   MyBC->BC_cauchy->tps = tps;
   MyBC->BC_robin->tps = tps;

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_2D *Bc2D_setCurrentSelectedAxe ( BC_2D* MyBc, int axe )
{
   if ( MyBc == NULL ) error (E_NULL, "Bc2D_setCurrentSelectedAxe");

   /* check */
   if ( (axe != AXEe_X) && (axe != AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_setCurrentSelectedAxe");

   MyBc->BC_dirichlet->current_selected_axe = axe;
   MyBc->BC_neumann->current_selected_axe = axe;
   MyBc->BC_cauchy->current_selected_axe = axe;
   MyBc->BC_robin->current_selected_axe = axe;

   return MyBc;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int           Bc2D_checkBC               ( const BC_2D* MyBC , int axe)
{
   int ref;
   BC_2D_TYPE type;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_checkBC");

   /* check if we se the functions */
   for (ref=0; ref<NBMAX_BC_2D_FUNCTIONS; ref++)
   {
      type = Bc2D_getBcType_NoWarn( MyBC, axe , ref);

      /* check that the function is set */
      switch(type)
      {
         case BC_2De_UNKNOWN:

            break;

         case BC_2De_DIRICHLET:

            Bc2D_checkBCdirichlet(MyBC, axe, ref);
            break;

         case BC_2De_NEUMANN:

            Bc2D_checkBCneumann(MyBC, axe, ref);
            break;

         case BC_2De_CAUCHY:

            Bc2D_checkBCcauchy(MyBC, axe, ref);
            break;

         case BC_2De_ROBIN:

            Bc2D_checkBCrobin(MyBC, axe, ref);
            break;
      }
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int  Bc2D_hasDirichletBC        ( const BC_2D* MyBC , int axe )
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_hasDirichletBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_hasDirichletBC");

   for (i=0; i<NBMAX_BC_2D_FUNCTIONS; i++)
   {
      if ( BC_2De_DIRICHLET == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int  Bc2D_hasNeumannBC         ( const BC_2D* MyBC , int axe )
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_hasNeumannBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_hasNeumannBC");

   for (i=0; i<NBMAX_BC_2D_FUNCTIONS; i++)
   {
      if ( BC_2De_NEUMANN == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc2D_hasCauchyBC          ( const BC_2D* MyBC , int axe  )
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_hasCauchyBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_hasCauchyBC");

   for (i=0; i<NBMAX_BC_2D_FUNCTIONS; i++)
   {
      if ( BC_2De_CAUCHY == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc2D_hasRobinBC          ( const BC_2D* MyBC , int axe  )
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_hasRobinBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_hasRobinBC");

   for (i=0; i<NBMAX_BC_2D_FUNCTIONS; i++)
   {
      if ( BC_2De_ROBIN == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc2D_getNumberOfBC          ( const BC_2D* MyBC , int axe  )
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_getNumberOfBC");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_getNumberOfBC");

   for (i=0; i<NBMAX_BC_2D_FUNCTIONS; i++)
   {
      if ( BC_2De_UNKNOWN != MyBC->BC_MASK[axe][i] )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc2D_getNumberOfBcOnSomms          ( const BC_2D* MyBC , int axe  )
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_getNumberOfBcOnSomms");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_getNumberOfBcOnSomms");

   for (i=0; i<NBMAX_BC_2D_FUNCTIONS; i++)
   {
      if ( (BC_2De_DIRICHLET == MyBC->BC_MASK[axe][i]) || (BC_2De_CAUCHY == MyBC->BC_MASK[axe][i]) || (BC_2De_ROBIN == MyBC->BC_MASK[axe][i]) )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc2D_getNumberOfBcOnFaces          ( const BC_2D* MyBC , int axe  )
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc2D_getNumberOfBcOnFaces");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y) ) error4(E_BC_WRONGAXENUMBER, "Bc2D_getNumberOfBcOnFaces");

   for (i=0; i<NBMAX_BC_2D_FUNCTIONS; i++)
   {
      if ( (BC_2De_NEUMANN == MyBC->BC_MASK[axe][i])  || (BC_2De_ROBIN == MyBC->BC_MASK[axe][i]) )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc2D_checkBCdirichlet(const BC_2D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_dirichlet->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xy  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyt  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xy_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyt_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_STATIONNARY:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xy_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_TRANSIENT:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xyt_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc2D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc2D_checkBCdirichlet");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc2D_checkBCneumann(const BC_2D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_neumann->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xy  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyt  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xy_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyt_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_STATIONNARY:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xy_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_TRANSIENT:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xyt_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc2D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc2D_checkBCneumann");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc2D_checkBCcauchy(const BC_2D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_cauchy->Fun1[AXEe_X][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xy  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyt  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xy_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyt_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_STATIONNARY:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xy_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_LUA_TRANSIENT:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xyt_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc2D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc2D_checkBCcauchy");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc2D_checkBCrobin(const BC_2D *MyBC, int axe, int ref)
{
   /* the type of function should be the same for the two robin's function on the same reference */
   if ( MyBC->BC_robin->Fun1[axe][ref].type != MyBC->BC_robin->Fun2[axe][ref].type )
   {
      error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");
      return EXIT_FAILURE;
   }

   /* check that the function are set , according to their type */
   switch( MyBC->BC_robin->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xy  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xy  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyt  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyt  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xy_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xy_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_LUA_STATIONNARY:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xy_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xy_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_LUA_TRANSIENT:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xyt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xyt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc2D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc2D_checkBCrobin");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

