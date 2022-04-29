
   /* -- Element P1 -------------------------------------- */
   
   Real MAT_Masse_P1[2][2] =  {
                               /*                                |  2   1 | */
      {  2.0/6 ,  1.0/6 },     /* Matrice de masse   : M = jac/6 |        | */
      {  1.0/6 ,  2.0/6 }};    /*                                |  1   2 | */
                               /* he = longueur de l'element considere.     */

   Real MAT_Convection_P1[2][2] =  {
                               /*                            | -1/2  +1/2 | */
      { -1.0/2 ,  1.0/2 },     /* Matrice de convection  C = |            | */
      { -1.0/2 ,  1.0/2 }} ;   /*                            | -1/2  +1/2 | */
                               /* (Ni*dNj)                                  */

   Real MAT_Rigid1_P1[2][2] = {
                               /*                                |  1  -1 | */
      {  1.0   , -1.0   },     /* Matrice de rigidite: R = 1/jac |        | */
      { -1.0   ,  1.0   }};    /*                                | -1   1 | */
                               /* he = longueur de l'element considere.     */
 
   Real MAT_Rigid2_P1[2][2] = {

      {  0.0   ,  0.0   },      /* Les derivees secondes sont nulles         */
      {  0.0   ,  0.0   }} ;    /* (ddNi*ddNj                                */


   Real MAT_Stab_I_xx_P1[2][2] = {

      {  0.0   ,  0.0   },      /* Les derivees secondes sont nulles         */
      {  0.0   ,  0.0   }} ;    /* (Ni*ddNj)                                 */
  
   Real MAT_Stab_x_xx_P1[2][2] = {

      {  0.0   ,  0.0   },      /* Les derivees secondes sont nulles         */
      {  0.0   ,  0.0   }} ;    /* (dNi*ddNj)                                */
    
    
    
    Real VEC_I__P1[2] =

      { 1.0/2 ,  1.0/2 } ;    /* Second Membre B = jac | +1/2  +1/2 |      */


    Real VEC_x__P1[2] =

      { -1.0  ,  1.0  } ;    

   
    Real VEC_xx_P1[2] =

      {  0.0  ,  0.0 } ;   



   Real TENSOR_Convection_P1[2][2][2] = {

     {{ -2.0/6 , -1.0/6 },      /* N1*dNj*Nk                                 */
      {  2.0/6 ,  1.0/6 }},

     {{ -1.0/6 , -2.0/6 },      /* N2*dNj*Nk                                 */
      {  1.0/6 ,  2.0/6 }} } ;  


   Real TENSOR_Masse_P1[2][2][2] = {

     {{  6.0/24 ,  2.0/24 },      /* N1*Nj*Nk                                  */
      {  2.0/24 ,  2.0/24 }},

     {{  2.0/24 ,  2.0/24 },      /* N2*Nj*Nk                                  */
      {  2.0/24 ,  6.0/24 }} } ;

