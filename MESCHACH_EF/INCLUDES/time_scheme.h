#ifndef TIME_SCHEME__H
#define TIME_SCHEME__H

#include "MESCHACH/INCLUDES/matrix.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define NB_STEPS_MAX 4


typedef enum
{
   TIME_SCHEMEe_TIME_DISCR_EULER       = 0,
   TIME_SCHEMEe_TIME_DISCR_GLOWINSKY   = 1

}  TIME_SCHEMEt_TIME_DISCR;

typedef enum
{
   TIME_SCHEMEe_LINEAR_DISCR_EULER_IMPLICIT    = 0,
   TIME_SCHEMEe_LINEAR_DISCR_EULER_EXPLICIT    = 1,
   TIME_SCHEMEe_LINEAR_DISCR_CRANK_NICHOLSON   = 2   

}  TIME_SCHEMEt_LINEAR_DISCR;

typedef enum
{
   TIME_SCHEMEe_NONLIN_DISCR_EULER_EXPLICIT     = 0,
   TIME_SCHEMEe_NONLIN_DISCR_ADAMS_BASHFORD     = 1,
   TIME_SCHEMEe_NONLIN_DISCR_GLOWINSKY          = 2

}  TIME_SCHEMEt_NONLIN_DISCR;



static Real time_terms_discretisation[3][NB_STEPS_MAX] =
{
   {1.000 , 1.000 , 0.000 , 0.000 },    /* dU/dt -> coeff  for U_np1, U_n, U_nm1, U_nm2     ( "EULER"    )       */
   {1.500 , 2.000 ,-0.500 , 0.000 },    /* dU/dt -> coeffs for U_np1, U_n, U_nm1, U_nm2     ( "GLOWINSKY")       */
   {0.000 , 0.000 , 0.000 , 0.000 }     /* dU/dt -> coeffs for U_np1, U_n, U_nm1, U_nm2     ( "xxxxxxxx")        */
};

static Real linear_terms_discretisation[3][NB_STEPS_MAX]  =
{
   { 1.000 , 0.000 , 0.000 , 0.000 },    /* A.U -> coeff  for U_np1, U_n, U_nm1, U_nm2       ( "EULER_IMPLICIT"  ) */
   { 0.000 , 1.000 , 0.000 , 0.000 },    /* A.U -> coeffs for U_np1, U_n, U_nm1, U_nm2       ( "EULER_EXPLICIT"  ) */
   { 0.500 , 0.500 , 0.000 , 0.000 }     /* A.U -> coeffs for U_np1, U_n, U_nm1, U_nm2       ( "CRANK_NICHOLSON" ) */
};

static Real nonlin_terms_discretisation[3][NB_STEPS_MAX] =
{
   { 0.000 , 1.000 , 0.000 , 0.000 },    /* B(U) -> coeff  for U_np1, U_n, U_nm1, U_nm2       ( "EULER_EXPLICIT" ) */
   { 0.000 , 1.500 ,-0.500 , 0.000 },    /* B(U) -> coeffs for U_np1, U_n, U_nm1, U_nm2       ( "ADAMS_BASHFORD" ) */
   { 0.000 , 1.000 , 0.000 , 0.000 }     /* B(U) -> coeffs for U_np1, U_n, U_nm1, U_nm2       ( "GLOWINSKY")       */
};

static int nonlin_terms_with_comb_linear_combinaison[3] =
{
   0,
   0,
   1
};


static Real nonlin_terms_comb_lin[3][NB_STEPS_MAX][NB_STEPS_MAX] =
{
   {{ 0.000 , 0.000 , 0.000 , 0.000 },   /* "EULER_EXPLICIT" : no linear combinaison of different terms */
    { 0.000 , 0.000 , 0.000 , 0.000 },   /* -not used- */
    { 0.000 , 0.000 , 0.000 , 0.000 },   /* -not used- */
    { 0.000 , 0.000 , 0.000 , 0.000 }},  /* -not used- */ 

   {{ 0.000 , 0.000 , 0.000 , 0.000 },   /* "ADAMS_BASHFORD" : no linear combinaison of different terms */
    { 0.000 , 0.000 , 0.000 , 0.000 },   /* -not used- */
    { 0.000 , 0.000 , 0.000 , 0.000 },   /* -not used- */
    { 0.000 , 0.000 , 0.000 , 0.000 }},  /* -not used- */

   {{ 0.000 , 0.000 , 0.000 , 0.000 },   /* "GLOWINSKY"      : yes, linear combinaison of different terms */
    { 0.000 , 2.000 ,-1.000 , 0.000 },   /* (2.Un - Unm1) -> Un */
    { 0.000 , 0.000 , 0.000 , 0.000 },   /* -not used- */
    { 0.000 , 0.000 , 0.000 , 0.000 }}   /* -not used- */
};



#define MY_LIST8   8

#define TYPE_TSCHEME  0


typedef struct
{
   TIME_SCHEMEt_TIME_DISCR      method1;
   TIME_SCHEMEt_LINEAR_DISCR    method2;
   TIME_SCHEMEt_NONLIN_DISCR    method3;

   int    nb_steps;                        /* nb steps for the time scheme                */

   int    nb_steps_time_discr;             /* nb steps for the time term  U_t             */
   int    nb_steps_linear_discr;           /* nb steps for the linear term (implicit)     */
   int    nb_steps_nonlin_discr;           /* nb steps for the non-linear term (explicit) */

   MAT*   Vn;                              /* tableaux des vecteurs solutions stockes     */

   MAT*   time_terms_discretisation;       /* multi-step method : coeff time term         */
   MAT*   linear_terms_discretisation;     /* multi-step method : coeff linear terms      */
   MAT*   nonlin_terms_discretisation;     /* multi-step method : coeff nonlin terms      */


} TIME_SCHEME ;



#define TMSNULL ((TIME_SCHEME  *)NULL) 


extern  int time_scheme_free(TIME_SCHEME *tms);

#define TIME_SCHEME_FREE(multi) (time_scheme_free(tms), (tms)=(TIME_SCHEME *)NULL)


/* Operations sur les TIME_SCHEME struct */
TIME_SCHEME   *time_scheme_get          (TIME_SCHEMEt_TIME_DISCR met1, TIME_SCHEMEt_LINEAR_DISCR met2, TIME_SCHEMEt_NONLIN_DISCR met3);
VEC           *time_scheme_v_pic        (TIME_SCHEME *TMS, int p, VEC *out);
TIME_SCHEME   *time_scheme_v_put        (TIME_SCHEME *TMS, int p, VEC *in );
TIME_SCHEME   *time_scheme_advance1step (TIME_SCHEME *TMS, VEC *Unp1);


/***********************************************************************/
static char *my_names8[] = { "TIME_SCHEME" };

static int (*my_frees8[])(void*) = { (int(*)(void*)) time_scheme_free };

static MEM_ARRAY my_tnums8[1]; /* initialised to zeros */
/***********************************************************************/


#ifdef __cplusplus
}
#endif


#endif

/* -------------------------------------------------------------------------

                            EXPLANATIONS : 


   # Discretisation of dU/dt :
   ############################################

   EULER IMPLICIT  : dU/dt -> (U_np1 - U_n)/DT
                                    time_terms_discretisation[0]  =  1.0  
                                    time_terms_discretisation[1]  =  1.0

   GLOWINSKY       : dU/dt -> (3/2.U_np1 - 2.U_n + 1/2.U_nm1)/DT
                                    time_terms_discretisation[0]  =  1.5
                                    time_terms_discretisation[1]  =  2.0
                                    time_terms_discretisation[2]  = -0.5



   # Discretisation of the linear terms (e.g. Uxxxx or  Uxx) :
   ###########################################################

   EULER IMPLICIT  : Uxx -> Uxx_np1
                                    linear_terms_discretisations[0]  =  1.0
                                    linear_terms_discretisations[1]  =  0.0

   EULER EXPLICIT  : Uxx -> Uxx_n 
                                    linear_terms_discretisations[0]  =  0.0
                                    linear_terms_discretisations[0]  =  1.0
  
   CRANK-NICHOLSON : Uxx -> (Uxx_np1 + Uxx_n)/2
                                    linear_terms_discretisations[0]  =  0.5
                                    linear_terms_discretisations[1]  =  0.5


   # Discretisation of the non-linear terms (e.g.  U.Ux) :
   ########################################################

   EULER EXPLICIT  : nonlinear_terms_discretisation[1] =  1.0   ->   Un.dUn

   ADAMS-BASHFORD  : nonlinear_terms_discretisation[1] =  1.5
                     nonlinear_terms_discretisation[2] = -0.5   ->  1.5*(Un.dUn) - 0.5*(Unm1.dUnm1)

 
   GLOWINSKY      : nonlinear_terms_discretisation[1] =  1.0  applied on (2.Un - Unm1) -> (2Un-Unm1).d(2Un-Unm1)
       this is the reason of the 4th array "nonlin_terms_comb_lin"
   
----------------------------------------------------------------------- */
