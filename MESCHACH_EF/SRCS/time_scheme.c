

#include "MESCHACH_EF/INCLUDES/time_scheme.h"

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

TIME_SCHEME *time_scheme_get(TIME_SCHEMEt_TIME_DISCR method1, TIME_SCHEMEt_LINEAR_DISCR method2, TIME_SCHEMEt_NONLIN_DISCR method3)
{
   TIME_SCHEME *tmschm;

   int nb_steps;
   int i,j;

   if ( (tmschm=NEW(TIME_SCHEME)) == (TIME_SCHEME *)NULL )
   {
      error(E_MEM, "time_scheme_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_TSCHEME, 0, sizeof(TIME_SCHEME) ,MY_LIST8);
      mem_numvar_list(TYPE_TSCHEME, 1, MY_LIST8);
   }


   tmschm->method1 = method1;
   tmschm->method2 = method2;
   tmschm->method3 = method3;


   switch(method1)
   {
      case TIME_SCHEMEe_TIME_DISCR_EULER:     tmschm->nb_steps_time_discr = 1; break;
      case TIME_SCHEMEe_TIME_DISCR_GLOWINSKY: tmschm->nb_steps_time_discr = 2; break;

      default: error(E_UNKNOWN, "time_scheme_get");
   }

   switch(method2)
   {
      case TIME_SCHEMEe_LINEAR_DISCR_EULER_IMPLICIT:  tmschm->nb_steps_linear_discr = 0; break;
      case TIME_SCHEMEe_LINEAR_DISCR_EULER_EXPLICIT:  tmschm->nb_steps_linear_discr = 1; break;
      case TIME_SCHEMEe_LINEAR_DISCR_CRANK_NICHOLSON: tmschm->nb_steps_linear_discr = 1; break;

      default: error(E_UNKNOWN, "time_scheme_get");
   }

   switch(method3)
   {
      case TIME_SCHEMEe_NONLIN_DISCR_EULER_EXPLICIT:  tmschm->nb_steps_linear_discr = 1; break;
      case TIME_SCHEMEe_NONLIN_DISCR_ADAMS_BASHFORD:  tmschm->nb_steps_linear_discr = 2; break;
      case TIME_SCHEMEe_NONLIN_DISCR_GLOWINSKY:       tmschm->nb_steps_linear_discr = 2; break;

      default: error(E_UNKNOWN, "time_scheme_get");
   }


   /* get the max. number of steps o the method */
   if ( tmschm->nb_steps_time_discr > tmschm->nb_steps_linear_discr )
   {
      if ( tmschm->nb_steps_time_discr > tmschm->nb_steps_nonlin_discr )
      {
         nb_steps = tmschm->nb_steps_time_discr;
      }
      else
      {
         nb_steps = tmschm->nb_steps_nonlin_discr;
      }
   }
   else
   {
      if ( tmschm->nb_steps_linear_discr > tmschm->nb_steps_nonlin_discr )
      {
         nb_steps = tmschm->nb_steps_linear_discr;
      }
      else
      {
         nb_steps = tmschm->nb_steps_nonlin_discr;
      }
   }

   /* finally */
   tmschm->nb_steps  = nb_steps;


   /* -------  SEE MULTISTEP_METHOD.h  TO UNDERSTAND  ------------ */
   tmschm->time_terms_discretisation    = m_get(3,NB_STEPS_MAX);
   tmschm->linear_terms_discretisation  = m_get(3,NB_STEPS_MAX);
   tmschm->nonlin_terms_discretisation  = m_get(3,NB_STEPS_MAX);


   for (i=0; i<3; i++)
   for (j=0; j<NB_STEPS_MAX; j++)
   {
      tmschm->time_terms_discretisation->me[i][j]    = time_terms_discretisation[i][j];
      tmschm->linear_terms_discretisation->me[i][j]  = linear_terms_discretisation[i][j];
      tmschm->nonlin_terms_discretisation->me[i][j]  = nonlin_terms_discretisation[i][j];
   }

      /* not yet allocated : array od solution at different times */
   tmschm->Vn = MNULL;

   return  tmschm;
}

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

int time_scheme_free(TIME_SCHEME *tmschm)
{
  if ( tmschm == TMSNULL )
  {
     return EXIT_FAILURE;
  }
  else
  {
    M_FREE(tmschm->Vn      );
    M_FREE(tmschm->time_terms_discretisation);
    M_FREE(tmschm->linear_terms_discretisation );
    M_FREE(tmschm->nonlin_terms_discretisation);

    if (mem_info_is_on())
    {
      mem_bytes_list(TYPE_TSCHEME, sizeof(TIME_SCHEME), 0, MY_LIST8);
      mem_numvar_list(TYPE_TSCHEME, -1, MY_LIST8);
    }

    free(tmschm);

    return EXIT_SUCCESS;
  }
}

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

VEC *time_scheme_v_pic(TIME_SCHEME *tmschm, int p, VEC *out)
{
   if ( tmschm     == NULL ) error(E_NULL, "time_scheme_v_pic");
   if ( tmschm->Vn == NULL ) error(E_NULL, "time_scheme_v_pic");

   get_row(tmschm->Vn, p, out);

   return out;
}

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

TIME_SCHEME *time_scheme_v_put(TIME_SCHEME *tmschm, int p, VEC *in)
{
   if ( tmschm     == NULL ) error(E_NULL, "time_scheme_v_put");

   if ( tmschm->Vn == MNULL )
   {
      tmschm->Vn = m_get(NB_STEPS_MAX, in->dim );
   }

   MEM_COPY(in->ve, &(tmschm->Vn->me[p][0]),  (in->dim)*sizeof(Real)); /* set_row(tmschm->Vn, p, in); */

   return tmschm;
}

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

TIME_SCHEME *time_scheme_advance1step( TIME_SCHEME *tmschm , VEC *Unp1 )
{
   int q;

   if ( tmschm     == NULL ) error(E_NULL, "time_scheme_advance1step");

   if ( tmschm->Vn == MNULL )
   {
      tmschm->Vn = m_get(NB_STEPS_MAX, Unp1->dim );
   }

   /* Put U_{n-q} in U_{n-(q+1)} */
   for (q=tmschm->nb_steps-1; q>0; q--)
   {
      MEM_COPY(&(tmschm->Vn->me[q-1][0]), &(tmschm->Vn->me[q][0]),  (Unp1->dim)*sizeof(Real));
   }  /* set_row(TMS->SOL,q,get_row(TMS->SOL,q-1,VECTOR)); */

   /* And now Unp1 at the first place in TAB->Vn */
   MEM_COPY( Unp1->ve, &(tmschm->Vn->me[0][0]), (Unp1->dim)*sizeof(Real));   /* set_row(tmschm->SOL,0,Unp1); */

   /* finito */
   return tmschm;
}

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

