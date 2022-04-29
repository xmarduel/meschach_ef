#! /usr/bin/env python
#
from Pol1D import *
from utils import *
#
from math import *
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2005/10/30 12:59:14 $
#  $Revision: 1.5 $
#
#  $Log: usePol1D.py,v $
#  Revision 1.5  2005/10/30 12:59:14  xavier
#  No log message.
#
#  Revision 1.4  2005/01/03 09:15:40  xavier
#  No log message.
#
#  Revision 1.2  2005/01/02 16:09:48  xavier
#  comments
#
#  ----------------------------------------------------------------
#
# build the assemblage matrices for element P1-P2-P3-H3-S3-S5 in DIM 1
#
#
#
if __name__ == "_main__":
    #
    DF = 0
    #
    elt1 = "P1"
    elt1 = "P2"
    elt1 = "P3"
    elt1 = "H3"
    elt1 = "S3"
    elt1 = "S5"
    #
    #
    p0_P1 = Poly1D("1-X") # "1-x"
    p1_P1 = Poly1D("X")   # "x"
    #
    #
    p0_P2 = Poly1D("2-2*X")*Poly1D("0.5-X")   # 2.0*(1.0-x)*(0.5-x)
    p1_P2 = Poly1D("4*X")*Poly1D("1-X")       # 4.0*(x)*(1.0-x)
    p2_P2 = Poly1D("2*X")*Poly1D("-0.5+X")    # 2.0*(x)*(x-0.5)
    #
    #
    p0_P3 = Poly1D("0.5")*Poly1D("1-X")*Poly1D("1.0-3*X")*Poly1D("2.0-3*X")   # 9.0/2.0*(1.0-x)*(1.0/3.0-x)*(2.0/3.0-x)
    p1_P3 = Poly1D("4.5")*Poly1D("X")*Poly1D("1-X")*Poly1D("2.0-3*X")         # 27.0/2.0*(x)*(1.0-x)*(2.0/3.0-x)
    p2_P3 = Poly1D("4.5")*Poly1D("X")*Poly1D("-1.0+3*X")*Poly1D("1-X")        # 27.0/2.0*(x)*(x-1.0/3.0)*(1.0-x)
    p3_P3 = Poly1D("0.5")*Poly1D("X")*Poly1D("-1.0+3*X")*Poly1D("-2.0+3*X")   # 9.0/2.0*(x)*(x-1.0/3.0)*(x-2.0/3.0)
    #
    #
    p0_H3 =  Poly1D("-1+X")*Poly1D("-1+X")*Poly1D("1+2*X")  # (x-1.0)*(x-1.0)*(2.0*x+1.0)
    p1_H3 =  Poly1D("X")*Poly1D("-1+X")*Poly1D("-1+X")      # x*(x-1.0)*(x-1.0)
    p2_H3 =  Poly1D("X")*Poly1D("X")*Poly1D("3-2*X ")       # x*x*(-2.0*x+3.0)
    p3_H3 =  Poly1D("X")*Poly1D("X")*Poly1D("-1+X")         # x*x*(x-1.0)
    #
    #
    p0_S3 =  Poly1D("1-X")*Poly1D("1-X")*Poly1D("1-X") 
    p1_S3 =  Poly1D("4.0-3*X") + Poly1D("3")*Poly1D("1-2*X+X^2") + Poly1D("-3+3*X")*Poly1D("1-2*X+X^2")
    p2_S3 =  Poly1D("1.0+3*X") + Poly1D("3*X^2")  + Poly1D("-3.0")*Poly1D("X^3") 
    p3_S3 =  Poly1D("X^3")
    #
    #
    p0_S5 = Poly1D("1-X")*Poly1D("1-X")*Poly1D("1-X")*Poly1D("1-X")*Poly1D("1-X")
    p1_S5 = Poly1D("26- 50*X + 20*X^2 + 20*X^3 - 20*X^4 + 5.0*X^5")
    p2_S5 = Poly1D("66 - 60*X^2 + 30*X^4 - 10*X^5") 
    p3_S5 = Poly1D("26 + 50*X + 20*X^2 - 20*X^3 - 20*X^4 + 10*X^5") 
    p4_S5 = Poly1D("1 + 5*X + 10*X^2 + 10*X^3 + 5*X^4 - 5*X^5")     
    p5_S5 = Poly1D("X^5")
    #
    #
    P   = []
    d1P = []
    d2P = []
    #
    #
    if ( elt1 == "P1" ) :
        #
        DF = 2
        #    
        P.append(p0_P1)
        P.append(p1_P1)
    #
    elif ( elt1 == "P2" ):
        #
        DF = 3
        # 
        P.append(p0_P2)
        P.append(p1_P2)
        P.append(p2_P2)
    #
    elif ( elt1 == "P3" ):
        #
        DF = 4
        # 
        P.append(p0_P3)
        P.append(p1_P3)
        P.append(p2_P3)
        P.append(p3_P3)
    #
    elif ( elt1 == "H3" ):
        #
        DF = 4
        # 
        P.append(p0_H3)
        P.append(p1_H3)
        P.append(p2_H3)
        P.append(p3_H3)
    #
    elif ( elt1 == "S3" ):
        #
        DF = 4
        # 
        P.append(p0_S3)
        P.append(p1_S3)
        P.append(p2_S3)
        P.append(p3_S3)
    #
    elif ( elt1 == "S5" ):
        #
        DF = 6
        # 
        P.append(p0_S5)
        P.append(p1_S5)
        P.append(p2_S5)
        P.append(p3_S5)
        P.append(p4_S5)
        P.append(p5_S5)
    #
    else:
        raise TypeError, " bad elt"
    #
    #
    for i in range(DF):
        d1P.append( P[i].diff() )
        d2P.append( d1P[i].diff() )
    #
    #
    for i in range(DF):
        print "P[",i,"] = ", P[i]
    #
    #
    for i in range(DF):
        print "d1P[",i,"] = ", d1P[i]
    #
    for i in range(DF):
        print "d2P[",i,"] = ", d2P[i]
    
    #
    #
    #
    mat_I__I_ = []
    mat_x__x_ = []
    mat_xx_xx = []
    mat_I__x_ = []
    mat_I__xx = []
    mat_x__xx = []
    #
    for i in range(DF):
        #
        mat_I__I_.append([])
        mat_x__x_.append([])
        mat_xx_xx.append([])
        mat_I__x_.append([])
        mat_I__xx.append([])
        mat_x__xx.append([])    
        #
    #
    for i in range(DF):
        for j in range(DF):
            mat_I__I_[i].append(P[i]*P[j])
            mat_x__x_[i].append(d1P[i]*d1P[j])
            mat_xx_xx[i].append(d2P[i]*d2P[j])
            mat_I__x_[i].append(P[i]*d1P[j])
            mat_I__xx[i].append(P[i]*d2P[j])
            mat_x__xx[i].append(d1P[i]*d2P[j])
    #
    #
    #
    mat_III  = []
    mat_IxI  = []
    #
    for k in range(DF):
        #
        mat_III.append([])
        mat_IxI.append([])
        #
        for j in range(DF):
            mat_III[k].append([])
            mat_IxI[k].append([])
        #
    #
    for k in range(DF):
        for i in range(DF):
            for j in range(DF):
                mat_III[k][i].append(P[k]*P[i]*P[j])
                mat_IxI[k][i].append(P[k]*d1P[i]*P[j])
    #
    #
    #
    # The integer with which the multiply the result to have a nice rational result
    coeff_mass   = 0
    coeff_stiff1 = 0
    coeff_stiff2 = 0
    coeff_convec = 0
    coeff_I_xx   = 0
    coeff_x_xx   = 0
    #
    coeff_vector = 0
    #
    coeff_III = 0
    coeff_IxI = 0
    #
    #
    if ( elt1 == "P1" ) :
        # 
        coeff_mass   = 1*2*3                      # P_P       is of deg  2+1
        coeff_stiff1 = 1                          # dP_dP     is of deg  0+1
        coeff_stiff2 = 0                          # ddP_ddP   is of deg  0
        coeff_convec = 1*2                        # P_dP      is of deg  1+1
        coeff_I_xx   = 0                          # P_ddP     is of deg  0
        coeff_x_xx   = 0                          # dP_ddP    is of deg  0
        #
        coeff_vector = 1*2                        # P         is of deg  1+1
        #
        coeff_III    = 1*2*3*4                    # P_P_P     is of deg  3+1
        coeff_IxI    = 1*2*3                      # P_dP_P    is of deg  2+1
    #
    elif ( elt1 == "P2" ):
        #
        coeff_mass   = 1*2*3*4*5                  # P_P       is of deg  4+1
        coeff_stiff1 = 1*2*3                      # dP_dP     is of deg  2+1
        coeff_stiff2 = 1                          # ddP_ddP   is of deg  0+1
        coeff_convec = 1*2*3*4                    # P_dP      is of deg  3+1
        coeff_I_xx   = 1*2*3                      # P_ddP     is of deg  2+1
        coeff_x_xx   = 1*2                        # dP_ddP    is of deg  1+1
        #
        coeff_vector = 2*3                        # P         is of deg  2+1
        #
        coeff_III    = 2*3*4*5*6*7                # P_P_P     is of deg  6+1
        coeff_IxI    = 2*3*4*5*6                  # P_dP_P    is of deg  5+1
    #
    elif ( elt1 == "P3" ):
        #
        coeff_mass   = 1*2*3*4*5*6*7              # P_P       is of deg  6+1
        coeff_stiff1 = 1*2*3*4*5                  # dP_dP     is of deg  4+1
        coeff_stiff2 = 1*2*3                      # ddP_ddP   is of deg  2+1
        coeff_convec = 1*2*3*4*5*6                # P_dP      is of deg  5+1
        coeff_I_xx   = 1*2*3*4*5                  # P_ddP     is of deg  4+1
        coeff_x_xx   = 1*2*3*4                    # dP_ddP    is of deg  3+1
        #
        #coeff_vector = 4                          # P         is of deg  3+1
        coeff_vector = 2*4                          # P         is of deg  3+1 # because coeff in front of X*X*X
        #
        coeff_III    = 2*3*4*5*6*7*8*9*10         # P_P_P     is of deg  9+1
        coeff_IxI    = 2*3*4*5*6*7*8*9            # P_dP_P    is of deg  8+1
    #
    elif ( elt1 == "H3" ):
        #
        coeff_mass   = 1*2*3*4*5*6*7              # P_P       is of deg  6+1
        coeff_stiff1 = 1*2*3*4*5                  # dP_dP     is of deg  4+1
        coeff_stiff2 = 1*2*3                      # ddP_ddP   is of deg  2+1
        #coeff_convec = 1*2*3*4*5*6                # P_dP      is of deg  5+1
        coeff_convec = 1*3*4*5                    # P_dP      is of deg  5+1
        coeff_I_xx   = 1*2*3*4*5                  # P_ddP     is of deg  4+1
        coeff_x_xx   = 1*2*3*4                    # dP_ddP    is of deg  3+1
        #
        coeff_vector = 3*4                          # P         is of deg  3+1
        #
        coeff_III    = 2*3*4*5*6*7*8*9*10         # P_P_P     is of deg  9+1
        coeff_IxI    = 2*3*4*5*6*7*8*9            # P_dP_P    is of deg  8+1
    #
    elif ( elt1 == "S3" ):
        #
        coeff_mass   = 1*2*3*4*5*6*7              # P_P       is of deg  6+1
        #coeff_stiff1 = 1*2*3*4*5                  # dP_dP     is of deg  4+1
        coeff_stiff1 = 1*2*3*4*5                  # dP_dP     is of deg  4+1
        coeff_stiff2 = 1*2*3                      # ddP_ddP   is of deg  2+1
        #coeff_convec = 1*2*3*4*5*6                # P_dP      is of deg  5+1
        coeff_convec = 1*4*5                  # P_dP      is of deg  5+1
        #coeff_I_xx   = 1*2*3*4*5                  # P_ddP     is of deg  4+1
        #coeff_x_xx   = 1*2*3*4                    # dP_ddP    is of deg  3+1
        coeff_I_xx   = 1*2*5                  # P_ddP     is of deg  4+1
        coeff_x_xx   = 1*2                    # dP_ddP    is of deg  3+1
        #
        coeff_vector = 4                          # P         is of deg  3+1
        #
        coeff_III    = 2*3*4*5*6*7*8*9*10         # P_P_P     is of deg  9+1
        coeff_IxI    = 2*3*4*5*6*7*8*9            # P_dP_P    is of deg  8+1
    #
    elif ( elt1 == "S5" ):
        #
        #coeff_mass  = 1*2*3*4*5*6*7*8*9*10*11    # P_P       is of deg  10+1
        coeff_mass   = 1*2*3*4*5*6*7*11           # P_P       is of deg  10+1
        #coeff_stiff1 = 1*2*3*4*5*6*7*8*9         # dP_dP     is of deg  8+1
        coeff_stiff1 = 1*2*3*6*7              # dP_dP     is of deg  8+1
        #coeff_stiff2 = 1*2*3*4*5*6*7             # ddP_ddP   is of deg  6+1
        coeff_stiff2 = 1*7              # ddP_ddP   is of deg  6+1
        #coeff_convec = 1*2*3*4*5*6*7*8*9*10       # P_dP      is of deg  9+1
        coeff_convec = 1*4*7*9       # P_dP      is of deg  9+1
        #coeff_I_xx   = 1*2*3*4*5*6*7*8*9          # P_ddP     is of deg  8+1
        #coeff_x_xx   = 1*2*3*4*5*6*7*8            # dP_ddP    is of deg  7+1
        coeff_I_xx   = 1*7*8*9          # P_ddP     is of deg  8+1
        coeff_x_xx   = 1*2*3*7            # dP_ddP    is of deg  7+1
        #
        coeff_vector = 6                          # P         is of deg  5+1
        #
        #coeff_III   = 1*2*3*4*5*6*7*8*9*10*11*12*13*14*15*16         # P_P_P     is of deg  15+1
        coeff_III    = 1*6*7*8*11*13*14                               # deg 15+1
        #coeff_IxI   = 1*2*3*4*5*6*7*8*9            # P_dP_P    is of deg  14+1
        coeff_IxI    = 1*6*7*8*11*13*14    # deg 14+1
    #
    else:
        raise TypeError, " bad elt"
    #
    #
    #
    int_I__I_ = []
    int_x__x_ = []
    int_xx_xx = []
    int_I__x_ = []
    int_I__xx = []
    int_x__xx = []
    #
    int_vector_I_ = []
    int_vector_x_ = []
    int_vector_xx = []
    #
    for i in range(DF):
        #
        int_I__I_.append([])
        int_x__x_.append([])
        int_xx_xx.append([])
        int_I__x_.append([])
        int_I__xx.append([])
        int_x__xx.append([])
        #
    #
    #
    int_III = []
    int_IxI = []
    #
    for k in range(DF):
        #
        int_III.append([])
        int_IxI.append([])
        #
        for i in range(DF):
            #
            int_III[k].append([])
            int_IxI[k].append([])
        #
    #
    #
    #
    for i in range(DF):
        #
        for j in range(DF):
            #
            val_I__I_ = rounding_value( mat_I__I_[i][j].integr_01() * coeff_mass   , 'val_I__I_')
            val_x__x_ = rounding_value( mat_x__x_[i][j].integr_01() * coeff_stiff1 , 'val_x__x_')
            val_xx_xx = rounding_value( mat_xx_xx[i][j].integr_01() * coeff_stiff2 , 'val_xx_xx')
            val_I__x_ = rounding_value( mat_I__x_[i][j].integr_01() * coeff_convec , 'val_I__x_')
            val_I__xx = rounding_value( mat_I__xx[i][j].integr_01() * coeff_I_xx   , 'val_I__xx')
            val_x__xx = rounding_value( mat_x__xx[i][j].integr_01() * coeff_x_xx   , 'val_x__xx')
            #
            #
            int_I__I_[i].append( val_I__I_ )
            int_x__x_[i].append( val_x__x_ )
            int_xx_xx[i].append( val_xx_xx )
            int_I__x_[i].append( val_I__x_ )
            int_I__xx[i].append( val_I__xx )
            int_x__xx[i].append( val_x__xx )
            #
        #
        #
        val_I_ = rounding_value(   P[i].integr_01() * coeff_vector, 'val_I_')
        val_x_ = rounding_value( d1P[i].integr_01() * coeff_vector, 'val_x_')
        val_xx = rounding_value( d2P[i].integr_01() * coeff_vector, 'val_xx')
        #
        #
        int_vector_I_.append( val_I_ )
        int_vector_x_.append( val_x_ )
        int_vector_xx.append( val_xx )
        #
    #
    #
    for k in range(DF):
        for i in range(DF):
            for j in range(DF):
            #
                pass
                #
                #val_III = rounding_value( coeff_III * mat_III[k][i][j].integr_01(), 'III')
                #val_IxI = rounding_value( coeff_IxI * mat_IxI[k][i][j].integr_01(), 'IxI')
                #
                #
                #int_III[k][i].append( val_III )
                #int_IxI[k][i].append( val_IxI )
            #
        #
    #
    #
    #
    #
    print "#################################################"
    print "Mass ="
    #print int_I__I_, " / ", coeff_mass
    #
    print "#################################################"
    print "stiff1  ="
    #print int_x__x_, " / ", coeff_stiff1
    #
    print "#################################################"
    print "stiff2  ="
    #print int_xx_xx, " / ", coeff_stiff2
    #
    print "#################################################"
    print "convec  ="
    #print int_I__x_, " / ", coeff_convec
    #
    print "#################################################"
    print "mat_I_xx  ="
    #print int_I__xx, " / ", coeff_I_xx
    #
    print "#################################################"
    print "mat_x_xx  ="
    #print int_x__xx, " / ", coeff_x_xx
    #
    print "#################################################"
    print "vector_I ="
    print int_vector_I_ , " / ",coeff_vector
    #
    print "#################################################"
    print "vector_x ="
    print int_vector_x_ , " / ",coeff_vector
    #
    print "#################################################"
    print "vector_xx ="
    print int_vector_xx , " / ",coeff_vector
    #
    #
    #
    print "#################################################"
    print ("Mat_III")
    #
    for k in range(DF) :
        print "III[",k,"]"
        print int_III[k], " / ",coeff_III
    #
    print "#################################################"
    print ("Mat_IxI")
    #
    for k in range(DF) :
        print "IxI[",k,"]"
        print int_IxI[k], " / ",coeff_IxI
    #
    #
