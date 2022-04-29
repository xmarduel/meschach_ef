#
#
from copy import *
from string import *
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2005/01/03 09:15:33 $
#  $Revision: 1.3 $
#  ----------------------------------------------------------------
#
#
def parse_string_for_monome3D(str):
    #
    coeff = 0.0
    degX = str.count('X')
    degY = str.count('Y')
    degZ = str.count('Z')
    #
    #
    str1 = copy(str)
    #
    str1 = str1.replace(' ','')
    #
    str1 = str1.replace('*','')
    str1 = str1.replace('X','')
    str1 = str1.replace('Y','')
    str1 = str1.replace('Z','')
    #
    #
    is_neg = True
    #
    if ( str1[0] == '-' ):
        is_neg = True
    else :
        is_neg = False
    #
    #
    str1 = str1.replace('-','')
    str1 = str1.replace('+','')
    #
    try:
        coeff = atof(str1)
    except: #exception ->  coeff = 1.0 or 0.0
        if ( degX==0 and degY==0 and degZ==0 ):
            coeff  = 0.0
        else:
            coeff = 1.0
    #
    #
    if ( is_neg ) :
        coeff *= -1
    #
    #
    return [degX, degY, degZ, coeff] 
    #
#
#
def parse_string_for_polynome3D(str):
    #
    data = [] # the list of monome3D
    #
    str1 = copy(str)
    str1 = str1.replace(' ','') # remove the empty space
    #
    if ( str1.startswith('+') or str1.startswith('-') ):
        pass
    else:
        str1 = '+' + str1
    #
    #
    nbMonomes  = 0
    nbMonomes += str1.count('+')
    nbMonomes += str1.count('-')
    #
    #loop over the monomes
    while ( nbMonomes > 0 ):
        #
        idx1 = str1.find('+',1)
        idx2 = str1.find('-',1)
        #
        idx = 0
        #
        if ( idx1 == -1 and idx2 == -1 ):
            #
            if ( nbMonomes  > 1 ):
                raise TypeError,"idx1 and idx2 = -1"
            else:
                idx = len(str1)
        elif ( idx1 == -1 ):
            idx = idx2
        elif ( idx2 == -1 ):
            idx = idx1
        else :
            if ( idx1 < idx2 ):
                idx = idx1
            else :
                idx = idx2
        #
        strMonome = str1[0:idx]
        #
        data.append( Monome3D(parse_string_for_monome3D(strMonome)))
        #
        # remove strMonome from str1
        str1 = str1[idx:]
        #
        nbMonomes -= 1
    #
    #
    return data
    #
#
#
#
def sort_fun( m1, m2 ):
    #
    res  =  m1.__cmp__(m2)
    #
    return res
    #
#    
#
def fact(n):
    #
    if ( n>1 ):
        return n*fact(n-1)
    else:
        return 1
#
#        
#################################################################################################
#
class Monome3D(object):
    #
    def __init__(self, data):
        #
        if ( isinstance(data, str) ):
            self.data = parse_string_for_monome3D(data)
        elif ( isinstance(data, list) ):
            self.degX = data[0]
            self.degY = data[1]
            self.degZ = data[2]
            self.coeff = data[3]
        elif ( isinstance(data, Monome3D) ):
            self.degX = copy(data.degX)
            self.degY = copy(data.degY)
            self.degZ = copy(data.degZ)
            self.coeff = copy(data.coeff)
        else:
            print "data for Monome3D.__init__", data
            raise TypeError, "Monome3D.__init__"
        #
        #
    #
    # Outputs
    #
    def __str__(self):
        return '%lf'%self.coeff + '*X^%1d'%self.degX + '*Y^%1d'%self.degY + '*Z^%1d'%self.degZ
    #
    def __repr__(self):
        return '%lf'%self.coeff + '*X^%1d'%self.degX + '*Y^%1d'%self.degY+ '*Z^%1d'%self.degZ
    #
    #
    #
    def int_01(self):
    # 
        num   = fact(self.degX)*fact(self.degY)*fact(self.degZ)
        denom = fact(self.degX+self.degY+self.degZ+3)
        #
        return ( 1.0*self.coeff*num ) / denom
    #
    #
    #
    #
    def __mul__(self,other):
        #
        if ( isinstance(other, Monome3D) ):
            degX = self.degX + other.degX
            degY = self.degY + other.degY
            degZ = self.degZ + other.degZ
            coeff = self.coeff * other.coeff
        elif (isinstance(other, int)) :
            degX = self.degX
            degY = self.degY
            degZ = self.degZ
            coeff = self.coeff * other
        elif (isinstance(other, float)) :
            degX = self.degX
            degY = self.degY
            degZ = self.degZ
            coeff = self.coeff * other
        else :
           raise typeError,"must  multiply with a int, a float or a Monome3D"
        #
        return Monome3D( [degX, degY, degZ, coeff] )
        #
    #
    #
    #
    def array_to_string(self):
        #
        str = ' %+lf'%self.coeff
        #
        for i in range(self.degX) :
            str += '*X'
        for i in range(self.degY) :
            str += '*Y'
        for i in range(self.degZ) :
            str += '*Z'
        #
        return str
    #
    #
    #
    def __cmp__(self,other):
        #
        total_deg1 = self.degX  + self.degY + self.degZ
        total_deg2 = other.degX + other.degY + other.degZ
        #
        if ( total_deg1 < total_deg2 ):
            return -1
        elif ( total_deg1 > total_deg2 ):
            return  1
        else:
            if ( self.degX < other.degX ):
                return  1
            elif ( self.degX > other.degX ):
                return -1
            else:
                if ( self.degY < other.degY ):
                    return  1
                elif ( self.degY > other.degY ):
                    return -1
                else:
                    if ( self.degZ < other.degZ ):
                        return  1
                    elif ( self.degZ > other.degZ ):
                        return -1
                    else:
                        return 0
                
#
#
#
#
#################################################################################################
#
class Poly3D(object):
    #
    def __init__(self, data):
        #
        if ( isinstance(data, str) ):
            self.data = parse_string_for_polynome3D(data)
        elif ( isinstance(data, list) ): # a list of monomes3D
            self.data = data
        else:
            print "data for Poly3D.__init__", data
            raise TypeError, "Poly3D.__init__"
        #
        self.simplify_list_of_monomes()
        #
    #
    #
    #
    def __str__(self):
        """
        output the polynome
        """
        str = ''
        #
        if ( len(self.data) == 0 ):
            str = " 0"
        else:
            for i in range(len(self.data)):
                str += self.data[i].array_to_string()
        #
        return str
    #
    def __repr__(self):
        """
        output the polynome
        """
        str = ''
        #
        if ( len(self.data) == 0 ):
            str = " 0"
        else:
            for i in range(len(self.data)):
                str += self.data[i].array_to_string()
        #
        return str
    #
    #
    #
    def simplify_list_of_monomes ( self ):
        #
        data= []
        #
        for i in range (len(self.data)):
            #
            degX      = self.data[i].degX
            degY      = self.data[i].degY
            degZ      = self.data[i].degZ
            coeff     = self.data[i].coeff
            #
            has_same_monome = False
            #
            for j in range(len(data)):
                #
                degX_2 = data[j].degX 
                degY_2 = data[j].degY
                degZ_2 = data[j].degZ
                #
                if ( (degX == degX_2)  and (degY == degY_2) and (degZ == degZ_2) ):
                    #
                    data[j].coeff +=  coeff
                    has_same_monome = True
                    #
                    # remove the monome ?
                    if ( data[j].coeff== 0.0 ):
                        del data[j]
                    #
                    break
                #
            #
            if ( has_same_monome == False ):
                data.append( copy(self.data[i]) )
            #
        #
        # sort
        data.sort(sort_fun)
        #
        self.data = data;
        #
    #
    #
    # Operation
    def __mul__(self, other):
        """
        """
        data = []
        #
        for i in range(len(self.data)):
            for j in range(len(other.data)):
                #
                degX = self.data[i].degX + other.data[j].degX
                degY = self.data[i].degY + other.data[j].degY
                degZ = self.data[i].degZ + other.data[j].degZ
                #
                coeff = self.data[i].coeff * other.data[j].coeff
                #
                data.append( Monome3D([degX, degY, degZ, coeff]) )
        #
        return Poly3D( data )
    #
    #
    def __add__(self, other):
        #
        data = []
        #
        for i in range(len(self.data)):
            data.append( self.data[i] )
        #
        for i in range(len(other.data)):
            data.append( other.data[i] )
        #
        #
        return Poly3D( data )
        #
    #
    #
    def __sub__(self, other):
        #
        data = []
        #
        for i in range(len(self.data)):
            data.append( self.data[i] )
        #
        for i in range(len(other.data)):
            data_tmp = copy(other.data[i])
            data_tmp.coeff *= -1
            data.append( data_tmp )
        #
        #
        return Poly3D( data )
    #
    #
    #
    #
    def __pos__(self):
        """
        """
        data = copy(self.data)
        #
        return Poly3D(data)
        #
    #
    #
    def __neg__(self):
        """
        """
        #
        data = copy(self.data)
        #
        for i in range(len(self.data)):
            data[i].coeff *= -1
        #
        return Poly3D(data)
    #
    #
    #
    
    def int_01(self):
        """
        integrate on the "base" triangle
        """
        val = 0.0
        #
        for i in range(len(self.data)):
        #
            #
            degX = self.data[i].degX
            degY = self.data[i].degY
            degZ = self.data[i].degZ
            #
            coeff = self.data[i].coeff
            #
            #
            num   = fact(degX)*fact(degY)*fact(degZ)
            denom = fact(degX + degY + degZ + 3)
            #
            val_tmp = ( 1.0*coeff*num ) / denom
            #
            val += val_tmp
        #
        return val
    #
    #
    #
    def dx(self):
        """
        differentiate on the X variable
        """
        data = []
        #
        for i in range(len(self.data)):
            #
            data_tmp  = copy(self.data[i])
            #
            if ( data_tmp.degX > 0 ):
                data_tmp.coeff *= data_tmp.degX
                data_tmp.degX  -= 1
                #
                data.append( data_tmp )
        #
        return Poly3D( data )
        #
    #
    #
    def dy(self):
        """
        differentiate on the Y variable
        """
        data = []
        #
        for i in range(len(self.data)):
            #
            data_tmp  = copy(self.data[i])
            #
            if ( data_tmp.degY > 0 ):
                data_tmp.coeff *= data_tmp.degY
                data_tmp.degY  -= 1
                #
                data.append( data_tmp )
        #
        return Poly3D( data )
    #
    #
    def dz(self):
        """
        differentiate on the Z variable
        """
        data = []
        #
        for i in range(len(self.data)):
            #
            data_tmp  = copy(self.data[i])
            #
            if ( data_tmp.degZ > 0 ):
                data_tmp.coeff *= data_tmp.degZ
                data_tmp.degZ  -= 1
                #
                data.append( data_tmp )
        #
        return Poly3D( data )
        #
    #
    #
