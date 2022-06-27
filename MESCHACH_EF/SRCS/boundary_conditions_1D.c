/** \file boundary_conditions_1D.c
 *
 * Contains the structure of boundary conditions in 1 dimension
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_1D_XTYPE* Bc1D_XTYPE_get (const char* name);
static int          Bc1D_XTYPE_free(BC_1D_XTYPE *MyBC);

static BC_1D_XTYPE* Bc1D_XTYPE_setFunction1   (BC_1D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
static BC_1D_XTYPE* Bc1D_XTYPE_setFunction2   (BC_1D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);

static Real         Bc1D_XTYPE_evalFunction1  (BC_1D_XTYPE* MyBC, int ref_s, int axe, Real x);
static Real         Bc1D_XTYPE_evalFunction2  (BC_1D_XTYPE* MyBC, int ref_s, int axe, Real x);

static BC_1D_TYPE   Bc1D_getBcType_NoWarn     (const BC_1D* MyBC, int axe, int ref);

static int Bc1D_checkBCdirichlet(const BC_1D *MyBC, int axe, int ref);
static int Bc1D_checkBCneumann  (const BC_1D *MyBC, int axe, int ref);
static int Bc1D_checkBCcauchy   (const BC_1D *MyBC, int axe, int ref);
static int Bc1D_checkBCrobin    (const BC_1D *MyBC, int axe, int ref);

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_1D_XTYPE*  Bc1D_XTYPE_get(const char* name)
{
   int i,j;
   BC_1D_XTYPE *My_BC;

   if ( (My_BC=NEW(BC_1D_XTYPE)) == (BC_1D_XTYPE *)NULL )
   {
      error(E_MEM, "BC1D_XTYPE_get");
   }
   else if (mem_info_is_on())
   {
      /*mem_bytes_list(TYPE_BC_1D_XTYPE, 0, sizeof(BC_1D_XTYPE), MY_LIST4);*/
      /*mem_numvar_list(TYPE_BC_1D_XTYPE, 1, MY_LIST4);*/
   }

   /* on initialise BC_dirichlet, BC_neumann, BCcauchy et BC_robin */
   strncpy(My_BC->BCname, name, 16);
   My_BC->BCname[15] = '\0';

   for (i=0; i<1; i++)
   for (j=0; j<NBMAX_BC_1D_FUNCTIONS; j++)
   {
      Fun1D_init(&(My_BC->Fun1[i][j]));
      Fun1D_init(&(My_BC->Fun2[i][j]));
   }

   return My_BC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc1D_XTYPE_free(BC_1D_XTYPE *MyBC)
{
   if ( MyBC == (BC_1D_XTYPE *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         /*mem_bytes_list(TYPE_BC_1D_XTYPE, sizeof(BC_1D_XTYPE), 0, MY_LIST4);*/
         /*mem_numvar_list(TYPE_BC_1D_XTYPE, -1, MY_LIST4);*/
      }

      /* free the structure BC_1D */
      free(MyBC);

      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_1D_XTYPE* Bc1D_XTYPE_setFunction1      (BC_1D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyBC == NULL )                      error(E_NULL, "Bc1D_XTYPE_setFunction1");

   if ( ref_s > NBMAX_BC_1D_FUNCTIONS )     error4(E_BC_WRONGIDXNUMBER, "Bc1D_XTYPE_setFunction1");
   if ( axe != AXEe_X )                     error4(E_BC_WRONGAXENUMBER, "Bc1D_XTYPE_setFunction1");

   /* set the function */
   Fun1D_setFunction(&(MyBC->Fun1[axe][ref_s]), type, phi, clientdata);

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_1D_XTYPE* Bc1D_XTYPE_setFunction2      (BC_1D_XTYPE* MyBC, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyBC == NULL )                      error(E_NULL, "Bc1D_XTYPE_setFunction2");

   if ( ref_s > NBMAX_BC_1D_FUNCTIONS )     error4(E_BC_WRONGIDXNUMBER, "Bc1D_XTYPE_setFunction2");
   if ( axe != AXEe_X )                     error4(E_BC_WRONGAXENUMBER, "Bc1D_XTYPE_setFunction2");

   /* set the function */
   Fun1D_setFunction(&(MyBC->Fun2[axe][ref_s]), type, phi, clientdata);

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real          Bc1D_XTYPE_evalFunction1        (BC_1D_XTYPE* MyBC, int ref_s, int axe, Real x)
{
   Real val = 0;

   if ( MyBC == NULL )                      error(E_NULL, "Bc1D_XTYPE_evalFunction1");

   /* check */
   if ( ref_s > NBMAX_BC_1D_FUNCTIONS )     error4(E_BC_WRONGIDXNUMBER, "Bc1D_XTYPE_evalFunction1");
   if ( axe != AXEe_X )                     error4(E_BC_WRONGAXENUMBER, "Bc1D_XTYPE_evalFunction1");

   val = MyBC->Fun1[axe][ref_s].eval(&MyBC->Fun1[axe][ref_s], x, MyBC->tps);

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real          Bc1D_XTYPE_evalFunction2        (BC_1D_XTYPE* MyBC, int ref_s, int axe , Real x)
{
   Real val = 0;

   if ( MyBC == NULL )                      error(E_NULL, "Bc1D_XTYPE_evalFunction2");

   /* check */
   if ( ref_s > NBMAX_BC_1D_FUNCTIONS )     error4(E_BC_WRONGIDXNUMBER, "Bc1D_XTYPE_evalFunction2");
   if ( axe != AXEe_X )                     error4(E_BC_WRONGAXENUMBER, "Bc1D_XTYPE_evalFunction2");

   val = MyBC->Fun2[axe][ref_s].eval(&MyBC->Fun2[axe][ref_s], x, MyBC->tps);

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D*  Bc1D_get(void)
{
   BC_1D *My_BC;
   int i;
   
   if ( (My_BC=NEW(BC_1D)) == (BC_1D *)NULL )
   {
      error(E_MEM, "BC_1D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_BC_1D, 0, sizeof(BC_1D), MY_LIST4);
      mem_numvar_list(TYPE_BC_1D, 1, MY_LIST4);
   }
   
   /* on initialise BC_dirichlet, BC_neumann, BCcauchy et BC_robin */
   /* ie set the pointer to the right functions          */
   My_BC->BC_dirichlet = Bc1D_XTYPE_get("dirichlet");
   My_BC->BC_neumann   = Bc1D_XTYPE_get("neumann");
   My_BC->BC_cauchy    = Bc1D_XTYPE_get("cauchy");  
   My_BC->BC_robin     = Bc1D_XTYPE_get("robin");
   
   for (i=0; i<NBMAX_BC_1D_FUNCTIONS; i++)
   {
      My_BC->BC_MASK[AXEe_X][i] = BC_1De_UNKNOWN;
   }

   return My_BC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setup_from_params(const PARAMS *MyParams)
{
	BC_1D* MyBC = Bc1D_get();

	int i;
	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		Bc1D_setBcType(MyBC, MyParams->bc_params.TabBCMask[AXEe_X][i], /*ref_s*/i, AXEe_X );
	}
		
	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		Bc1D_setLUAFunction(MyBC, BC_1De_DIRICHLET, /*ref_s*/i, AXEe_X, MyParams->bc_params.TabPhi_BCdirichlet[AXEe_X][i].fundef);
	}
	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		Bc1D_setLUAFunction(MyBC, BC_1De_NEUMANN, /*ref_s*/i, AXEe_X, MyParams->bc_params.TabPhi_BCneumann[AXEe_X][i].fundef);
	}
	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		Bc1D_setLUAFunction(MyBC, BC_1De_CAUCHY, /*ref_s*/i, AXEe_X, MyParams->bc_params.TabPhi_BCcauchy[AXEe_X][i].fundef);
	}
	for (i=1; i<=MyParams->bc_params.nb_BC; i++)
	{
		Bc1D_setLUAFunction (MyBC, BC_1De_ROBIN, /*ref_s*/i, AXEe_X, MyParams->bc_params.TabPhi_BCrobin1[AXEe_X][i].fundef);
		Bc1D_setLUAFunction2(MyBC, BC_1De_ROBIN, /*ref_s*/i, AXEe_X, MyParams->bc_params.TabPhi_BCrobin2[AXEe_X][i].fundef);
	}
      
   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Bc1D_free(BC_1D *MyBC)
{
   if ( MyBC == (BC_1D *)NULL )
   {
        return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_BC_1D, sizeof(BC_1D), 0, MY_LIST4);
         mem_numvar_list(TYPE_BC_1D, -1, MY_LIST4);
      }
      
      /* free the structure BC_1D */
      BC_1D_XTYPE_FREE(MyBC->BC_dirichlet);
      BC_1D_XTYPE_FREE(MyBC->BC_neumann  );      
      BC_1D_XTYPE_FREE(MyBC->BC_cauchy   );
      BC_1D_XTYPE_FREE(MyBC->BC_robin    );
      
      free(MyBC);
      
      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D        *Bc1D_setBcType            (BC_1D* MyBC, BC_1D_TYPE type, int ref, int axe)
{
   if ( MyBC == NULL )                    error(E_NULL, "Bc1D_setBcType");

   if ( ref > NBMAX_BC_1D_FUNCTIONS )     error4(E_BC_WRONGIDXNUMBER, "Bc1D_setBcType");
   if ( axe != AXEe_X )                   error4(E_BC_WRONGAXENUMBER, "Bc1D_setBcType");

   if (    (type != BC_1De_DIRICHLET) && (type != BC_1De_NEUMANN)
        && (type != BC_1De_CAUCHY)    && (type != BC_1De_ROBIN) ) error4(E_BC_WRONGBCTYPE, "Bc1D_setBcType");
   
   MyBC->BC_MASK[axe][ref] = type;
   
   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D_TYPE    Bc1D_getBcType            (const BC_1D* MyBC, int axe, int ref)
{
   if ( MyBC == NULL )                    error(E_NULL, "Bc1D_getBcType");

   if ( ref > NBMAX_BC_1D_FUNCTIONS )     error4(E_BC_WRONGIDXNUMBER, "Bc1D_getBcType");
   if ( axe != AXEe_X )                   error4(E_BC_WRONGAXENUMBER, "Bc1D_getBcType");

   if ( BC_1De_UNKNOWN == MyBC->BC_MASK[axe][ref] ) warning4(W_BC_TYPENOTSET, "Bc1D_getBcType");

   return MyBC->BC_MASK[axe][ref];
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static BC_1D_TYPE    Bc1D_getBcType_NoWarn      (const BC_1D* MyBC, int axe, int ref)
{
   if ( MyBC == NULL )                    error(E_NULL, "Bc1D_getBcType_NoWarn");

   if ( ref > NBMAX_BC_1D_FUNCTIONS )     error4(E_BC_WRONGIDXNUMBER, "Bc1D_getBcType_NoWarn");
   if ( axe != AXEe_X )                   error4(E_BC_WRONGAXENUMBER, "Bc1D_getBcType_NoWarn");

   return MyBC->BC_MASK[axe][ref];
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setFunction1      (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_setFunction1");

   
   if ( bctype == BC_1De_DIRICHLET )
   {
      Bc1D_XTYPE_setFunction1( MyBC->BC_dirichlet, ref_s, axe, type, phi, clientdata);
   }
   else
   if ( bctype == BC_1De_NEUMANN ) 
   {
      Bc1D_XTYPE_setFunction1( MyBC->BC_neumann  , ref_s, axe, type, phi, clientdata);
   }
   else
   if ( bctype == BC_1De_CAUCHY )
   {
      Bc1D_XTYPE_setFunction1( MyBC->BC_cauchy   , ref_s, axe, type, phi, clientdata);
   }
   else
   if ( bctype == BC_1De_ROBIN )
   {
      Bc1D_XTYPE_setFunction1( MyBC->BC_robin    , ref_s, axe, type, phi, clientdata);
   }
   else
   {
      error(E_UNKNOWN, "Bc1D_setFunction1");
   }
   
   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setFunction2      ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_setFunction2");


   if ( bctype == BC_1De_DIRICHLET )
   {
      warning(W_BC_SECONDFUNCDIRICHLET, "Bc1D_setFunction2");
   }
   else
   if ( bctype == BC_1De_NEUMANN )
   {
      warning(W_BC_SECONDFUNCNEUMANN, "Bc1D_setFunction2");
   }
   else
   if ( bctype == BC_1De_CAUCHY )
   {
      warning(W_BC_SECONDFUNCCAUCHY, "Bc1D_setFunction2");
   }
   else
   if ( bctype == BC_1De_ROBIN )
   {
      Bc1D_XTYPE_setFunction2( MyBC->BC_robin    , ref_s, axe, type, phi, clientdata);
   }
   else
   {
      error(E_UNKNOWN, "Bc1D_setFunction2");
   }

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setCFunction      ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , FUNC_1D phi )
{
   return Bc1D_setFunction1( MyBC, bctype, ref_s, axe, FUN_C_STATIONNARY, phi, NULL );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setLUAFunction    ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , const char* def )
{
   void *L = make_lua_interpreter(def, "1D");
   
   return Bc1D_setFunction1( MyBC, bctype, ref_s, axe, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction1D, L );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setCFunction2      ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , FUNC_1D phi )
{
   return Bc1D_setFunction2( MyBC, bctype, ref_s, axe, FUN_C_STATIONNARY, phi, NULL );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setLUAFunction2    ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , const char* def )
{
   void *L = make_lua_interpreter(def, "1D");
   
   return Bc1D_setFunction2( MyBC, bctype, ref_s, axe, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction1D, L );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setCFunctionTransient ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , FUNC_2D phi )
{
   return Bc1D_setFunction1( MyBC, bctype, ref_s, axe, FUN_C_TRANSIENT, phi, NULL );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setLUAFunctionTransient ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe  , const char* def )
{
   void *L = make_lua_interpreter(def, "1D_TR");
   
   return Bc1D_setFunction1( MyBC, bctype, ref_s, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction2D, L );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setCFunctionTransient2 ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , FUNC_2D phi )
{
   return Bc1D_setFunction2( MyBC, bctype, ref_s, axe, FUN_C_TRANSIENT, phi, NULL );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setLUAFunctionTransient2 ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , const char* def )
{
   void *L = make_lua_interpreter(def, "1D_TR");
   
   return Bc1D_setFunction2( MyBC, bctype, ref_s, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction2D, L );
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

Real  Bc1D_evalFunction1  ( const BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , Real x)
{
   Real val = 0.0;
   
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_evalFunction1");

   
   if ( bctype == BC_1De_DIRICHLET )
   {
      val = Bc1D_XTYPE_evalFunction1( MyBC->BC_dirichlet, ref_s, axe, x );
   }
   else
   if ( bctype == BC_1De_NEUMANN ) 
   {
      val = Bc1D_XTYPE_evalFunction1( MyBC->BC_neumann  , ref_s, axe, x );
   }
   else
   if ( bctype == BC_1De_CAUCHY )
   {
      val = Bc1D_XTYPE_evalFunction1( MyBC->BC_cauchy   , ref_s, axe, x );
   }
   else
   if ( bctype == BC_1De_ROBIN )
   {
      val = Bc1D_XTYPE_evalFunction1( MyBC->BC_robin    , ref_s, axe, x );
   }
   else
   {
      error(E_UNKNOWN, "Bc1D_evalFunction1");
   }
   
   return val;
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

Real  Bc1D_evalFunction2  ( const BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , Real x)
{
   Real val = 0.0;

   if ( MyBC == NULL ) error(E_NULL, "Bc1D_evalFunction2");


   if ( bctype == BC_1De_ROBIN )
   {
      val = Bc1D_XTYPE_evalFunction2( MyBC->BC_robin    , ref_s, axe, x );
   }
   else
   {
      error(E_UNKNOWN, "Bc1D_evalFunction2");
   }

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

BC_1D *Bc1D_setTps ( BC_1D* MyBC, Real tps )
{
   if ( MyBC == NULL ) error (E_NULL, "Bc1D_setTps");
 
   MyBC->BC_dirichlet->tps = tps;
   MyBC->BC_neumann->tps = tps;
   MyBC->BC_cauchy->tps = tps;
   MyBC->BC_robin->tps = tps;

   return MyBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int           Bc1D_checkBC               ( const BC_1D* MyBC )
{
   int ref;
   BC_1D_TYPE type;
   
   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_checkBC");

   /* check if we se the functions */
   for (ref=0; ref<NBMAX_BC_1D_FUNCTIONS; ref++)
   {
      type = Bc1D_getBcType_NoWarn( MyBC, AXEe_X , ref);

      /* check that the function is set */
      switch(type)
      {
         case BC_1De_UNKNOWN:

            break;

         case BC_1De_DIRICHLET:

            Bc1D_checkBCdirichlet(MyBC, AXEe_X, ref);
				break;  

         case BC_1De_NEUMANN:

            Bc1D_checkBCneumann(MyBC, AXEe_X, ref);
				break;

         case BC_1De_CAUCHY:

            Bc1D_checkBCcauchy(MyBC, AXEe_X, ref);
				break;

         case BC_1De_ROBIN:

            Bc1D_checkBCrobin(MyBC, AXEe_X, ref);
				break;            
      }
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int  Bc1D_hasDirichletBC        ( const BC_1D* MyBC , int axe)
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_hasDirichletBC");
   if ( axe  != AXEe_X ) error4(E_BC_WRONGAXENUMBER, "Bc1D_hasDirichletBC");
   
   for (i=0; i<NBMAX_BC_1D_FUNCTIONS; i++)
   {
      if ( BC_1De_DIRICHLET == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int  Bc1D_hasNeumannBC         ( const BC_1D* MyBC , int axe)
{
   int i;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_hasNeumannBC");
   if ( axe  != AXEe_X ) error4(E_BC_WRONGAXENUMBER, "Bc1D_hasNeumannBC");
   
   for (i=0; i<NBMAX_BC_1D_FUNCTIONS; i++)
   {
      if ( BC_1De_NEUMANN == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc1D_hasCauchyBC          ( const BC_1D* MyBC, int axe )
{
   int i;

   /* check arg */
   if ( MyBC == NULL   ) error(E_NULL, "Bc1D_hasCauchyBC");
   if ( axe  != AXEe_X ) error4(E_BC_WRONGAXENUMBER, "Bc1D_hasCauchyBC");
   
   for (i=0; i<NBMAX_BC_1D_FUNCTIONS; i++)
   {
      if ( BC_1De_CAUCHY == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc1D_hasRobinBC          ( const BC_1D* MyBC, int axe )
{
   int i;

   /* check arg */
   if ( MyBC == NULL   ) error(E_NULL, "Bc1D_hasRobinBC");
   if ( axe  != AXEe_X ) error4(E_BC_WRONGAXENUMBER, "Bc1D_hasRobinBC");

   for (i=0; i<NBMAX_BC_1D_FUNCTIONS; i++)
   {
      if ( BC_1De_ROBIN == MyBC->BC_MASK[axe][i] )
      {
         return 1;
      }
   }

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc1D_getNumberOfBC          ( const BC_1D* MyBC , int axe  )
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_getNumberOfBC");
   if ( (axe != AXEe_X) ) error4(E_BC_WRONGAXENUMBER, "Bc1D_getNumberOfBC");

   for (i=0; i<NBMAX_BC_1D_FUNCTIONS; i++)
   {
      if ( BC_1De_UNKNOWN != MyBC->BC_MASK[axe][i] )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc1D_getNumberOfBcOnSomms          ( const BC_1D* MyBC , int axe  )
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_getNumberOfBcOnSomms");
   if ( (axe != AXEe_X) ) error4(E_BC_WRONGAXENUMBER, "Bc1D_getNumberOfBcOnSomms");

   for (i=0; i<NBMAX_BC_1D_FUNCTIONS; i++)
   {
      if ( (BC_1De_DIRICHLET == MyBC->BC_MASK[axe][i]) || (BC_1De_CAUCHY == MyBC->BC_MASK[axe][i]) || (BC_1De_ROBIN == MyBC->BC_MASK[axe][i]) )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int   Bc1D_getNumberOfBcOnFaces          ( const BC_1D* MyBC , int axe  )
{
   int i;
   int num=0;

   /* check arg */
   if ( MyBC == NULL ) error(E_NULL, "Bc1D_getNumberOfBcOnFaces");
   if ( (axe != AXEe_X) ) error4(E_BC_WRONGAXENUMBER, "Bc1D_getNumberOfBcOnFaces");

   for (i=0; i<NBMAX_BC_1D_FUNCTIONS; i++)
   {
      if ( (BC_1De_NEUMANN == MyBC->BC_MASK[axe][i]) || (BC_1De_ROBIN == MyBC->BC_MASK[axe][i]) )
      {
         num = num + 1;
      }
   }

   return num;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc1D_checkBCdirichlet(const BC_1D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_dirichlet->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_x  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xt  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_x_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xt_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;
         
      case FUN_LUA_STATIONNARY:
         
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_x_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;
         
      case FUN_LUA_TRANSIENT:
         
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].phi_xt_v  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         if ( MyBC->BC_dirichlet->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_DIRICHLETFUNNOTSET, "Bc1D_checkBCdirichlet");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc1D_checkBCdirichlet");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc1D_checkBCneumann(const BC_1D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_neumann->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_x  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xt  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_x_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xt_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         break;
         
      case FUN_LUA_STATIONNARY:
         
         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_x_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         break;
         
      case FUN_LUA_TRANSIENT:
         
         if ( MyBC->BC_neumann->Fun1[axe][ref].phi_xt_v  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         if ( MyBC->BC_neumann->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_NEUMANNFUNNOTSET, "Bc1D_checkBCneumann");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc1D_checkBCneumann");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc1D_checkBCcauchy(const BC_1D *MyBC, int axe, int ref)
{
   /* check that the function are set , according to their type */
   switch( MyBC->BC_cauchy->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_x  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xt  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_x_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xt_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         break;
         
      case FUN_LUA_STATIONNARY:
         
         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_x_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         break;
         
      case FUN_LUA_TRANSIENT:
         
         if ( MyBC->BC_cauchy->Fun1[axe][ref].phi_xt_v  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         if ( MyBC->BC_cauchy->Fun1[axe][ref].clientdata  == NULL )
         {   error4(E_BC_CAUCHYFUNNOTSET, "Bc1D_checkBCcauchy");  return EXIT_FAILURE;}
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc1D_checkBCcauchy");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Bc1D_checkBCrobin(const BC_1D *MyBC, int axe, int ref)
{
   /* the type of function should be the same for the two robin's function on the same reference */
   if ( MyBC->BC_robin->Fun1[axe][ref].type != MyBC->BC_robin->Fun2[axe][ref].type )
   {
      error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");
      return EXIT_FAILURE;
   }

   /* check that the function are set , according to their type */
   switch( MyBC->BC_robin->Fun1[axe][ref].type )
   {
      case FUN_C_STATIONNARY:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_x  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_x  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_C_TRANSIENT:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xt  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xt  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_PY_STATIONNARY:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_x_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_x_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}

         break;

      case FUN_PY_TRANSIENT:

         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}

         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}

         break;
         
      case FUN_LUA_STATIONNARY:
         
         if ( MyBC->BC_robin->Fun1[axe][ref].phi_x_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}
         
         if ( MyBC->BC_robin->Fun2[axe][ref].phi_x_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}
         
         break;
         
      case FUN_LUA_TRANSIENT:
         
         if ( MyBC->BC_robin->Fun1[axe][ref].phi_xt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun1[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}
         
         if ( MyBC->BC_robin->Fun2[axe][ref].phi_xt_v  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}
         if ( MyBC->BC_robin->Fun2[axe][ref].clientdata  == NULL )
         {  error4(E_BC_ROBINFUNNOTSET, "Bc1D_checkBCrobin");  return EXIT_FAILURE;}
         
         break;

      default:

         warning4(W_BC_MASKSETBUTNOFUNCTION, "Bc1D_checkBCrobin");
   }

   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

   
   

