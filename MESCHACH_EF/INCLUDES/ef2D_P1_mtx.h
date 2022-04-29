   
   /* -- Element P1 ------------------------------------- */

   Real VEC_Masse_P1[3] =   {  1.0/6  ,  1.0/6  ,  1.0/6  } ;

   Real VEC_Dx___P1[3]  =   { -3.0/6  ,  0.0    ,  3.0/6  } ;

   Real VEC_Dy___P1[3]  =   { -3.0/6  ,  0.0    ,  3.0/6  } ;

   Real VEC_DxDx_P1[3]  =   {  0.0    ,  0.0    ,  0.0    } ;

   Real VEC_DxDy_P1[3]  =   {  0.0    ,  0.0    ,  0.0    } ;

   Real VEC_DyDy_P1[3]  =   {  0.0    ,  0.0    ,  0.0    } ;




   Real MAT_Masse_P1[3][3] = {

      {  2.0/24 ,  1.0/24 ,  1.0/24 },
      {  1.0/24 ,  2.0/24 ,  1.0/24 },
      {  1.0/24 ,  1.0/24 ,  2.0/24 }} ;



   Real MAT_Rigid_P1[4][3][3] = {

      {{  1.0/2  , -1.0/2  ,  0.0    },     /* dNidx * dNjdx */
       { -1.0/2  ,  1.0/2  ,  0.0    },
       {  0.0    ,  0.0    ,  0.0    }},
 
      {{  1.0/2  ,  0.0    , -1.0/2  },     /* dNidx * dNjdy */
       { -1.0/2  ,  0.0    ,  1.0/2  },
       {  0.0    ,  0.0    ,  0.0    }},
 
      {{  1.0/2  , -1.0/2  ,  0.0    },     /* dNidy * dNjdx */
       {  0.0    ,  0.0    ,  0.0    },
       { -1.0/2  ,  1.0/2  ,  0.0    }},

      {{  1.0/2  ,  0.0    , -1.0/2  },     /* dNidy * dNjdy */
       {  0.0    ,  0.0    ,  0.0    },
       { -1.0/2  ,  0.0    ,  1.0/2  }} } ;


   Real MAT_Conv_x_P1[3][3] = {
 
      { -1.0/6  ,  1.0/6  ,  0.0    },
      { -1.0/6  ,  1.0/6  ,  0.0    },
      { -1.0/6  ,  1.0/6  ,  0.0    }} ;

   Real MAT_Conv_y_P1[3][3] = {
  
      { -1.0/6  ,  0.0    ,  1.0/6 } ,
      { -1.0/6  ,  0.0    ,  1.0/6  },
      { -1.0/6  ,  0.0    ,  1.0/6  }} ;


   Real MAT_Conv_x_P1dP1[3][3] = {

      { -1.0/6  ,  1.0/6  ,  0.0    },
      { -1.0/6  ,  1.0/6  ,  0.0    },
      { -1.0/6  ,  1.0/6  ,  0.0    }} ;

   Real MAT_Conv_y_P1dP1[3][3] = {

      { -1.0/6  ,  0.0    ,  1.0/6  },
      { -1.0/6  ,  0.0    ,  1.0/6  },
      { -1.0/6  ,  0.0    ,  1.0/6  }} ;



   Real TENSOR_Mass_P1[3][3][3] = {

   {{   6.0/120 ,  2.0/120 ,  2.0/120 },     /* N1*Nj*Nk */
    {   2.0/120 ,  2.0/120 ,  1.0/120 },
    {   2.0/120 ,  1.0/120 ,  2.0/120 }},

   {{   2.0/120 ,  2.0/120 ,  1.0/120 },     /* N2*Nj*Nk */
    {   2.0/120 ,  6.0/120 ,  2.0/120 },
    {   1.0/120 ,  2.0/120 ,  2.0/120 }},
   
   {{   2.0/120 ,  1.0/120 ,  2.0/120 },     /* N3*Nj*Nk */
    {   1.0/120 ,  2.0/120 ,  2.0/120 },
    {   2.0/120 ,  2.0/120 ,  6.0/120 }} } ;


   Real TENSOR_ConvX_P1[3][3][3] = {

   {{  -2.0/24 , -1.0/24 , -1.0/24 },     /* N1*dNjdx*Nk */
    {   2.0/24 ,  1.0/24 ,  1.0/24 },
    {   0.0/24 ,  0.0/24 ,  0.0/24 }},
   
   {{  -1.0/24 , -2.0/24 , -1.0/24 },     /* N2*dNjdx*Nk */
    {   1.0/24 ,  2.0/24 ,  1.0/24 },
    {   0.0/24 ,  0.0/24 ,  0.0/24 }},
   
   {{  -1.0/24 , -1.0/24 , -2.0/24 },     /* N3*dNjdx*Nk */
    {   1.0/24 ,  1.0/24 ,  2.0/24 },
    {   0.0/24 ,  0.0/24 ,  0.0/24 }} } ;


   Real TENSOR_ConvY_P1[3][3][3] = {

   {{  -2.0/24 , -1.0/24 , -1.0/24 },     /* N1*dNjdy*Nk */  
    {   0.0/24 ,  0.0/24 ,  0.0/24 },
    {   2.0/24 ,  1.0/24 ,  1.0/24 }},

   {{  -1.0/24 , -2.0/24 , -1.0/24 },     /* N2*dNjdy*Nk */
    {   0.0/24 ,  0.0/24 ,  0.0/24 },
    {   1.0/24 ,  2.0/24 ,  1.0/24 }},

   {{  -1.0/24 , -1.0/24 , -2.0/24 },     /* N3*dNjdy*Nk */
    {   0.0/24 ,  0.0/24 ,  0.0/24 },
    {   1.0/24 ,  1.0/24 ,  2.0/24 }} } ;





   Real TENSOR_DD_DD_P1[9][3][3] = {

      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xx_xx  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }},

      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xx_xy  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }},

      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xx_yy  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }}, 

      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xy_xx  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }},

      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xy_xy  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }},
 
      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xy_yy  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }},

      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_yy_xx  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }},

      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_yy_xy  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }},

      {{   0.0    ,  0.0    ,  0.0    },   /* MAT_yy_yy  */
       {   0.0    ,  0.0    ,  0.0    },
       {   0.0    ,  0.0    ,  0.0    }} } ;

   Real TENSOR_DD_D__P1[6][3][3] = {

     {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xx_x  */
      {   0.0    ,  0.0    ,  0.0    },
      {   0.0    ,  0.0    ,  0.0    }},

     {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xx_y  */
      {   0.0    ,  0.0    ,  0.0    },
      {   0.0    ,  0.0    ,  0.0    }},

     {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xy_x  */
      {   0.0    ,  0.0    ,  0.0    },
      {   0.0    ,  0.0    ,  0.0    }}, 

     {{   0.0    ,  0.0    ,  0.0    },   /* MAT_xy_y  */
      {   0.0    ,  0.0    ,  0.0    },
      {   0.0    ,  0.0    ,  0.0    }},
 
     {{   0.0    ,  0.0    ,  0.0    },   /* MAT_yy_x  */
      {   0.0    ,  0.0    ,  0.0    },
      {   0.0    ,  0.0    ,  0.0    }},

     {{   0.0    ,  0.0    ,  0.0    },   /* MAT_yy_y  */
      {   0.0    ,  0.0    ,  0.0    },
      {   0.0    ,  0.0    ,  0.0    }} } ;

