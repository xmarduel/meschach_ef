#ifndef MEF_GAUSS_2D_TRI_H
#define MEF_GAUSS_2D_TRI_H

#define NBPTS_GAUSS_2D_TRI   7


static Real   w2D[7] = { 0.1125000000 ,
                         0.0661970763 ,
                         0.0661970763 ,
                         0.0661970763 ,
                         0.0629695902 ,
                         0.0629695902 ,
                         0.0629695902 } ;

static Real ksi2D[7] = { 1.0 / 3      ,
                         0.4701420641 ,
                         0.0597158717 ,
                         0.4701420641 ,
                         0.1012865073 ,
                         0.7974269853 ,
                         0.1012865073 } ;

static Real eta2D[7] = { 1.0 / 3      ,
                         0.4701420641 ,
                         0.4701420641 ,
                         0.0597158717 ,
                         0.1012865073 ,
                         0.1012865073 ,
                         0.7974269853 } ;

#endif
