//
// Created by xavier on 27.05.2022.  twenty years later...
//
// Not yet in use !! just for fun...

// B1(t) =    (1-t)**4
// B2(t) = -4*(1-t)**4 +  4*(1-t)**3 + 6*(1-t)**2 + 4*(1-t)**1 + 1
// B3(t) =  6*t**4 - 12*t**3 - 6*t**2 + 12*t**1 + 11
// B4(t) = -4*t**4 +  4*t**3 + 6*t**2 +  4*t**1 + 1
// B5(t) =    t**4

/* -- Element S4 -------------------------------------- */

Real MAT_Masse_S4[5][5] = {
   {     1.0/9, 1121.0/630,  527.0/210,  251.0/630,    1.0/630},
   {1121.0/630,  2281.0/63, 7166.0/105, 5723.0/315,  251.0/630},
   { 527.0/210, 7166.0/105,   1227.0/7, 7166.0/105,  527.0/210},
   { 251.0/630, 5723.0/315, 7166.0/105,  2281.0/63, 1121.0/630},
   {   1.0/630,  251.0/630,  527.0/210, 1121.0/630,      1.0/9}
};

Real MAT_Convection_S4[5][5] = {
   
   {   -1.0/2,   -88.0/35,   61.0/35,   44.0/35,   1.0/70},
   {-297.0/35,      -60,  802.0/35, 1516.0/35,  79.0/35},
   {-446.0/35, -4652.0/35,       0, 4652.0/35, 446.0/35},
   { -79.0/35, -1516.0/35, -802.0/35,      60, 297.0/35},
   {  -1.0/70,   -44.0/35,  -61.0/35,   88.0/35,    1.0/2}
} ;

Real MAT_Rigid1_S4[5][5] = {
   {   16.0/7,   436.0/35, -276.0/35,  -236.0/35,   -4.0/35},
   { 436.0/35,    760.0/7, -744.0/35, -3256.0/35, -236.0/35},
   {-276.0/35,  -744.0/35,   408.0/7,  -744.0/35, -276.0/35},
   {-236.0/35, -3256.0/35, -744.0/35,    760.0/7,  436.0/35},
   {  -4.0/35,  -236.0/35, -276.0/35,   436.0/35,    16.0/7}
} ;


Real MAT_Rigid2_S4[5][5] = {
   { 144.0/5,    24.0/5,  -456.0/5,   264.0/5,   24.0/5},
   {  24.0/5,   624.0/5, -1056.0/5,   144.0/5,  264.0/5},
   {-456.0/5, -1056.0/5,  3024.0/5, -1056.0/5, -456.0/5},
   { 264.0/5,   144.0/5, -1056.0/5,   624.0/5,   24.0/5},
   {  24.0/5,   264.0/5,  -456.0/5,    24.0/5,  144.0/5}
} ;


Real MAT_Stab_I_xx_S4[5][5] = {
   {   12.0/7,  -16.0/35,  -144.0/35,   96.0/35,    4.0/35},
   {1104.0/35,   136.0/7, -4296.0/35, 2136.0/35,  376.0/35},
   {1816.0/35, 3824.0/35,  -2256.0/7, 3824.0/35, 1816.0/35},
   { 376.0/35, 2136.0/35, -4296.0/35,   136.0/7, 1104.0/35},
   {   4.0/35,   96.0/35,  -144.0/35,  -16.0/35,    12.0/7}
} ;

Real MAT_Stab_x_xx_S4[5][5] = {
   {    -8,    4.0/5,   108.0/5,  -68.0/5,   -4.0/5},
   {-244.0/5,    -64,  1224.0/5, -512.0/5, -148.0/5},
   { 132.0/5, -264.0/5,       0,  264.0/5, -132.0/5},
   { 148.0/5,  512.0/5, -1224.0/5,     64,  244.0/5},
   {   4.0/5,   68.0/5,  -108.0/5,   -4.0/5,      8}
} ;



Real VEC_I__S4[5] = {   1.0/5,   26.0/5,   66.0/5,   26.0/5,    1.0/5 } ;

Real VEC_x__S4[5] = {   -1.0,   -10.0,     0.0,    10.0,     1.0 } ;

Real VEC_xx_S4[5] = {    4.0,     8.0,   -24.0,     8.0,     4.0} ;


Real TENSOR_Convection_S4[5][5][5] = {
   
   {  /* N1*dNj*Nk */
      {    -1.0/3,  -4751.0/990,  -2003.0/330,  -263.0/330,   -1.0/990},
      {-694.0/495, -11119.0/495,    -473.0/15, -3415.0/693, -59.0/3465},
      { 188.0/165,   6428.0/385,    1636.0/77, 3172.0/1155,          0},
      {  98.0/165,   5191.0/495, 18661.0/1155,  1123.0/385,  59.0/3465},
      {   1.0/495,   109.0/1386,   461.0/2310,    61.0/990,    1.0/990}
   },

   {  /* N2*dNj*Nk */
      { -4751.0/990,  -37657.0/495, -122618.0/1155, -57377.0/3465,    -61.0/990},
      {-11119.0/495,     -1330.0/3, -908276.0/1155,  -91526.0/495,  -1123.0/385},
      {  6428.0/385, 291952.0/1155,     21808.0/77,             0, -3172.0/1155},
      {  5191.0/495,  128602.0/495,  669236.0/1155,   91526.0/495,   3415.0/693},
      {  109.0/1386,    2631.0/385,     4934.0/165,  57377.0/3465,    263.0/330}
   },

   {  /* N3*dNj*Nk */
      { -2003.0/330, -122618.0/1155,  -12589.0/77,    -4934.0/165,   -461.0/2310},
      {   -473.0/15, -908276.0/1155, -136786.0/77, -669236.0/1155, -18661.0/1155},
      {   1636.0/77,     21808.0/77,            0,    -21808.0/77,    -1636.0/77},
      {18661.0/1155,  669236.0/1155,  136786.0/77,  908276.0/1155,      473.0/15},
      {  461.0/2310,     4934.0/165,   12589.0/77,  122618.0/1155,    2003.0/330}
   },

   {  /* N4*dNj*Nk */
      { -263.0/330, -57377.0/3465,    -4934.0/165,    -2631.0/385, -109.0/1386},
      {-3415.0/693,  -91526.0/495, -669236.0/1155,  -128602.0/495, -5191.0/495},
      {3172.0/1155,             0,    -21808.0/77, -291952.0/1155, -6428.0/385},
      { 1123.0/385,   91526.0/495,  908276.0/1155,       1330.0/3, 11119.0/495},
      {   61.0/990,  57377.0/3465,  122618.0/1155,    37657.0/495,  4751.0/990}
   },

   {  /* N5*dNj*Nk */
      {  -1.0/990,    -61.0/990,   -461.0/2310, -109.0/1386,   -1.0/495},
      {-59.0/3465,  -1123.0/385, -18661.0/1155, -5191.0/495,  -98.0/165},
      {         0, -3172.0/1155,    -1636.0/77, -6428.0/385, -188.0/165},
      { 59.0/3465,   3415.0/693,      473.0/15, 11119.0/495,  694.0/495},
      {   1.0/990,    263.0/330,    2003.0/330,  4751.0/990,      1.0/3}
   },
};


Real TENSOR_Masse_S4[5][5][5] = {
   {
      {     1.0/13,    6964.0/6435,    8612.0/6435,    1088.0/6435,     1.0/6435},
      {6964.0/6435,   35296.0/2145, 990289.0/45045,  47678.0/15015,  349.0/45045},
      {8612.0/6435, 990289.0/45045,   95182.0/3003, 233689.0/45045, 1004.0/45045},
      {1088.0/6435,  47678.0/15015, 233689.0/45045,  46028.0/45045,  349.0/45045},
      {   1.0/6435,    349.0/45045,   1004.0/45045,    349.0/45045,     1.0/6435}
   },
   {
      {   6964.0/6435,     35296.0/2145,   990289.0/45045,    47678.0/15015,    349.0/45045},
      {  35296.0/2145,     125470.0/429, 20987144.0/45045,    381202.0/4095,  46028.0/45045},
      {990289.0/45045, 20987144.0/45045,     386678.0/429, 10968824.0/45045, 233689.0/45045},
      { 47678.0/15015,    381202.0/4095, 10968824.0/45045,    381202.0/4095,  47678.0/15015},
      {   349.0/45045,    46028.0/45045,   233689.0/45045,    47678.0/15015,    1088.0/6435}
   },
   {
      {   8612.0/6435,   990289.0/45045,   95182.0/3003,   233689.0/45045,   1004.0/45045},
      {990289.0/45045, 20987144.0/45045,   386678.0/429, 10968824.0/45045, 233689.0/45045},
      {  95182.0/3003,     386678.0/429, 2343112.0/1001,     386678.0/429,   95182.0/3003},
      {233689.0/45045, 10968824.0/45045,   386678.0/429, 20987144.0/45045, 990289.0/45045},
      {  1004.0/45045,   233689.0/45045,   95182.0/3003,   990289.0/45045,    8612.0/6435}
   },
   {
      {   1088.0/6435,    47678.0/15015,   233689.0/45045,    46028.0/45045,    349.0/45045},
      { 47678.0/15015,    381202.0/4095, 10968824.0/45045,    381202.0/4095,  47678.0/15015},
      {233689.0/45045, 10968824.0/45045,     386678.0/429, 20987144.0/45045, 990289.0/45045},
      { 46028.0/45045,    381202.0/4095, 20987144.0/45045,     125470.0/429,   35296.0/2145},
      {   349.0/45045,    47678.0/15015,   990289.0/45045,     35296.0/2145,    6964.0/6435}
   },
   {
      {    1.0/6435,    349.0/45045,   1004.0/45045,    349.0/45045,    1.0/6435},
      { 349.0/45045,  46028.0/45045, 233689.0/45045,  47678.0/15015, 1088.0/6435},
      {1004.0/45045, 233689.0/45045,   95182.0/3003, 990289.0/45045, 8612.0/6435},
      { 349.0/45045,  47678.0/15015, 990289.0/45045,   35296.0/2145, 6964.0/6435},
      {    1.0/6435,    1088.0/6435,    8612.0/6435,    6964.0/6435,      1.0/13}
   },
};

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

//  TODO  for non periodix problems

/* TildePhi_0 = aij*Phi_0 + aij*Phi_1 + aij*Phi_2 + aij*Phi_3 */
/* TildePhi_1 = aij*Phi_0 + aij*Phi_1 + aij*Phi_2 + aij*Phi_3*/
/* TildePhi_2 = aij*Phi_0 + aij*Phi_1 + aij*Phi_2 + aij*Phi_3 */
/* TildePhi_3 = aij*Phi_0 + aij*Phi_1 + aij*Phi_2 + aij*Phi_3 */

Real MAT_PermBasis_S4_a[4][4] = {  /* A_ii = 1 */
   {   1.0   ,  0.0 ,  0.0,    0.0},
   {   0.0   ,  1.0 ,  0.0,    0.0},
   {   0.0   ,  0.0 ,  1.0,    0.0},
   {   0.0   ,  0.0 ,  0.0,    1.0}
} ;

Real MAT_PermBasis_S4_b[4][4] = {    /* SUMM_i (A_ij) = 1 */
   {   1.0   ,  0.0 ,  0.0,    0.0},
   {   0.0   ,  1.0 ,  0.0,    0.0},
   {   0.0   ,  0.0 ,  1.0,    0.0},
   {   0.0   ,  0.0 ,  0.0,    1.0}
} ;

Real MAT_PermBasis_S4_c[4][4] = {    /* SUMM_i (A_ij) = 0 */
   {   1.0   ,  0.0 ,  0.0,    0.0},
   {   0.0   ,  1.0 ,  0.0,    0.0},
   {   0.0   ,  0.0 ,  1.0,    0.0},
   {   0.0   ,  0.0 ,  0.0,    1.0}
} ;

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

