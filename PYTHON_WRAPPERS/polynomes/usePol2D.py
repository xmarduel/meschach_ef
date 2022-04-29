#! /usr/bin/env python
#
import Pol2D
from utils import *
#
from math import *
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2005/10/30 12:59:14 $
#  $Revision: 1.5 $
#
#  $Log: usePol2D.py,v $
#  Revision 1.5  2005/10/30 12:59:14  xavier
#  No log message.
#
#  Revision 1.4  2005/01/03 09:17:18  xavier
#  No log message.
#
#  Revision 1.3  2005/01/03 09:15:40  xavier
#  No log message.
#
#  Revision 1.2  2005/01/02 16:09:48  xavier
#  comments
#
#  ----------------------------------------------------------------
#
#
    if __name__ == "_main__":
        #
    elt1 = "P1"
    elt1 = "P2"
    #elt1 = "P3"
    #elt1 = "P1b"
    #
    elt2 = "P1"
    #elt2 = "P2"
    #
    #
    p0_P1 = Pol2D.Poly2D("1.0-X-Y")
    p1_P1 = Pol2D.Poly2D("X")
    p2_P1 = Pol2D.Poly2D("Y")
    #
    p0_P1b = Pol2D.Poly2D("1.0-X-Y")
    p1_P1b = Pol2D.Poly2D("X")
    p2_P1b = Pol2D.Poly2D("Y")
    p3_P1b = Pol2D.Poly2D("+ 27.0*X*Y - 27.0*X*X*Y - 27.0*X*Y*Y") # 27*X*Y*(1-X-Y)
    #
    p0_P2 = Pol2D.Poly2D("1.0 - 3*X - 3*Y + 2*X*X + 4*X*Y + 2*Y*Y")
    p1_P2 = Pol2D.Poly2D("2*X*X - X")
    p2_P2 = Pol2D.Poly2D("2*Y*Y - Y")
    p3_P2 = Pol2D.Poly2D("4*X - 4*X*X - 4*X*Y")
    p4_P2 = Pol2D.Poly2D("4*X*Y")
    p5_P2 = Pol2D.Poly2D("4*Y - 4*Y*X - 4*Y*Y")
    #
    p0_P3 = Pol2D.Poly2D("1.0 - 5.5*X - 5.5*Y +  9.0*X*X + 18.0*X*Y +  9.0*Y*Y -  4.5*X*X*X - 13.5*X*X*Y - 13.5*X*Y*Y -  4.5*Y*Y*Y")
    p1_P3 = Pol2D.Poly2D("1.0*X -  4.5*X*X  +  4.5*X*X*X")
    p2_P3 = Pol2D.Poly2D("1.0*Y -  4.5*Y*Y  +  4.5*Y*Y*Y")
    p3_P3 = Pol2D.Poly2D("9.0*X - 22.5*X*X - 22.5*X*Y  + 13.5*X*X*X + 27.0*X*X*Y + 13.5*X*Y*Y")
    p4_P3 = Pol2D.Poly2D("- 4.5*X + 18.0*X*X +  4.5*X*Y - 13.5*X*X*X - 13.5*X*X*Y ")
    p5_P3 = Pol2D.Poly2D("- 4.5*X*Y + 13.5*X*X*Y")
    p6_P3 = Pol2D.Poly2D("- 4.5*X*Y + 13.5*X*Y*Y ")
    p7_P3 = Pol2D.Poly2D("+ 9.0*Y   - 22.5*X*Y - 22.5*Y*Y + 13.5*X*X*Y + 27.0*X*Y*Y + 13.5*Y*Y*Y")
    p8_P3 = Pol2D.Poly2D("- 4.5*Y   +  4.5*X*Y + 18.0*Y*Y - 13.5*X*Y*Y - 13.5*Y*Y*Y")
    p9_P3 = Pol2D.Poly2D("+ 27.0*X*Y - 27.0*X*X*Y - 27.0*X*Y*Y")
    #
    #
    P1 = []
    P2 = []
    dP1dx = []
    dP1dy = []
    ddP1dxx = []
    ddP1dxy = []
    ddP1dyy = []
    #
    #
    if ( elt1 == "P1" ) :
        P1.append(p0_P1)
        P1.append(p1_P1)
        P1.append(p2_P1)
    #
    elif ( elt1 == "P1b" ):
        P1.append(p0_P1b)
        P1.append(p1_P1b)
        P1.append(p2_P1b)
        P1.append(p3_P1b)
    #
    elif ( elt1 == "P2" ):
        P1.append(p0_P2)
        P1.append(p1_P2)
        P1.append(p2_P2)
        P1.append(p3_P2)
        P1.append(p4_P2)
        P1.append(p5_P2)
    #
    elif ( elt1 == "P3" ):
        P1.append(p0_P3)
        P1.append(p1_P3)
        P1.append(p2_P3)
        P1.append(p3_P3)
        P1.append(p4_P3)
        P1.append(p5_P3)
        P1.append(p6_P3)
        P1.append(p7_P3)
        P1.append(p8_P3)
        P1.append(p9_P3)
    #
    else:
        raise TypeError, " bad elt"
    #
    #
    #
    if ( elt2 == "P1" ) :
        P2.append(p0_P1)
        P2.append(p1_P1)
        P2.append(p2_P1)
    #
    elif ( elt2 == "P1b" ):
        P2.append(p0_P1b)
        P2.append(p1_P1b)
        P2.append(p2_P1b)
        P2.append(p3_P1b)
    #
    elif ( elt2 == "P2" ):
        P2.append(p0_P2)
        P2.append(p1_P2)
        P2.append(p2_P2)
        P2.append(p3_P2)
        P2.append(p4_P2)
        P2.append(p5_P2)
    #
    elif ( elt2 == "P3" ):
        P2.append(p0_P3)
        P2.append(p1_P3)
        P2.append(p2_P3)
        P2.append(p3_P3)
        P2.append(p4_P3)
        P2.append(p5_P3)
        P2.append(p6_P3)
        P2.append(p7_P3)
        P2.append(p8_P3)
        P2.append(p9_P3)
    #
    else:
        raise TypeError, " bad elt"
    #
    #
    #
    N1 = len(P1)
    N2 = len(P2)
    #
    #
    print "P1"
    for i in range(N1):
        print P1[i]
    #
    print "P2"
    for i in range(N2):
        print P2[i]
    #
    for i in range(N1):
        dP1dx.append( P1[i].dx() )
        dP1dy.append( P1[i].dy() )
    #
    for i in range(N1):
        ddP1dxx.append( dP1dx[i].dx() )
        ddP1dxy.append( dP1dx[i].dy() )
        ddP1dyy.append( dP1dy[i].dy() )
    #
    #
    print "dP1dx"
    for i in range(N1):
        print dP1dx[i]
    print "dP1dy"
    for i in range(N1):
        print dP1dy[i]
    #
    print "ddP1dxx"
    for i in range(N1):
        print ddP1dxx[i]
    print "ddP1dxy"
    for i in range(N1):
        print ddP1dxy[i]
    print "ddP1dyy"
    for i in range(N1):
        print ddP1dyy[i]
    #
    #
    #   
    I     = []
    #
    x     = []
    y     = []
    xx    = []
    xy    = []
    yy    = []
    #
    I_I    = []
    I_x    = []
    I_y    = []
    x_x    = []
    x_y    = []
    y_x    = []
    y_y    = []
    #
    xx_xx = []
    xx_xy = []
    xx_yy = []
    xy_xx = []
    xy_xy = []
    xy_yy = []
    yy_xx = []
    yy_xy = []
    yy_yy = []
    #
    xx_x = []
    xx_y = []
    xy_x = []
    xy_y = []
    yy_x = []
    yy_y = []
    #
    for i in range(N1):
        #
        I_I.append([])
        I_x.append([])
        I_y.append([])
        x_x.append([])
        x_y.append([])
        y_x.append([])
        y_y.append([])
        #
        xx_xx.append([])
        xx_xy.append([])
        xx_yy.append([])
        xy_xx.append([])
        xy_xy.append([])
        xy_yy.append([])
        yy_xx.append([])
        yy_xy.append([])
        yy_yy.append([])
        #
        xx_x.append([])
        xx_y.append([])
        xy_x.append([])
        xy_y.append([])
        yy_x.append([])
        yy_y.append([])
        #
    #
    #
    III = []
    IxI = []
    IyI = []
    #
    for k in range(N1):
        #
        III.append([])
        IxI.append([])
        IyI.append([])
        #
        for i in range(N1):
            #
            III[k].append([])
            IxI[k].append([])
            IyI[k].append([])
        #
    #
    #
    P2dP1dx = []
    P2dP1dy = []
    #
    for i in range(N2):
        #
        P2dP1dx.append([])
        P2dP1dy.append([])
    #
    #
    #
    #
    for i in range(N1):
        for j in range(N1):
            #
            #pass
            #
            I_I[i].append(P1[i]*P1[j])
            I_x[i].append(P1[i]*dP1dx[j])
            I_y[i].append(P1[i]*dP1dy[j])
            x_x[i].append(dP1dx[i]*dP1dx[j])
            x_y[i].append(dP1dx[i]*dP1dy[j])
            y_x[i].append(dP1dy[i]*dP1dx[j])
            y_y[i].append(dP1dy[i]*dP1dy[j])
            #
            xx_xx[i].append(ddP1dxx[i]*ddP1dxx[j] )
            xx_xy[i].append(ddP1dxx[i]*ddP1dxy[j] )
            xx_yy[i].append(ddP1dxx[i]*ddP1dyy[j] )
            xy_xx[i].append(ddP1dxy[i]*ddP1dxx[j] )
            xy_xy[i].append(ddP1dxy[i]*ddP1dxy[j] )
            xy_yy[i].append(ddP1dxy[i]*ddP1dyy[j] )
            yy_xx[i].append(ddP1dyy[i]*ddP1dxx[j] )
            yy_xy[i].append(ddP1dyy[i]*ddP1dxy[j] )
            yy_yy[i].append(ddP1dyy[i]*ddP1dyy[j] )
            #
            xx_x[i].append(ddP1dxx[i]*dP1dx[j])
            xx_y[i].append(ddP1dxx[i]*dP1dy[j])
            xy_x[i].append(ddP1dxy[i]*dP1dx[j])
            xy_y[i].append(ddP1dxy[i]*dP1dy[j])
            yy_x[i].append(ddP1dyy[i]*dP1dx[j])
            yy_y[i].append(ddP1dyy[i]*dP1dy[j])
            #
        #
    #
    #
    for i in range(N2):
        for j in range(N1):
            #
            #pass
            #
            P2dP1dx[i].append(P2[i]*dP1dx[j])
            P2dP1dy[i].append(P2[i]*dP1dy[j])
        #
    #
    #
    for k in range(N1):
        for i in range(N1):
            for j in range(N1):
            #
                pass
                #
                #III[k][i].append(P1[k]*P1[i]*P1[j])
                #IxI[k][i].append(P1[k]*dP1dx[i]*P1[j])
                #IyI[k][i].append(P1[k]*dP1dy[i]*P1[j])
                #
            #
        #
    #
    #
    #
    #
    coeff_I  = 0
    coeff_I_I = 0
    coeff_I_D = 0
    coeff_D_D = 0
    #
    coeff_DD_DD = 0
    coeff_DD_D  = 0
    #
    coeff_P2dP1dx = 0
    coeff_P2dP1dy = 0
    #
    coeff_III =  0
    coeff_IDI =  0
    #
    #
    if ( elt1 == "P1" ) :
    #
        #
        coeff_I  =       3*2*1
        coeff_I_I =    4*3*2*1
        coeff_I_D =      3*2*1
        coeff_D_D =        2*1
        #
        coeff_DD_DD =        1
        coeff_DD_D  =        1
        #
        coeff_P2dP1dx = 4*3*2*1
        coeff_P2dP1dy = 4*3*2*1
        #
        coeff_III =   5*4*3*2*1
        coeff_IDI =     4*3*2*1
        #
    #
    elif ( elt1 == "P1b" ):
    #
        #
        coeff_I  =        5*4*3*2*1
        #
        coeff_I_I = 8*7*6*5*4*3*2*1
        coeff_I_D =   7*6*5*4*3*2*1
        coeff_D_D =     6*5*4*3*2*1
        #
        coeff_DD_DD =       4*3*2*1
        coeff_DD_D  =     5*4*3*2*1
        #
        coeff_P2dP1dx =   5*4*3*2*1
        coeff_P2dP1dy =   5*4*3*2*1
        #
        coeff_III =  11*10*9*8*7*6*5*4*3*2*1
        coeff_IDI =     10*9*8*7*6*5*4*3*2*1
        #
    #
    elif ( elt1 == "P2" ):
    #
        #
        coeff_I  =        3*2*1
        coeff_I_I = 6*5*4*3*2*1
        coeff_I_D =   5*4*3*2*1
        coeff_D_D =       3*2*1
        #
        coeff_DD_DD =         1*1*1*1
        coeff_DD_D  =       5*1*3*1*1
        #
        coeff_P2dP1dx =   6*5*4*3*2*1
        coeff_P2dP1dy =   6*5*4*3*2*1
        #
        coeff_III =   8*7*6*5*4*3*2*1
        coeff_IDI =     7*6*5*4*3*2*1
        #
    #
    elif ( elt1 == "P3" ):
        #
        coeff_I  =        5*4*3*2*1*2
        coeff_I_I = 8*7*6*5*4*3*2*1
        coeff_I_D =   7*6*5*4*3*2*1*4
        coeff_D_D =     6*5*4*3*2*1*2
        #
        coeff_DD_DD =       4*3*2*1*2
        coeff_DD_D  =     5*4*3*2*1*2
        #
        coeff_P2dP1dx = 6*5*4*3*2*1*2
        coeff_P2dP1dy = 6*5*4*3*2*1*2
        #
        coeff_III =  11*10*9*8*7*6*5*4*3*2*1*2
        coeff_IDI =     10*9*8*7*6*5*4*3*2*1*2
    #
    #
    else:
        raise TypeError, " bad elt"
    #
    #
    #
    int_I     = []
    #
    int_x      = []
    int_y      = []
    int_xx     = []
    int_xy     = []
    int_yy     = []
    #
    int_I_I    = []
    int_I_x    = []
    int_I_y    = []
    int_x_x    = []
    int_x_y    = []
    int_y_x    = []
    int_y_y    = []
    #
    int_xx_xx = []
    int_xx_xy = []
    int_xx_yy = []
    int_xy_xx = []
    int_xy_xy = []
    int_xy_yy = []
    int_yy_xx = []
    int_yy_xy = []
    int_yy_yy = []
    #
    int_xx_x = []
    int_xx_y = []
    int_xy_x = []
    int_xy_y = []
    int_yy_x = []
    int_yy_y = []
    #
    for i in range(N1):
        #
        int_I_I.append([])
        int_I_x.append([])
        int_I_y.append([])
        int_x_x.append([])
        int_x_y.append([])
        int_y_x.append([])
        int_y_y.append([])
        #
        int_xx_xx.append([])
        int_xx_xy.append([])
        int_xx_yy.append([])
        int_xy_xx.append([])
        int_xy_xy.append([])
        int_xy_yy.append([])
        int_yy_xx.append([])
        int_yy_xy.append([])
        int_yy_yy.append([])
        #
        int_xx_x.append([])
        int_xx_y.append([])
        int_xy_x.append([])
        int_xy_y.append([])
        int_yy_x.append([])
        int_yy_y.append([])
    #
    #
    int_P2dP1dx = []
    int_P2dP1dy = []
    #
    for i in range(N2):
        #
        int_P2dP1dx.append([])
        int_P2dP1dy.append([])
        #
    #
    #
    int_III = []
    int_IxI = []
    int_IyI = []
    #
    for k in range(N1):
        #
        int_III.append([])
        int_IxI.append([])
        int_IyI.append([])
        #
        for i in range(N1):
            #
            int_III[k].append([])
            int_IxI[k].append([])
            int_IyI[k].append([])
        #
    #
    #
    #
    #
    #
    #
    #
    for i in range(N1):
        #
        #pass
        #
        val_I = rounding_value( coeff_I * P1[i].int_01(), 'I')
        #
        val_x = rounding_value( coeff_I * dP1dy[i].int_01(), 'x')
        val_y = rounding_value( coeff_I * dP1dy[i].int_01(), 'y')
        #
        val_xx = rounding_value( coeff_I * ddP1dxx[i].int_01(), 'xx')
        val_xy = rounding_value( coeff_I * ddP1dxy[i].int_01(), 'xy')
        val_yy = rounding_value( coeff_I * ddP1dyy[i].int_01(), 'yy')
        #
        #
        int_I.append(val_I)
        int_x.append(val_x)
        int_y.append(val_y)
        int_xx.append(val_xx)
        int_xy.append(val_xy)
        int_yy.append(val_yy)
        #
    #
    #
    for i in range(N1):
        for j in range(N1):
            #
            pass
            #
            val_I_I = rounding_value( coeff_I_I * I_I[i][j].int_01(),'I_I')
            val_I_x = rounding_value( coeff_I_D * I_x[i][j].int_01(),'I_x')
            val_I_y = rounding_value( coeff_I_D * I_y[i][j].int_01(),'I_y')
            val_x_x = rounding_value( coeff_D_D * x_x[i][j].int_01(),'x_x')
            val_x_y = rounding_value( coeff_D_D * x_y[i][j].int_01(),'x_y')
            val_y_x = rounding_value( coeff_D_D * y_x[i][j].int_01(),'y_x')
            val_y_y = rounding_value( coeff_D_D * y_y[i][j].int_01(),'y_y')
            #
            #
            int_I_I[i].append( val_I_I )
            int_I_x[i].append( val_I_x )
            int_I_y[i].append( val_I_y )
            int_x_x[i].append( val_x_x )
            int_x_y[i].append( val_x_y )
            int_y_x[i].append( val_y_x )
            int_y_y[i].append( val_y_y )
        #
    #
    for i in range(N1):
        for j in range(N1):
            #
            #pass
            #
            val_xx_xx = rounding_value( coeff_DD_DD * xx_xx[i][j].int_01(), 'val_xx_xx')
            val_xx_xy = rounding_value( coeff_DD_DD * xx_xy[i][j].int_01(), 'val_xx_xy')
            val_xx_yy = rounding_value( coeff_DD_DD * xx_yy[i][j].int_01(), 'val_xx_yy')
            val_xy_xx = rounding_value( coeff_DD_DD * xy_xx[i][j].int_01(), 'val_xy_xx')
            val_xy_xy = rounding_value( coeff_DD_DD * xy_xy[i][j].int_01(), 'val_xy_xy')
            val_xy_yy = rounding_value( coeff_DD_DD * xy_yy[i][j].int_01(), 'val_xy_yy')
            val_yy_xx = rounding_value( coeff_DD_DD * yy_xx[i][j].int_01(), 'val_yy_xx')
            val_yy_xy = rounding_value( coeff_DD_DD * yy_xy[i][j].int_01(), 'val_yy_xy')
            val_yy_yy = rounding_value( coeff_DD_DD * yy_yy[i][j].int_01(), 'val_yy_yy')
            #
            #
            int_xx_xx[i].append( val_xx_xx )
            int_xx_xy[i].append( val_xx_xy )
            int_xx_yy[i].append( val_xx_yy )
            int_xy_xx[i].append( val_xy_xx )
            int_xy_xy[i].append( val_xy_xy )
            int_xy_yy[i].append( val_xy_yy )
            int_yy_xx[i].append( val_yy_xx )
            int_yy_xy[i].append( val_yy_xy )
            int_yy_yy[i].append( val_yy_yy )
        #
    #
    for i in range(N1):
        for j in range(N1):
            #
            #pass
            #
            val_xx_x = rounding_value( coeff_DD_D * xx_x[i][j].int_01(),'val_xx_x')
            val_xx_y = rounding_value( coeff_DD_D * xx_y[i][j].int_01(),'val_xx_y')
            val_xy_x = rounding_value( coeff_DD_D * xy_x[i][j].int_01(),'val_xy_x')
            val_xy_y = rounding_value( coeff_DD_D * xy_y[i][j].int_01(),'val_xy_y')
            val_yy_x = rounding_value( coeff_DD_D * yy_x[i][j].int_01(),'val_yy_x')
            val_yy_y = rounding_value( coeff_DD_D * yy_y[i][j].int_01(),'val_yy_y')
            #
            #
            int_xx_x[i].append( val_xx_x )
            int_xx_y[i].append( val_xx_y )
            int_xy_x[i].append( val_xy_x )
            int_xy_y[i].append( val_xy_y )
            int_yy_x[i].append( val_yy_x )
            int_yy_y[i].append( val_yy_y )
        #
    #
    for i in range(N2):
        for j in range(N1):
            #
            pass
            #
            #val_P2dP1dx = rounding_value( coeff_P2dP1dx * P2dP1dx[i][j].int_01(),'P2dP1dx')
            #val_P2dP1dy = rounding_value( coeff_P2dP1dx * P2dP1dy[i][j].int_01(),'P2dP1dy')
            #
            #
            #int_P2dP1dx[i].append( val_P2dP1dx )
            #int_P2dP1dy[i].append( val_P2dP1dy )
            #
        #
    #
    #
    for k in range(N1):
        for i in range(N1):
            for j in range(N1):
            #
                pass
                #
                #val_III = rounding_value( coeff_III * III[k][i][j].int_01(), 'III')
                #val_IxI = rounding_value( coeff_IDI * IxI[k][i][j].int_01(), 'IxI')
                #val_IyI = rounding_value( coeff_IDI * IyI[k][i][j].int_01(), 'IyI')
                #
                #
                #int_III[k][i].append( val_III )
                #int_IxI[k][i].append( val_IxI )
                #int_IyI[k][i].append( val_IyI )
            #
        #
    #
    #
    #
    print "Vec_I"
    print int_I , " / ", coeff_I
    #
    print "Vec_x"
    print int_x , " / ", coeff_I
    #
    print "Vec_y"
    print int_y , " / ", coeff_I
    #
    print "Vec_xx"
    print int_xx , " / ", coeff_I
    #
    print "Vec_xy"
    print int_xy , " / ", coeff_I
    #
    print "Vec_yy"
    print int_yy , " / ", coeff_I
    #
    #
    #
    print ("Mat_I_I")
    #print int_I_I, " / ", coeff_I_I
    #
    print ("Mat_I_x")
    #print int_I_x, " / ", coeff_I_D
    #
    print ("Mat_I_y")
    #print int_I_y, " / ", coeff_I_D
    #
    #
    print ("Mat_x_x")
    print int_x_x, " / ", coeff_D_D
    #
    print ("Mat_x_y")
    print int_x_y, " / ", coeff_D_D
    #
    print ("Mat_y_x")
    print int_y_x, " / ", coeff_D_D
    #
    print ("Mat_y_y")
    #print int_y_y, " / ", coeff_D_D
    #
    #
    print ("Mat_xx_xx")
    #print int_xx_xx, " / ", coeff_DD_DD
    #
    print ("Mat_xx_xy")
    #print int_xx_xy, " / ", coeff_DD_DD
    #
    print ("Mat_xx_yy")
    #print int_xx_yy, " / ", coeff_DD_DD
    #
    print ("Mat_xy_xx")
    #print int_xy_xx, " / ", coeff_DD_DD
    #
    print ("Mat_xy_xy")
    #print int_xy_xy, " / ", coeff_DD_DD
    #
    print ("Mat_xy_yy")
    #print int_xy_yy, " / ", coeff_DD_DD
    #
    print ("Mat_yy_xx")
    #print int_yy_xx, " / ", coeff_DD_DD
    #
    print ("Mat_yy_xy")
    #print int_yy_xy, " / ", coeff_DD_DD
    #
    print ("Mat_yy_yy")
    #print int_yy_yy, " / ", coeff_DD_DD
    #
    #
    print ("Mat_xx_x")
    #print int_xx_x, " / ", coeff_DD_D
    #
    print ("Mat_xx_y")
    #print int_xx_y, " / ", coeff_DD_D
    #
    print ("Mat_xy_x")
    #print int_xy_x, " / ", coeff_DD_D
    #
    print ("Mat_xy_y")
    #print int_xy_y, " / ", coeff_DD_D
    #
    print ("Mat_yy_x")
    #print int_yy_x, " / ", coeff_DD_D
    #
    print ("Mat_yy_y")
    #print int_yy_y, " / ", coeff_DD_D
    #
    #
    #
    print ("Mat_P2dP1dx")
    print int_P2dP1dx, " / ",coeff_P2dP1dx
    #
    print ("Mat_P2dP1dy")
    print int_P2dP1dy, " / ",coeff_P2dP1dy
    #
    #
    #
    print ("Mat_III")
    #
    #for k in range(len(P1)) :
    #    print "III[",k,"]"
    #    print int_III[k], " / ",coeff_III
    #
    #
    print ("Mat_IxI")
    #
    #for k in range(len(P1)) :
    #    print "IxI[",k,"]"
    #    print int_IxI[k], " / ",coeff_IDI
    #
    #
    print ("Mat_IyI")
    #
    #for k in range(len(P1)) :
    #    print "IyI[",k,"]"
    #    print int_IyI[k], " / ",coeff_IDI
    #
