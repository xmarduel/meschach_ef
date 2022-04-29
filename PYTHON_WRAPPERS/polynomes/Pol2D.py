#
#
from copy import *
from string import *
#
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2005/01/02 16:10:02 $
#  $Revision: 1.7 $
#  ----------------------------------------------------------------
#
#
def parse_string_for_monome2D(str):
    #
    degX = str.count('X')
    degY = str.count('Y')
    coeff = 0.0
    #
    #
    str1 = copy(str)
    #
    str1 = str1.replace(' ','')
    #
    str1 = str1.replace('*','')
    str1 = str1.replace('X','')
    str1 = str1.replace('Y','')
    #
    #
    is_neg = False
    #
    if ( str1[0] == '-' ):
        is_neg = True
    else :
        is_neg = False
    #
    str1 = str1.replace('-','')
    str1 = str1.replace('+','')
    #
    try:
        coeff = atof(str1)
    except: # exception dtr1 is not a number  ->  coeff = 1.0 or 0.0
        if ( degX==0 and degY==0 ):
            coeff  = 0.0
        else:
            coeff = 1.0
    #
    #
    if ( is_neg ) :
        coeff *= -1
    #
    #
    return [ degX, degY, coeff] 
    #
#
#
def parse_string_for_polynome2D(str):
    #
    list_data = [] # the list of monome2D
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
        list_data.append(Monome2D(parse_string_for_monome2D(strMonome)))
        #
        # remove strMonome from str1
        str1 = str1[idx:]
        #
        nbMonomes -= 1
    #
    return list_data
    #
#
#
#
def sort_fun( Monome1, Monome2 ):
    #
    res  =  Monome1.__cmp__(Monome2)
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
class Monome2D(object):
    """
    the main class : Monome2D
    """
    def __init__(self, data):
        """
        setup te 3 members of the class : degX, degY, coeff
        """
        if ( isinstance(data, str) ):
            self.data = parse_string_for_monome2D(data)
        elif ( isinstance(data, list) ):
            self.degX = data[0]
            self.degY = data[1]
            self.coeff = data[2]
        elif ( isinstance(data, Monome2D) ):
            self.degX = copy(data.degX)
            self.degY = copy(data.degY)
            self.coeff = copy(data.coeff)
        else:
            print "data for Monome2D.__init__", data
            raise TypeError, "Monome2D.__init__"
        #
        #
    #
    # Outputs
    #
    def __str__(self):
        return '%lf'%self.coeff + '*X^%1d'%self.degX + '*Y^%1d'%self.degY
    #
    def __repr__(self):
        return '%lf'%self.coeff + '*X^%1d'%self.degX + '*Y^%1d'%self.degY
    #
    #
    #
    def int_01(self):
    # 
        num   = fact(self.degX)*fact(self.degY)
        denom = fact(self.degX+self.degY+2)
        #
        return ( 1.0*self.coeff*num ) / denom
    #
    #
    # Operations
    #
    def __mul__(self,other):
        #
        if ( isinstance(other, Monome2D) ):
            degX = self.degX + other.degX
            degY = self.degY + other.degY
            coeff = self.coeff * other.coeff
        elif (isinstance(other, int)) :
            degX = self.degX
            degY = self.degY
            coeff = self.coeff * other
        elif (isinstance(other, float)) :
            degX = self.degX
            degY = self.degY
            coeff = self.coeff * other
        else :
           raise typeError,"must  multiply with a int, a float or a Monome2D"
        #
        return Monome2D( [degX, degY, coeff] )
        #
    #
    #
    def __pos__(self):
        #
        degX = copy(self.degX)
        degY = copy(self.degY)
        coeff = copy(self.coeff)
        #
        return Monome2D( [degX, degY, coeff] )
        #
    #
    #
    def __neg__(self):
        #
        degX = copy(self.degX)
        degY = copy(self.degY)
        coeff  = copy(self.coeff)
        #
        return Monome2D( [degX, degY, -coeff] )
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
        #
        return str
    #
    #
    #
    def __cmp__(self,other):
        #
        total_deg1 = self.degX  + self.degY
        total_deg2 = other.degX + other.degY
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
                    return 0
                
#
#
#
#
#################################################################################################
#
class Poly2D(object):
    """
    the main class : Polynome2D
    """
    def __init__(self, data):
        """
        setup self.data : list of monomes2D
        """
        #
        if ( isinstance(data, str) ):
            self.data = parse_string_for_polynome2D(data)
        elif ( isinstance(data, list) ): # a list of Monomes2D
            self.data = data
        else:
            print "data for Poly2D.__init__", data
            raise TypeError, "Poly2D.__init__"
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
        """
        """
        data = []
        #
        for i in range (len(self.data)):
            #
            degX      = self.data[i].degX
            degY      = self.data[i].degY
            coeff       = self.data[i].coeff
            #
            has_same_monome = False
            #
            for j in range(len(data)):
                #
                degX_2 = data[j].degX 
                degY_2 = data[j].degY
                #
                if ( (degX == degX_2)  and (degY == degY_2) ):
                    #
                    data[j].coeff +=  coeff
                    has_same_monome = True
                    #
                    # remove the monome ?
                    if ( data[j].coeff == 0.0 ):
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
    # Operations
    #
    def __mul__(self, other):
        """
        return ret = self  * other
        """
        data = []
        #
        for i in range(len(self.data)):
            for j in range(len(other.data)):
                #
                degX = self.data[i].degX + other.data[j].degX
                degY = self.data[i].degY + other.data[j].degY
                #
                coeff = self.data[i].coeff * other.data[j].coeff
                #
                data.append( Monome2D([degX, degY, coeff]) )
        #
        return Poly2D( data )
    #
    #
    def __add__(self, other):
        """
        return ret = self  + other
        """
        data = []
        #
        for i in range(len(self.data)):
            data.append( self.data[i] )
        #
        for i in range(len(other.data)):
            data.append( other.data[i] )
        #
        #
        return Poly2D(data)
        #
    #
    #
    def __sub__(self, other):
        """
        return ret = self  - other
        """
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
        return Poly2D( data )
    #
    #
    #
    def __pos__(self):
        """
        return pol =  self  ( a copy of the polynome )
        """
        data = copy(self.data)
        #
        return Poly2D(data)
        #
    #
    #
    def __neg__(self):
        #
        """
        return pol = - self
        """
        data = copy(self.data)
        #
        for i in range(len(self.data)):
            data[i].coeff *= -1
        #
        return Poly2D(data)
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
            degX = self.data[i].degX
            degY = self.data[i].degY
            #
            coeff = self.data[i].coeff
            #
            num   = fact(degX)*fact(degY)
            denom = fact(degX + degY + 2)
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
        return Poly2D( data )
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
        return Poly2D( data )
        #
    #
