
/*
 * easy administration for the structures "functions"
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"   /* for axes enumerations */
#include "MESCHACH_EF/INCLUDES/functions_structs.h"


/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_3D *Fun3D_get(void)
{
   FUN_3D *Fun;
   
   if ( (Fun=NEW(FUN_3D)) == (FUN_3D *)NULL )
   {
      error(E_MEM, "Fun3D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_FUN_3D, 0, sizeof(FUN_3D), MY_LIST5);
      mem_numvar_list(TYPE_FUN_3D, 1, MY_LIST5);
   }
   
   Fun3D_init(Fun);
      
   return  Fun;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int Fun3D_free(FUN_3D *Fun)
{
   
   if ( Fun == FUN_3D_NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_FUN_3D, sizeof(FUN_3D), 0, MY_LIST5);
         mem_numvar_list(TYPE_FUN_3D, -1, MY_LIST5);
      }
      
      free(Fun);
      
      return EXIT_SUCCESS;
   }
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_3D *Fun3D_init ( FUN_3D *MyFun)
{
   if ( MyFun == NULL ) error(E_NULL, "Fun3D_init");

   MyFun->clientdata     = NULL;

   MyFun->phi_xyz        = NULL;
   MyFun->phi_xyzt       = NULL;
   MyFun->phi_xyz_v      = NULL;
   MyFun->phi_xyzt_v     = NULL;

   MyFun->type           = FUN_UNDEFINED;

   MyFun->eval           = NULL;

   return MyFun;
}

/* ---------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------- */

FUN_3D * Fun3D_setFunction( FUN_3D* MyFun, FUN_TYPE type, void* phi , void* clientdata)
{
   if ( MyFun == NULL ) error(E_NULL, "Fun3D_setFunction");

   /* set type */
   MyFun->type = type;

   /* assign members */
   switch(type)
   {
      case FUN_C_STATIONNARY:  MyFun->phi_xyz    = (FUNC_3D)phi;
                               MyFun->eval       = (FUN_3D_EVAL__FUN_3D_VAARGS)Fun3D_evalCFunction;
                               break;
      case FUN_C_TRANSIENT:    MyFun->phi_xyzt   = (FUNC_4D)phi;
                               MyFun->eval       = (FUN_3D_EVAL__FUN_3D_VAARGS)Fun3D_evalCFunctionTransient;
                               break;
      case FUN_PY_STATIONNARY: MyFun->phi_xyz_v  = (FUNC_3D_PLUS_VOID)phi;
                               MyFun->clientdata = clientdata;
                               MyFun->eval       = (FUN_3D_EVAL__FUN_3D_VAARGS)Fun3D_evalPyFunction;
                               break;
      case FUN_PY_TRANSIENT:   MyFun->phi_xyzt_v = (FUNC_4D_PLUS_VOID)phi;
                               MyFun->clientdata = clientdata;
                               MyFun->eval       = (FUN_3D_EVAL__FUN_3D_VAARGS)Fun3D_evalPyFunctionTransient;
                               break;

      default: error5(E_FUN_WRONGTYPE, "Fun3D_setFunction");
   }

   return  MyFun;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_3D * Fun3D_setCFunction(FUN_3D* MyFun, FUNC_3D phi)
{
   return Fun3D_setFunction(MyFun, FUN_C_STATIONNARY, phi, NULL);
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_3D * Fun3D_setCFunctionTransient( FUN_3D* MyFun, FUNC_4D phi)
{
   return Fun3D_setFunction(MyFun, FUN_C_TRANSIENT, phi, NULL);
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun3D_evalCFunction     ( const FUN_3D* MyFun, Real x, Real y, Real z)
{
   Real res = 0.0;
   
   if ( MyFun          == NULL )  error(E_NULL, "Fun3D_evalCFunction");
   if ( MyFun->phi_xyz == NULL )  error(E_NULL, "Fun3D_evalCFunction");

   /* eval function */
   res = MyFun->phi_xyz(x, y, z);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun3D_evalPyFunction     (const FUN_3D* MyFun, Real x, Real y, Real z)
{
   Real res = 0.0;

   if ( MyFun              == NULL )  error(E_NULL, "Fun3D_evalPyFunction");
   if ( MyFun->phi_xyz_v   == NULL )  error(E_NULL, "Fun3D_evalPyFunction");
   if ( MyFun->clientdata  == NULL )  error(E_NULL, "Fun3D_evalPyFunction");
   
   /* eval function */
   res = MyFun->phi_xyz_v(x, y, z, MyFun->clientdata);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun3D_evalCFunctionTransient     (const FUN_3D* MyFun, Real x, Real y, Real z, Real tps)
{
   Real res = 0.0;
   
   if ( MyFun           == NULL )  error(E_NULL, "Fun3D_evalCFunctionTransient");
   if ( MyFun->phi_xyzt == NULL )  error(E_NULL, "Fun3D_evalCFunctionTransient");

   /* eval function */
   res = MyFun->phi_xyzt(x, y, z, tps);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun3D_evalPyFunctionTransient     (const FUN_3D* MyFun, Real x, Real y, Real z, Real tps)
{
   Real res = 0.0;

   if ( MyFun             == NULL )  error(E_NULL, "Fun3D_evalPyFunctionTransient");
   if ( MyFun->phi_xyzt_v == NULL )  error(E_NULL, "Fun3D_evalPyFunctionTransient");
   if ( MyFun->clientdata == NULL )  error(E_NULL, "Fun3D_evalPyFunctionTransient");
   
   /* eval function */
   res = MyFun->phi_xyzt_v(x, y, z, tps, MyFun->clientdata);

   return res;
}
