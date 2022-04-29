#! /usr/bin/env python
#
import Pol3D
from utils import *
#
from math import *
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2005/10/30 12:59:14 $
#  $Revision: 1.3 $
#
#  $Log: usePol3D.py,v $
#  Revision 1.3  2005/10/30 12:59:14  xavier
#  No log message.
#
#  Revision 1.2  2005/01/03 09:15:40  xavier
#  No log message.
#
#  Revision 1.1  2004/01/31 17:18:44  xavier
#  *** empty log message ***
#
#  ----------------------------------------------------------------
#
#
if __name__ == "_main__":
    #
    #elt1 = "P1"
    #elt1 = "P1b"
    elt1 = "P2"
    #
    #elt2 = "P1"
    #elt2 = "P1b"
    elt2 = "P2"
    #
    DF = 0
    #
    p0_P1 = Pol3D.Poly3D("1.0 - X - Y - Z")
    p1_P1 = Pol3D.Poly3D("X")
    p2_P1 = Pol3D.Poly3D("Y")
    p3_P1 = Pol3D.Poly3D("Z")
    #
    p0_P1b = Pol3D.Poly3D("1.0 - X - Y - Z")
    p1_P1b = Pol3D.Poly3D("X")
    p2_P1b = Pol3D.Poly3D("Y")
    p3_P1b = Pol3D.Poly3D("Z")
    p4_P1b = Pol3D.Poly3D("1.0 - X - Y - Z")*Pol3D.Poly3D("X")*Pol3D.Poly3D("Y")*Pol3D.Poly3D("Z")
    #
    p0_P2 = Pol3D.Poly3D("1.0 - 3.0*X - 3.0*Y - 3.0*Z + 4.0*X*Y + 4.0*X*Z + 4.0*Y*Z + 2.0*X*X + 2.0*Y*Y + 2.0*Z*Z")
    p1_P2 = Pol3D.Poly3D("-X + 2.0*X*X")
    p2_P2 = Pol3D.Poly3D("-Y + 2.0*Y*Y")
    p3_P2 = Pol3D.Poly3D("-Z + 2.0*Z*Z")
    p4_P2 = Pol3D.Poly3D("4.0*X - 4.0*X*X - 4.0*X*Y - 4.0*X*Z")
    p5_P2 = Pol3D.Poly3D("4.0*Y - 4.0*Y*Y - 4.0*Y*X - 4.0*Y*Z")
    p6_P2 = Pol3D.Poly3D("4.0*Z - 4.0*Z*Z - 4.0*Z*X - 4.0*Z*Y")
    p7_P2 = Pol3D.Poly3D("4.0*X*Y")
    p8_P2 = Pol3D.Poly3D("4.0*Y*Z")
    p9_P2 = Pol3D.Poly3D("4.0*X*Z")
    #
    #
    #
    P1 = []
    #
    dP1dx = []
    dP1dy = []
    dP1dz = []
    #
    if ( elt1 == "P1" ) :
        P1.append(p0_P1)
        P1.append(p1_P1)
        P1.append(p2_P1)
        P1.append(p3_P1)
    #
    elif ( elt1 == "P1b" ):
        P1.append(p0_P1b)
        P1.append(p1_P1b)
        P1.append(p2_P1b)
        P1.append(p3_P1b)
        P1.append(p4_P1b)
    #
    elif ( elt1 == "P2" ):
        P1.append(p0_P2)
        P1.append(p1_P2)
        P1.append(p2_P2)
        P1.append(p3_P2)
        P1.append(p4_P2)
        P1.append(p5_P2)
        P1.append(p6_P2)
        P1.append(p7_P2)
        P1.append(p8_P2)
        P1.append(p9_P2)
    #
    else:
        raise TypeError, " bad elt"
    #
    #
    P2 = []
    #
    if ( elt2 == "P1" ) :
        P2.append(p0_P1)
        P2.append(p1_P1)
        P2.append(p2_P1)
        P2.append(p3_P1)
    #
    elif ( elt2 == "P1b" ):
        P2.append(p0_P1b)
        P2.append(p1_P1b)
        P2.append(p2_P1b)
        P2.append(p3_P1b)
        P2.append(p4_P1b)
    #
    elif ( elt2 == "P2" ):
        P2.append(p0_P2)
        P2.append(p1_P2)
        P2.append(p2_P2)
        P2.append(p3_P2)
        P2.append(p4_P2)
        P2.append(p5_P2)
        P2.append(p6_P2)
        P2.append(p7_P2)
        P2.append(p8_P2)
        P2.append(p9_P2)
    #
    else:
        raise TypeError, " bad elt"
    #
    
    #
    #
    #
    N1 = len(P1)
    N2 = len(P2)
    #
    print "N1 = ", N1
    print "N2 = ", N2
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
        dP1dz.append( P1[i].dz() )
    #
    #
    print "dP1dx"
    for i in range(N1):
        print dP1dx[i]
    print "dP1dy"
    for i in range(N1):
        print dP1dy[i]
    print "dP1dz"
    for i in range(N1):
        print dP1dz[i]
    #    
    #
    I     = []
    #
    II    = []
    Ix    = []
    Iy    = []
    Iz    = []
    xx    = []
    xy    = []
    xz    = []
    yx    = []
    yy    = []
    yz    = []
    zx    = []
    zy    = []
    zz    = []
    #
    for i in range(N1):
        #
        II.append([])
        Ix.append([])
        Iy.append([])
        Iz.append([])
        xx.append([])
        xy.append([])
        xz.append([])
        yx.append([])
        yy.append([])
        yz.append([])
        zx.append([])
        zy.append([])
        zz.append([])
    #
    #
    III = []
    IxI = []
    IyI = []
    IzI = []
    #
    for k in range(N1):
        #
        III.append([])
        IxI.append([])
        IyI.append([])
        IzI.append([])
        #
        for i in range(N1):
            #
            III[k].append([])
            IxI[k].append([])
            IyI[k].append([])
            IzI[k].append([])
        #
    #
    #
    P2dP1dx = []
    P2dP1dy = []
    P2dP1dz = []
    #
    for i in range(N2):
        #
        P2dP1dx.append([])
        P2dP1dy.append([])
        P2dP1dz.append([])
    #
    #
    #
    #
    for i in range(N1):
        for j in range(N1):
            #
            #pass
            #
            II[i].append(P1[i]*P1[j])
            Ix[i].append(P1[i]*dP1dx[j])
            Iy[i].append(P1[i]*dP1dy[j])
            Iz[i].append(P1[i]*dP1dz[j])
            xx[i].append(dP1dx[i]*dP1dx[j])
            xy[i].append(dP1dx[i]*dP1dy[j])
            xz[i].append(dP1dx[i]*dP1dz[j])
            yx[i].append(dP1dy[i]*dP1dx[j])
            yy[i].append(dP1dy[i]*dP1dy[j])
            yz[i].append(dP1dy[i]*dP1dz[j])
            zx[i].append(dP1dz[i]*dP1dx[j])
            zy[i].append(dP1dz[i]*dP1dy[j])
            zz[i].append(dP1dz[i]*dP1dz[j])
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
            P2dP1dz[i].append(P2[i]*dP1dz[j])
        #
    #
    #
    for k in range(N1):
        for i in range(N1):
            for j in range(N1):
            #
                #pass
                #
                III[k][i].append(P1[k]*P1[i]*P1[j])
                IxI[k][i].append(P1[k]*dP1dx[i]*P1[j])
                IyI[k][i].append(P1[k]*dP1dy[i]*P1[j])
                IzI[k][i].append(P1[k]*dP1dz[i]*P1[j])
                #
            #
        #
    #
    #
    #
    #
    coeff_I  = 1
    coeff_I_I = 1
    coeff_I_D = 1
    coeff_D_D = 1
    #
    coeff_P2dP1 = 1
    #
    coeff_I_I_I =  1
    coeff_I_D_I =  1
    #
    #
    if ( elt1 == "P1" ):
        #
        coeff_I  =    4*3*2*1
        coeff_I_I =  5*4*3*2*1
        coeff_I_D =    4*3*2*1
        coeff_D_D =      3*2*1
        #
        coeff_P2dP1 = 4*3*2*1
        #
        coeff_I_I_I =  6*5*4*3*2*1
        coeff_I_D_I =    5*4*3*2*1
        #
    #
    elif ( elt1 == "P1b" ):
        #
        coeff_I  =             7*6*5*4*3*2*1   # p4 of degre 4 
        coeff_I_I =  11*10*9*8*7*6*5*4*3*2*1
        coeff_I_D =     10*9*8*7*6*5*4*3*2*1
        coeff_D_D =        9*8*7*6*5*4*3*2*1
        #
        coeff_P2dP1 =             8*7*6*5*4*3*2*1
        #
        coeff_I_I_I =  12*11*10*9*8*7*6*5*4*3*2*1
        coeff_I_D_I =     11*10*9*8*7*6*5*4*3*2*1
    #
    elif ( elt1 == "P2" ): 
        #
        coeff_I  =       5*4*3*2*1
        coeff_I_I =  7*6*5*4*3*2*1
        coeff_I_D =    6*5*4*3*2*1
        coeff_D_D =      5*4*3*2*1
        #
        coeff_P2dP1 =          5*4*3*2*1
        #
        coeff_I_I_I =  9*8*7*6*5*4*3*2*1
        coeff_I_D_I =    8*7*6*5*4*3*2*1
    #
    else:
        #
        raise TypError," P1, P2, P1b only"
        #
    #
    #
    #
    #
    int_I     = []
    #
    int_II    = []
    int_Ix    = []
    int_Iy    = []
    int_Iz    = []
    int_xx    = []
    int_xy    = []
    int_xz    = []
    int_yx    = []
    int_yy    = []
    int_yz    = []
    int_zx    = []
    int_zy    = []
    int_zz    = []
    #
    for i in range(N1):
        #
        int_II.append([])
        int_Ix.append([])
        int_Iy.append([])
        int_Iz.append([])
        int_xx.append([])
        int_xy.append([])
        int_xz.append([])
        int_yx.append([])
        int_yy.append([])
        int_yz.append([])
        int_zx.append([])
        int_zy.append([])
        int_zz.append([])
        #
    #
    #
    int_III = []
    int_IxI = []
    int_IyI = []
    int_IzI = []
    #
    for k in range(N1):
        #
        int_III.append([])
        int_IxI.append([])
        int_IyI.append([])
        int_IzI.append([])
        #
        for i in range(N1):
            #
            int_III[k].append([])
            int_IxI[k].append([])
            int_IyI[k].append([])
            int_IzI[k].append([])
        #
    #
    #
    int_P2dP1dx = []
    int_P2dP1dy = []
    int_P2dP1dz = []
    #
    for i in range(N2):
        #
        int_P2dP1dx.append([])
        int_P2dP1dy.append([])
        int_P2dP1dz.append([])
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
        int_I.append(val_I)
        #
    #
    #
    for i in range(N1):
        for j in range(N1):
            #
            #pass
            #
            val_II = rounding_value( coeff_I_I * II[i][j].int_01(), 'II')
            val_Ix = rounding_value( coeff_I_D * Ix[i][j].int_01(), 'Ix')
            val_Iy = rounding_value( coeff_I_D * Iy[i][j].int_01(), 'Iy')
            val_Iz = rounding_value( coeff_I_D * Iz[i][j].int_01(), 'Iz')
            #
            val_xx = rounding_value( coeff_D_D * xx[i][j].int_01(), 'xx')
            val_xy = rounding_value( coeff_D_D * xy[i][j].int_01(), 'xy')
            val_xz = rounding_value( coeff_D_D * xz[i][j].int_01(), 'xz')
            val_yx = rounding_value( coeff_D_D * yx[i][j].int_01(), 'yx')
            val_yy = rounding_value( coeff_D_D * yy[i][j].int_01(), 'yy')
            val_yz = rounding_value( coeff_D_D * yz[i][j].int_01(), 'yz')
            val_zx = rounding_value( coeff_D_D * zx[i][j].int_01(), 'zx')
            val_zy = rounding_value( coeff_D_D * zy[i][j].int_01(), 'zy')
            val_zz = rounding_value( coeff_D_D * zz[i][j].int_01(), 'zz')
            #
            #
            int_II[i].append( val_II )
            int_Ix[i].append( val_Ix )
            int_Iy[i].append( val_Iy )
            int_Iz[i].append( val_Iy )
            #
            int_xx[i].append( val_xx )
            int_xy[i].append( val_xy )
            int_xz[i].append( val_xz )
            int_yx[i].append( val_yx )
            int_yy[i].append( val_yy )
            int_yz[i].append( val_yz )
            int_zx[i].append( val_zx )
            int_zy[i].append( val_zy )
            int_zz[i].append( val_zz )
            #
        #
    #
    #
    for i in range(N2):
        for j in range(N1):
            #
            #pass
            #
            val_P2dP1dx = rounding_value ( coeff_P2dP1 * P2dP1dx[i][j].int_01(), 'P2dP1dx')
            val_P2dP1dy = rounding_value ( coeff_P2dP1 * P2dP1dy[i][j].int_01(), 'P2dP1dy')
            val_P2dP1dz = rounding_value ( coeff_P2dP1 * P2dP1dz[i][j].int_01(), 'P2dP1dz')
            #
            int_P2dP1dx[i].append( val_P2dP1dx )
            int_P2dP1dy[i].append( val_P2dP1dy )
            int_P2dP1dz[i].append( val_P2dP1dz )
            #
        #
    #
    #
    for k in range(N1):
        for i in range(N1):
            for j in range(N1):
            #
                val_III = rounding_value( coeff_I_I_I * III[k][i][j].int_01(), 'III')
                val_IxI = rounding_value( coeff_I_D_I * IxI[k][i][j].int_01(), 'IxI')
                val_IyI = rounding_value( coeff_I_D_I * IyI[k][i][j].int_01(), 'IyI')
                val_IzI = rounding_value( coeff_I_D_I * IzI[k][i][j].int_01(), 'IzI')
                #
                #
                int_III[k][i].append( val_III )
                int_IxI[k][i].append( val_IxI )
                int_IyI[k][i].append( val_IyI )
                int_IzI[k][i].append( val_IzI )
            #
        #
    #
    #
    #
    print "Vec_I"
    print int_I , " / ", coeff_I
    #
    print "Mat_II"
    print int_II, " / ", coeff_I_I
    #
    #
    print "Mat_Ix"
    print int_Ix, " / ", coeff_I_D
    #
    print "Mat_Iy"
    print int_Iy, " / ", coeff_I_D
    #
    print "Mat_Iz"
    print int_Iz, " / ", coeff_I_D
    #
    #
    print "Mat_xx"
    print int_xx, " / ", coeff_D_D
    #
    print "Mat_xy"
    print int_xy, " / ", coeff_D_D
    #
    print "Mat_xz"
    print int_xz, " / ", coeff_D_D
    #
    print "Mat_yx"
    print int_yx, " / ", coeff_D_D
    #
    print "Mat_yy"
    print int_yy, " / ", coeff_D_D
    #
    print "Mat_yz"
    print int_yz, " / ", coeff_D_D
    #
    print "Mat_zx"
    print int_zx, " / ", coeff_D_D
    #
    print "Mat_zy"
    print int_zy, " / ", coeff_D_D
    #
    print "Mat_zz"
    print int_zz, " / ", coeff_D_D
    #
    #
    #
    print ("Mat_P2dP1dx")
    print int_P2dP1dx, " / ",coeff_P2dP1
    #
    print ("Mat_P2dP1dy")
    print int_P2dP1dy, " / ",coeff_P2dP1
    #
    print ("Mat_P2dP1dz")
    print int_P2dP1dz, " / ",coeff_P2dP1
    #
    #
    #
    print ("Mat_III")
    #
    for k in range(len(P1)) :
        print "III[",k,"]"
    #    print int_III[k], " / ",coeff_I_I_I
    #
    #
    print ("Mat_IxI")
    #
    for k in range(len(P1)) :
        print "IxI[",k,"]"
    #    print int_IxI[k], " / ",coeff_I_D_I
    #
    #
    print ("Mat_IyI")
    #
    for k in range(len(P1)) :
        print "IyI[",k,"]"
    #    print int_IyI[k], " / ", coeff_I_D_I
    #
    print ("Mat_IzI")
    #
    for k in range(len(P1)) :
        print "IzI[",k,"]"
    #    print int_IzI[k], " / ", coeff_I_D_I
    #
